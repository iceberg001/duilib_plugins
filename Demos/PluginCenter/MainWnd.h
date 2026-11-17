#ifndef __MAINWND_H__
#define __MAINWND_H__

#include "UIlib.h"
#include "CSplitContainerUI.h"
#include <vector>

struct InstalledComponent
{
    CDuiString sClassName;
    CDuiString sPluginName;
    CControlUI* pControl;
    CContainerUI* pPane;
};

class CMainWnd : public CWindowWnd, public INotifyUI
{
public:
    CMainWnd();
    ~CMainWnd();

    LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
    void OnFinalMessage(HWND hWnd);

    void Notify(TNotifyUI& msg);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void InstallComponent(const CDuiString& sClassName, const CDuiString& sPluginName);
    void SplitActivePane(CSplitContainerUI::SplitMode mode);
    void Reset();
    CPaintManagerUI* GetPaintManager() { return &m_pm; }

private:
    void InitWindow();
    void OnPluginCenter();
    void SetActivePane(CContainerUI* pPane);
    CContainerUI* FindPaneAtPoint(POINT pt);
    void UpdatePaneBorders();
    void AddPlaceholder(CContainerUI* pPane);

    CPaintManagerUI m_pm;
    CSplitContainerUI* rootContainer_;
    CContainerUI* activePane_;
    std::vector<InstalledComponent> installedComponents_;
};

#endif