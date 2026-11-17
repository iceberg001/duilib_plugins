#include "StdAfx.h"
#include "CTradeOrderUI.h"

using namespace DuiLib;

IMPLEMENT_DUICONTROL(CTradeOrderUI)

CTradeOrderUI::CTradeOrderUI()
    : comboInstrument_(NULL), editPrice_(NULL), editVolume_(NULL)
    , optBuy_(NULL), optSell_(NULL), btnOrder_(NULL)
{
    SetFixedWidth(250);
    SetFixedHeight(200);
    SetBkColor(0xFFF5F5F5);
    SetAttribute(_T("border"), _T("1"));
    SetAttribute(_T("bordercolor"), _T("#FFCCCCCC"));
    SetAttribute(_T("inset"), _T("8,8,8,8"));
    BuildUI();
}

void CTradeOrderUI::BuildUI()
{
    CVerticalLayoutUI* pMain = new CVerticalLayoutUI;
    pMain->SetChildPadding(6);

    CLabelUI* pTitle = new CLabelUI;
    pTitle->SetText(_T("交易下单"));
    pTitle->SetFixedHeight(24);
    pTitle->SetAttribute(_T("align"), _T("center"));
    pTitle->SetAttribute(_T("font"), _T("1"));
    pMain->Add(pTitle);

    CHorizontalLayoutUI* pRow1 = new CHorizontalLayoutUI;
    pRow1->SetChildPadding(4);
    CLabelUI* pLbl1 = new CLabelUI;
    pLbl1->SetText(_T("合约:"));
    pLbl1->SetFixedWidth(40);
    pLbl1->SetFixedHeight(24);
    pRow1->Add(pLbl1);
    comboInstrument_ = new CComboUI;
    comboInstrument_->SetFixedHeight(24);
    comboInstrument_->SetAttribute(_T("bkcolor"), _T("#FFFFFFFF"));
    comboInstrument_->SetAttribute(_T("border"), _T("1"));
    comboInstrument_->SetAttribute(_T("bordercolor"), _T("#FF999999"));
    CListLabelElementUI* pItem1 = new CListLabelElementUI;
    pItem1->SetText(_T("IF2406"));
    comboInstrument_->Add(pItem1);
    CListLabelElementUI* pItem2 = new CListLabelElementUI;
    pItem2->SetText(_T("rb2406"));
    comboInstrument_->Add(pItem2);
    CListLabelElementUI* pItem3 = new CListLabelElementUI;
    pItem3->SetText(_T("au2406"));
    comboInstrument_->Add(pItem3);
    comboInstrument_->SelectItem(0);
    pRow1->Add(comboInstrument_);
    pMain->Add(pRow1);

    CHorizontalLayoutUI* pRow2 = new CHorizontalLayoutUI;
    pRow2->SetChildPadding(4);
    CLabelUI* pLbl2 = new CLabelUI;
    pLbl2->SetText(_T("价格:"));
    pLbl2->SetFixedWidth(40);
    pLbl2->SetFixedHeight(24);
    pRow2->Add(pLbl2);
    editPrice_ = new CEditUI;
    editPrice_->SetFixedHeight(24);
    editPrice_->SetAttribute(_T("bkcolor"), _T("#FFFFFFFF"));
    editPrice_->SetAttribute(_T("border"), _T("1"));
    editPrice_->SetAttribute(_T("bordercolor"), _T("#FF999999"));
    editPrice_->SetText(_T("3850.0"));
    pRow2->Add(editPrice_);
    pMain->Add(pRow2);

    CHorizontalLayoutUI* pRow3 = new CHorizontalLayoutUI;
    pRow3->SetChildPadding(4);
    CLabelUI* pLbl3 = new CLabelUI;
    pLbl3->SetText(_T("数量:"));
    pLbl3->SetFixedWidth(40);
    pLbl3->SetFixedHeight(24);
    pRow3->Add(pLbl3);
    editVolume_ = new CEditUI;
    editVolume_->SetFixedHeight(24);
    editVolume_->SetAttribute(_T("bkcolor"), _T("#FFFFFFFF"));
    editVolume_->SetAttribute(_T("border"), _T("1"));
    editVolume_->SetAttribute(_T("bordercolor"), _T("#FF999999"));
    editVolume_->SetText(_T("1"));
    pRow3->Add(editVolume_);
    pMain->Add(pRow3);

    CHorizontalLayoutUI* pRow4 = new CHorizontalLayoutUI;
    pRow4->SetChildPadding(8);
    optBuy_ = new COptionUI;
    optBuy_->SetName(_T("optBuy"));
    optBuy_->SetText(_T("买入"));
    optBuy_->SetFixedWidth(60);
    optBuy_->SetFixedHeight(24);
    optBuy_->SetAttribute(_T("bkcolor"), _T("#FFFF4444"));
    optBuy_->SetAttribute(_T("selectedbkcolor"), _T("#FFCC0000"));
    optBuy_->SetAttribute(_T("textcolor"), _T("#FFFFFFFF"));
    optBuy_->SetAttribute(_T("selectedtextcolor"), _T("#FFFFFFFF"));
    optBuy_->SetAttribute(_T("group"), _T("direction"));
    optBuy_->Selected(true);
    pRow4->Add(optBuy_);
    optSell_ = new COptionUI;
    optSell_->SetName(_T("optSell"));
    optSell_->SetText(_T("卖出"));
    optSell_->SetFixedWidth(60);
    optSell_->SetFixedHeight(24);
    optSell_->SetAttribute(_T("bkcolor"), _T("#FF44AA44"));
    optSell_->SetAttribute(_T("selectedbkcolor"), _T("#FF008800"));
    optSell_->SetAttribute(_T("textcolor"), _T("#FFFFFFFF"));
    optSell_->SetAttribute(_T("selectedtextcolor"), _T("#FFFFFFFF"));
    optSell_->SetAttribute(_T("group"), _T("direction"));
    pRow4->Add(optSell_);
    pMain->Add(pRow4);

    btnOrder_ = new CButtonUI;
    btnOrder_->SetName(_T("btnOrder"));
    btnOrder_->SetText(_T("提交订单"));
    btnOrder_->SetFixedHeight(32);
    btnOrder_->SetAttribute(_T("bkcolor"), _T("#FF0078D7"));
    btnOrder_->SetAttribute(_T("textcolor"), _T("#FFFFFFFF"));
    pMain->Add(btnOrder_);

    this->Add(pMain);
}

LPCTSTR CTradeOrderUI::GetClass() const
{
    return _T("CTradeOrderUI");
}

LPCTSTR CTradeOrderUI::GetDescription() const
{
    return _T("交易下单面板 - 合约选择/价格数量/买卖方向");
}

LPVOID CTradeOrderUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, _T("CTradeOrderUI")) == 0) return static_cast<CTradeOrderUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}