
/**********************************************************\

  Auto-generated config.h

\**********************************************************/

#define FBSTRING_CompanyName "FireBreath"
#define FBSTRING_PLUGIN_VERSION "1.0.0"
#define FBSTRING_LegalCopyright "Copyright 2009 Firebreath development team"
#define FBSTRING_PluginFileName "npBasicMediaPlayer"
#define FBSTRING_ProductName "FireBreath BasicMediaPlayer Plugin"
#define FBSTRING_PluginName "FireBreath BasicMediaPlayer Plugin"

// Format is Major.Minor.Patch.Build

#define FBSTRING_VERSION_MAJOR 1
#define FBSTRING_VERSION_MINOR 0
#define FBSTRING_VERSION_PATCH 0
#define FBSTRING_VERSION_BUILD 0

#define FBSTRiNG_MIMETypeCombined

#define FBSTRING_MIMETypeList "application/x-vnd.FBBasicMediaPlayer"

#define FBSTRING_MIMEType0 "application/x-vnd.FBBasicMediaPlayer"
#define FBSTRING_X11MIMEType0 "application/x-vnd.FBBasicMediaPlayer::BasicMediaPlayer Plugin for the FireBreath Project"
#define FBSTRING_PluginDescription0 "BasicMediaPlayer Plugin for the FireBreath Project"
#define FBSTRING_FileDescription0 "BasicMediaPlayer Plugin for the FireBreath Project"
#define FBSTRING_FileExtents0 ""
#define FBSTRING_PluginDescription FBSTRING_PluginDescription0
#define FBSTRING_FileDescription FBSTRING_FileDescription0
#define FBSTRING_FileExtents FBSTRING_FileExtents0
#define FBSTRING_X11MIMEType \
                            "application/x-vnd.FBBasicMediaPlayer::BasicMediaPlayer Plugin for the FireBreath Project;" \
                            ""
#define FBSTRING_MIMEType   FBSTRING_MIMETypeList
#define FB_GUI_DISABLED         0
/* #undef FB_ATLREG_MACHINEWIDE */
#if FB_GUI_DISABLED == 1
#define FBMAC_USE_CARBON                    0
#define FBMAC_USE_COCOA                     1
#define FBMAC_USE_QUICKDRAW                 0
#define FBMAC_USE_COREGRAPHICS              1
#define FBMAC_USE_COREANIMATION             0
#define FBMAC_USE_INVALIDATINGCOREANIMATION 0
#else
#define FBMAC_USE_CARBON                    1
#define FBMAC_USE_COCOA                     1
#define FBMAC_USE_QUICKDRAW                 1
#define FBMAC_USE_COREGRAPHICS              1
#define FBMAC_USE_COREANIMATION             1
#define FBMAC_USE_INVALIDATINGCOREANIMATION 1
#endif
#define FB_
