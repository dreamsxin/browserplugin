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
		RakNet::RakPeerInterface *client = RakNet::RakPeerInterface::GetInstance();
		client->Startup(1, &socketDescriptor, 1);

		bool ret = (client->Connect(host.c_str(), port, NULL, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED);

		if (!ret) {
			return -1;
		}
		boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&EotuSocketAPI::receiveUDP, this, client, callback)));
		threads.push_back(thread);
		udpClients.insert(std::pair<int, RakNet::RakPeerInterface*>(threads.size(), client));
	} else {
		RakNet::PacketizedTCP *client = RakNet::PacketizedTCP::GetInstance();
		if (client->Start(0, 1) == false) {
			return -1;
		}

		client->Connect(host.c_str(), port, NULL, 0, 0);
		boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&EotuSocketAPI::receiveTCP, this, client, callback)));
		threads.push_back(thread);
		tcpClients.insert(std::pair<int, RakNet::PacketizedTCP*>(threads.size(), client));
	}

	return threads.size();
}

void EotuSocketAPI::send(const int sock, const std::string& data) {
	if (udpClients.find(sock) != udpClients.end()) {
		RakNet::RakPeerInterface *client = udpClients[sock];
		client->Send(data.c_str(), data.length(), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	} else if (tcpClients.find(sock) != tcpClients.end()) {
		RakNet::PacketizedTCP *client = tcpClients[sock];
		client->Send(data.c_str(), data.length(), client->HasCompletedConnectionAttempt(), false);
	}
}

void EotuSocketAPI::receiveUDP(RakNet::RakPeerInterface * client, const FB::JSObjectPtr &callback)
{
	fire_Debug("receiveUDP");
	RakNet::Packet* packet;
	while (!boost::this_thread::interruption_requested())
	{
		for (packet=client->Receive(); packet; client->DeallocatePacket(packet), packet=client->Receive()) {
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
	client->Shutdown(500,0);
	RakNet::RakPeerInterface::DestroyInstance(client);
	fire_Debug("receiveUDP stop");
}

void EotuSocketAPI::receiveTCP(RakNet::PacketizedTCP *client, const FB::JSObjectPtr &callback)
{
	fire_Debug("receiveTCP");
	RakNet::SystemAddress serverAddress;
	RakNet::Packet* packet;
	while (!boost::this_thread::interruption_requested())
	{
		RakNet::SystemAddress notificationAddress;
		notificationAddress = client->HasCompletedConnectionAttempt();
		if (notificationAddress != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
		{
			serverAddress = notificationAddress;
			fire_StatusChange(ID_CONNECTION_REQUEST_ACCEPTED, "ID_CONNECTION_REQUEST_ACCEPTED");
			fire_Connected();
		}
		notificationAddress = client->HasFailedConnectionAttempt();
		if (notificationAddress!=RakNet::UNASSIGNED_SYSTEM_ADDRESS) {
			client->CloseConnection(serverAddress);
			fire_StatusChange(ID_CONNECTION_ATTEMPT_FAILED, "ID_CONNECTION_ATTEMPT_FAILED");
			break;
		}
		notificationAddress = client->HasNewIncomingConnection();
		if (notificationAddress!=RakNet::UNASSIGNED_SYSTEM_ADDRESS) {
			fire_StatusChange(ID_NEW_INCOMING_CONNECTION, "ID_NEW_INCOMING_CONNECTION");
		}
		notificationAddress = client->HasLostConnection();
		if (notificationAddress!=RakNet::UNASSIGNED_SYSTEM_ADDRESS) {
			client->CloseConnection(serverAddress);
			fire_StatusChange(ID_CONNECTION_LOST, "ID_CONNECTION_LOST");
			break;
		}
		for (packet=client->Receive(); packet; client->DeallocatePacket(packet), packet=client->Receive()) {
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
	client->Stop();
	RakNet::PacketizedTCP::DestroyInstance(client);
	fire_Debug("receiveTCP stop");
}