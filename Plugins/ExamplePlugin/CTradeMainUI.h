#ifndef __CTRADEMAINUI_H__
#define __CTRADEMAINUI_H__

#include "StdAfx.h"

class CTradeMainUI : public DuiLib::CContainerUI
{
    DECLARE_DUICONTROL(CTradeMainUI)
public:
    CTradeMainUI();

    LPCTSTR GetClass() const;
    LPVOID  GetInterface(LPCTSTR pstrName);
    LPCTSTR GetDescription() const;

private:
    void BuildUI();
    DuiLib::CHorizontalLayoutUI* CreateTopNav();
    DuiLib::CHorizontalLayoutUI* CreateMainContent();
    DuiLib::CHorizontalLayoutUI* CreateBottomArea();
    DuiLib::CHorizontalLayoutUI* CreateStatusBar();
    DuiLib::CButtonUI* CreateNavButton(LPCTSTR pszText, int w = 60);
    DuiLib::CLabelUI* CreateStatusLabel(LPCTSTR pszText, DWORD dwColor = 0xFFC0C0C8);
};

#endif