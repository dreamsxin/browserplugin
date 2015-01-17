/**********************************************************\

  Auto-generated EotuSocketAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "EotuSocketAPI.h"

///////////////////////////////////////////////////////////////////////////////
/// @fn FB::variant EotuSocketAPI::echo(const FB::variant& msg)
///
/// @brief  Echos whatever is passed from Javascript.
///         Go ahead and change it. See what happens!
///////////////////////////////////////////////////////////////////////////////
FB::variant EotuSocketAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo("So far, you clicked this many times: ", n++);

    // return "foobar";
    return msg;
}

///////////////////////////////////////////////////////////////////////////////
/// @fn EotuSocketPtr EotuSocketAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
EotuSocketPtr EotuSocketAPI::getPlugin()
{
    EotuSocketPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

// Read/Write property testString
std::string EotuSocketAPI::get_testString()
{
    return m_testString;
}

void EotuSocketAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string EotuSocketAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

void EotuSocketAPI::testEvent()
{
    fire_test();
}

int EotuSocketAPI::connect(const std::string& host, const int port, const FB::JSObjectPtr &callback, const bool useTCP)
{
	if (!useTCP) {
		RakNet::SocketDescriptor socketDescriptor(0, 0);
		RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
		peer->Startup(1, &socketDescriptor, 1);

		bool ret = (peer->Connect(host.c_str(), port, NULL, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED);

		if (!ret) {
			return -1;
		}
		++m_lastsock;
		Client *client = RakNet::OP_NEW<Client>(__FILE__, __LINE__);
		client->useTCP = useTCP;
		client->sock = m_lastsock;
		client->udpPeer = peer;
		client->systemAddresses = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
		clients[m_lastsock] = client;
		boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&EotuSocketAPI::receiveUDP, this, m_lastsock, callback)));
		threads.push_back(thread);
	} else {
		// RakNet::TCPInterface *peer = RakNet::TCPInterface::GetInstance();
		RakNet::PacketizedTCP *peer = RakNet::PacketizedTCP::GetInstance();
		if (peer->Start(0, 1) == false) {
			return -1;
		}

		peer->Connect(host.c_str(), port, NULL, 0, 0);

		++m_lastsock;
		Client *client = RakNet::OP_NEW<Client>(__FILE__, __LINE__);
		client->useTCP = useTCP;
		client->sock = m_lastsock;
		client->tcpPeer = peer;
		client->systemAddresses = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
		clients[m_lastsock] = client;
		boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&EotuSocketAPI::receiveTCP, this, m_lastsock, callback)));
		threads.push_back(thread);
	}


	return m_lastsock;
}

bool EotuSocketAPI::send(const int sock, const std::string& data) {
	if (clients.find(sock) == clients.end()) {
		return false;
	}
	Client* client = clients[sock];
	if (client->useTCP) {
		RakNet::PacketizedTCP *tcpPeer = client->tcpPeer;
		tcpPeer->Send(data.c_str(), data.length(), client->systemAddresses, false);
	} else {
		RakNet::RakPeerInterface *udpPeer = client->udpPeer;
		udpPeer->Send(data.c_str(), data.length(), HIGH_PRIORITY, RELIABLE_ORDERED, 0, client->systemAddresses, false);	
	}
}

void EotuSocketAPI::receiveUDP(const int sock, const FB::JSObjectPtr &callback)
{
	if (clients.find(sock) == clients.end()) {
		return;
	}
	Client* client = clients[sock];
	RakNet::RakPeerInterface *peer = client->udpPeer;
	fire_Debug("receiveUDP");
	RakNet::Packet* packet;
	while (!boost::this_thread::interruption_requested())
	{
		for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive()) {
			unsigned char typeID;
			RakNet::BitStream stream(packet->data, packet->length, false);
			stream.Read(typeID);
			switch (typeID) {
			case ID_ALREADY_CONNECTED:
				{
					fire_StatusChange(ID_ALREADY_CONNECTED, "ID_ALREADY_CONNECTED");
				}
				break;

			case ID_REMOTE_CONNECTION_LOST:
				{
					fire_StatusChange(ID_REMOTE_CONNECTION_LOST, "ID_REMOTE_CONNECTION_LOST");
				}
				break;

			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				{
					fire_StatusChange(ID_REMOTE_NEW_INCOMING_CONNECTION, "ID_REMOTE_NEW_INCOMING_CONNECTION");
				}
				break;
			case ID_CONNECTION_LOST:
				{
					fire_StatusChange(ID_CONNECTION_LOST, "ID_CONNECTION_LOST");
				}
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				{
					fire_StatusChange(ID_CONNECTION_ATTEMPT_FAILED, "ID_CONNECTION_ATTEMPT_FAILED");
				}
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					client->systemAddresses = packet->systemAddress;
					fire_StatusChange(ID_CONNECTION_REQUEST_ACCEPTED, "ID_CONNECTION_REQUEST_ACCEPTED");
					fire_Connected();
				}
				break;
			case ID_USER_PACKET_ENUM:
				{
					fire_StatusChange(ID_USER_PACKET_ENUM, "ID_USER_PACKET_ENUM");
					try {
						callback->Invoke("", FB::variant_list_of(packet->data));
					} catch (const FB::script_error& ex) {
						m_host->htmlLog(std::string("Function call failed with ") + ex.what());
					}
				}
				break;
			default:
				break;
			}
		}

		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}
	peer->Shutdown(500,0);
	RakNet::RakPeerInterface::DestroyInstance(peer);
	fire_Debug("receiveUDP stop");
}

void EotuSocketAPI::receiveTCP(const int sock, const FB::JSObjectPtr &callback)
{
	if (clients.find(sock) == clients.end()) {
		return;
	}
	Client* client = clients[sock];
	RakNet::PacketizedTCP *peer = client->tcpPeer;
	fire_Debug("receiveTCP");
	RakNet::SystemAddress serverAddress;
	RakNet::Packet* packet;
	while (!boost::this_thread::interruption_requested())
	{
		RakNet::SystemAddress notificationAddress;
		notificationAddress = peer->HasCompletedConnectionAttempt();
		if (notificationAddress != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
		{
			serverAddress = notificationAddress;
			client->systemAddresses = serverAddress;
			fire_StatusChange(ID_CONNECTION_REQUEST_ACCEPTED, "ID_CONNECTION_REQUEST_ACCEPTED");
			fire_Connected();
		}
		notificationAddress = peer->HasFailedConnectionAttempt();
		if (notificationAddress!=RakNet::UNASSIGNED_SYSTEM_ADDRESS) {
			peer->CloseConnection(serverAddress);
			fire_StatusChange(ID_CONNECTION_ATTEMPT_FAILED, "ID_CONNECTION_ATTEMPT_FAILED");
			break;
		}
		notificationAddress = peer->HasNewIncomingConnection();
		if (notificationAddress!=RakNet::UNASSIGNED_SYSTEM_ADDRESS) {
			fire_StatusChange(ID_NEW_INCOMING_CONNECTION, "ID_NEW_INCOMING_CONNECTION");
		}
		notificationAddress = peer->HasLostConnection();
		if (notificationAddress!=RakNet::UNASSIGNED_SYSTEM_ADDRESS) {
			peer->CloseConnection(serverAddress);
			fire_StatusChange(ID_CONNECTION_LOST, "ID_CONNECTION_LOST");
			break;
		}
		for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive()) {
			if(packet->systemAddress == serverAddress) {
				fire_StatusChange(ID_USER_PACKET_ENUM, "ID_USER_PACKET_ENUM");
				try {
					callback->Invoke("", FB::variant_list_of(packet->data));
				} catch (const FB::script_error& ex) {
					m_host->htmlLog(std::string("Function call failed with ") + ex.what());
				}
			}
		}
	}
	peer->Stop();
	RakNet::PacketizedTCP::DestroyInstance(peer);
	fire_Debug("receiveTCP stop");
}