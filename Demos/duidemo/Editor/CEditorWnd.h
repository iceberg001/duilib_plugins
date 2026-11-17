#ifndef __CEDITORWND_H__
#define __CEDITORWND_H__

#include "StdAfx.h"

namespace DuiLib { class CPluginManager; }

struct EditorPage
{
    CDuiString sName;
    CDuiString sFilePath;
    CDuiString sXmlContent;
};

class CEditorWnd : public DuiLib::WindowImplBase
{
public:
    CEditorWnd();
    ~CEditorWnd();

    DUI_DECLARE_MESSAGE_MAP()

public:
    DuiLib::CDuiString GetSkinFile();
    LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
    void OnFinalMessage(HWND hWnd);
    void InitWindow();
    DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
    void Notify(TNotifyUI& msg);
    LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
    void OnClick(TNotifyUI& msg);
    void OnListItemDoubleClick(TNotifyUI& msg);

    void InitComponentList();
    void AddComponentToCanvas(LPCTSTR pszClass);
    void SelectControl(DuiLib::CControlUI* pCtrl);
    void DeleteSelectedControl();
    void ClearSelection();
    void RefreshCanvasComponentList();

    void ShowProperties(DuiLib::CControlUI* pCtrl);
    void ClearPropertyPanel();
    void AddPropertyRow(LPCTSTR pszName, LPCTSTR pszValue);
    void OnPropertyValueChange(DuiLib::CDuiString sPropName, DuiLib::CDuiString sValue);

    void NewPage();
    void SavePage();
    void SaveCanvasToXml(DuiLib::CDuiString& sXml);
    void LoadPageFromXml(LPCTSTR pszXml);
    void AddPageTab(LPCTSTR pszName, int index);
    void SwitchToPage(int index);

    void OnLoadPlugin();
    void RefreshComponentList();

    DuiLib::CContainerUI*  m_pCanvas;
    DuiLib::CListUI*       m_pComponentList;
    DuiLib::CListUI*       m_pCanvasComponentList;
    DuiLib::CVerticalLayoutUI* m_pPropertyPanel;
    DuiLib::CHorizontalLayoutUI* m_pPageTabs;
    DuiLib::CControlUI*    m_pSelected;

    bool   m_bDragging;
    bool   m_bResizing;
    int    m_nResizeEdge;
    POINT  m_ptDragStart;
    POINT  m_ptCtrlStart;
    SIZE   m_szCtrlStart;

    std::vector<EditorPage> m_pages;
    int    m_nCurrentPage;

    static const int RESIZE_NONE = 0;
    static const int RESIZE_LEFT = 1;
    static const int RESIZE_RIGHT = 2;
    static const int RESIZE_TOP = 3;
    static const int RESIZE_BOTTOM = 4;
    static const int RESIZE_MOVE = 5;
};

#endif // __CEDITORWND_H__