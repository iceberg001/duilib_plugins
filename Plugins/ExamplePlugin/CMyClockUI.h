#ifndef __CMYCLOCKUI_H__
#define __CMYCLOCKUI_H__

#include "StdAfx.h"

class CMyClockUI : public DuiLib::CLabelUI
{
    DECLARE_DUICONTROL(CMyClockUI)
public:
    CMyClockUI();
    ~CMyClockUI();

    LPCTSTR GetClass() const;
    LPVOID  GetInterface(LPCTSTR pstrName);
    LPCTSTR GetDescription() const;
    void    SetManager(DuiLib::CPaintManagerUI* pManager, DuiLib::CControlUI* pParent, bool bInit = true);
    void    DoEvent(DuiLib::TEventUI& event);
    bool    DoPaint(HDC hDC, const RECT& rcPaint, DuiLib::CControlUI* pStopControl);

private:
    UINT    m_nTimerId;
};

#endif // __CMYCLOCKUI_H__