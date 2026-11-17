#include "StdAfx.h"
#include "CTradeQueryUI.h"

using namespace DuiLib;

IMPLEMENT_DUICONTROL(CTradeQueryUI)

CTradeQueryUI::CTradeQueryUI()
{
    SetFixedWidth(500);
    SetFixedHeight(300);
    SetBkColor(0xFFF5F5F5);
    SetAttribute(_T("border"), _T("1"));
    SetAttribute(_T("bordercolor"), _T("#FFCCCCCC"));
    SetAttribute(_T("inset"), _T("4,4,4,4"));
    BuildUI();
}

CListUI* CTradeQueryUI::CreateQueryList(LPCTSTR pszName)
{
    CListUI* pList = new CListUI;
    pList->SetName(pszName);
    pList->SetAttribute(_T("bkcolor"), _T("#FFFFFFFF"));
    pList->SetAttribute(_T("border"), _T("1"));
    pList->SetAttribute(_T("bordercolor"), _T("#FFCCCCCC"));
    pList->SetAttribute(_T("vscrollbar"), _T("true"));

    CListHeaderUI* pHeader = new CListHeaderUI;
    LPCTSTR headers[] = { _T("合约"), _T("方向"), _T("价格"), _T("数量"), _T("状态") };
    int widths[] = { 80, 50, 80, 60, 80 };
    for (int i = 0; i < 5; ++i)
    {
        CListHeaderItemUI* pHeaderItem = new CListHeaderItemUI;
        pHeaderItem->SetText(headers[i]);
        pHeaderItem->SetFixedWidth(widths[i]);
        pHeader->Add(pHeaderItem);
    }
    pList->Add(pHeader);

    for (int row = 0; row < 3; ++row)
    {
        CListTextElementUI* pRow = new CListTextElementUI;
        pRow->SetFixedHeight(22);
        for (int col = 0; col < 5; ++col)
        {
            TCHAR buf[32] = {0};
            if (col == 0) _tcscpy(buf, _T("IF2406"));
            else if (col == 1) _tcscpy(buf, _T("买"));
            else if (col == 2) _stprintf(buf, _T("%d.0"), 3850 - row);
            else if (col == 3) _stprintf(buf, _T("%d"), row + 1);
            else _tcscpy(buf, _T("已成"));
            pRow->SetText(col, buf);
        }
        pList->Add(pRow);
    }
    return pList;
}

void CTradeQueryUI::BuildUI()
{
    CVerticalLayoutUI* pMain = new CVerticalLayoutUI;
    pMain->SetChildPadding(4);

    CLabelUI* pTitle = new CLabelUI;
    pTitle->SetText(_T("交易查询"));
    pTitle->SetFixedHeight(24);
    pTitle->SetAttribute(_T("align"), _T("center"));
    pTitle->SetAttribute(_T("font"), _T("1"));
    pMain->Add(pTitle);

    CHorizontalLayoutUI* pTabBar = new CHorizontalLayoutUI;
    pTabBar->SetFixedHeight(26);
    pTabBar->SetChildPadding(2);

    LPCTSTR tabNames[] = { _T("持仓查询"), _T("当日委托"), _T("当日成交"), _T("可撤委托") };
    for (int i = 0; i < 4; ++i)
    {
        COptionUI* pTab = new COptionUI;
        TCHAR szName[32] = {0};
        _stprintf(szName, _T("query_tab_%d"), i);
        pTab->SetName(szName);
        pTab->SetText(tabNames[i]);
        pTab->SetFixedWidth(80);
        pTab->SetFixedHeight(24);
        pTab->SetAttribute(_T("group"), _T("query_tabs"));
        pTab->SetAttribute(_T("bkcolor"), _T("#FFE0E0E0"));
        pTab->SetAttribute(_T("selectedbkcolor"), _T("#FF0078D7"));
        pTab->SetAttribute(_T("textcolor"), _T("#FF333333"));
        pTab->SetAttribute(_T("selectedtextcolor"), _T("#FFFFFFFF"));
        if (i == 0) pTab->Selected(true);
        pTabBar->Add(pTab);
    }
    pMain->Add(pTabBar);

    CTabLayoutUI* pTabLayout = new CTabLayoutUI;
    pTabLayout->SetName(_T("query_tab_layout"));

    for (int i = 0; i < 4; ++i)
    {
        TCHAR szName[32] = {0};
        _stprintf(szName, _T("query_list_%d"), i);
        CListUI* pList = CreateQueryList(szName);
        pTabLayout->Add(pList);
    }
    pMain->Add(pTabLayout);

    this->Add(pMain);
}

LPCTSTR CTradeQueryUI::GetClass() const
{
    return _T("CTradeQueryUI");
}

LPCTSTR CTradeQueryUI::GetDescription() const
{
    return _T("交易查询组件 - 持仓/委托/成交/可撤查询");
}

LPVOID CTradeQueryUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, _T("CTradeQueryUI")) == 0) return static_cast<CTradeQueryUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}