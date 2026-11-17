#ifndef __CMARKETPRICEUI_H__
#define __CMARKETPRICEUI_H__

#include "StdAfx.h"

class CMarketPriceUI : public DuiLib::CContainerUI
{
    DECLARE_DUICONTROL(CMarketPriceUI)
public:
    CMarketPriceUI();

    LPCTSTR GetClass() const;
    LPVOID  GetInterface(LPCTSTR pstrName);
    LPCTSTR GetDescription() const;

private:
    void BuildUI();
    void AddPriceRow(DuiLib::CVerticalLayoutUI* pParent, LPCTSTR pszLabel, LPCTSTR pszPrice, LPCTSTR pszVolume, DWORD dwColor);
};

#endif