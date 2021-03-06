/**********************************************************\

  Auto-generated EotuSocketAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"
#include "json/json.h"
#include "fbjson.h"

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

int EotuSocketAPI::connect(const std::string& host, const int port, const FB::JSObjectPtr& callback)
{
	bool tcp = false;
	try {
		tcp = callback->GetProperty("tcp").convert_cast<bool>();
	} catch(FB::bad_variant_cast& e) {
		throw FB::script_error(e.what());
	}

	if (!tcp) {
		RakNet::SocketDescriptor socketDescriptor(0, 0);
		boost::shared_ptr<RakNet::RakPeerInterface> peer(RakNet::RakPeerInterface::GetInstance(), &RakNet::RakPeerInterface::DestroyInstance);
		//RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
		peer->Startup(1, &socketDescriptor, 1);

		bool ret = (peer->Connect(host.c_str(), port, NULL, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED);

		if (!ret) {
			return -1;
		}
		++m_lastsock;
		boost::shared_ptr<Client> client = boost::make_shared<Client>();
		client->useTCP = tcp;
		client->sock = m_lastsock;
		client->udpPeer = peer;
		client->systemAddresses = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
		client->connected = false;
		client->closed = false;
		clients[m_lastsock] = client;
		boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&EotuSocketAPI::receiveUDP, this, m_lastsock, callback)));
		threads.push_back(thread);
	} else {
		boost::shared_ptr<RakNet::TCPInterface> peer(RakNet::TCPInterface::GetInstance(), &RakNet::TCPInterface::DestroyInstance);
		if (peer->Start(0, 32) == false) {
			return -1;
		}

		RakNet::SystemAddress server = peer->Connect(host.c_str(), port, false);

		++m_lastsock;
		boost::shared_ptr<Client> client = boost::make_shared<Client>();
		client->useTCP = tcp;
		client->sock = m_lastsock;
		client->tcpPeer = peer;
		client->systemAddresses = server;
		client->connected = false;
		client->closed = false;
		clients[m_lastsock] = client;
		boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&EotuSocketAPI::receiveTCP, this, m_lastsock, callback)));
		threads.push_back(thread);
	}

	return m_lastsock;
}

int EotuSocketAPI::login(const int sock, const FB::variant &data) {
	RakNet::BitStream sendStream;
	sendStream.Write((RakNet::MessageID)ID_USER_PACKET_ENUM);
	sendStream.Write(EOTU_AUTH_MESSAGE);

	Json::Value json = FB::variantToJsonValue(data);
	this->writeString(sendStream, json.toString());

	return this->sendStream(sock, sendStream);
}

bool EotuSocketAPI::send(const int sock, const std::string& data) {
	
	return this->sendString(sock, data);
}

bool EotuSocketAPI::sendMessage(const int sock, const FB::variant &data) {

	RakNet::BitStream sendStream;
	sendStream.Write((RakNet::MessageID)ID_USER_PACKET_ENUM);
	sendStream.Write(EOTU_FORWARD_MESSAGE);
	sendStream.Write(EOTU_FORWARD_TEXT_MESSAGE);
	Json::Value json = FB::variantToJsonValue(data);
	this->writeString(sendStream, json.toString());

	return this->sendStream(sock, sendStream);
}

void EotuSocketAPI::writeString(RakNet::BitStream& stream, const std::string& str)
{
	RakNet::RakString data;
	data.Set("%s", str.c_str());

	stream.Write(data);
}

bool EotuSocketAPI::sendString(const int sock, const std::string& str) {
	if (clients.find(sock) == clients.end()) {
		return false;
	}

	RakNet::RakString request;
	request.Set("%s", str.c_str());

	boost::shared_ptr<Client> client = clients[sock];
	if (client->connected == false) {
		return false;
	}
	if (client->useTCP) {
		client->tcpPeer->Send(request.C_String(), (unsigned int)request.GetLength(), client->systemAddresses, false);
	} else {
		client->udpPeer->Send(request.C_String(), (unsigned int)request.GetLength(), HIGH_PRIORITY, RELIABLE_ORDERED, 0, client->systemAddresses, false);	
	}

	return true;
}

bool EotuSocketAPI::sendStream(const int sock, RakNet::BitStream& stream)
{
	if (clients.find(sock) == clients.end()) {
		return false;
	}

	boost::shared_ptr<Client> client = clients[sock];
	if (client->connected == false) {
		return false;
	}
	if (client->useTCP) {
		client->tcpPeer->Send((const char*)stream.GetData(), stream.GetNumberOfBytesUsed(), client->systemAddresses, false);
	} else {
		client->udpPeer->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, client->systemAddresses, false);	
	}

	return true;
}

void EotuSocketAPI::receiveUDP(const int sock, const FB::JSObjectPtr &callback)
{
	if (clients.find(sock) == clients.end()) {
		return;
	}
	boost::shared_ptr<Client> client = clients[sock];
#ifdef _DEBUG
	fire_Debug("receiveUDP");
#endif
	RakNet::Packet* packet;
	while (!boost::this_thread::interruption_requested())
	{
		for (packet=client->udpPeer->Receive(); packet; client->udpPeer->DeallocatePacket(packet), packet=client->udpPeer->Receive()) {
			unsigned char typeID;
			RakNet::BitStream stream(packet->data, packet->length, false);
			stream.Read(typeID);
			switch (typeID) {
			case ID_ALREADY_CONNECTED:
				{
					fire_StatusChange(ID_ALREADY_CONNECTED, "ID_ALREADY_CONNECTED");
					try {
						callback->Invoke("change", FB::variant_list_of(ID_ALREADY_CONNECTED)("ID_ALREADY_CONNECTED"));
					} catch (const FB::script_error& ex) {
						m_host->htmlLog(std::string("Function call failed with ") + ex.what());
					}
				}
				break;

			case ID_REMOTE_CONNECTION_LOST:
				{
					fire_StatusChange(ID_REMOTE_CONNECTION_LOST, "ID_REMOTE_CONNECTION_LOST");
					try {
						callback->Invoke("change", FB::variant_list_of(ID_REMOTE_CONNECTION_LOST)("ID_REMOTE_CONNECTION_LOST"));
					} catch (const FB::script_error& ex) {
						m_host->htmlLog(std::string("Function call failed with ") + ex.what());
					}
					goto close;
				}
				break;

			case ID_CONNECTION_LOST:
				{
					client->closed = true;
					fire_StatusChange(ID_CONNECTION_LOST, "ID_CONNECTION_LOST");
					try {
						callback->Invoke("change", FB::variant_list_of(ID_CONNECTION_LOST)("ID_CONNECTION_LOST"));
					} catch (const FB::script_error& ex) {
						m_host->htmlLog(std::string("Function call failed with ") + ex.what());
					}
					goto close;
				}
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				{
					client->connected = false;
					fire_StatusChange(ID_CONNECTION_ATTEMPT_FAILED, "ID_CONNECTION_ATTEMPT_FAILED");
					try {
						callback->Invoke("change", FB::variant_list_of(ID_CONNECTION_ATTEMPT_FAILED)("ID_CONNECTION_ATTEMPT_FAILED"));
					} catch (const FB::script_error& ex) {
						m_host->htmlLog(std::string("Function call failed with ") + ex.what());
					}
					goto close;
				}
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					client->systemAddresses = packet->systemAddress;
					client->connected = true;
					fire_StatusChange(ID_CONNECTION_REQUEST_ACCEPTED, "ID_CONNECTION_REQUEST_ACCEPTED");
					fire_Connected();
					try {
						callback->Invoke("change", FB::variant_list_of(ID_CONNECTION_REQUEST_ACCEPTED)("ID_CONNECTION_REQUEST_ACCEPTED"));
						callback->Invoke("connected", FB::variant_list_of());
					} catch (const FB::script_error& ex) {
						m_host->htmlLog(std::string("Function call failed with ") + ex.what());
					}
				}
				break;
			case ID_USER_PACKET_ENUM:
				{
					fire_StatusChange(ID_USER_PACKET_ENUM, "ID_USER_PACKET_ENUM");
					int type;
					stream.Read(type);
					int message_type;
					RakNet::RakString json;
					try {
						if (type == EOTU_AUTH_MESSAGE)  {
							stream.Read(message_type);
							stream.Read(json);
							if (message_type == EOTU_AUTH_SUCCESS) {
								callback->Invoke("authSuccess", FB::variant_list_of(json.C_String()));
							} else {
								callback->Invoke("authFail", FB::variant_list_of(json.C_String()));
							}
							
						} else if (type == EOTU_FORWARD_MESSAGE)  {
							stream.Read(message_type);
							stream.Read(json);
							if (message_type == EOTU_FORWARD_TEXT_MESSAGE) {
								callback->Invoke("textMessage", FB::variant_list_of(json.C_String()));
							} else {
								callback->Invoke("message", FB::variant_list_of(message_type)(json.C_String()));
							}
							
						} else {
							callback->Invoke("change", FB::variant_list_of());
							callback->Invoke("receive", FB::variant_list_of(packet->data));
						}
					} catch (const FB::script_error& ex) {
						m_host->htmlLog(std::string("Function call failed with ") + ex.what());
					}
				}
				break;
			default:
				break;
			}
		}
		RakSleep(30);
	}
close:
	client->udpPeer->Shutdown(500,0);
#ifdef _DEBUG
	fire_Debug("receiveUDP stop");
#endif
}

void EotuSocketAPI::receiveTCP(const int sock, const FB::JSObjectPtr &callback)
{
	if (clients.find(sock) == clients.end()) {
		return;
	}
	boost::shared_ptr<Client> client = clients[sock];
#ifdef _DEBUG
	fire_Debug("receiveTCP");
#endif
	RakNet::Packet* packet;
	while (!boost::this_thread::interruption_requested())
	{
		for (packet=client->tcpPeer->Receive(); packet; client->tcpPeer->DeallocatePacket(packet), packet=client->tcpPeer->Receive()) {
			fire_StatusChange(ID_USER_PACKET_ENUM, FB::variant_list_of("ID_USER_PACKET_ENUM"));
			RakNet::RakString incomingTemp = RakNet::RakString::NonVariadic((const char*) packet->data);
			try {
				callback->Invoke("change", FB::variant_list_of(ID_USER_PACKET_ENUM)("ID_USER_PACKET_ENUM"));
				callback->Invoke("receive", FB::variant_list_of(incomingTemp.C_String()));
			} catch (const FB::script_error& ex) {
				m_host->htmlLog(std::string("Function call failed with ") + ex.what());
			}
		}
		if (client->closed == true) {
			goto close;
		}
		RakNet::SystemAddress notificationAddress;
		if (client->connected == false) {
			RakNet::Time timeout = RakNet::GetTime()+3000;
			while (RakNet::GetTime() < timeout) {
				notificationAddress = client->tcpPeer->HasCompletedConnectionAttempt();
				if (notificationAddress != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
				{
					client->systemAddresses = notificationAddress;
					client->connected = true;
					fire_StatusChange(ID_CONNECTION_REQUEST_ACCEPTED, "ID_CONNECTION_REQUEST_ACCEPTED");
					fire_Connected();
					try {
						callback->Invoke("change", FB::variant_list_of(ID_CONNECTION_REQUEST_ACCEPTED)("ID_CONNECTION_REQUEST_ACCEPTED"));
						callback->Invoke("connected", FB::variant_list_of());
					} catch (const FB::script_error& ex) {
						m_host->htmlLog(std::string("Function call failed with ") + ex.what());
					}
					break;
				}
				RakSleep(30);
			}

			if (notificationAddress == RakNet::UNASSIGNED_SYSTEM_ADDRESS) {
				client->connected = false;
				fire_StatusChange(ID_CONNECTION_ATTEMPT_FAILED, "ID_CONNECTION_ATTEMPT_FAILED");
				try {
					callback->Invoke("change", FB::variant_list_of(ID_CONNECTION_ATTEMPT_FAILED)("ID_CONNECTION_ATTEMPT_FAILED"));
				} catch (const FB::script_error& ex) {
					m_host->htmlLog(std::string("Function call failed with ") + ex.what());
				}
				goto close;
				break;
			}
		}

		notificationAddress = client->tcpPeer->HasFailedConnectionAttempt();
		if (notificationAddress!=RakNet::UNASSIGNED_SYSTEM_ADDRESS) {
			fire_StatusChange(ID_CONNECTION_ATTEMPT_FAILED, "ID_CONNECTION_ATTEMPT_FAILED");
			try {
				callback->Invoke("change", FB::variant_list_of(ID_CONNECTION_ATTEMPT_FAILED)("ID_CONNECTION_ATTEMPT_FAILED"));
			} catch (const FB::script_error& ex) {
				m_host->htmlLog(std::string("Function call failed with ") + ex.what());
			}
			goto close;
			break;
		}
		notificationAddress = client->tcpPeer->HasLostConnection();
		if (notificationAddress!=RakNet::UNASSIGNED_SYSTEM_ADDRESS) {
			client->closed = true;
			fire_StatusChange(ID_CONNECTION_LOST, "ID_CONNECTION_LOST");
			try {
				callback->Invoke("change", FB::variant_list_of(ID_CONNECTION_LOST)("ID_CONNECTION_LOST"));
			} catch (const FB::script_error& ex) {
				m_host->htmlLog(std::string("Function call failed with ") + ex.what());
			}
		}
		RakSleep(30);
	}
close:
	client->tcpPeer->Stop();
#ifdef _DEBUG
	fire_Debug("receiveTCP stop");
#endif
}