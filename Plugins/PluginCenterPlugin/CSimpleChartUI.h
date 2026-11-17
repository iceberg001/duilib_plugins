#ifndef __CSIMPLECHARTUI_H__
#define __CSIMPLECHARTUI_H__

#include "StdAfx.h"

class CSimpleChartUI : public DuiLib::CContainerUI
{
    DECLARE_DUICONTROL(CSimpleChartUI)
public:
    CSimpleChartUI();

    LPCTSTR GetClass() const;
    LPVOID  GetInterface(LPCTSTR pstrName);

private:
    void BuildUI();

    DuiLib::CButtonUI* btnRefresh_;
    DuiLib::CLabelUI*  labelTitle_;
    DuiLib::CContainerUI* chartArea_;
    std::vector<int> chartData_;
};

#endif