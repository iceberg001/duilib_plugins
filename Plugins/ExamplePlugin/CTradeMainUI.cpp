#include "StdAfx.h"
#include "CTradeMainUI.h"
#include "CTradeOrderUI.h"
#include "CMarketPriceUI.h"
#include "CTradeQueryUI.h"

using namespace DuiLib;

IMPLEMENT_DUICONTROL(CTradeMainUI)

CTradeMainUI::CTradeMainUI()
{
    SetBkColor(0xFF0A0A0F);
    SetAttribute(_T("inset"), _T("2,2,2,2"));
    BuildUI();
}

CButtonUI* CTradeMainUI::CreateNavButton(LPCTSTR pszText, int w)
{
    CButtonUI* pBtn = new CButtonUI;
    pBtn->SetText(pszText);
    pBtn->SetFixedWidth(w);
    pBtn->SetFixedHeight(28);
    pBtn->SetAttribute(_T("bkcolor"), _T("#FF1F1F2E"));
    pBtn->SetAttribute(_T("textcolor"), _T("#FFC0C0C8"));
    pBtn->SetAttribute(_T("border"), _T("1"));
    pBtn->SetAttribute(_T("bordercolor"), _T("#FF2A2A35"));
    return pBtn;
}

CLabelUI* CTradeMainUI::CreateStatusLabel(LPCTSTR pszText, DWORD dwColor)
{
    CLabelUI* pLbl = new CLabelUI;
    pLbl->SetText(pszText);
    pLbl->SetFixedHeight(22);
    pLbl->SetAttribute(_T("textcolor"), _T("#FFC0C0C8"));
    pLbl->SetAttribute(_T("align"), _T("center"));
    return pLbl;
}

CHorizontalLayoutUI* CTradeMainUI::CreateTopNav()
{
    CHorizontalLayoutUI* pNav = new CHorizontalLayoutUI;
    pNav->SetFixedHeight(32);
    pNav->SetAttribute(_T("bkcolor"), _T("#FF1F1F2E"));
    pNav->SetAttribute(_T("childpadding"), _T("2"));
    pNav->SetAttribute(_T("inset"), _T("4,2,4,2"));

    CLabelUI* pLogo = new CLabelUI;
    pLogo->SetText(_T("  华龙点金智慧版  "));
    pLogo->SetFixedWidth(140);
    pLogo->SetFixedHeight(28);
    pLogo->SetAttribute(_T("textcolor"), _T("#FFFFFFFF"));
    pLogo->SetAttribute(_T("font"), _T("1"));
    pNav->Add(pLogo);

    LPCTSTR menus[] = { _T("首页"), _T("行情"), _T("分析"), _T("功能"),
        _T("资讯"), _T("交易"), _T("理财"), _T("投顾"), _T("帮助") };
    for (int i = 0; i < 9; ++i)
    {
        CButtonUI* pBtn = CreateNavButton(menus[i], 50);
        if (i == 5) pBtn->SetAttribute(_T("bkcolor"), _T("#FF0078D7"));
        pNav->Add(pBtn);
    }

    CContainerUI* pSpacer = new CContainerUI;
    pNav->Add(pSpacer);

    CLabelUI* pUser = new CLabelUI;
    pUser->SetText(_T("  1015****0010 游客  "));
    pUser->SetFixedWidth(160);
    pUser->SetFixedHeight(28);
    pUser->SetAttribute(_T("textcolor"), _T("#FF808088"));
    pUser->SetAttribute(_T("align"), _T("right"));
    pNav->Add(pUser);

    return pNav;
}

CHorizontalLayoutUI* CTradeMainUI::CreateMainContent()
{
    CHorizontalLayoutUI* pMain = new CHorizontalLayoutUI;
    pMain->SetAttribute(_T("bkcolor"), _T("#FF0D0D12"));
    pMain->SetAttribute(_T("childpadding"), _T("2"));
    pMain->SetAttribute(_T("inset"), _T("2,2,2,2"));

    CTradeOrderUI* pOrder = new CTradeOrderUI;
    pOrder->SetFixedWidth(280);
    pOrder->SetFixedHeight(0);
    pOrder->SetAttribute(_T("bkcolor"), _T("#FF1A1A24"));
    pOrder->SetAttribute(_T("bordercolor"), _T("#FF2A2A35"));
    pMain->Add(pOrder);

    CMarketPriceUI* pPrice = new CMarketPriceUI;
    pPrice->SetFixedWidth(300);
    pPrice->SetFixedHeight(0);
    pPrice->SetAttribute(_T("bkcolor"), _T("#FF1A1A24"));
    pPrice->SetAttribute(_T("bordercolor"), _T("#FF2A2A35"));
    pMain->Add(pPrice);

    CVerticalLayoutUI* pChart = new CVerticalLayoutUI;
    pChart->SetAttribute(_T("bkcolor"), _T("#FF1A1A24"));
    pChart->SetAttribute(_T("border"), _T("1"));
    pChart->SetAttribute(_T("bordercolor"), _T("#FF2A2A35"));
    pChart->SetAttribute(_T("inset"), _T("4,4,4,4"));

    CHorizontalLayoutUI* pChartTabs = new CHorizontalLayoutUI;
    pChartTabs->SetFixedHeight(26);
    pChartTabs->SetAttribute(_T("childpadding"), _T("2"));
    LPCTSTR tabs[] = { _T("分时"), _T("多日"), _T("日线"), _T("周线") };
    for (int i = 0; i < 4; ++i)
    {
        COptionUI* pOpt = new COptionUI;
        pOpt->SetText(tabs[i]);
        pOpt->SetFixedWidth(50);
        pOpt->SetFixedHeight(24);
        pOpt->SetAttribute(_T("bkcolor"), _T("#FF2A2A35"));
        pOpt->SetAttribute(_T("selectedbkcolor"), _T("#FF0078D7"));
        pOpt->SetAttribute(_T("textcolor"), _T("#FFC0C0C8"));
        pOpt->SetAttribute(_T("selectedtextcolor"), _T("#FFFFFFFF"));
        pOpt->SetAttribute(_T("group"), _T("charttab"));
        if (i == 0) pOpt->Selected(true);
        pChartTabs->Add(pOpt);
    }
    pChart->Add(pChartTabs);

    CContainerUI* pChartArea = new CContainerUI;
    pChartArea->SetAttribute(_T("bkcolor"), _T("#FF111118"));
    CLabelUI* pChartLbl = new CLabelUI;
    pChartLbl->SetText(_T("分时走势图\n\n  平安银行 000001\n  均价:11.40  涨跌幅:+1.15%"));
    pChartLbl->SetAttribute(_T("textcolor"), _T("#FF808088"));
    pChartLbl->SetAttribute(_T("align"), _T("center"));
    pChartArea->Add(pChartLbl);
    pChart->Add(pChartArea);
    pMain->Add(pChart);

    return pMain;
}

CHorizontalLayoutUI* CTradeMainUI::CreateBottomArea()
{
    CHorizontalLayoutUI* pBottom = new CHorizontalLayoutUI;
    pBottom->SetFixedHeight(180);
    pBottom->SetAttribute(_T("bkcolor"), _T("#FF0D0D12"));
    pBottom->SetAttribute(_T("childpadding"), _T("2"));
    pBottom->SetAttribute(_T("inset"), _T("2,2,2,2"));

    CTradeQueryUI* pQuery = new CTradeQueryUI;
    pQuery->SetFixedWidth(0);
    pQuery->SetFixedHeight(0);
    pQuery->SetAttribute(_T("bkcolor"), _T("#FF1A1A24"));
    pQuery->SetAttribute(_T("bordercolor"), _T("#FF2A2A35"));
    pBottom->Add(pQuery);

    CVerticalLayoutUI* pOrderRec = new CVerticalLayoutUI;
    pOrderRec->SetFixedWidth(380);
    pOrderRec->SetAttribute(_T("bkcolor"), _T("#FF1A1A24"));
    pOrderRec->SetAttribute(_T("border"), _T("1"));
    pOrderRec->SetAttribute(_T("bordercolor"), _T("#FF2A2A35"));
    pOrderRec->SetAttribute(_T("inset"), _T("4,4,4,4"));

    CLabelUI* pTitle = new CLabelUI;
    pTitle->SetText(_T("当日委托"));
    pTitle->SetFixedHeight(22);
    pTitle->SetAttribute(_T("textcolor"), _T("#FFC0C0C8"));
    pOrderRec->Add(pTitle);

    CListUI* pList = new CListUI;
    pList->SetAttribute(_T("bkcolor"), _T("#FF111118"));
    pList->SetAttribute(_T("border"), _T("1"));
    pList->SetAttribute(_T("bordercolor"), _T("#FF2A2A35"));
    CListHeaderUI* pHeader = new CListHeaderUI;
    CDuiString headers[] = { _T("时间"), _T("代码"), _T("名称"), _T("价格"), _T("数量"), _T("状态") };
    int widths[] = { 60, 60, 60, 50, 50, 60 };
    for (int i = 0; i < 6; ++i)
    {
        CListHeaderItemUI* pHdr = new CListHeaderItemUI;
        pHdr->SetText(headers[i]);
        pHdr->SetFixedWidth(widths[i]);
        pHdr->SetAttribute(_T("textcolor"), _T("#FF808088"));
        pHdr->SetAttribute(_T("bkcolor"), _T("#FF1E1E28"));
        pHeader->Add(pHdr);
    }
    pList->Add(pHeader);

    LPCTSTR records[][6] = {
        { _T("10:20:42"), _T("000001"), _T("平安银行"), _T("11.44"), _T("100"), _T("已报") },
        { _T("10:18:15"), _T("000078"), _T("泛海建设"), _T("3.21"), _T("200"), _T("已成") },
        { _T("10:15:03"), _T("600036"), _T("招商银行"), _T("35.60"), _T("500"), _T("已报") },
        { _T("10:10:28"), _T("000002"), _T("万科A"), _T("10.85"), _T("300"), _T("已撤") },
    };
    for (int r = 0; r < 4; ++r)
    {
        CListLabelElementUI* pItem = new CListLabelElementUI;
        CDuiString sText;
        sText.Format(_T("%s  %s  %s  %s  %s  %s"),
            records[r][0], records[r][1], records[r][2],
            records[r][3], records[r][4], records[r][5]);
        pItem->SetText(sText);
        pItem->SetFixedHeight(24);
        pItem->SetAttribute(_T("textcolor"), _T("#FFC0C0C8"));
        pList->Add(pItem);
    }
    pOrderRec->Add(pList);
    pBottom->Add(pOrderRec);

    return pBottom;
}

CHorizontalLayoutUI* CTradeMainUI::CreateStatusBar()
{
    CHorizontalLayoutUI* pStatus = new CHorizontalLayoutUI;
    pStatus->SetFixedHeight(24);
    pStatus->SetAttribute(_T("bkcolor"), _T("#FF1F1F2E"));
    pStatus->SetAttribute(_T("childpadding"), _T("8"));
    pStatus->SetAttribute(_T("inset"), _T("8,2,8,2"));

    struct IndexInfo { LPCTSTR name; LPCTSTR value; LPCTSTR change; DWORD color; };
    static const IndexInfo indices[] = {
        { _T("上证"), _T("3653.29"), _T("-0.77%"), 0xFF00CC66 },
        { _T("深成"), _T("11188.88"), _T("-2.48%"), 0xFF00CC66 },
        { _T("北证50"), _T("914.65"), _T("-1.46%"), 0xFF00CC66 },
        { _T("创业板"), _T("2537.84"), _T("-3.68%"), 0xFF00CC66 },
        { _T("科创50"), _T("1229.61"), _T("-4.16%"), 0xFF00CC66 },
    };
    for (int i = 0; i < 5; ++i)
    {
        CDuiString sText;
        sText.Format(_T("%s:%s %s"), indices[i].name, indices[i].value, indices[i].change);
        CLabelUI* pLbl = new CLabelUI;
        pLbl->SetText(sText);
        pLbl->SetFixedHeight(20);
        pLbl->SetAttribute(_T("textcolor"), _T("#FFC0C0C8"));
        pStatus->Add(pLbl);
    }

    return pStatus;
}

void CTradeMainUI::BuildUI()
{
    CVerticalLayoutUI* pRoot = new CVerticalLayoutUI;
    pRoot->SetAttribute(_T("childpadding"), _T("2"));
    pRoot->SetAttribute(_T("inset"), _T("1,1,1,1"));

    pRoot->Add(CreateTopNav());

    CHorizontalLayoutUI* pToolbar = new CHorizontalLayoutUI;
    pToolbar->SetFixedHeight(30);
    pToolbar->SetAttribute(_T("bkcolor"), _T("#FF1F1F2E"));
    pToolbar->SetAttribute(_T("childpadding"), _T("2"));
    pToolbar->SetAttribute(_T("inset"), _T("4,2,4,2"));
    LPCTSTR tools[] = { _T("自选"), _T("买入"), _T("卖出"), _T("期权"), _T("理财"),
        _T("选股"), _T("盯盘"), _T("行情"), _T("板块"), _T("资金"), _T("F10") };
    for (int i = 0; i < 11; ++i)
    {
        CButtonUI* pBtn = CreateNavButton(tools[i], 48);
        pToolbar->Add(pBtn);
    }
    CContainerUI* pSpacer = new CContainerUI;
    pToolbar->Add(pSpacer);
    pRoot->Add(pToolbar);

    pRoot->Add(CreateMainContent());
    pRoot->Add(CreateBottomArea());
    pRoot->Add(CreateStatusBar());

    this->Add(pRoot);
}

LPCTSTR CTradeMainUI::GetClass() const
{
    return _T("CTradeMainUI");
}

LPCTSTR CTradeMainUI::GetDescription() const
{
    return _T("交易主页面 - 组合下单/行情/查询的完整交易终端界面");
}

LPVOID CTradeMainUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, _T("CTradeMainUI")) == 0) return static_cast<CTradeMainUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}