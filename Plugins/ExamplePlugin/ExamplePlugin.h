#ifndef __EXAMPLEPLUGIN_H__
#define __EXAMPLEPLUGIN_H__

#include "StdAfx.h"
#include "../../DuiLib/Core/Plugin/IPlugin.h"

class ExamplePlugin : public DuiLib::IPlugin
{
public:
    bool OnLoad(DuiLib::CPaintManagerUI* pm);
    void OnUnload();
    DuiLib::PluginInfo GetInfo() const;
};

#endif // __EXAMPLEPLUGIN_H__