#include "StdAfx.h"
#include "MainWnd.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    HRESULT Hr = ::CoInitialize(NULL);
    if (FAILED(Hr)) return 0;

    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourceType(UILIB_FILE);

    CMainWnd* pMainWnd = new CMainWnd();
    if (pMainWnd == NULL) return 0;
    pMainWnd->Create(NULL, _T("插件中心 - Plugin Center Demo"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 900, 600);
    pMainWnd->CenterWindow();

    CPaintManagerUI::MessageLoop();


    CPaintManagerUI::Term();
    ::CoUninitialize();

    return 0;
}