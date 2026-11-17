#include "StdAfx.h"
#include "PluginCenterPlugin.h"
#include "CSimpleChartUI.h"
#include "CNotePadUI.h"

using namespace DuiLib;

bool PluginCenterPlugin::OnLoad(CPaintManagerUI* pm)
{
    REGIST_DUICONTROL(CSimpleChartUI);
    REGIST_DUICONTROL(CNotePadUI);

    CPluginManager& mgr = CPluginManager::GetInstance();
    mgr.RegisterDescription(_T("CSimpleChartUI"), _T("简单图表组件 - 柱状图展示，支持动态数据更新"));
    mgr.RegisterDescription(_T("CNotePadUI"), _T("记事本组件 - 简易文本编辑器，支持多行输入和保存"));
    return true;
}

void PluginCenterPlugin::OnUnload()
{
}

PluginInfo PluginCenterPlugin::GetInfo() const
{
    PluginInfo info;
    info.sName = _T("PluginCenterPlugin");
    info.sVersion = _T("1.0.0");
    info.sAuthor = _T("DuiLib");
    info.sDescription = _T("插件中心示例插件: 简单图表/记事本");
    info.sComponents = _T("CSimpleChartUI|CNotePadUI");
    return info;
}

PLUGIN_EXPORT IPlugin* CreatePlugin()
{
    return new PluginCenterPlugin();
}

PLUGIN_EXPORT void DestroyPlugin(IPlugin* p)
{
    delete p;
}