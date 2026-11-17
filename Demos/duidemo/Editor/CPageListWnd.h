#ifndef __CPAGELISTWND_H__
#define __CPAGELISTWND_H__

#include "StdAfx.h"

class CPageListWnd : public DuiLib::WindowImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CPageListWnd();
    ~CPageListWnd();

public:
    DuiLib::CDuiString GetSkinFile();
    LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
    void OnFinalMessage(HWND hWnd);
    void InitWindow();
    DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
    void Notify(TNotifyUI& msg);

private:
    void OnClick(TNotifyUI& msg);
    void OnListItemClick(TNotifyUI& msg);
    void ScanPages();
    void OpenSelectedPage();
    void DeleteSelectedPage();
    static CDuiString GetPagesDir();

    DuiLib::CListUI*       m_pPageList;
    DuiLib::CLabelUI*      m_pStatusLabel;
};

#endif