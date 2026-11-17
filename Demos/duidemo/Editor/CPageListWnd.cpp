#include "StdAfx.h"
#include "CPageListWnd.h"
#include "CPageWnd.h"
#include "../../DuiLib/Core/Plugin/CPluginManager.h"
#include <shlobj.h>

using namespace DuiLib;

DUI_BEGIN_MESSAGE_MAP(CPageListWnd, WindowImplBase)
    DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
    DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnListItemClick)
DUI_END_MESSAGE_MAP()

CPageListWnd::CPageListWnd()
    : m_pPageList(NULL), m_pStatusLabel(NULL)
{
}

CPageListWnd::~CPageListWnd() {}

CDuiString CPageListWnd::GetSkinFile() { return _T("XML_PAGE_LIST"); }
LPCTSTR CPageListWnd::GetWindowClassName() const { return _T("PageListWnd"); }
UINT CPageListWnd::GetClassStyle() const { return CS_DBLCLKS; }

void CPageListWnd::OnFinalMessage(HWND hWnd)
{
    __super::OnFinalMessage(hWnd);
    delete this;
}

void CPageListWnd::InitWindow()
{
    m_pPageList = static_cast<CListUI*>(m_pm.FindControl(_T("page_list")));
    m_pStatusLabel = static_cast<CLabelUI*>(m_pm.FindControl(_T("status_label")));
    ScanPages();
}

CControlUI* CPageListWnd::CreateControl(LPCTSTR pstrClass)
{
    return CControlFactory::GetInstance()->CreateControl(pstrClass);
}

CDuiString CPageListWnd::GetPagesDir()
{
    TCHAR szPath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szPath, MAX_PATH);
    TCHAR* p = _tcsrchr(szPath, _T('\\'));
    if (p) *p = 0;
    CDuiString sDir = szPath;
    sDir += _T("\\skin\\duidemo\\pages");
    return sDir;
}

void CPageListWnd::ScanPages()
{
    if (!m_pPageList) return;
    m_pPageList->RemoveAll();

    CDuiString sDir = GetPagesDir();
    CreateDirectory(sDir.GetData(), NULL);

    CDuiString sSearch = sDir + _T("\\*.xml");

    WIN32_FIND_DATA fd = {0};
    HANDLE hFind = FindFirstFile(sSearch.GetData(), &fd);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        if (m_pStatusLabel) m_pStatusLabel->SetText(_T("暂无保存的页面，请在编辑器中保存页面"));
        return;
    }

    int nCount = 0;
    do
    {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            CListLabelElementUI* pItem = new CListLabelElementUI;
            pItem->SetText(fd.cFileName);
            pItem->SetFixedHeight(28);
            pItem->SetToolTip(fd.cFileName);
            pItem->SetToolTipWidth(300);
            m_pPageList->Add(pItem);
            nCount++;
        }
    } while (FindNextFile(hFind, &fd));
    FindClose(hFind);

    if (m_pStatusLabel)
    {
        CDuiString sStatus;
        sStatus.Format(_T("共 %d 个页面  目录: %s"), nCount, sDir.GetData());
        m_pStatusLabel->SetText(sStatus);
    }
}

void CPageListWnd::OpenSelectedPage()
{
    if (!m_pPageList) return;
    int nSel = m_pPageList->GetCurSel();
    if (nSel < 0) return;

    CListLabelElementUI* pItem = static_cast<CListLabelElementUI*>(m_pPageList->GetItemAt(nSel));
    if (!pItem) return;

    CDuiString sFile = GetPagesDir() + _T("\\") + pItem->GetText();

    CPageWnd* pPageWnd = new CPageWnd();
    pPageWnd->SetXmlFile(sFile.GetData());
    pPageWnd->Create(m_hWnd, pItem->GetText(), UI_WNDSTYLE_FRAME, 0L, 0, 0, 1024, 700);
    pPageWnd->CenterWindow();
    pPageWnd->ShowWindow();
}

void CPageListWnd::DeleteSelectedPage()
{
    if (!m_pPageList) return;
    int nSel = m_pPageList->GetCurSel();
    if (nSel < 0) return;

    CListLabelElementUI* pItem = static_cast<CListLabelElementUI*>(m_pPageList->GetItemAt(nSel));
    if (!pItem) return;

    CDuiString sFile = GetPagesDir() + _T("\\") + pItem->GetText();
    if (DeleteFile(sFile.GetData()))
    {
        m_pPageList->RemoveAt(nSel);
        if (m_pStatusLabel) m_pStatusLabel->SetText(_T("已删除页面"));
    }
}

void CPageListWnd::OnClick(TNotifyUI& msg)
{
    CDuiString sName = msg.pSender->GetName();
    if (sName == _T("closebtn")) { Close(); return; }
    if (sName == _T("open_btn")) { OpenSelectedPage(); return; }
    if (sName == _T("delete_btn")) { DeleteSelectedPage(); return; }
    if (sName == _T("refresh_btn")) { ScanPages(); return; }
}

void CPageListWnd::OnListItemClick(TNotifyUI& msg)
{
    OpenSelectedPage();
}

void CPageListWnd::Notify(TNotifyUI& msg)
{
    __super::Notify(msg);
}