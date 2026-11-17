#ifndef __CTRADEQUERYUI_H__
#define __CTRADEQUERYUI_H__

#include "StdAfx.h"

class CTradeQueryUI : public DuiLib::CContainerUI
{
    DECLARE_DUICONTROL(CTradeQueryUI)
public:
    CTradeQueryUI();

    LPCTSTR GetClass() const;
    LPVOID  GetInterface(LPCTSTR pstrName);
    LPCTSTR GetDescription() const;

private:
    void BuildUI();
    DuiLib::CListUI* CreateQueryList(LPCTSTR pszName);
};

#endif