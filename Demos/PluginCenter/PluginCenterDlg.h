#ifndef __PLUGINCENTERDLG_H__
#define __PLUGINCENTERDLG_H__

#include "UIlib.h"
#include "MainWnd.h"

void WriteLog(const TCHAR* pszFormat, ...);

class CPluginCenterDlg : public CWindowWnd, public INotifyUI
{
public:
    CPluginCenterDlg();
    ~CPluginCenterDlg();

    void SetMainWnd(CMainWnd* pMainWnd);

    LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
    void OnFinalMessage(HWND hWnd);

    void Notify(TNotifyUI& msg);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    void InitWindow();
    void OnImportPlugin();
    void OnSelectComponent();
    void OnInstallComponent();
    void RefreshComponentList();
    void UpdateDescription();

    CPaintManagerUI m_pm;
    CMainWnd* pMainWnd_;
    CDuiString selectedComponent_;
    CDuiString selectedPlugin_;
};

#endif