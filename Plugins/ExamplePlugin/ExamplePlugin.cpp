#include "StdAfx.h"
#include "ExamplePlugin.h"
#include "CMyClockUI.h"
#include "CMyLedUI.h"
#include "CTradeOrderUI.h"
#include "CMarketPriceUI.h"
#include "CTradeQueryUI.h"
#include "CTradeMainUI.h"
#include "CNodeAddUI.h"
#include "CNodeResultUI.h"

using namespace DuiLib;

bool ExamplePlugin::OnLoad(CPaintManagerUI* pm)
{
    REGIST_DUICONTROL(CMyClockUI);
    REGIST_DUICONTROL(CMyLedUI);
    REGIST_DUICONTROL(CTradeOrderUI);
    REGIST_DUICONTROL(CMarketPriceUI);
    REGIST_DUICONTROL(CTradeQueryUI);
    REGIST_DUICONTROL(CTradeMainUI);
    REGIST_DUICONTROL(CNodeAddUI);
    REGIST_DUICONTROL(CNodeResultUI);

    CPluginManager& mgr = CPluginManager::GetInstance();
    mgr.RegisterDescription(_T("CMyClockUI"),     _T("时钟组件 - 实时显示当前时间"));
    mgr.RegisterDescription(_T("CMyLedUI"),       _T("LED组件 - LED文本显示"));
    mgr.RegisterDescription(_T("CTradeOrderUI"),  _T("交易下单面板 - 合约选择/价格数量/买卖方向"));
    mgr.RegisterDescription(_T("CMarketPriceUI"), _T("行情价格组件 - 五档买卖价格显示"));
    mgr.RegisterDescription(_T("CTradeQueryUI"),  _T("交易查询组件 - 持仓/委托/成交/可撤查询"));
    mgr.RegisterDescription(_T("CTradeMainUI"),   _T("交易主页面 - 组合下单/行情/查询的完整交易终端界面"));
    mgr.RegisterDescription(_T("CNodeAddUI"),     _T("加法节点组件 - 输入两个数并发送结果"));
    mgr.RegisterDescription(_T("CNodeResultUI"),  _T("结果节点组件 - 接收并显示加法结果"));
    return true;
}

void ExamplePlugin::OnUnload()
{
}

PluginInfo ExamplePlugin::GetInfo() const
{
    PluginInfo info;
    info.sName = _T("ExamplePlugin");
    info.sVersion = _T("1.0.0");
    info.sAuthor = _T("DuiLib");
    info.sDescription = _T("示例插件: 时钟/LED/交易/加法节点/结果节点");
    info.sComponents = _T("CMyClockUI|CMyLedUI|CTradeOrderUI|CMarketPriceUI|CTradeQueryUI|CTradeMainUI|CNodeAddUI|CNodeResultUI");
    return info;
}

PLUGIN_EXPORT IPlugin* CreatePlugin()
{
    return new ExamplePlugin();
}

PLUGIN_EXPORT void DestroyPlugin(IPlugin* p)
{
    delete p;
}