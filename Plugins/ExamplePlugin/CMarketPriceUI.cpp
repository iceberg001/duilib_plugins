#include "StdAfx.h"
#include "CMarketPriceUI.h"

using namespace DuiLib;

IMPLEMENT_DUICONTROL(CMarketPriceUI)

CMarketPriceUI::CMarketPriceUI()
{
    SetFixedWidth(250);
    SetFixedHeight(300);
    SetBkColor(0xFFF5F5F5);
    SetAttribute(_T("border"), _T("1"));
    SetAttribute(_T("bordercolor"), _T("#FFCCCCCC"));
    SetAttribute(_T("inset"), _T("4,4,4,4"));
    BuildUI();
}

void CMarketPriceUI::AddPriceRow(CVerticalLayoutUI* pParent, LPCTSTR pszLabel, LPCTSTR pszPrice, LPCTSTR pszVolume, DWORD dwColor)
{
    CHorizontalLayoutUI* pRow = new CHorizontalLayoutUI;
    pRow->SetFixedHeight(20);
    pRow->SetChildPadding(2);

    CLabelUI* pLbl = new CLabelUI;
    pLbl->SetText(pszLabel);
    pLbl->SetFixedWidth(30);
    pLbl->SetAttribute(_T("align"), _T("center"));
    pLbl->SetTextColor(dwColor);
    pRow->Add(pLbl);

    CLabelUI* pPrice = new CLabelUI;
    pPrice->SetText(pszPrice);
    pPrice->SetFixedWidth(80);
    pPrice->SetAttribute(_T("align"), _T("center"));
    pRow->Add(pPrice);

    CLabelUI* pVol = new CLabelUI;
    pVol->SetText(pszVolume);
    pVol->SetFixedWidth(60);
    pVol->SetAttribute(_T("align"), _T("center"));
    pRow->Add(pVol);

    pParent->Add(pRow);
}

void CMarketPriceUI::BuildUI()
{
    CVerticalLayoutUI* pMain = new CVerticalLayoutUI;
    pMain->SetChildPadding(1);

    CLabelUI* pTitle = new CLabelUI;
    pTitle->SetText(_T("行情价格"));
    pTitle->SetFixedHeight(24);
    pTitle->SetAttribute(_T("align"), _T("center"));
    pTitle->SetAttribute(_T("font"), _T("1"));
    pMain->Add(pTitle);

    AddPriceRow(pMain, _T("卖5"), _T("3854.0"), _T("120"), 0xFF00AA00);
    AddPriceRow(pMain, _T("卖4"), _T("3853.0"), _T("80"),  0xFF00AA00);
    AddPriceRow(pMain, _T("卖3"), _T("3852.0"), _T("60"),  0xFF00AA00);
    AddPriceRow(pMain, _T("卖2"), _T("3851.0"), _T("40"),  0xFF00AA00);
    AddPriceRow(pMain, _T("卖1"), _T("3850.0"), _T("20"),  0xFF00AA00);

    CControlUI* pSep = new CControlUI;
    pSep->SetFixedHeight(1);
    pSep->SetBkColor(0xFFCCCCCC);
    pMain->Add(pSep);

    AddPriceRow(pMain, _T("买1"), _T("3849.0"), _T("30"),  0xFFAA0000);
    AddPriceRow(pMain, _T("买2"), _T("3848.0"), _T("50"),  0xFFAA0000);
    AddPriceRow(pMain, _T("买3"), _T("3847.0"), _T("70"),  0xFFAA0000);
    AddPriceRow(pMain, _T("买4"), _T("3846.0"), _T("90"),  0xFFAA0000);
    AddPriceRow(pMain, _T("买5"), _T("3845.0"), _T("110"), 0xFFAA0000);

    this->Add(pMain);
}

LPCTSTR CMarketPriceUI::GetClass() const
{
    return _T("CMarketPriceUI");
}

LPCTSTR CMarketPriceUI::GetDescription() const
{
    return _T("行情价格组件 - 五档买卖价格显示");
}

LPVOID CMarketPriceUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, _T("CMarketPriceUI")) == 0) return static_cast<CMarketPriceUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}