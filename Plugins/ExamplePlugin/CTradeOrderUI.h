#ifndef __CTRADEORDERUI_H__
#define __CTRADEORDERUI_H__

#include "StdAfx.h"

class CTradeOrderUI : public DuiLib::CContainerUI
{
    DECLARE_DUICONTROL(CTradeOrderUI)
public:
    CTradeOrderUI();

    LPCTSTR GetClass() const;
    LPVOID  GetInterface(LPCTSTR pstrName);
    LPCTSTR GetDescription() const;

private:
    void BuildUI();

    DuiLib::CComboUI*       comboInstrument_;
    DuiLib::CEditUI*        editPrice_;
    DuiLib::CEditUI*        editVolume_;
    DuiLib::COptionUI*      optBuy_;
    DuiLib::COptionUI*      optSell_;
    DuiLib::CButtonUI*      btnOrder_;
};

#endif