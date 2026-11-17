#include "StdAfx.h"
#include "CSplitContainerUI.h"

using namespace DuiLib;

IMPLEMENT_DUICONTROL(CSplitContainerUI)

CSplitContainerUI::CSplitContainerUI()
    : pane1_(NULL)
    , pane2_(NULL)
    , splitter_(NULL)
    , splitMode_(None)
    , splitRatio_(0.5f)
    , bDragging_(false)
{
    InitPanes();
}

CSplitContainerUI::~CSplitContainerUI()
{
}

LPCTSTR CSplitContainerUI::GetClass() const
{
    return _T("CSplitContainerUI");
}

LPVOID CSplitContainerUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcsicmp(pstrName, _T("CSplitContainerUI")) == 0)
        return this;
    return CContainerUI::GetInterface(pstrName);
}

void CSplitContainerUI::InitPanes()
{
    pane1_ = new CContainerUI;
    pane1_->SetName(_T("pane1"));
    Add(pane1_);

    splitter_ = new CControlUI;
    splitter_->SetName(_T("splitter"));
    splitter_->SetFixedWidth(2);
    splitter_->SetFixedHeight(2);
    splitter_->SetBkColor(0x4A4A4A);
    splitter_->SetVisible(false);
    Add(splitter_);

    pane2_ = new CContainerUI;
    pane2_->SetName(_T("pane2"));
    pane2_->SetVisible(false);
    Add(pane2_);
}

void CSplitContainerUI::LayoutChildren(RECT rc, bool bNeedInvalidate)
{
    if (splitMode_ == None)
    {
        pane1_->SetPos(rc, bNeedInvalidate);
        return;
    }

    int nSplitter = 2;

    if (splitMode_ == Horizontal)
    {
        int nTotalWidth = rc.right - rc.left;
        int nPane1Width = (int)((nTotalWidth - nSplitter) * splitRatio_);
        if (nPane1Width < 20) nPane1Width = 20;
        int nPane2Width = nTotalWidth - nSplitter - nPane1Width;
        if (nPane2Width < 20) nPane2Width = 20;

        RECT rcPane1 = { rc.left, rc.top, rc.left + nPane1Width, rc.bottom };
        RECT rcSplitter = { rc.left + nPane1Width, rc.top, rc.left + nPane1Width + nSplitter, rc.bottom };
        RECT rcPane2 = { rc.left + nPane1Width + nSplitter, rc.top, rc.right, rc.bottom };

        pane1_->SetPos(rcPane1, bNeedInvalidate);
        splitter_->SetPos(rcSplitter, bNeedInvalidate);
        pane2_->SetPos(rcPane2, bNeedInvalidate);
    }
    else
    {
        int nTotalHeight = rc.bottom - rc.top;
        int nPane1Height = (int)((nTotalHeight - nSplitter) * splitRatio_);
        if (nPane1Height < 20) nPane1Height = 20;
        int nPane2Height = nTotalHeight - nSplitter - nPane1Height;
        if (nPane2Height < 20) nPane2Height = 20;

        RECT rcPane1 = { rc.left, rc.top, rc.right, rc.top + nPane1Height };
        RECT rcSplitter = { rc.left, rc.top + nPane1Height, rc.right, rc.top + nPane1Height + nSplitter };
        RECT rcPane2 = { rc.left, rc.top + nPane1Height + nSplitter, rc.right, rc.bottom };

        pane1_->SetPos(rcPane1, bNeedInvalidate);
        splitter_->SetPos(rcSplitter, bNeedInvalidate);
        pane2_->SetPos(rcPane2, bNeedInvalidate);
    }
}

void CSplitContainerUI::SetSplitMode(SplitMode mode)
{
    if (splitMode_ == mode) return;
    splitMode_ = mode;

    if (mode == None)
    {
        splitter_->SetVisible(false);
        pane2_->SetVisible(false);
    }
    else
    {
        splitter_->SetVisible(true);
        pane2_->SetVisible(true);
    }

    NeedParentUpdate();
}

CSplitContainerUI::SplitMode CSplitContainerUI::GetSplitMode() const
{
    return splitMode_;
}

void CSplitContainerUI::SetSplitRatio(float ratio)
{
    if (ratio < 0.05f) ratio = 0.05f;
    if (ratio > 0.95f) ratio = 0.95f;
    splitRatio_ = ratio;
    NeedParentUpdate();
}

float CSplitContainerUI::GetSplitRatio() const
{
    return splitRatio_;
}

CContainerUI* CSplitContainerUI::GetFirstPane() const
{
    return pane1_;
}

CContainerUI* CSplitContainerUI::GetSecondPane() const
{
    return pane2_;
}

void CSplitContainerUI::SetFirstPane(CControlUI* pControl)
{
    if (pane1_)
    {
        pane1_->RemoveAll();
        if (pControl)
            pane1_->Add(pControl);
    }
}

void CSplitContainerUI::SetSecondPane(CControlUI* pControl)
{
    if (pane2_)
    {
        pane2_->RemoveAll();
        if (pControl)
            pane2_->Add(pControl);
    }
}

void CSplitContainerUI::SetPos(RECT rc, bool bNeedInvalidate)
{
    CControlUI::SetPos(rc, bNeedInvalidate);
    LayoutChildren(rc, false);
}

void CSplitContainerUI::DoEvent(TEventUI& event)
{
    if (splitMode_ == None)
    {
        CContainerUI::DoEvent(event);
        return;
    }

    if (event.Type == UIEVENT_BUTTONDOWN && event.pSender == splitter_)
    {
        bDragging_ = true;
        dragStartPos_ = event.ptMouse;
        dragStartRatio_ = splitRatio_;
        return;
    }

    if (event.Type == UIEVENT_MOUSEMOVE && bDragging_)
    {
        RECT rc = GetPos();
        if (splitMode_ == Horizontal)
        {
            int nTotalWidth = rc.right - rc.left;
            int nSplitter = 2;
            int nDelta = event.ptMouse.x - dragStartPos_.x;
            float newRatio = dragStartRatio_ + (float)nDelta / (nTotalWidth - nSplitter);
            if (newRatio < 0.05f) newRatio = 0.05f;
            if (newRatio > 0.95f) newRatio = 0.95f;
            splitRatio_ = newRatio;
        }
        else
        {
            int nTotalHeight = rc.bottom - rc.top;
            int nSplitter = 2;
            int nDelta = event.ptMouse.y - dragStartPos_.y;
            float newRatio = dragStartRatio_ + (float)nDelta / (nTotalHeight - nSplitter);
            if (newRatio < 0.05f) newRatio = 0.05f;
            if (newRatio > 0.95f) newRatio = 0.95f;
            splitRatio_ = newRatio;
        }

        LayoutChildren(rc, false);
        Invalidate();
        return;
    }

    if (event.Type == UIEVENT_BUTTONUP && bDragging_)
    {
        bDragging_ = false;
        return;
    }

    CContainerUI::DoEvent(event);
}