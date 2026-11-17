#include "StdAfx.h"
#include "CPageWnd.h"

#include "../../DuiLib/Core/Plugin/CPluginManager.h"
#include "../../DuiLib/Core/CNodeUIManager.h"

using namespace DuiLib;

DUI_BEGIN_MESSAGE_MAP(CPageWnd, WindowImplBase)
    DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CPageWnd::CPageWnd() {}
CPageWnd::~CPageWnd() {}

void CPageWnd::SetXmlFile(LPCTSTR pszFile)
{
    if (pszFile) m_sXmlFile = pszFile;
    FILE* fp = NULL;
    _tfopen_s(&fp, pszFile, _T("rb"));
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        long nLen = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char* pBuf = new char[nLen + 1];
        fread(pBuf, 1, nLen, fp);
        pBuf[nLen] = 0;
        fclose(fp);
#ifdef UNICODE
        int nWide = MultiByteToWideChar(CP_UTF8, 0, pBuf, nLen, NULL, 0);
        LPWSTR pWide = new WCHAR[nWide + 1];
        MultiByteToWideChar(CP_UTF8, 0, pBuf, nLen, pWide, nWide);
        pWide[nWide] = 0;
        m_sXmlContent = pWide;
        delete[] pWide;
#else
        m_sXmlContent = pBuf;
#endif
        delete[] pBuf;
    }
}

CDuiString CPageWnd::GetSkinFile()
{
    if (!m_sXmlContent.IsEmpty()) return m_sXmlContent;
    return m_sXmlFile;
}

LPCTSTR CPageWnd::GetWindowClassName() const { return _T("PageWnd"); }
UINT CPageWnd::GetClassStyle() const { return CS_DBLCLKS; }

void CPageWnd::OnFinalMessage(HWND hWnd)
{
    CNodeUIManager::GetInstance().Clear();
    __super::OnFinalMessage(hWnd);
    delete this;
}

void CPageWnd::InitWindow()
{
}

CControlUI* CPageWnd::CreateControl(LPCTSTR pstrClass)
{

    CControlUI* pCtrl = CControlFactory::GetInstance()->CreateControl(pstrClass);
    if (pCtrl) return pCtrl;
    return CControlFactory::GetInstance()->CreateControl(CDuiString(pstrClass) + _T("UI"));
}

void CPageWnd::Notify(TNotifyUI& msg)
{
    CDuiString sName = msg.pSender->GetName();
    if (sName == _T("closebtn")) { Close(); return; }
    __super::Notify(msg);
}