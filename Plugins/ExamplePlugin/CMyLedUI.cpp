#include "StdAfx.h"
#include "CMyLedUI.h"

using namespace DuiLib;

IMPLEMENT_DUICONTROL(CMyLedUI)

CMyLedUI::CMyLedUI() : m_dwLedColor(0xFFFF0000)
{
    SetText(_T("LED"));
    SetBkColor(0xFF1A1A1A);
    SetTextColor(0xFFFF0000);
    SetAttribute(_T("align"), _T("center"));
    SetAttribute(_T("valign"), _T("vcenter"));
    SetFixedHeight(28);
}

LPCTSTR CMyLedUI::GetClass() const { return _T("CMyLedUI"); }

LPCTSTR CMyLedUI::GetDescription() const
{
    return _T("LED组件 - LED文本显示");
}

LPVOID CMyLedUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, _T("CMyLedUI")) == 0) return static_cast<CMyLedUI*>(this);
    return CLabelUI::GetInterface(pstrName);
}

void CMyLedUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if (_tcsicmp(pstrName, _T("ledcolor")) == 0)
    {
        if (pstrValue[0] == _T('#')) SetLedColor(_tcstoul(pstrValue + 1, NULL, 16));
        else SetLedColor(_tcstoul(pstrValue, NULL, 16));
    }
    else
        CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CMyLedUI::SetLedColor(DWORD dwColor)
{
    m_dwLedColor = dwColor;
    SetTextColor(dwColor);
    if (m_pManager) { RECT rc = GetPos(); m_pManager->Invalidate(rc); }
}

DWORD CMyLedUI::GetLedColor() const { return m_dwLedColor; }

bool CMyLedUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
    return CLabelUI::DoPaint(hDC, rcPaint, pStopControl);
}