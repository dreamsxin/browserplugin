/**********************************************************\

  Auto-generated EotuSocketAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include <boost/thread/thread.hpp>
#include "Source/MessageIdentifiers.h"
#include "Source/RakPeerInterface.h"
#include "Source/RakNetTypes.h"
#include "Source/BitStream.h"
#include "Source/TCPInterface.h"
#include "Source/RakSleep.h"
#include "Source/RakString.h"
#include "Source/Gets.h"
#include "Source/Getche.h"
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
		registerMethod("receive",      make_method(this, &EotuSocketAPI::receive));


		RakNet::SocketDescriptor socketDescriptor(0, 0);
		client = RakNet::RakPeerInterface::GetInstance();
		client->Startup(1, &socketDescriptor, 1);
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn EotuSocketAPI::~EotuSocketAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EotuSocketAPI() {
		RakNet::RakPeerInterface::DestroyInstance(client);
	};

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
	bool connect(const std::string& host, const int port, const FB::JSObjectPtr &callback);
	// Method receive
	void receive(const FB::JSObjectPtr &callback);

	// Event
	FB_JSAPI_EVENT(Connected, 0, ());
	FB_JSAPI_EVENT(StatusChange, 2, (const int, const FB::variant&));

private:
    EotuSocketWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

	std::string m_testString;

	boost::thread m_thread;
	RakNet::RakPeerInterface *client;
};

#endif // H_EotuSocketAPI

