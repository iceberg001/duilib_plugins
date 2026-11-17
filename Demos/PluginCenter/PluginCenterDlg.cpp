#include "StdAfx.h"
#include "PluginCenterDlg.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

using namespace DuiLib;

void WriteLog(const TCHAR* pszFormat, ...)
{
    TCHAR szBuffer[2048];
    va_list args;
    va_start(args, pszFormat);
    _vstprintf_s(szBuffer, 2048, pszFormat, args);
    va_end(args);

    TCHAR szPath[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, szPath, MAX_PATH);
    TCHAR* p = _tcsrchr(szPath, _T('\\'));
    if (p) *(p + 1) = 0;
    _tcscat_s(szPath, MAX_PATH, _T("plugin_center.log"));

    FILE* fp = NULL;
    _tfopen_s(&fp, szPath, _T("a"));
    if (fp)
    {
        time_t t = time(NULL);
        struct tm tmv;
        localtime_s(&tmv, &t);
        _ftprintf(fp, _T("[%04d-%02d-%02d %02d:%02d:%02d] %s\n"),
            tmv.tm_year + 1900, tmv.tm_mon + 1, tmv.tm_mday,
            tmv.tm_hour, tmv.tm_min, tmv.tm_sec, szBuffer);
        fclose(fp);
    }
}

CPluginCenterDlg::CPluginCenterDlg()
    : pMainWnd_(NULL)
{
}

CPluginCenterDlg::~CPluginCenterDlg()
{
}

void CPluginCenterDlg::SetMainWnd(CMainWnd* pMainWnd)
{
    pMainWnd_ = pMainWnd;
}

LPCTSTR CPluginCenterDlg::GetWindowClassName() const
{
    return _T("PluginCenterDlg");
}

UINT CPluginCenterDlg::GetClassStyle() const
{
    return CS_DBLCLKS;
}

void CPluginCenterDlg::OnFinalMessage(HWND hWnd)
{
    delete this;
}

void CPluginCenterDlg::InitWindow()
{
    WriteLog(_T("InitWindow called"));
    RefreshComponentList();
}

void CPluginCenterDlg::Notify(TNotifyUI& msg)
{
    if (msg.sType == _T("click"))
    {
        WriteLog(_T("Notify click: sender=%s"), msg.pSender->GetName().GetData());
        if (msg.pSender->GetName() == _T("btn_import"))
        {
            OnImportPlugin();
        }
        else if (msg.pSender->GetName() == _T("btn_install"))
        {
            OnInstallComponent();
        }
    }
    else if (msg.sType == _T("itemselect"))
    {
        WriteLog(_T("Notify itemselect: sender=%s"), msg.pSender->GetName().GetData());
        if (msg.pSender->GetName() == _T("component_list"))
        {
            OnSelectComponent();
        }
    }
}

LRESULT CPluginCenterDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        WriteLog(_T("WM_CREATE: initializing dialog"));
        m_pm.Init(m_hWnd);
        CDialogBuilder builder;
        CDuiString strResourcePath = m_pm.GetInstancePath();
        strResourcePath += _T("plugin_center_dlg.xml");
        WriteLog(_T("Loading XML: %s"), strResourcePath.GetData());
        CControlUI* pRoot = builder.Create(strResourcePath.GetData(), (UINT)0, NULL, &m_pm);
        if (pRoot == NULL)
        {
            WriteLog(_T("ERROR: Failed to load dialog XML"));
            MessageBox(m_hWnd, _T("加载布局文件失败"), _T("错误"), MB_OK);
            return -1;
        }
        WriteLog(_T("XML loaded successfully, root class=%s"), pRoot->GetClass());
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        InitWindow();
        return 0;
    }

    LRESULT lRes = 0;
    if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))
        return lRes;

    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CPluginCenterDlg::OnImportPlugin()
{
    WriteLog(_T("OnImportPlugin called"));

    TCHAR szFilter[] = _T("Plugin DLL (*.dll)\0*.dll\0All Files (*.*)\0*.*\0");
    TCHAR szFile[MAX_PATH] = { 0 };

    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = szFilter;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (!GetOpenFileName(&ofn))
    {
        WriteLog(_T("GetOpenFileName returned false or cancelled"));
        return;
    }

    WriteLog(_T("Selected DLL: %s"), szFile);

    CPluginManager& mgr = CPluginManager::GetInstance();
    CPaintManagerUI* pm = NULL;
    if (pMainWnd_)
    {
        pm = pMainWnd_->GetPaintManager();
    }
    WriteLog(_T("PaintManager ptr: %p"), pm);

    WriteLog(_T("Calling LoadPlugin..."));
    bool bResult = mgr.LoadPlugin(szFile, pm);
    WriteLog(_T("LoadPlugin returned: %s"), bResult ? _T("true") : _T("false"));

    if (!bResult)
    {
        MessageBox(m_hWnd, _T("加载插件失败"), _T("错误"), MB_OK);
        return;
    }

    WriteLog(_T("Plugin count after load: %d"), mgr.GetPluginCount());

    const LoadedPlugin* pLoaded = mgr.GetPlugin(mgr.GetPluginCount() - 1);
    if (pLoaded)
    {
        WriteLog(_T("Plugin info: name=%s, components=%s"),
            pLoaded->info.sName.GetData(),
            pLoaded->info.sComponents.GetData());
    }

    RefreshComponentList();
}

void CPluginCenterDlg::RefreshComponentList()
{
    WriteLog(_T("RefreshComponentList called"));

    CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("component_list")));
    if (!pList)
    {
        WriteLog(_T("ERROR: component_list not found"));
        return;
    }
    WriteLog(_T("component_list found, current count=%d"), pList->GetCount());

    pList->RemoveAll();
    WriteLog(_T("After RemoveAll, count=%d"), pList->GetCount());

    CPluginManager& mgr = CPluginManager::GetInstance();
    int nPluginCount = mgr.GetPluginCount();
    WriteLog(_T("Plugin count: %d"), nPluginCount);

    int nTotalItems = 0;
    for (int i = 0; i < nPluginCount; ++i)
    {
        const LoadedPlugin* pPlugin = mgr.GetPlugin(i);
        if (!pPlugin)
        {
            WriteLog(_T("Plugin %d: NULL"), i);
            continue;
        }

        WriteLog(_T("Plugin %d: name=%s, components=%s"),
            i, pPlugin->info.sName.GetData(), pPlugin->info.sComponents.GetData());

        CDuiString sComponents = pPlugin->info.sComponents;
        int start = 0;
        while (start >= 0)
        {
            int end = sComponents.Find(_T("|"), start);
            CDuiString sItem;
            if (end < 0)
                sItem = sComponents.Mid(start);
            else
                sItem = sComponents.Mid(start, end - start);

            WriteLog(_T("  Parsed component: '%s' (start=%d, end=%d)"), sItem.GetData(), start, end);

            if (!sItem.IsEmpty())
            {
                CListLabelElementUI* pElement = new CListLabelElementUI;
                pElement->SetFixedHeight(28);
                pElement->SetText(sItem);
                LPCTSTR pDesc = mgr.GetComponentDescription(sItem.GetData());
                if (pDesc) pElement->SetToolTip(pDesc);
                bool bAdded = pList->Add(pElement);
                WriteLog(_T("  Add returned: %s, text='%s'"), bAdded ? _T("true") : _T("false"), sItem.GetData());
                nTotalItems++;
            }

            if (end < 0) break;
            start = end + 1;
        }
    }

    WriteLog(_T("Total items added: %d, list count=%d"), nTotalItems, pList->GetCount());
}

void CPluginCenterDlg::OnSelectComponent()
{
    WriteLog(_T("OnSelectComponent called"));

    CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("component_list")));
    if (!pList) return;

    int nSel = pList->GetCurSel();
    WriteLog(_T("Selected index: %d"), nSel);
    if (nSel < 0) return;

    CListLabelElementUI* pElement = static_cast<CListLabelElementUI*>(pList->GetItemAt(nSel));
    if (!pElement) return;

    selectedComponent_ = pElement->GetText();
    WriteLog(_T("Selected: component=%s"), selectedComponent_.GetData());

    CPluginManager& mgr = CPluginManager::GetInstance();
    for (int i = 0; i < mgr.GetPluginCount(); ++i)
    {
        const LoadedPlugin* pPlugin = mgr.GetPlugin(i);
        if (!pPlugin) continue;
        CDuiString sComponents = pPlugin->info.sComponents;
        int start = 0;
        while (start >= 0)
        {
            int end = sComponents.Find(_T("|"), start);
            CDuiString sItem;
            if (end < 0)
                sItem = sComponents.Mid(start);
            else
                sItem = sComponents.Mid(start, end - start);
            if (sItem.CompareNoCase(selectedComponent_) == 0)
            {
                selectedPlugin_ = pPlugin->info.sName;
                WriteLog(_T("Found plugin: %s"), selectedPlugin_.GetData());
                break;
            }
            if (end < 0) break;
            start = end + 1;
        }
    }

    UpdateDescription();
}

void CPluginCenterDlg::UpdateDescription()
{
    CPluginManager& mgr = CPluginManager::GetInstance();
    LPCTSTR pszDesc = mgr.GetComponentDescription(selectedComponent_);
    WriteLog(_T("Description for %s: %s"), selectedComponent_.GetData(), pszDesc ? pszDesc : _T("(null)"));

    CTextUI* pDescText = static_cast<CTextUI*>(m_pm.FindControl(_T("desc_text")));
    if (pDescText)
    {
        if (pszDesc)
            pDescText->SetText(pszDesc);
        else
            pDescText->SetText(_T("无描述信息"));
    }

    CLabelUI* pCompName = static_cast<CLabelUI*>(m_pm.FindControl(_T("selected_component")));
    if (pCompName)
    {
        pCompName->SetText(selectedComponent_);
    }
}

void CPluginCenterDlg::OnInstallComponent()
{
    WriteLog(_T("OnInstallComponent: component=%s, plugin=%s"),
        selectedComponent_.GetData(), selectedPlugin_.GetData());

    if (selectedComponent_.IsEmpty())
    {
        MessageBox(m_hWnd, _T("请先选择一个组件"), _T("提示"), MB_OK);
        return;
    }

    if (pMainWnd_)
    {
        pMainWnd_->InstallComponent(selectedComponent_, selectedPlugin_);
    }

}