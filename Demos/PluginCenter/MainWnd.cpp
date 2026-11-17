#include "StdAfx.h"
#include "MainWnd.h"
#include "PluginCenterDlg.h"

using namespace DuiLib;

CMainWnd::CMainWnd()
    : rootContainer_(NULL)
    , activePane_(NULL)
{
}

CMainWnd::~CMainWnd()
{
}

LPCTSTR CMainWnd::GetWindowClassName() const
{
    return _T("PluginCenterMainWnd");
}

UINT CMainWnd::GetClassStyle() const
{
    return CS_DBLCLKS;
}

void CMainWnd::OnFinalMessage(HWND hWnd)
{
    delete this;
}

void CMainWnd::InitWindow()
{
    rootContainer_ = new CSplitContainerUI;
    rootContainer_->SetName(_T("root_container"));
    rootContainer_->SetSplitMode(CSplitContainerUI::None);

    CContainerUI* pContentArea = static_cast<CContainerUI*>(m_pm.FindControl(_T("content_area")));
    if (pContentArea)
    {
        pContentArea->Add(rootContainer_);
    }

    activePane_ = rootContainer_->GetFirstPane();
    AddPlaceholder(activePane_);
    UpdatePaneBorders();
}

void CMainWnd::AddPlaceholder(CContainerUI* pPane)
{
    if (!pPane) return;
    pPane->RemoveAll();
    CLabelUI* pPlaceholder = new CLabelUI;
    pPlaceholder->SetText(_T("请从插件中心安装组件"));
    pPlaceholder->SetTextStyle(DT_CENTER | DT_VCENTER);
    pPlaceholder->SetFont(2);
    pPlaceholder->SetTextColor(0xFF808080);
    pPane->Add(pPlaceholder);
}

void CMainWnd::Notify(TNotifyUI& msg)
{
    if (msg.sType == _T("click"))
    {
        if (msg.pSender->GetName() == _T("btn_plugin_center"))
        {
            OnPluginCenter();
        }
        else if (msg.pSender->GetName() == _T("btn_split_h"))
        {
            SplitActivePane(CSplitContainerUI::Horizontal);
        }
        else if (msg.pSender->GetName() == _T("btn_split_v"))
        {
            SplitActivePane(CSplitContainerUI::Vertical);
        }
        else if (msg.pSender->GetName() == _T("btn_reset"))
        {
            Reset();
        }
    }
}

LRESULT CMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        m_pm.Init(m_hWnd);
        CDialogBuilder builder;
        CDuiString strResourcePath = m_pm.GetInstancePath();
        strResourcePath += _T("plugin_center_main.xml");
        CControlUI* pRoot = builder.Create(strResourcePath.GetData(), (UINT)0, NULL, &m_pm);
        if (pRoot == NULL)
        {
            MessageBox(m_hWnd, _T("加载布局文件失败"), _T("错误"), MB_OK);
            return -1;
        }
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        InitWindow();
        return 0;
    }
    else if (uMsg == WM_DESTROY)
    {
        ::PostQuitMessage(0);
    }
    else if (uMsg == WM_LBUTTONDOWN)
    {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        CContainerUI* pPane = FindPaneAtPoint(pt);
        if (pPane)
        {
            SetActivePane(pPane);
        }
    }

    LRESULT lRes = 0;
    if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))
        return lRes;

    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CMainWnd::OnPluginCenter()
{
    CPluginCenterDlg* pDlg = new CPluginCenterDlg;
    pDlg->SetMainWnd(this);
    pDlg->Create(m_hWnd, _T("插件中心"), UI_WNDSTYLE_DIALOG, 0L, 0, 0, 600, 480);
    pDlg->CenterWindow();
    pDlg->ShowWindow(SW_SHOW);
}

CContainerUI* CMainWnd::FindPaneAtPoint(POINT pt)
{
    CControlUI* pControl = m_pm.FindControl(pt);
    if (!pControl) return NULL;

    while (pControl)
    {
        CSplitContainerUI* pSplit = static_cast<CSplitContainerUI*>(pControl->GetInterface(_T("CSplitContainerUI")));
        if (pSplit)
        {
            RECT rc = pControl->GetPos();
            if (PtInRect(&rc, pt))
            {
                CContainerUI* pPane1 = pSplit->GetFirstPane();
                CContainerUI* pPane2 = pSplit->GetSecondPane();

                if (pSplit->GetSplitMode() == CSplitContainerUI::None)
                {
                    return pPane1;
                }

                RECT rcPane1 = pPane1->GetPos();
                RECT rcPane2 = pPane2->GetPos();
                if (PtInRect(&rcPane1, pt))
                {
                    if (pPane1->GetCount() > 0 && pPane1->GetItemAt(0)->GetInterface(_T("CSplitContainerUI")))
                    {
                        pControl = pPane1->GetItemAt(0);
                        continue;
                    }
                    return pPane1;
                }
                if (PtInRect(&rcPane2, pt))
                {
                    if (pPane2->GetCount() > 0 && pPane2->GetItemAt(0)->GetInterface(_T("CSplitContainerUI")))
                    {
                        pControl = pPane2->GetItemAt(0);
                        continue;
                    }
                    return pPane2;
                }
                return NULL;
            }
        }
        pControl = pControl->GetParent();
    }
    return NULL;
}

void CMainWnd::SetActivePane(CContainerUI* pPane)
{
    if (!pPane) return;
    activePane_ = pPane;
    UpdatePaneBorders();
}

void CMainWnd::UpdatePaneBorders()
{
    if (!rootContainer_) return;

    struct BorderUpdater
    {
        CContainerUI* activePane;
        void Update(CSplitContainerUI* pSplit)
        {
            if (!pSplit) return;
            CContainerUI* pPane1 = pSplit->GetFirstPane();
            CContainerUI* pPane2 = pSplit->GetSecondPane();

            bool bPane1Leaf = true;
            bool bPane2Leaf = true;

            if (pPane1 && pPane1->GetCount() > 0 && pPane1->GetItemAt(0)->GetInterface(_T("CSplitContainerUI")))
                bPane1Leaf = false;
            if (pPane2 && pPane2->GetCount() > 0 && pPane2->GetItemAt(0)->GetInterface(_T("CSplitContainerUI")))
                bPane2Leaf = false;

            if (bPane1Leaf && pPane1)
            {
                if (pPane1 == activePane)
                {
                    pPane1->SetBorderSize(2);
                    pPane1->SetBorderColor(0xFF2196F3);
                }
                else
                {
                    pPane1->SetBorderSize(1);
                    pPane1->SetBorderColor(0xFFCCCCCC);
                }
            }

            if (bPane2Leaf && pPane2 && pSplit->GetSplitMode() != CSplitContainerUI::None)
            {
                if (pPane2 == activePane)
                {
                    pPane2->SetBorderSize(2);
                    pPane2->SetBorderColor(0xFF2196F3);
                }
                else
                {
                    pPane2->SetBorderSize(1);
                    pPane2->SetBorderColor(0xFFCCCCCC);
                }
            }

            if (pPane1 && pPane1->GetCount() > 0)
            {
                CSplitContainerUI* pChild = static_cast<CSplitContainerUI*>(pPane1->GetItemAt(0)->GetInterface(_T("CSplitContainerUI")));
                if (pChild) Update(pChild);
            }
            if (pPane2 && pPane2->GetCount() > 0 && pSplit->GetSplitMode() != CSplitContainerUI::None)
            {
                CSplitContainerUI* pChild = static_cast<CSplitContainerUI*>(pPane2->GetItemAt(0)->GetInterface(_T("CSplitContainerUI")));
                if (pChild) Update(pChild);
            }
        }
    };

    BorderUpdater updater;
    updater.activePane = activePane_;
    updater.Update(rootContainer_);
}

void CMainWnd::InstallComponent(const CDuiString& sClassName, const CDuiString& sPluginName)
{
    CControlUI* pControl = CControlFactory::GetInstance()->CreateControl(sClassName);
    if (!pControl)
    {
        CDuiString sMsg = _T("无法创建组件: ");
        sMsg += sClassName;
        MessageBox(m_hWnd, sMsg.GetData(), _T("错误"), MB_OK);
        return;
    }

    if (!activePane_)
    {
        activePane_ = rootContainer_->GetFirstPane();
    }

    activePane_->RemoveAll();
    activePane_->Add(pControl);

    InstalledComponent comp;
    comp.sClassName = sClassName;
    comp.sPluginName = sPluginName;
    comp.pControl = pControl;
    comp.pPane = activePane_;
    installedComponents_.push_back(comp);

    UpdatePaneBorders();
}

void CMainWnd::SplitActivePane(CSplitContainerUI::SplitMode mode)
{
    if (!activePane_) return;

    CSplitContainerUI* pParentSplit = NULL;
    CContainerUI* pParentPane = activePane_;

    while (pParentPane)
    {
        CControlUI* pParent = pParentPane->GetParent();
        if (!pParent) break;
        CSplitContainerUI* pSplit = static_cast<CSplitContainerUI*>(pParent->GetInterface(_T("CSplitContainerUI")));
        if (pSplit)
        {
            pParentSplit = pSplit;
            break;
        }
        pParentPane = static_cast<CContainerUI*>(pParent);
    }

    if (!pParentSplit)
    {
        pParentSplit = rootContainer_;
        pParentPane = rootContainer_->GetFirstPane();
    }

    bool bIsPane1 = (pParentSplit->GetFirstPane() == activePane_);

    if (pParentSplit->GetSplitMode() == CSplitContainerUI::None)
    {
        pParentSplit->SetSplitMode(mode);
        AddPlaceholder(pParentSplit->GetSecondPane());
        activePane_ = pParentSplit->GetFirstPane();
    }
    else
    {
        CSplitContainerUI* pNewSplit = new CSplitContainerUI;
        pNewSplit->SetSplitMode(mode);

        CControlUI* pOldContent = NULL;
        if (activePane_->GetCount() > 0)
        {
            pOldContent = activePane_->GetItemAt(0);
            activePane_->SetAutoDestroy(false);
            activePane_->RemoveAll();
            activePane_->SetAutoDestroy(true);
        }

        if (pOldContent)
        {
            pNewSplit->GetFirstPane()->Add(pOldContent);
        }
        else
        {
            AddPlaceholder(pNewSplit->GetFirstPane());
        }
        AddPlaceholder(pNewSplit->GetSecondPane());

        activePane_->Add(pNewSplit);
        activePane_ = pNewSplit->GetFirstPane();
    }

    UpdatePaneBorders();
}

void CMainWnd::Reset()
{
    CContainerUI* pContentArea = static_cast<CContainerUI*>(m_pm.FindControl(_T("content_area")));
    if (pContentArea)
    {
        pContentArea->RemoveAll();
    }

    installedComponents_.clear();

    rootContainer_ = new CSplitContainerUI;
    rootContainer_->SetName(_T("root_container"));
    rootContainer_->SetSplitMode(CSplitContainerUI::None);

    if (pContentArea)
    {
        pContentArea->Add(rootContainer_);
    }

    activePane_ = rootContainer_->GetFirstPane();
    AddPlaceholder(activePane_);
    UpdatePaneBorders();
}