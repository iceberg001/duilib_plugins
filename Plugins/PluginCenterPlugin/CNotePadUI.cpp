#include "StdAfx.h"
#include "CNotePadUI.h"

IMPLEMENT_DUICONTROL(CNotePadUI)

CNotePadUI::CNotePadUI()
    : editContent_(NULL)
    , btnSave_(NULL)
    , btnClear_(NULL)
    , labelTitle_(NULL)
{
    BuildUI();
}

LPCTSTR CNotePadUI::GetClass() const
{
    return _T("CNotePadUI");
}

LPVOID CNotePadUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcsicmp(pstrName, _T("CNotePadUI")) == 0)
        return this;
    return CContainerUI::GetInterface(pstrName);
}

void CNotePadUI::BuildUI()
{
    SetInset(RECT{4, 4, 4, 4});

    CVerticalLayoutUI* pMainLayout = new CVerticalLayoutUI;

    labelTitle_ = new CLabelUI;
    labelTitle_->SetText(_T("简易记事本"));
    labelTitle_->SetFont(1);
    labelTitle_->SetTextStyle(DT_CENTER);
    labelTitle_->SetTextColor(0xFF333333);
    labelTitle_->SetFixedHeight(24);
    pMainLayout->Add(labelTitle_);

    CHorizontalLayoutUI* pToolbar = new CHorizontalLayoutUI;
    pToolbar->SetFixedHeight(30);

    btnSave_ = new CButtonUI;
    btnSave_->SetName(_T("btn_save_notepad"));
    btnSave_->SetText(_T("保存"));
    btnSave_->SetFixedWidth(60);
    btnSave_->SetBkColor(0xFF4CAF50);
    btnSave_->SetTextColor(0xFFFFFFFF);
    pToolbar->Add(btnSave_);

    btnClear_ = new CButtonUI;
    btnClear_->SetName(_T("btn_clear_notepad"));
    btnClear_->SetText(_T("清空"));
    btnClear_->SetFixedWidth(60);
    btnClear_->SetBkColor(0xFFF44336);
    btnClear_->SetTextColor(0xFFFFFFFF);
    pToolbar->Add(btnClear_);

    CControlUI* pSpacer = new CControlUI;
    pToolbar->Add(pSpacer);
    pMainLayout->Add(pToolbar);

    editContent_ = new CRichEditUI;
    editContent_->SetName(_T("notepad_edit"));
    editContent_->SetBkColor(0xFFFFFFFF);
    editContent_->SetTextColor(0xFF333333);
    editContent_->SetWantTab(true);
    editContent_->SetWantReturn(true);
    editContent_->SetWantCtrlReturn(true);
    pMainLayout->Add(editContent_);

    Add(pMainLayout);
}