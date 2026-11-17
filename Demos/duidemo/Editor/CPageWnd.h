#ifndef __CPAGEWND_H__
#define __CPAGEWND_H__

#include "StdAfx.h"

class CPageWnd : public DuiLib::WindowImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CPageWnd();
    ~CPageWnd();

    void SetXmlFile(LPCTSTR pszFile);

public:
    DuiLib::CDuiString GetSkinFile();
    LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
    void OnFinalMessage(HWND hWnd);
    void InitWindow();
    DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
    void Notify(TNotifyUI& msg);

private:
    DuiLib::CDuiString m_sXmlFile;
    DuiLib::CDuiString m_sXmlContent;
};

#endif // __CPAGEWND_H__