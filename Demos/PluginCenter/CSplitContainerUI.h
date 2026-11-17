#ifndef __CSPLITCONTAINERUI_H__
#define __CSPLITCONTAINERUI_H__

#include "UIlib.h"

class CSplitContainerUI : public DuiLib::CContainerUI
{
    DECLARE_DUICONTROL(CSplitContainerUI)
public:
    enum SplitMode { None, Horizontal, Vertical };

    CSplitContainerUI();
    ~CSplitContainerUI();

    LPCTSTR GetClass() const;
    LPVOID  GetInterface(LPCTSTR pstrName);

    void SetSplitMode(SplitMode mode);
    SplitMode GetSplitMode() const;

    void SetSplitRatio(float ratio);
    float GetSplitRatio() const;

    DuiLib::CContainerUI* GetFirstPane() const;
    DuiLib::CContainerUI* GetSecondPane() const;

    void SetFirstPane(DuiLib::CControlUI* pControl);
    void SetSecondPane(DuiLib::CControlUI* pControl);

    void SetPos(RECT rc, bool bNeedInvalidate = true);
    void DoEvent(DuiLib::TEventUI& event);

    bool IsSplitterDragging() const { return bDragging_; }

private:
    void InitPanes();
    void LayoutChildren(RECT rc, bool bNeedInvalidate);

    DuiLib::CContainerUI* pane1_;
    DuiLib::CContainerUI* pane2_;
    DuiLib::CControlUI* splitter_;
    SplitMode splitMode_;
    float splitRatio_;
    bool bDragging_;
    POINT dragStartPos_;
    float dragStartRatio_;
};

#endif