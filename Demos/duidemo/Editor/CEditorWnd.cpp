#include "StdAfx.h"
#include "CEditorWnd.h"

#include "../../DuiLib/Core/Plugin/CPluginManager.h"
#include "../../DuiLib/Core/CNodeUIManager.h"
#include <windows.h>

using namespace DuiLib;

DUI_BEGIN_MESSAGE_MAP(CEditorWnd, WindowImplBase)
    DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
    DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnListItemDoubleClick)
DUI_END_MESSAGE_MAP()

CEditorWnd::CEditorWnd()
    : m_pCanvas(NULL)
    , m_pComponentList(NULL)
    , m_pCanvasComponentList(NULL)
    , m_pPropertyPanel(NULL)
    , m_pPageTabs(NULL)
    , m_pSelected(NULL)
    , m_bDragging(false)
    , m_bResizing(false)
    , m_nResizeEdge(RESIZE_NONE)
    , m_nCurrentPage(-1)
{
    memset(&m_ptDragStart, 0, sizeof(m_ptDragStart));
    memset(&m_ptCtrlStart, 0, sizeof(m_ptCtrlStart));
    memset(&m_szCtrlStart, 0, sizeof(m_szCtrlStart));
}

CEditorWnd::~CEditorWnd()
{
}

CDuiString CEditorWnd::GetSkinFile()
{
    return _T("editor.xml");
}

LPCTSTR CEditorWnd::GetWindowClassName() const
{
    return _T("EditorWnd");
}

UINT CEditorWnd::GetClassStyle() const
{
    return CS_DBLCLKS;
}

void CEditorWnd::OnFinalMessage(HWND hWnd)
{
    CNodeUIManager::GetInstance().Clear();
    __super::OnFinalMessage(hWnd);
    delete this;
}

void CEditorWnd::InitWindow()
{
    m_pCanvas = static_cast<CContainerUI*>(m_pm.FindControl(_T("canvas")));
    m_pComponentList = static_cast<CListUI*>(m_pm.FindControl(_T("component_list")));
    m_pCanvasComponentList = static_cast<CListUI*>(m_pm.FindControl(_T("canvas_component_list")));
    m_pPropertyPanel = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("property_panel")));
    m_pPageTabs = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("page_tabs")));

    CPluginManager& mgr = CPluginManager::GetInstance();
    mgr.RegisterDescription(_T("Button"),           _T("按钮控件 - 通用按钮"));
    mgr.RegisterDescription(_T("Label"),            _T("标签控件 - 显示文本"));
    mgr.RegisterDescription(_T("Edit"),             _T("编辑框控件 - 输入文本"));
    mgr.RegisterDescription(_T("Option"),           _T("单选按钮 - 单选选项"));
    mgr.RegisterDescription(_T("CheckBox"),         _T("复选按钮 - 多选选项"));
    mgr.RegisterDescription(_T("Progress"),         _T("进度条控件 - 显示进度"));
    mgr.RegisterDescription(_T("Slider"),           _T("滑块控件 - 拖动选择值"));
    mgr.RegisterDescription(_T("Combo"),            _T("下拉框控件 - 下拉选择"));
    mgr.RegisterDescription(_T("ComboBox"),         _T("可编辑下拉框 - 输入并下拉选择"));
    mgr.RegisterDescription(_T("List"),             _T("列表控件 - 显示列表项"));
    mgr.RegisterDescription(_T("RichEdit"),         _T("富文本编辑 - 支持格式文本"));
    mgr.RegisterDescription(_T("ActiveX"),          _T("ActiveX控件 - 嵌入COM组件"));
    mgr.RegisterDescription(_T("Text"),             _T("文本控件 - 显示静态文本"));
    mgr.RegisterDescription(_T("RollText"),         _T("滚动文本控件 - 循环滚动显示"));
    mgr.RegisterDescription(_T("GroupBox"),         _T("分组框 - 聚拢相关控件"));
    mgr.RegisterDescription(_T("DateTime"),         _T("日期时间控件 - 选择日期时间"));
    mgr.RegisterDescription(_T("IPAddress"),        _T("IP地址控件 - 输入IP地址"));
    mgr.RegisterDescription(_T("HotKey"),           _T("热键控件 - 设置快捷键"));
    mgr.RegisterDescription(_T("FadeButton"),       _T("渐变按钮 - 淡入淡出效果"));
    mgr.RegisterDescription(_T("GifAnim"),          _T("GIF动画控件 - 播放GIF图片"));
    mgr.RegisterDescription(_T("WebBrowser"),       _T("网页浏览器控件 - 嵌入网页"));
    mgr.RegisterDescription(_T("Ring"),             _T("圆环控件 - 环形指示器"));
    mgr.RegisterDescription(_T("Loading"),          _T("加载控件 - 旋转加载指示"));
    mgr.RegisterDescription(_T("ColorPalette"),     _T("颜色板控件 - 颜色选择"));
    mgr.RegisterDescription(_T("TabLayout"),        _T("页签布局 - 页签切换布局"));
    mgr.RegisterDescription(_T("HorizontalLayout"), _T("水平布局 - 控件水平排列"));
    mgr.RegisterDescription(_T("VerticalLayout"),   _T("垂直布局 - 控件垂直排列"));
    mgr.RegisterDescription(_T("Container"),        _T("容器控件 - 包含子控件"));
    mgr.RegisterDescription(_T("TileLayout"),       _T("平铺布局 - 网格平铺排列"));
    mgr.RegisterDescription(_T("TreeView"),         _T("树形视图 - 层级结构显示"));
    mgr.RegisterDescription(_T("ChildLayout"),      _T("子布局控件 - 嵌入子布局"));


    InitComponentList();
    NewPage();
}

CControlUI* CEditorWnd::CreateControl(LPCTSTR pstrClass)
{

    CControlUI* pCtrl = CControlFactory::GetInstance()->CreateControl(pstrClass);
    if (pCtrl) return pCtrl;
    return CControlFactory::GetInstance()->CreateControl(CDuiString(pstrClass) + _T("UI"));
}

void CEditorWnd::Notify(TNotifyUI& msg)
{
    CDuiString sName = msg.pSender->GetName();
    if (sName == _T("closebtn")) { Close(); return; }
    __super::Notify(msg);
}

LRESULT CEditorWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (uMsg == WM_LBUTTONDOWN)
    {
        if (m_pSelected && m_bResizing)
        {
            m_bResizing = false;
            m_nResizeEdge = RESIZE_NONE;
        }
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        if (m_pCanvas)
        {
            RECT rcCanvas = m_pCanvas->GetPos();
            if (PtInRect(&rcCanvas, pt))
            {
                CControlUI* pHit = NULL;
                for (int i = m_pCanvas->GetCount() - 1; i >= 0; --i)
                {
                    CControlUI* pCtrl = m_pCanvas->GetItemAt(i);
                    RECT rcCtrl = pCtrl->GetPos();
                    if (PtInRect(&rcCtrl, pt))
                    {
                        pHit = pCtrl;
                        break;
                    }
                }
                if (pHit)
                {
                    SelectControl(pHit);
                    m_bDragging = true;
                    m_ptDragStart = pt;
                    CDuiSize xy = pHit->GetFixedXY();
                    m_ptCtrlStart.x = xy.cx;
                    m_ptCtrlStart.y = xy.cy;
                    m_szCtrlStart.cx = pHit->GetFixedWidth();
                    m_szCtrlStart.cy = pHit->GetFixedHeight();
                    RECT rc = pHit->GetPos();
                    if (pt.x >= rc.right - 6) m_nResizeEdge = RESIZE_RIGHT;
                    else if (pt.x <= rc.left + 6) m_nResizeEdge = RESIZE_LEFT;
                    else if (pt.y >= rc.bottom - 6) m_nResizeEdge = RESIZE_BOTTOM;
                    else if (pt.y <= rc.top + 6) m_nResizeEdge = RESIZE_TOP;
                    else m_nResizeEdge = RESIZE_MOVE;
                    bHandled = TRUE;
                }
                else
                {
                    ClearSelection();
                }
            }
        }
    }
    else if (uMsg == WM_LBUTTONUP)
    {
        m_bDragging = false;
        m_nResizeEdge = RESIZE_NONE;
    }
    else if (uMsg == WM_MOUSEMOVE)
    {
        if (m_bDragging && m_pSelected)
        {
            POINT pt = { LOWORD(lParam), HIWORD(lParam) };
            int dx = pt.x - m_ptDragStart.x;
            int dy = pt.y - m_ptDragStart.y;
            CDuiSize xy = m_pSelected->GetFixedXY();
            int w = m_pSelected->GetFixedWidth();
            int h = m_pSelected->GetFixedHeight();
            if (m_nResizeEdge == RESIZE_MOVE)
            {
                m_pSelected->SetFixedXY(CDuiSize(m_ptCtrlStart.x + dx, m_ptCtrlStart.y + dy));
            }
            else if (m_nResizeEdge == RESIZE_RIGHT)
            {
                m_pSelected->SetFixedWidth(m_szCtrlStart.cx + dx);
            }
            else if (m_nResizeEdge == RESIZE_LEFT)
            {
                m_pSelected->SetFixedXY(CDuiSize(m_ptCtrlStart.x + dx, m_ptCtrlStart.y));
                m_pSelected->SetFixedWidth(m_szCtrlStart.cx - dx);
            }
            else if (m_nResizeEdge == RESIZE_BOTTOM)
            {
                m_pSelected->SetFixedHeight(m_szCtrlStart.cy + dy);
            }
            else if (m_nResizeEdge == RESIZE_TOP)
            {
                m_pSelected->SetFixedXY(CDuiSize(m_ptCtrlStart.x, m_ptCtrlStart.y + dy));
                m_pSelected->SetFixedHeight(m_szCtrlStart.cy - dy);
            }
            RECT rcInv = m_pSelected->GetPos();
            m_pm.Invalidate(rcInv);
            bHandled = TRUE;
        }
    }
    else if (uMsg == WM_RBUTTONDOWN)
    {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        if (m_pCanvas)
        {
            for (int i = m_pCanvas->GetCount() - 1; i >= 0; --i)
            {
                CControlUI* pCtrl = m_pCanvas->GetItemAt(i);
                RECT rcCtrl = pCtrl->GetPos();
                if (PtInRect(&rcCtrl, pt))
                {
                    SelectControl(pCtrl);
                    POINT ptScreen = pt;
                    ClientToScreen(m_hWnd, &ptScreen);
                    HMENU hMenu = CreatePopupMenu();
                    AppendMenu(hMenu, MF_STRING, 1, _T("删除组件"));
                    AppendMenu(hMenu, MF_STRING, 2, _T("复制组件"));
                    int ret = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, ptScreen.x, ptScreen.y, 0, m_hWnd, NULL);
                    DestroyMenu(hMenu);
                    if (ret == 1) DeleteSelectedControl();
                    else if (ret == 2)
                    {
                        CDuiString sClass = m_pSelected->GetClass();
                        AddComponentToCanvas(sClass.GetData());
                    }
                    bHandled = TRUE;
                    break;
                }
            }
        }
    }
    else if (uMsg == WM_TIMER && wParam == 1001)
    {
        if (m_pSelected && m_hWnd)
        {
            RECT rc = m_pSelected->GetPos();
            HDC hdc = GetDC(m_hWnd);
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
            HGDIOBJ hOldPen = SelectObject(hdc, hPen);
            HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);
            Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
            int hs = 5;
            Rectangle(hdc, rc.left - hs, rc.top - hs, rc.left + hs, rc.top + hs);
            Rectangle(hdc, rc.right - hs, rc.top - hs, rc.right + hs, rc.top + hs);
            Rectangle(hdc, rc.left - hs, rc.bottom - hs, rc.left + hs, rc.bottom + hs);
            Rectangle(hdc, rc.right - hs, rc.bottom - hs, rc.right + hs, rc.bottom + hs);
            SelectObject(hdc, hOldPen);
            SelectObject(hdc, hOldBrush);
            DeleteObject(hPen);
            ReleaseDC(m_hWnd, hdc);
        }
    }
    return 0;
}

void CEditorWnd::OnClick(TNotifyUI& msg)
{
    CDuiString sName = msg.pSender->GetName();
    if (sName == _T("new_page_btn")) { NewPage(); return; }
    if (sName == _T("save_page_btn")) { SavePage(); return; }
    if (sName == _T("load_plugin_btn")) { OnLoadPlugin(); return; }
    if (sName == _T("delete_btn")) { DeleteSelectedControl(); return; }
    if (sName == _T("canvas_component_list"))
    {
        int nSel = m_pCanvasComponentList->GetCurSel();
        if (nSel >= 0)
        {
            CListLabelElementUI* pItem = static_cast<CListLabelElementUI*>(m_pCanvasComponentList->GetItemAt(nSel));
            if (pItem)
            {
                CControlUI* pCtrl = (CControlUI*)pItem->GetTag();
                if (pCtrl) SelectControl(pCtrl);
            }
        }
        return;
    }
    if (sName.Find(_T("page_tab_")) == 0)
    {
        CDuiString sIdx = sName.Mid(9);
        int index = _ttoi(sIdx.GetData());
        SwitchToPage(index);
        return;
    }
    if (sName.Find(_T("prop_")) == 0)
    {
        CDuiString sPropName = sName.Mid(5);
        CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(sName + _T("_edit")));
        if (pEdit)
        {
            OnPropertyValueChange(sPropName, pEdit->GetText());
        }
        return;
    }
}

void CEditorWnd::OnListItemDoubleClick(TNotifyUI& msg)
{
    if (msg.pSender && msg.pSender->GetName().Find(_T("canvas_item")) == 0) return;
    if (!m_pComponentList) return;
    int nSel = m_pComponentList->GetCurSel();
    if (nSel < 0) return;
    CListLabelElementUI* pItem = static_cast<CListLabelElementUI*>(m_pComponentList->GetItemAt(nSel));
    if (!pItem) return;
    AddComponentToCanvas(pItem->GetText().GetData());
}

void CEditorWnd::InitComponentList()
{
    if (!m_pComponentList) return;
    m_pComponentList->RemoveAll();

    CDuiString sComponents = _T("Button|Label|Edit|Option|CheckBox|Progress|Slider|Combo|ComboBox|List|RichEdit|ActiveX|Text|RollText|GroupBox|DateTime|IPAddress|HotKey|FadeButton|GifAnim|WebBrowser|Ring|Loading|ColorPalette|TabLayout|HorizontalLayout|VerticalLayout|Container|TileLayout|TreeView|ChildLayout");

    CPluginManager& mgr = CPluginManager::GetInstance();
    for (int i = 0; i < mgr.GetPluginCount(); ++i)
    {
        const LoadedPlugin* pPlugin = mgr.GetPlugin(i);
        if (pPlugin && !pPlugin->info.sComponents.IsEmpty())
        {
            sComponents += _T("|");
            sComponents += pPlugin->info.sComponents;
        }
    }

    CDuiString sItem;
    int start = 0;
    while (start >= 0)
    {
        int end = sComponents.Find(_T("|"), start);
        if (end < 0) sItem = sComponents.Mid(start);
        else sItem = sComponents.Mid(start, end - start);
        if (!sItem.IsEmpty())
        {
            CListLabelElementUI* pElement = new CListLabelElementUI;
            pElement->SetText(sItem);
            pElement->SetFixedHeight(22);
            LPCTSTR pDesc = mgr.GetComponentDescription(sItem.GetData());
            if (pDesc) pElement->SetToolTip(pDesc);
            m_pComponentList->Add(pElement);
        }
        if (end < 0) break;
        start = end + 1;
    }
}

void CEditorWnd::AddComponentToCanvas(LPCTSTR pszClass)
{
    if (!m_pCanvas || !pszClass) return;
    CControlUI* pCtrl = CControlFactory::GetInstance()->CreateControl(pszClass);
    if (pCtrl == NULL)
    {
        CDuiString sFullClass;
        sFullClass.Format(_T("C%sUI"), pszClass);
        pCtrl = CControlFactory::GetInstance()->CreateControl(sFullClass);
    }
    if (pCtrl == NULL)
    {
        pCtrl = new CLabelUI;
        static_cast<CLabelUI*>(pCtrl)->SetText(pszClass);
    }
    CDuiString sName;
    sName.Format(_T("%s_%d"), pszClass, m_pCanvas->GetCount());
    pCtrl->SetName(sName);
    pCtrl->SetFloat(true);
    pCtrl->SetFixedXY(CDuiSize(10, 10));
    pCtrl->SetFixedWidth(120);
    pCtrl->SetFixedHeight(30);
    if (_tcsicmp(pszClass, _T("Button")) == 0 || _tcsicmp(pszClass, _T("Label")) == 0)
        pCtrl->SetText(_T("Text"));
    pCtrl->SetBkColor(0xFFF0F0F0);
    m_pCanvas->Add(pCtrl);
    SelectControl(pCtrl);
    RefreshCanvasComponentList();
}

void CEditorWnd::SelectControl(CControlUI* pCtrl)
{
    if (m_pSelected && m_pSelected != pCtrl)
    {
        m_pSelected->SetBorderSize(0);
        m_pSelected->SetBorderColor(0);
        RECT rcInv = m_pSelected->GetPos();
        m_pm.Invalidate(rcInv);
    }
    m_pSelected = pCtrl;
    if (m_pSelected)
    {
        m_pSelected->SetBorderSize(2);
        m_pSelected->SetBorderColor(0xFFFF0000);
        RECT rcInv = m_pSelected->GetPos();
        m_pm.Invalidate(rcInv);
        SetTimer(m_hWnd, 1001, 50, NULL);
    }
    ShowProperties(pCtrl);
}

void CEditorWnd::DeleteSelectedControl()
{
    if (!m_pSelected || !m_pCanvas) return;
    CContainerUI* pParent = static_cast<CContainerUI*>(m_pSelected->GetParent());
    if (pParent)
    {
        pParent->Remove(m_pSelected);
    }
    else
    {
        m_pCanvas->Remove(m_pSelected);
    }
    m_pSelected = NULL;
    ClearPropertyPanel();
    RefreshCanvasComponentList();
}

void CEditorWnd::ClearSelection()
{
    KillTimer(m_hWnd, 1001);
    if (m_pSelected)
    {
        m_pSelected->SetBorderSize(0);
        m_pSelected->SetBorderColor(0);
        RECT rcInv = m_pSelected->GetPos();
        m_pm.Invalidate(rcInv);
        m_pSelected = NULL;
    }
    ClearPropertyPanel();
}

void CEditorWnd::RefreshCanvasComponentList()
{
    if (!m_pCanvasComponentList || !m_pCanvas) return;
    m_pCanvasComponentList->RemoveAll();
    for (int i = 0; i < m_pCanvas->GetCount(); ++i)
    {
        CControlUI* pCtrl = m_pCanvas->GetItemAt(i);
        CDuiString sText;
        sText.Format(_T("%d: %s [%s]"), i, pCtrl->GetClass(), pCtrl->GetName().GetData());
        CListLabelElementUI* pElement = new CListLabelElementUI;
        pElement->SetName(_T("canvas_item"));
        pElement->SetText(sText);
        pElement->SetFixedHeight(20);
        pElement->SetTag((UINT_PTR)pCtrl);
        m_pCanvasComponentList->Add(pElement);
    }
}

void CEditorWnd::ShowProperties(CControlUI* pCtrl)
{
    ClearPropertyPanel();
    if (!pCtrl || !m_pPropertyPanel) return;

    CVerticalLayoutUI* pLayout = new CVerticalLayoutUI;
    pLayout->SetFixedHeight(0);
    pLayout->SetAutoDestroy(true);

    CDuiString sClass = pCtrl->GetClass();
    CDuiString sName = pCtrl->GetName();

    AddPropertyRow(_T("Class"), sClass.GetData());
    AddPropertyRow(_T("Name"), sName.GetData());

    TCHAR szVal[64] = {0};
    CDuiSize xy = pCtrl->GetFixedXY();
    _stprintf(szVal, _T("%d"), xy.cx);
    AddPropertyRow(_T("X"), szVal);
    _stprintf(szVal, _T("%d"), xy.cy);
    AddPropertyRow(_T("Y"), szVal);
    _stprintf(szVal, _T("%d"), pCtrl->GetFixedWidth());
    AddPropertyRow(_T("Width"), szVal);
    _stprintf(szVal, _T("%d"), pCtrl->GetFixedHeight());
    AddPropertyRow(_T("Height"), szVal);

    if (pCtrl->GetText().GetLength() > 0)
    {
        AddPropertyRow(_T("Text"), pCtrl->GetText().GetData());
    }

    _stprintf(szVal, _T("#%08X"), pCtrl->GetBkColor());
    AddPropertyRow(_T("BkColor"), szVal);

    CHorizontalLayoutUI* pBtnRow = new CHorizontalLayoutUI;
    pBtnRow->SetFixedHeight(24);
    CButtonUI* pDelBtn = new CButtonUI;
    pDelBtn->SetName(_T("delete_btn"));
    pDelBtn->SetText(_T("删除此组件"));
    pDelBtn->SetBkColor(0xFFFFCCCC);
    pBtnRow->Add(pDelBtn);
    m_pPropertyPanel->Add(pBtnRow);
}

void CEditorWnd::ClearPropertyPanel()
{
    if (!m_pPropertyPanel) return;
    m_pPropertyPanel->RemoveAll();
}

void CEditorWnd::AddPropertyRow(LPCTSTR pszName, LPCTSTR pszValue)
{
    if (!m_pPropertyPanel || !pszName) return;
    CHorizontalLayoutUI* pRow = new CHorizontalLayoutUI;
    pRow->SetFixedHeight(22);

    CLabelUI* pLabel = new CLabelUI;
    CDuiString sLabelName;
    sLabelName.Format(_T("prop_%s"), pszName);
    pLabel->SetName(sLabelName);
    pLabel->SetText(pszName);
    pLabel->SetFixedWidth(60);
    pLabel->SetBkColor(0xFFF0F0F0);
    pRow->Add(pLabel);

    CEditUI* pEdit = new CEditUI;
    CDuiString sEditName;
    sEditName.Format(_T("prop_%s_edit"), pszName);
    pEdit->SetName(sEditName);
    pEdit->SetText(pszValue ? pszValue : _T(""));
    pRow->Add(pEdit);

    m_pPropertyPanel->Add(pRow);
}

void CEditorWnd::OnPropertyValueChange(CDuiString sPropName, CDuiString sValue)
{
    if (!m_pSelected) return;
    if (sPropName == _T("Name")) m_pSelected->SetName(sValue);
    else if (sPropName == _T("Text")) m_pSelected->SetText(sValue);
    else if (sPropName == _T("X")) m_pSelected->SetFixedXY(CDuiSize(_ttoi(sValue.GetData()), m_pSelected->GetFixedXY().cy));
    else if (sPropName == _T("Y")) m_pSelected->SetFixedXY(CDuiSize(m_pSelected->GetFixedXY().cx, _ttoi(sValue.GetData())));
    else if (sPropName == _T("Width")) m_pSelected->SetFixedWidth(_ttoi(sValue.GetData()));
    else if (sPropName == _T("Height")) m_pSelected->SetFixedHeight(_ttoi(sValue.GetData()));
    else if (sPropName == _T("BkColor"))
    {
        if (sValue[0] == _T('#')) sValue = sValue.Mid(1);
        DWORD dwColor = _tcstoul(sValue.GetData(), NULL, 16);
        m_pSelected->SetBkColor(dwColor);
    }
}

void CEditorWnd::NewPage()
{
    EditorPage page;
    page.sName.Format(_T("Page%d"), (int)m_pages.size() + 1);
    page.sXmlContent = _T("");
    m_pages.push_back(page);
    m_nCurrentPage = (int)m_pages.size() - 1;
    AddPageTab(page.sName.GetData(), m_nCurrentPage);
    if (m_pCanvas) m_pCanvas->RemoveAll();
    ClearSelection();
}

void CEditorWnd::SavePage()
{
    if (m_nCurrentPage < 0 || m_nCurrentPage >= (int)m_pages.size()) return;
    SaveCanvasToXml(m_pages[m_nCurrentPage].sXmlContent);

    TCHAR szExePath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szExePath, MAX_PATH);
    TCHAR* p = _tcsrchr(szExePath, _T('\\'));
    if (p) *p = 0;
    CDuiString sPagesDir = szExePath;
    sPagesDir += _T("\\skin\\duidemo\\pages");
    CreateDirectory(sPagesDir.GetData(), NULL);

    OPENFILENAME ofn = {0};
    TCHAR szFile[MAX_PATH] = {0};
    CDuiString sDefaultName = m_pages[m_nCurrentPage].sName;
    sDefaultName += _T(".xml");
    _tcsncpy(szFile, sDefaultName.GetData(), MAX_PATH - 1);
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = _T("XML Files\0*.xml\0All Files\0*.*\0");
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrInitialDir = sPagesDir.GetData();
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = _T("xml");
    if (GetSaveFileName(&ofn))
    {
        m_pages[m_nCurrentPage].sFilePath = szFile;
        FILE* fp = NULL;
        _tfopen_s(&fp, szFile, _T("wb"));
        if (fp)
        {
            CDuiString sData = m_pages[m_nCurrentPage].sXmlContent;
#ifdef UNICODE
            int nLen = WideCharToMultiByte(CP_UTF8, 0, sData.GetData(), -1, NULL, 0, NULL, NULL);
            if (nLen > 0)
            {
                char* pUtf8 = new char[nLen];
                WideCharToMultiByte(CP_UTF8, 0, sData.GetData(), -1, pUtf8, nLen, NULL, NULL);
                fwrite(pUtf8, 1, nLen - 1, fp);
                delete[] pUtf8;
            }
#else
            fwrite(sData.GetData(), sizeof(TCHAR), sData.GetLength(), fp);
#endif
            fclose(fp);
        }
    }
}

void CEditorWnd::SaveCanvasToXml(CDuiString& sXml)
{
    if (!m_pCanvas) return;
    sXml = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
    sXml += _T("<Window size=\"800,600\">\r\n  <VerticalLayout>\r\n");
    sXml += _T("    <HorizontalLayout height=\"30\" bkcolor=\"#FF2196F3\">\r\n");
    sXml += _T("      <Label text=\"页面预览\" textcolor=\"#FFFFFFFF\" padding=\"8,0,0,0\"/>\r\n");
    sXml += _T("      <Button name=\"closebtn\" text=\"X\" width=\"30\" height=\"30\" bkcolor=\"#FFCC0000\" textcolor=\"#FFFFFFFF\" align=\"center\"/>\r\n");
    sXml += _T("    </HorizontalLayout>\r\n");
    sXml += _T("    <Container name=\"canvas\" bkcolor=\"#FFFFFFFF\">\r\n");
    for (int i = 0; i < m_pCanvas->GetCount(); ++i)
    {
        CControlUI* pCtrl = m_pCanvas->GetItemAt(i);
        CDuiSize xy = pCtrl->GetFixedXY();
        int w = pCtrl->GetFixedWidth();
        int h = pCtrl->GetFixedHeight();
        TCHAR szLine[512] = {0};
        _stprintf(szLine, _T("      <%s name=\"%s\" float=\"true\" pos=\"%d,%d,%d,%d\""),
            pCtrl->GetClass(), pCtrl->GetName().GetData(), xy.cx, xy.cy, xy.cx + w, xy.cy + h);
        sXml += szLine;
        if (pCtrl->GetText().GetLength() > 0)
        {
            _stprintf(szLine, _T(" text=\"%s\""), pCtrl->GetText().GetData());
            sXml += szLine;
        }
        if (pCtrl->GetBkColor() != 0)
        {
            _stprintf(szLine, _T(" bkcolor=\"#%08X\""), pCtrl->GetBkColor());
            sXml += szLine;
        }
        sXml += _T("/>\r\n");
    }
    sXml += _T("    </Container>\r\n  </VerticalLayout>\r\n</Window>\r\n");
}

void CEditorWnd::LoadPageFromXml(LPCTSTR pszXml)
{
    if (!m_pCanvas || !pszXml) return;
    m_pCanvas->RemoveAll();
    ClearSelection();
    CDialogBuilder builder;
    CControlUI* pRoot = builder.Create(pszXml, NULL, this, &m_pm, NULL);
    if (pRoot)
    {
        CContainerUI* pRootContainer = static_cast<CContainerUI*>(pRoot);
        CContainerUI* pContainer = static_cast<CContainerUI*>(pRootContainer->FindSubControl(_T("canvas")));
        if (pContainer)
        {
            for (int i = 0; i < pContainer->GetCount(); ++i)
            {
                CControlUI* pCtrl = pContainer->GetItemAt(i);
                pContainer->SetAutoDestroy(false);
                m_pCanvas->Add(pCtrl);
            }
        }
        delete pRoot;
    }
    RefreshCanvasComponentList();
}

void CEditorWnd::AddPageTab(LPCTSTR pszName, int index)
{
    if (!m_pPageTabs) return;
    CDuiString sBtnName;
    sBtnName.Format(_T("page_tab_%d"), index);
    CButtonUI* pBtn = new CButtonUI;
    pBtn->SetName(sBtnName);
    pBtn->SetText(pszName);
    pBtn->SetFixedHeight(22);
    pBtn->SetFixedWidth(80);
    pBtn->SetBkColor(0xFFE0E0E0);
    pBtn->SetBorderSize(1);
    pBtn->SetBorderColor(0xFFCCCCCC);
    m_pPageTabs->Add(pBtn);
}

void CEditorWnd::SwitchToPage(int index)
{
    if (index < 0 || index >= (int)m_pages.size()) return;
    if (m_nCurrentPage >= 0 && m_nCurrentPage < (int)m_pages.size())
    {
        SaveCanvasToXml(m_pages[m_nCurrentPage].sXmlContent);
    }
    m_nCurrentPage = index;
    LoadPageFromXml(m_pages[index].sXmlContent.GetData());
}

void CEditorWnd::OnLoadPlugin()
{
    TCHAR szExePath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szExePath, MAX_PATH);
    TCHAR* p = _tcsrchr(szExePath, _T('\\'));
    if (p) *p = 0;
    CDuiString sPluginDir = szExePath;

    OPENFILENAME ofn = {0};
    TCHAR szFile[MAX_PATH] = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = _T("Plugin DLL\0*.dll\0All Files\0*.*\0");
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrInitialDir = sPluginDir.GetData();
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (GetOpenFileName(&ofn))
    {
        CPluginManager::GetInstance().LoadPlugin(szFile, &m_pm);
        RefreshComponentList();
    }
}

void CEditorWnd::RefreshComponentList()
{
    InitComponentList();
}