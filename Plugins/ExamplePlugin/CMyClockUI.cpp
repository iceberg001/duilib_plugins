#include "StdAfx.h"
#include "CMyClockUI.h"
#include <time.h>

using namespace DuiLib;

IMPLEMENT_DUICONTROL(CMyClockUI)

CMyClockUI::CMyClockUI() : m_nTimerId(100)
{
    SetText(_T("00:00:00"));
    SetTextColor(0xFF00FF00);
    SetBkColor(0xFF000000);
    SetAttribute(_T("align"), _T("center"));
    SetAttribute(_T("valign"), _T("vcenter"));
    SetFixedHeight(30);
}

CMyClockUI::~CMyClockUI() {}

LPCTSTR CMyClockUI::GetClass() const { return _T("CMyClockUI"); }

LPCTSTR CMyClockUI::GetDescription() const
{
    return _T("时钟组件 - 实时显示当前时间");
}

LPVOID CMyClockUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, _T("CMyClockUI")) == 0) return static_cast<CMyClockUI*>(this);
    return CLabelUI::GetInterface(pstrName);
}

void CMyClockUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
{
    CPaintManagerUI* pOldManager = m_pManager;
    CLabelUI::SetManager(pManager, pParent, bInit);
    if (bInit && pManager)
    {
        pManager->SetTimer(this, m_nTimerId, 1000);
    }
    else if (pManager == NULL && pOldManager)
    {
        pOldManager->KillTimer(this, m_nTimerId);
    }
}

void CMyClockUI::DoEvent(TEventUI& event)
{
    if (event.Type == UIEVENT_TIMER && event.wParam == m_nTimerId)
    {
        time_t t; time(&t);
        struct tm* lt = localtime(&t);
        TCHAR buf[16] = {0};
        _stprintf(buf, _T("%02d:%02d:%02d"), lt->tm_hour, lt->tm_min, lt->tm_sec);
        SetText(buf);
        if (m_pManager) { RECT rc = GetPos(); m_pManager->Invalidate(rc); }
        return;
    }
    CLabelUI::DoEvent(event);
}

bool CMyClockUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
    return CLabelUI::DoPaint(hDC, rcPaint, pStopControl);
}