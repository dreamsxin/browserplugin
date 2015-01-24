#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for EotuSocket
#
#\**********************************************************/

set(PLUGIN_NAME "EotuSocket")
set(PLUGIN_PREFIX "EOTU")
set(COMPANY_NAME "GuoSheng")

# ActiveX constants:
set(FBTYPELIB_NAME EotuSocketLib)
set(FBTYPELIB_DESC "EotuSocket 1.0 Type Library")
set(IFBControl_DESC "EotuSocket Control Interface")
set(FBControl_DESC "EotuSocket Control Class")
set(IFBComJavascriptObject_DESC "EotuSocket IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "EotuSocket ComJavascriptObject Class")
set(IFBComEventSource_DESC "EotuSocket IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID 6d435208-2873-5b21-a3da-7a011c536d66)
set(IFBControl_GUID d4ec9c57-7c8e-56b0-8259-044bc084aeff)
set(FBControl_GUID fc8f0121-8871-5e3a-b1e2-5233a5135a0c)
set(IFBComJavascriptObject_GUID 0eb2922d-bc80-5e1b-a229-cc5e1e65ca19)
set(FBComJavascriptObject_GUID 8c6888aa-1a22-563e-9a7e-a4aa4a1d1c95)
set(IFBComEventSource_GUID 8dac1c6f-14b9-5215-8820-e1b8b234ca6d)
if ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID c9dd5b52-2f3c-56ea-adeb-97fe4b32f29f)
else ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 33f38596-cc7d-5656-bb42-5382f7e62f40)
endif ( FB_PLATFORM_ARCH_32 )

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "GuoSheng.EotuSocket")
if ( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "eotu.com/EotuSocket")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "eotu.com/EotuSocket_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )

# strings
set(FBSTRING_CompanyName "GuoSheng")
set(FBSTRING_PluginDescription "Eotu Socket Plugin")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2015 GuoSheng")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}")
set(FBSTRING_ProductName "EotuSocket")
set(FBSTRING_FileExtents "")
if ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "EotuSocket")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "EotuSocket_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )
set(FBSTRING_MIMEType "application/x-eotusocket")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 0)
set(FBMAC_USE_COCOA 0)
set(FBMAC_USE_COREGRAPHICS 0)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)

add_firebreath_library(jsoncpp)
