#ifndef __CMYLEDUI_H__
#define __CMYLEDUI_H__

#include "StdAfx.h"

class CMyLedUI : public DuiLib::CLabelUI
{
    DECLARE_DUICONTROL(CMyLedUI)
public:
    CMyLedUI();

    LPCTSTR GetClass() const;
    LPVOID  GetInterface(LPCTSTR pstrName);
    LPCTSTR GetDescription() const;
    void    SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    bool    DoPaint(HDC hDC, const RECT& rcPaint, DuiLib::CControlUI* pStopControl);

    void    SetLedColor(DWORD dwColor);
    DWORD   GetLedColor() const;

private:
    DWORD   m_dwLedColor;
};

#endif // __CMYLEDUI_H__