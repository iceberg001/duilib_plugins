#ifndef __CNOTEPADUI_H__
#define __CNOTEPADUI_H__

#include "StdAfx.h"

class CNotePadUI : public DuiLib::CContainerUI
{
    DECLARE_DUICONTROL(CNotePadUI)
public:
    CNotePadUI();

    LPCTSTR GetClass() const;
    LPVOID  GetInterface(LPCTSTR pstrName);

private:
    void BuildUI();

    DuiLib::CRichEditUI* editContent_;
    DuiLib::CButtonUI*   btnSave_;
    DuiLib::CButtonUI*   btnClear_;
    DuiLib::CLabelUI*    labelTitle_;
};

#endif