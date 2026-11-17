#include "StdAfx.h"
#include "CSimpleChartUI.h"
#include <cstdlib>
#include <ctime>

IMPLEMENT_DUICONTROL(CSimpleChartUI)

CSimpleChartUI::CSimpleChartUI()
    : btnRefresh_(NULL)
    , labelTitle_(NULL)
    , chartArea_(NULL)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < 6; ++i)
        chartData_.push_back(rand() % 80 + 20);
    BuildUI();
}

LPCTSTR CSimpleChartUI::GetClass() const
{
    return _T("CSimpleChartUI");
}

LPVOID CSimpleChartUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcsicmp(pstrName, _T("CSimpleChartUI")) == 0)
        return this;
    return CContainerUI::GetInterface(pstrName);
}

void CSimpleChartUI::BuildUI()
{
    SetInset(RECT{4, 4, 4, 4});

    CVerticalLayoutUI* pMainLayout = new CVerticalLayoutUI;

    labelTitle_ = new CLabelUI;
    labelTitle_->SetText(_T("简单柱状图"));
    labelTitle_->SetFont(1);
    labelTitle_->SetTextStyle(DT_CENTER);
    labelTitle_->SetTextColor(0xFF333333);
    labelTitle_->SetFixedHeight(24);
    pMainLayout->Add(labelTitle_);

    CHorizontalLayoutUI* pToolbar = new CHorizontalLayoutUI;
    pToolbar->SetFixedHeight(30);

    btnRefresh_ = new CButtonUI;
    btnRefresh_->SetName(_T("btn_refresh_chart"));
    btnRefresh_->SetText(_T("刷新数据"));
    btnRefresh_->SetFixedWidth(80);
    btnRefresh_->SetBkColor(0xFF4CAF50);
    btnRefresh_->SetTextColor(0xFFFFFFFF);
    pToolbar->Add(btnRefresh_);

    CControlUI* pSpacer = new CControlUI;
    pToolbar->Add(pSpacer);
    pMainLayout->Add(pToolbar);

    chartArea_ = new CContainerUI;
    chartArea_->SetBkColor(0xFFF0F0F0);
    pMainLayout->Add(chartArea_);

    Add(pMainLayout);
}