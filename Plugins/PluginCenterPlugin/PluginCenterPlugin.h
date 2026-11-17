#ifndef __PLUGINCENTERPLUGIN_H__
#define __PLUGINCENTERPLUGIN_H__

#include "StdAfx.h"
#include "../../DuiLib/Core/Plugin/IPlugin.h"

class PluginCenterPlugin : public DuiLib::IPlugin
{
public:
    bool OnLoad(DuiLib::CPaintManagerUI* pm);
    void OnUnload();
    DuiLib::PluginInfo GetInfo() const;
};

#endif