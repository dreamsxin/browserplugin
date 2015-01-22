/**********************************************************\

  Auto-generated EotuSocketAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/thread/thread.hpp>
#include "Source/MessageIdentifiers.h"
#include "Source/RakPeerInterface.h"
#include "Source/PacketizedTCP.h"
#include "Source/RakNetTypes.h"
#include "Source/BitStream.h"
#include "Source/TCPInterface.h"
#include "Source/RakSleep.h"
#include "Source/RakString.h"
#include "Source/Gets.h"
#include "Source/Getche.h"
#include "Source/GetTime.h"
#include "Source/DS_OrderedList.h"
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "EotuSocket.h"

#ifndef H_EotuSocketAPI
#define H_EotuSocketAPI

class EotuSocketAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn EotuSocketAPI::EotuSocketAPI(const EotuSocketPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    EotuSocketAPI(const EotuSocketPtr& plugin, const FB::BrowserHostPtr& host) :
        m_plugin(plugin), m_host(host)
    {
        registerMethod("echo",      make_method(this, &EotuSocketAPI::echo));
        registerMethod("testEvent", make_method(this, &EotuSocketAPI::testEvent));
        
        // Read-write property
        registerProperty("testString",
                         make_property(this,
                                       &EotuSocketAPI::get_testString,
                                       &EotuSocketAPI::set_testString));
        
        // Read-only property
        registerProperty("version",
                         make_property(this,
                                       &EotuSocketAPI::get_version));


		registerMethod("connect",      make_method(this, &EotuSocketAPI::connect));
		registerMethod("login",      make_method(this, &EotuSocketAPI::login));
		registerMethod("send",      make_method(this, &EotuSocketAPI::send));
		registerMethod("sendMessage",      make_method(this, &EotuSocketAPI::sendMessage));
		registerMethod("close",      make_method(this, &EotuSocketAPI::close));

		m_lastsock = 0;
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn EotuSocketAPI::~EotuSocketAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EotuSocketAPI() {
		close();
	};

	virtual void shutdown() {
		close();
	}

    EotuSocketPtr getPlugin();

    // Read/Write property ${PROPERTY.ident}
    std::string get_testString();
    void set_testString(const std::string& val);

    // Read-only property ${PROPERTY.ident}
    std::string get_version();

    // Method echo
    FB::variant echo(const FB::variant& msg);
    
    // Event helpers
    FB_JSAPI_EVENT(test, 0, ());
    FB_JSAPI_EVENT(echo, 2, (const FB::variant&, const int));

    // Method test-event
	void testEvent();

	// Method connect
	int connect(const std::string& host, const int port, const FB::JSObjectPtr& callback);
	// Method login
	int login(const int sock, const std::string& username, const std::string& password);
	// Method send
	bool send(const int sock, const std::string& data);
	// Method sendMessage
	bool sendMessage(const int sock, const int type, const std::string& toUser, const std::string& message);
	// Method close
	void close() {
		for (std::size_t i = 0; i < threads.size(); ++i) {
			threads[i]->interrupt();
			threads[i]->join();
		}
	}

	void writeString(RakNet::BitStream& stream, const std::string& str);
	bool sendString(const int sock, const std::string& str);
	bool sendStream(const int sock, RakNet::BitStream& stream);
	void receiveUDP(const int sock, const FB::JSObjectPtr &callback);
	void receiveTCP(const int sock, const FB::JSObjectPtr &callback);

	// Event
	FB_JSAPI_EVENT(Connected, 0, ());
	FB_JSAPI_EVENT(StatusChange, 2, (const int, const FB::variant&));
	FB_JSAPI_EVENT(Debug, 1, (const FB::variant&));

private:
    EotuSocketWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

	std::string m_testString;

	int m_lastsock;

	std::vector<boost::shared_ptr<boost::thread>> threads;

	struct Client
	{
		int sock;
		bool useTCP;
		boost::shared_ptr<RakNet::TCPInterface> tcpPeer;
		boost::shared_ptr<RakNet::RakPeerInterface> udpPeer;
		RakNet::SystemAddress systemAddresses;
		bool connected;
		bool closed;
	};

	std::map<int, boost::shared_ptr<Client>> clients;
};

#endif // H_EotuSocketAPI

