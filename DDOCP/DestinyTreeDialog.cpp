// DestinyTreeDialog.cpp
//

#include "stdafx.h"
#include "DestinyTreeDialog.h"
#include "DDOCPDoc.h"
#include "GlobalSupportFunctions.h"
#include "SelectionSelectDialog.h"
#include "MainFrm.h"

namespace
{
    const size_t c_controlSpacing = 3;
    // destiny window size
    const size_t c_sizeX = 300;
    const size_t c_sizeY = 526;
    enum ImageBackgroundEnum
    {
        IBE_CoreEnhancementOverlay = 0,
        IBE_CoreInnateAbilityOverlay,
        IBE_CoreClickie,
        IBE_CorePassive,
        IBE_ItemClickie,
        IBE_ItemPassive,
        IBE_CoreClickieDisabled,
        IBE_CorePassiveDisabled,
        IBE_ItemClickieDisabled,
        IBE_ItemPassiveDisabled,
        IBE_ItemUnavailableClickie,
        IBE_ItemUnavailablePassive,
        IBE_ArrowLeft,
        IBE_ArrowRight,
        IBE_ArrowUp,
        IBE_LongArrowUp,
        IBE_ExtraLongArrowUp,
        IBE_PassiveAvailable,
        IBE_ActiveAvailable,
        IBE_PassiveNotAvailable,
        IBE_PassiveNotAllowed,
        IBE_ActiveNotAvailable,
        IBE_ActiveNotAllowed,
        IBE_CorePassiveAvailable,
        IBE_CoreActiveAvailable,
        IBE_ResetTreeDisabled,
        IBE_ResetTreeEnabled,
        IBE_count
    };
    const int c_leftOffsetCore = 16;
    const int c_leftOffsetItem = 11;
    const int c_topCore = 424;
    const int c_topItem1 = 430;
    const int c_xItemSpacingCore = 46;
    const int c_xItemSpacingItem = 59;
    const int c_yItemSpacing = 71;
    const int c_iconLeft = 10;
    const int c_iconTop = 484;
}
// global image data used for drawing all destiny trees.
// this is only initialised once (declared in EnhancementTreeDialog.cpp)
extern bool s_imagesInitiliased;
extern CImage s_imageBorders[IBE_count];

void CDestinyTreeDialog::InitialiseStaticImages()
{
    if (!s_imagesInitiliased)
    {
        // load all the standard static background images used when rendering
        // a destiny tree
        LoadImageFile(IT_ui, "CoreEnhancementOverlay", &s_imageBorders[IBE_CoreEnhancementOverlay]);
        LoadImageFile(IT_ui, "CoreInnateAbilityOverlay", &s_imageBorders[IBE_CoreInnateAbilityOverlay]);
        LoadImageFile(IT_ui, "CoreClickie", &s_imageBorders[IBE_CoreClickie]);
        LoadImageFile(IT_ui, "CorePassive", &s_imageBorders[IBE_CorePassive]);
        LoadImageFile(IT_ui, "ItemClickie", &s_imageBorders[IBE_ItemClickie]);
        LoadImageFile(IT_ui, "ItemPassive", &s_imageBorders[IBE_ItemPassive]);
        LoadImageFile(IT_ui, "CoreClickie", &s_imageBorders[IBE_CoreClickieDisabled]);
        LoadImageFile(IT_ui, "CorePassive", &s_imageBorders[IBE_CorePassiveDisabled]);
        LoadImageFile(IT_ui, "ItemClickie", &s_imageBorders[IBE_ItemClickieDisabled]);
        LoadImageFile(IT_ui, "ItemPassive", &s_imageBorders[IBE_ItemPassiveDisabled]);
        LoadImageFile(IT_ui, "ItemUnavailableClickie", &s_imageBorders[IBE_ItemUnavailableClickie]);
        LoadImageFile(IT_ui, "ItemUnavailablePassive", &s_imageBorders[IBE_ItemUnavailablePassive]);
        LoadImageFile(IT_ui, "EnhancementArrowLeft", &s_imageBorders[IBE_ArrowLeft]);
        LoadImageFile(IT_ui, "EnhancementArrowRight", &s_imageBorders[IBE_ArrowRight]);
        LoadImageFile(IT_ui, "EnhancementArrowUp", &s_imageBorders[IBE_ArrowUp]);
        LoadImageFile(IT_ui, "EnhancementLongArrowUp", &s_imageBorders[IBE_LongArrowUp]);
        LoadImageFile(IT_ui, "EnhancementExtraLongArrowUp", &s_imageBorders[IBE_ExtraLongArrowUp]);
        LoadImageFile(IT_ui, "PassiveAvailable", &s_imageBorders[IBE_PassiveAvailable]);
        LoadImageFile(IT_ui, "ActiveAvailable", &s_imageBorders[IBE_ActiveAvailable]);
        LoadImageFile(IT_ui, "PassiveNotAvailable", &s_imageBorders[IBE_PassiveNotAvailable]);
        LoadImageFile(IT_ui, "PassiveNotAllowed", &s_imageBorders[IBE_PassiveNotAllowed]);
        LoadImageFile(IT_ui, "ActiveNotAvailable", &s_imageBorders[IBE_ActiveNotAvailable]);
        LoadImageFile(IT_ui, "ActiveNotAllowed", &s_imageBorders[IBE_ActiveNotAllowed]);
        LoadImageFile(IT_ui, "CorePassiveAvailable", &s_imageBorders[IBE_CorePassiveAvailable]);
        LoadImageFile(IT_ui, "CoreActiveAvailable", &s_imageBorders[IBE_CoreActiveAvailable]);
        LoadImageFile(IT_ui, "ResetTreeEnabled", &s_imageBorders[IBE_ResetTreeEnabled]);
        LoadImageFile(IT_ui, "ResetTreeEnabled", &s_imageBorders[IBE_ResetTreeDisabled]);
        for (size_t i = 0; i < IBE_count; ++i)
        {
            s_imageBorders[i].SetTransparentColor(c_transparentColour);
        }
        MakeGrayScale(&s_imageBorders[IBE_CoreClickieDisabled], c_transparentColour);
        MakeGrayScale(&s_imageBorders[IBE_CorePassiveDisabled], c_transparentColour);
        MakeGrayScale(&s_imageBorders[IBE_ItemClickieDisabled], c_transparentColour);
        MakeGrayScale(&s_imageBorders[IBE_ItemPassiveDisabled], c_transparentColour);
        MakeGrayScale(&s_imageBorders[IBE_ResetTreeDisabled], c_transparentColour);
        s_imagesInitiliased = true;
    }
}

#pragma warning(push)
#pragma warning(disable: 4407) // warning C4407: cast between different pointer to member representations, compiler may generate incorrect code
BEGIN_MESSAGE_MAP(CDestinyTreeDialog, CDialog)
    //{{AFX_MSG_MAP(CDestinyTreeDialog)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
    ON_WM_MOUSEMOVE()
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()
#pragma warning(pop)

CDestinyTreeDialog::CDestinyTreeDialog(
        CWnd* pParent,
        Character * pCharacter,
        const EnhancementTree & tree,
        TreeType type) :
    CDialog(CDestinyTreeDialog::IDD, pParent),
    m_tree(tree),
    m_type(type),
    m_pCharacter(pCharacter),
    m_bitmapSize(CSize(0, 0)),
    m_bCreateHitBoxes(false),
    m_tipCreated(false),
    m_pTooltipItem(NULL)
{
    InitialiseStaticImages();
    //{{AFX_DATA_INIT(CDestinyTreeDialog)
    //}}AFX_DATA_INIT
    LoadImageFile(IT_ui, m_tree.Background(), &m_imageBackground);
    m_pCharacter->AttachObserver(this);
}

void CDestinyTreeDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDestinyTreeDialog)
    //}}AFX_DATA_MAP
}

BOOL CDestinyTreeDialog::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_tooltip.Create(this);
    m_tipCreated = true;

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDestinyTreeDialog::OnEraseBkgnd(CDC* pDC)
{
    return 0;
}

void CDestinyTreeDialog::OnPaint()
{
    CPaintDC pdc(this); // validates the client area on destruction
    pdc.SaveDC();
    // ensure we have a background bitmap
    if (m_bitmapSize == CSize(0, 0))
    {
        m_cachedDisplay.DeleteObject(); // ensure
        // create the bitmap we will render to
        m_cachedDisplay.CreateCompatibleBitmap(
                &pdc,
                c_sizeX,
                c_sizeY);
        m_bitmapSize = CSize(c_sizeX, c_sizeY);
    }
    // draw to a compatible device context and then splat to screen
    CDC memoryDc;
    memoryDc.CreateCompatibleDC(&pdc);
    memoryDc.SaveDC();
    memoryDc.SelectObject(&m_cachedDisplay);
    memoryDc.SelectStockObject(DEFAULT_GUI_FONT);

    // first render the tree background
    m_imageBackground.BitBlt(
            memoryDc.GetSafeHdc(),
            CRect(0, 0, c_sizeX, c_sizeY),
            CPoint(0, 0),
            SRCCOPY);
    // the rest of the stuff only gets added
    // if this tree has enhancement tree items. The tree could be an empty tree
    if (m_tree.Items().size() > 0)
    {
        // add the core innate ability overlay
        s_imageBorders[IBE_CoreInnateAbilityOverlay].TransparentBlt(
                memoryDc.GetSafeHdc(),
                0,
                c_sizeY - 110,
                c_sizeX,
                71);

        // add the tree icon and name
        CImage image;
        LoadImageFile(IT_ui, m_tree.Icon(), &image);
        image.SetTransparentColor(c_transparentColour);
        image.TransparentBlt(
                memoryDc.GetSafeHdc(),
                c_iconLeft,
                c_iconTop,
                32,
                32);
        CSize textSize = memoryDc.GetTextExtent(m_tree.Name().c_str());
        memoryDc.SetBkMode(TRANSPARENT);
        memoryDc.SetTextColor(RGB(255, 255, 255));  // white
        memoryDc.TextOut(
                c_iconLeft + 32 + c_controlSpacing,
                c_iconTop + (32 - textSize.cy) / 2,
                m_tree.Name().c_str());

        // now number of AP spent in tree
        size_t spentInTree = m_pCharacter->APSpentInTree(m_tree.Name());
        size_t available = m_pCharacter->TotalPointsAvailable(m_tree.Name(), m_type);
        CString text;
        text.Format("%d of %d Spent", spentInTree, available);
        textSize = memoryDc.GetTextExtent(text);
        memoryDc.TextOut(
                c_sizeX - textSize.cx - c_controlSpacing,
                c_iconTop - 3,
                text);

        // show the Reset Tree button in the correct state
        if (spentInTree > 0)
        {
            // reset tree is available if the user has spent points within it
            s_imageBorders[IBE_ResetTreeEnabled].TransparentBlt(
                    memoryDc.GetSafeHdc(),
                    c_sizeX - 79 - c_controlSpacing,
                    c_sizeY - 24 - c_controlSpacing,
                    79,
                    24);
        }
        else
        {
            // reset not available
            s_imageBorders[IBE_ResetTreeDisabled].TransparentBlt(
                    memoryDc.GetSafeHdc(),
                    c_sizeX - 79 - c_controlSpacing,
                    c_sizeY - 24 - c_controlSpacing,
                    79,
                    24);
        }
        // use a smaller font for drawing any x/y when items are rendered
        LOGFONT lf;
        ZeroMemory((PVOID)&lf, sizeof(LOGFONT));
        strcpy_s(lf.lfFaceName, "Consolas");
        lf.lfHeight = 11;
        CFont smallFont;
        smallFont.CreateFontIndirect(&lf);
        CFont * pOldFont = memoryDc.SelectObject(&smallFont);

        m_bCreateHitBoxes = (m_hitBoxes.size() == 0);
        // now add all the tree enhancements and their states
        const std::vector<EnhancementTreeItem> & items = m_tree.Items();
        std::vector<EnhancementTreeItem>::const_iterator it = items.begin();
        while (it != items.end())
        {
            RenderTreeItem((*it), &memoryDc);
            ++it;
        }
        memoryDc.SelectObject(pOldFont);
    }
    // now draw to display
    pdc.BitBlt(
            0,
            0,
            m_bitmapSize.cx,
            m_bitmapSize.cy,
            &memoryDc,
            0,
            0,
            SRCCOPY);
    memoryDc.RestoreDC(-1);
    memoryDc.DeleteDC();
    pdc.RestoreDC(-1);
}

void CDestinyTreeDialog::RenderTreeItem(
        const EnhancementTreeItem & item,
        CDC * pDC)
{
    bool isCore = (item.InternalName().find("Core") != std::string::npos);

    if (isCore)
    {
        RenderItemCore(item, pDC);
    }
    else
    {
        // first render the items border image
        CRect itemRect(0, 0, 43, 53);
        // now apply the item position to the rectangle
        itemRect += CPoint(
                c_leftOffsetItem + c_xItemSpacingItem * item.XPosition(),
                c_topItem1 - c_yItemSpacing * item.YPosition());

        if (item.HasClickie())
        {
            RenderItemClickie(item, pDC, itemRect);
        }
        else
        {
            RenderItemPassive(item, pDC, itemRect);
        }
        // show how many have been acquired of max ranks
        bool isAllowed = item.IsAllowed(*m_pCharacter, "", m_tree.Name());
        if (isAllowed)
        {
            // only show trained x/y if item is allowed
            const TrainedEnhancement * te = m_pCharacter->IsTrained(item.InternalName(), "", TT_epicDestiny);
            CString text;
            text.Format(
                    "%d/%d",
                    (te == NULL) ? 0: te->Ranks(),
                    item.Ranks());
            CSize textSize = pDC->GetTextExtent(text);
            pDC->TextOut(
                    itemRect.left + (itemRect.Width() - textSize.cx) / 2,
                    itemRect.bottom - textSize.cy - 2,
                    text);
        }
        if (item.HasArrowLeft())
        {
            s_imageBorders[IBE_ArrowLeft].TransparentBlt(
                    pDC->GetSafeHdc(),
                    itemRect.left - 15,
                    itemRect.top + (itemRect.Height() - 14) / 2,
                    13,
                    14);
        }
        if (item.HasArrowRight())
        {
            s_imageBorders[IBE_ArrowRight].TransparentBlt(
                    pDC->GetSafeHdc(),
                    itemRect.right + 2,
                    itemRect.top + (itemRect.Height() - 14) / 2,
                    13,
                    14);
        }
        if (item.HasArrowUp())
        {
            s_imageBorders[IBE_ArrowUp].TransparentBlt(
                    pDC->GetSafeHdc(),
                    itemRect.left + (itemRect.Width() - 14) / 2,
                    itemRect.top - 17,
                    14,
                    15);
        }
        if (item.HasLongArrowUp())
        {
            s_imageBorders[IBE_LongArrowUp].TransparentBlt(
                    pDC->GetSafeHdc(),
                    itemRect.left + (itemRect.Width() - 14) / 2,
                    itemRect.top - 88,
                    14,
                    86);
        }
        if (item.HasExtraLongArrowUp())
        {
             s_imageBorders[IBE_ExtraLongArrowUp].TransparentBlt(
                    pDC->GetSafeHdc(),
                    itemRect.left + (itemRect.Width() - 14) / 2,
                    itemRect.top - 161,
                    14,
                    157);
       }
    }
}

void CDestinyTreeDialog::RenderItemCore(
        const EnhancementTreeItem & item,
        CDC * pDC)
{
    // core items are rendered across the bottom section of the bitmap
    CRect itemRect(0, 0, 38, 38);
    size_t spentInTree = m_pCharacter->APSpentInTree(m_tree.Name());
    bool isAllowed = item.IsAllowed(*m_pCharacter, "", m_tree.Name());
    bool isTrainable = item.CanTrain(*m_pCharacter, m_tree.Name(), spentInTree, m_type);
    // now apply the item position to the rectangle
    itemRect += CPoint(c_leftOffsetCore + c_xItemSpacingCore * item.XPosition(), c_topCore);
    if (item.HasClickie())
    {
        s_imageBorders[IBE_CoreClickie].TransparentBlt(
                pDC->GetSafeHdc(),
                itemRect.left,
                itemRect.top,
                itemRect.Width(),
                itemRect.Height());
    }
    else
    {
        s_imageBorders[IBE_CorePassive].TransparentBlt(
                pDC->GetSafeHdc(),
                itemRect.left,
                itemRect.top,
                itemRect.Width(),
                itemRect.Height());
    }
    // render the cores icon, this may depend on a selector sub item
    CRect iconRect(itemRect);
    iconRect.DeflateRect(3, 3, 3, 3);
    ASSERT(iconRect.Width() == 32);
    ASSERT(iconRect.Height() == 32);
    RenderItemSelection(item, pDC, iconRect);

    // show the core index number
    CString text;
    text.Format(
            "%d",
            item.XPosition());
    CSize textSize = pDC->GetTextExtent(text);
    pDC->TextOut(
            itemRect.left + (itemRect.Width() - textSize.cx) / 2,
            itemRect.bottom + 2,
            text);

    if (m_bCreateHitBoxes)
    {
        DestinyHitBox hitBox(item, itemRect);
        m_hitBoxes.push_back(hitBox);
    }

    if (isTrainable && isAllowed)
    {
        // available outline is larger
        itemRect.InflateRect(5, 5, 5, 5);
        s_imageBorders[item.HasClickie() ? IBE_CoreActiveAvailable : IBE_CorePassiveAvailable].TransparentBlt(
                pDC->GetSafeHdc(),
                itemRect.left,
                itemRect.top,
                itemRect.Width(),
                itemRect.Height());
    }
}

void CDestinyTreeDialog::RenderItemClickie(
        const EnhancementTreeItem & item,
        CDC * pDC,
        CRect itemRect)
{
    // it has a square active border
    s_imageBorders[IBE_ItemClickie].TransparentBlt(
            pDC->GetSafeHdc(),
            itemRect.left,
            itemRect.top,
            itemRect.Width(),
            itemRect.Height());
    // show whether it can be trained or not
    RenderItemState(item, pDC, itemRect);
    // render the items icon, this may depend on a selector sub item
    CRect iconRect(itemRect);
    iconRect.DeflateRect(6, 7, 5, 14);
    ASSERT(iconRect.Width() == 32);
    ASSERT(iconRect.Height() == 32);
    RenderItemSelection(item, pDC, iconRect);

    if (m_bCreateHitBoxes)
    {
        DestinyHitBox hitBox(item, itemRect);
        m_hitBoxes.push_back(hitBox);
    }
}

void CDestinyTreeDialog::RenderItemPassive(
        const EnhancementTreeItem & item,
        CDC * pDC,
        CRect itemRect)
{
    // it has a passive hexagonal border
    s_imageBorders[IBE_ItemPassive].TransparentBlt(
            pDC->GetSafeHdc(),
            itemRect.left,
            itemRect.top,
            itemRect.Width(),
            itemRect.Height());
    // show whether it can be trained or not
    RenderItemState(item, pDC, itemRect);
    // render the items icon, this may depend on a selector sub item
    CRect iconRect(itemRect);
    iconRect.DeflateRect(6, 6, 5, 15);
    ASSERT(iconRect.Width() == 32);
    ASSERT(iconRect.Height() == 32);
    RenderItemSelection(item, pDC, iconRect);

    if (m_bCreateHitBoxes)
    {
        DestinyHitBox hitBox(item, itemRect);
        m_hitBoxes.push_back(hitBox);
    }
}

void CDestinyTreeDialog::RenderItemState(
        const EnhancementTreeItem & item,
        CDC * pDC,
        CRect itemRect) // work on a copy
{
    size_t spentInTree = m_pCharacter->APSpentInTree(m_tree.Name());
    bool isAllowed = item.IsAllowed(*m_pCharacter, "", m_tree.Name());
    bool isTrainable = item.CanTrain(*m_pCharacter, m_tree.Name(), spentInTree, m_type);
    if (!isAllowed)
    {
        s_imageBorders[item.HasClickie() ? IBE_ActiveNotAllowed : IBE_PassiveNotAllowed].TransparentBlt(
                pDC->GetSafeHdc(),
                itemRect.left,
                itemRect.top,
                itemRect.Width(),
                itemRect.Height());
    }
    else if (isTrainable)
    {
        // available outline is larger
        itemRect.InflateRect(4, 4, 4, 4);
        s_imageBorders[item.HasClickie() ? IBE_ActiveAvailable : IBE_PassiveAvailable].TransparentBlt(
                pDC->GetSafeHdc(),
                itemRect.left,
                itemRect.top,
                itemRect.Width(),
                itemRect.Height());
    }
}

void CDestinyTreeDialog::RenderItemSelection(
        const EnhancementTreeItem & item,
        CDC * pDC,
        const CRect & itemRect)
{
    if (m_pCharacter != NULL)
    {
        item.RenderIcon(*m_pCharacter, pDC, itemRect);
    }
}

void CDestinyTreeDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialog::OnLButtonDown(nFlags, point);
    if (m_tree.Items().size() > 0)
    {
        // determine which enhancement has been clicked on
        const EnhancementTreeItem * item = FindByPoint();
        if (item != NULL)
        {
            // an item has been clicked, see if we can train a rank in it
            size_t spentInTree = m_pCharacter->APSpentInTree(m_tree.Name());
            const TrainedEnhancement * te = m_pCharacter->IsTrained(item->InternalName(), "", TT_epicDestiny);
            bool isAllowed = item->MeetRequirements(*m_pCharacter, "", m_tree.Name());
            bool isTrainable = item->CanTrain(*m_pCharacter, m_tree.Name(), spentInTree, m_type);
            ASSERT(m_type == TT_epicDestiny);
            if (isAllowed && isTrainable)
            {
                // if its a selector that needs selecting do selection, else
                // just train it
                if (item->HasSelections()
                        && te == NULL)
                {
                    // need to show the selection dialog
                    CSelectionSelectDialog dlg(
                            AfxGetApp()->m_pMainWnd->GetActiveWindow(),
                            *m_pCharacter,
                            *item,
                            m_tree.Name(),
                            m_type);
                    // no tooltips while a dialog is displayed
                    GetMouseHook()->SaveState();
                    if (dlg.DoModal() == IDOK)
                    {
                        // they made a valid selection
                        m_pCharacter->EpicDestiny_TrainEnhancement(
                                m_tree.Name(),
                                item->InternalName(),
                                dlg.Selection(),
                                dlg.Cost());
                        Invalidate();
                    }
                    GetMouseHook()->RestoreState();
                }
                else
                {
                    if (te == NULL          // if its not trained
                            || te->Ranks() < item->Ranks()) // or has ranks still to be trained
                    {
                        m_pCharacter->EpicDestiny_TrainEnhancement(
                                m_tree.Name(),
                                item->InternalName(),
                                (te != NULL && te->HasSelection()) ? te->Selection() : "",
                                item->Cost());
                        Invalidate();
                    }
                }
            }
            else
            {
                // although this item cannot be trained, still allow the selection
                // dialog to be displayed for item if it has one.
                if (item->HasSelections()
                        && te == NULL)
                {
                    // need to show the selection dialog
                    CSelectionSelectDialog dlg(
                            AfxGetApp()->m_pMainWnd->GetActiveWindow(),
                            *m_pCharacter,
                            *item,
                            m_tree.Name(),
                            m_type);
                    // no tooltips while a dialog is displayed
                    GetMouseHook()->SaveState();
                    dlg.DoModal();
                    GetMouseHook()->RestoreState();
                }
            }
        }
        // check for a click of the reset tree button
        CRect rctResetButton(
                c_sizeX - 79 - c_controlSpacing,
                c_sizeY - 24 - c_controlSpacing,
                c_sizeX - c_controlSpacing,
                c_sizeY - c_controlSpacing);
        if (rctResetButton.PtInRect(point))
        {
            size_t spentInTree = m_pCharacter->APSpentInTree(m_tree.Name());
            if (spentInTree > 0)
            {
                UINT ret = AfxMessageBox("Are you sure you want to reset this tree?", MB_YESNO | MB_ICONWARNING);
                if (ret == IDYES)
                {
                    // lets go for a tree reset action
                    m_pCharacter->EpicDestiny_ResetEnhancementTree(m_tree.Name(), false);
                }
            }
        }
    }
}

void CDestinyTreeDialog::OnRButtonDown(UINT nFlags, CPoint point)
{
    CDialog::OnRButtonDown(nFlags, point);
    // revoke the last enhancement trained in this tree
    if (m_pCharacter != NULL)
    {
        if (m_tree.Items().size() > 0)
        {
            ASSERT(m_type == TT_epicDestiny);
            m_pCharacter->EpicDestiny_RevokeEnhancement(m_tree.Name());
            Invalidate();   // redraw
        }
    }
}

const EnhancementTreeItem * CDestinyTreeDialog::FindByPoint(CRect * pRect) const
{
    CPoint point;
    GetCursorPos(&point);
    ScreenToClient(&point);
    // identify the item under the cursor
    const EnhancementTreeItem * item = NULL;
    std::vector<DestinyHitBox>::const_iterator it = m_hitBoxes.begin();
    while (item == NULL && it != m_hitBoxes.end())
    {
        if ((*it).IsInRect(point))
        {
            // mouse is over this item
            item = &(*it).Item();
            if (pRect != NULL)
            {
                *pRect = (*it).Rect();
            }
        }
        ++it;
    }
    return item;
}

void CDestinyTreeDialog::OnMouseMove(UINT nFlags, CPoint point)
{
    // determine which enhancement the mouse may be over
    CRect itemRect;
    const EnhancementTreeItem * item = FindByPoint(&itemRect);
    if (item != NULL
            && item != m_pTooltipItem)
    {
        // over a new item or a different item
        m_pTooltipItem = item;
        ShowTip(*item, itemRect);
    }
    else
    {
        if (m_showingTip
                && item != m_pTooltipItem)
        {
            // no longer over the same item
            HideTip();
        }
    }
    // as the mouse is over the enhancement tree, ensure the status bar message prompts available actions
    GetMainFrame()->SetStatusBarPromptText("Left click to train highlighted enhancement, right click to revoke last enhancement trained in this tree");
}

LRESULT CDestinyTreeDialog::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
    // hide any tooltip when the mouse leaves the area its being shown for
    HideTip();
    GetMainFrame()->SetStatusBarPromptText("Ready.");
    return 0;
}

void CDestinyTreeDialog::ShowTip(const EnhancementTreeItem & item, CRect itemRect)
{
    if (m_showingTip)
    {
        m_tooltip.Hide();
    }
    ClientToScreen(&itemRect);
    CPoint tipTopLeft(itemRect.left, itemRect.bottom + 2);
    CPoint tipAlternate(itemRect.left, itemRect.top - 2);
    SetTooltipText(item, tipTopLeft, tipAlternate);
    m_showingTip = true;
    // track the mouse so we know when it leaves our window
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.hwndTrack = m_hWnd;
    tme.dwFlags = TME_LEAVE;
    tme.dwHoverTime = 1;
    _TrackMouseEvent(&tme);
}

void CDestinyTreeDialog::HideTip()
{
    // tip not shown if not over an assay
    if (m_tipCreated && m_showingTip)
    {
        m_tooltip.Hide();
        m_showingTip = false;
        m_pTooltipItem = NULL;
    }
}

void CDestinyTreeDialog::SetTooltipText(
        const EnhancementTreeItem & item,
        CPoint tipTopLeft,
        CPoint tipAlternate)
{
    // different tooltip if the item is trained or not
    // if its trained, we need to show the selected sub-item tooltip
    const TrainedEnhancement * te = m_pCharacter->IsTrained(item.InternalName(), "", TT_epicDestiny);
    const EnhancementSelection * es = NULL;
    std::string selection;
    m_tooltip.SetOrigin(tipTopLeft, tipAlternate, false);
    if (te != NULL)
    {
        // this item is trained, we may need to show the selected sub-item tooltip text
        if (te->HasSelection())
        {
            selection = te->Selection();
            const Selector & selector = item.Selections();
            const std::vector<EnhancementSelection> & selections = selector.Selections();
            // find the selected item
            std::vector<EnhancementSelection>::const_iterator it = selections.begin();
            while (it != selections.end())
            {
                if ((*it).Name() == te->Selection())
                {
                    es = &(*it);
                    break;
                }
                ++it;
            }
        }
    }
    if (es != NULL)
    {
        m_tooltip.SetEnhancementSelectionItem(
                *m_pCharacter,
                &item,
                es,
                te->Ranks());
    }
    else
    {
        // its a top level item without sub-options
        m_tooltip.SetEnhancementTreeItem(
                *m_pCharacter,
                &item,
                selection,
                m_pCharacter->APSpentInTree(m_tree.Name()));
    }
    m_tooltip.Show();
}

// CharacterObserver overrides
void CDestinyTreeDialog::UpdateFeatTrained(
        Character * charData,
        const std::string & featName)
{
    // selectable enhancements may have changed
    Invalidate();
}

void CDestinyTreeDialog::UpdateFeatRevoked(
        Character * charData,
        const std::string & featName)
{
    // message only shown if bEnhancementsRevoked is true
    std::stringstream ss;
    ss << "Warning - Tree \"" << m_tree.Name() << "\" had some / all enhancements revoked as the "
            "requirements are no longer met.\r\n\r\n";
    bool bEnhancementsRevoked = false;
    // review all trained enhancements in this tree
    // if they are no longer valid, revoke enhancements in the
    // reverse order they were trained until all trained tree
    // enhancements are valid
    bool bTreeHasInvalidItems = false;
    const std::vector<EnhancementTreeItem> & items = m_tree.Items();
    do 
    {
        bTreeHasInvalidItems = false;
        EpicDestinySpendInTree * esit = m_pCharacter->EpicDestiny_FindTree(m_tree.Name());
        if (esit != NULL)
        {
            std::vector<TrainedEnhancement> enhancements = esit->Enhancements();
            std::vector<TrainedEnhancement>::const_iterator it = enhancements.begin();
            while (!bTreeHasInvalidItems && it != enhancements.end())
            {
                const EnhancementTreeItem * pItem = FindEnhancement((*it).EnhancementName());
                bool isAllowed = pItem->MeetRequirements(
                        *m_pCharacter,
                        (*it).HasSelection() ? (*it).Selection() : "",
                        m_tree.Name());
                if (!isAllowed)
                {
                    bTreeHasInvalidItems = true;
                    break;
                }
                ++it;
            }
        }
        if (bTreeHasInvalidItems)
        {
            // get the name of the tree item about to be revoked and add it to the
            // reported revoked message.
            std::string revokedEnhancement;
            std::string revokedSelection;
            // the tree does have invalid items in it.
            // revoke and item in this tree and try again to see if the tree is now valid
            m_pCharacter->EpicDestiny_RevokeEnhancement(
                    m_tree.Name(),
                    &revokedEnhancement,
                    &revokedSelection);
            bEnhancementsRevoked = true;
            const EnhancementTreeItem * pItem = FindEnhancement(revokedEnhancement);
            if (pItem != NULL)
            {
                // show the tier of the enhancement
                switch (pItem->YPosition())
                {
                case 0: ss << "Core  "; break;
                case 1: ss << "Tier1 "; break;
                case 2: ss << "Tier2 "; break;
                case 3: ss << "Tier3 "; break;
                case 4: ss << "Tier4 "; break;
                case 5: ss << "Tier5 "; break;
                case 6: ss << "Tier6 "; break;
                }
                ss << pItem->DisplayName(revokedSelection) << "\r\n";
            }
        }
    } while (bTreeHasInvalidItems);
    if (bEnhancementsRevoked)
    {
        // let the user know that some enhancements were revoked due to the feat change
        AfxMessageBox(ss.str().c_str(), MB_ICONWARNING);
    }
}

void CDestinyTreeDialog::UpdateEnhancementTreeReset(Character * charData)
{
    // just redraw for this one
    Invalidate();
}

void CDestinyTreeDialog::OnMButtonDown(UINT nFlags, CPoint point)
{
    // copy the bitmap content to the clipboard
    CDC screenDC;
    screenDC.Attach(::GetDC(NULL));
    CDC clipboardDC;
    CDC bitmapDC;
    CBitmap clipboardBitmap;

    bitmapDC.CreateCompatibleDC(&screenDC);
    bitmapDC.SaveDC();
    bitmapDC.SelectObject(&m_cachedDisplay);
    // draw to a compatible device context and then copy to clipboard
    clipboardDC.CreateCompatibleDC(&screenDC);
    clipboardDC.SaveDC();
    clipboardBitmap.CreateCompatibleBitmap(
            &screenDC,
            c_sizeX,
            c_sizeY);
    clipboardDC.SelectObject(&clipboardBitmap);
    clipboardDC.BitBlt(0, 0, c_sizeX, c_sizeY, &bitmapDC, 0, 0, SRCCOPY);
    clipboardDC.RestoreDC(-1);
    clipboardDC.DeleteDC();
    ::ReleaseDC(NULL, screenDC.Detach());

    bitmapDC.RestoreDC(-1);
    bitmapDC.DeleteDC();

    // Open the clipboard
    if (::OpenClipboard(NULL))
    {
        ::EmptyClipboard();
        SetClipboardData(CF_BITMAP, clipboardBitmap.Detach());     // clipboard now owns the object
        ::CloseClipboard();
    }
}
