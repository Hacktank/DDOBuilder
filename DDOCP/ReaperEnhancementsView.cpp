// ReaperEnhancementsView.cpp
//
#include "stdafx.h"
#include "ReaperEnhancementsView.h"

#include "DDOCP.h"
#include "EnhancementtreeDialog.h"
#include "GlobalSupportFunctions.h"

namespace
{
    const int c_controlSpacing = 3;
    const UINT UWM_NEW_DOCUMENT = ::RegisterWindowMessage(_T("NewActiveDocument"));
    // enhancement window size
    const size_t c_sizeX = 299;
    const size_t c_sizeY = 466;
}

IMPLEMENT_DYNCREATE(CReaperEnhancementsView, CFormView)

CReaperEnhancementsView::CReaperEnhancementsView() :
    CFormView(CReaperEnhancementsView::IDD),
    m_pCharacter(NULL),
    m_pDocument(NULL)
{
}

CReaperEnhancementsView::~CReaperEnhancementsView()
{
    for (size_t vi = 0; vi < m_treeViews.size(); ++vi)
    {
        delete m_treeViews[vi];
    }
}

void CReaperEnhancementsView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
}

#pragma warning(push)
#pragma warning(disable: 4407) // warning C4407: cast between different pointer to member representations, compiler may generate incorrect code
BEGIN_MESSAGE_MAP(CReaperEnhancementsView, CFormView)
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_REGISTERED_MESSAGE(UWM_NEW_DOCUMENT, OnNewDocument)
END_MESSAGE_MAP()
#pragma warning(pop)

#ifdef _DEBUG
void CReaperEnhancementsView::AssertValid() const
{
    CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CReaperEnhancementsView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CReaperEnhancementsView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    UpdateWindowTitle();
}

void CReaperEnhancementsView::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    if (m_treeViews.size() > 0
            && IsWindow(m_treeViews[0]->GetSafeHwnd()))
    {
        // we can position and show all the visible enhancement windows
        // with scrolls bars as required

        // default location of first enhancement tree
        CRect itemRect(
                c_controlSpacing,
                c_controlSpacing,
                c_sizeX + c_controlSpacing,
                c_sizeY + c_controlSpacing);

        ASSERT(m_treeViews.size() == MST_Number);
        std::vector<bool> isShown(MST_Number, false);  // gets set to true when displayed
        for (size_t ti = 0; ti < m_visibleTrees.size(); ++ti)
        {
            size_t index = m_visibleTrees[ti];
            ASSERT(index >= 0 && index < MST_Number);
            // move the window to the correct location
            m_treeViews[index]->MoveWindow(itemRect, false);
            m_treeViews[index]->ShowWindow(SW_SHOW);        // ensure visible
            isShown[index] = true;
            // now move the rectangle to the next tree location
            itemRect += CPoint(itemRect.Width() + c_controlSpacing, 0);
        }
        SetScrollSizes(
                MM_TEXT,
                CSize(itemRect.left, itemRect.bottom + c_controlSpacing));
    }
}

LRESULT CReaperEnhancementsView::OnNewDocument(WPARAM wParam, LPARAM lParam)
{
    // wParam is the document pointer
    CDocument * pDoc = (CDocument*)(wParam);
    m_pDocument = pDoc;
    // lParam is the character pointer
    Character * pCharacter = (Character *)(lParam);
    if (pCharacter != NULL)
    {
        m_pCharacter = pCharacter;
        m_pCharacter->AttachObserver(this);
        if (IsWindow(GetSafeHwnd()))
        {
            SetScrollPos(SB_HORZ, 0, TRUE);
            SetScrollPos(SB_VERT, 0, TRUE);
        }
        // trees definitely change if the character has changed
        m_availableTrees = DetermineTrees();
        DestroyEnhancementWindows();
        CreateEnhancementWindows();
    }
    else
    {
        if (m_pCharacter != NULL)
        {
            m_pCharacter->DetachObserver(this);
            m_pCharacter = NULL;
        }
        DestroyEnhancementWindows();
    }
    UpdateWindowTitle();
    return 0L;
}

BOOL CReaperEnhancementsView::OnEraseBkgnd(CDC* pDC)
{
    static int controlsNotToBeErased[] =
    {
        0 // end marker
    };

    // exclude any visible enhancement view windows
    for (size_t i = 0; i < m_treeViews.size(); ++i)
    {
        if (::IsWindow(m_treeViews[i]->GetSafeHwnd())
                && ::IsWindowVisible(m_treeViews[i]->GetSafeHwnd()))
        {
            CRect rctWnd;
            m_treeViews[i]->GetWindowRect(&rctWnd);
            ScreenToClient(&rctWnd);
            pDC->ExcludeClipRect(&rctWnd);
        }
    }

    return OnEraseBackground(this, pDC, controlsNotToBeErased);
}

std::vector<EnhancementTree> CReaperEnhancementsView::DetermineTrees()
{
    // the three standard reaper trees are always the ones available
    std::vector<EnhancementTree> trees;
    const std::vector<EnhancementTree> & allTrees = EnhancementTrees();
    std::vector<EnhancementTree>::const_iterator it = allTrees.begin();
    while (it != allTrees.end())
    {
        // get all the trees that are compatible with the race/class setup
        if ((*it).HasIsReaperTree())
        {
            // yes this is one of our tree's add it
            trees.push_back((*it));
        }
        ++it;
    }
    return trees;
}

void CReaperEnhancementsView::CreateEnhancementWindows()
{
    LockWindowUpdate();
    // all enhancement windows are a set size and max of MST_Number available to the user
    CRect itemRect(
            c_controlSpacing,
            c_controlSpacing,
            c_sizeX + c_controlSpacing,
            c_sizeY + c_controlSpacing);
    for (size_t i = 0; i < MST_Number; ++i)
    {
        // create the tree dialog
        // show an enhancement dialog
        CEnhancementTreeDialog * dlg = new CEnhancementTreeDialog(
                this,
                m_pCharacter,
                GetEnhancementTree(m_availableTrees[i].Name()),
                TT_reaper);
        dlg->Create(CEnhancementTreeDialog::IDD, this);
        dlg->MoveWindow(&itemRect);
        dlg->ShowWindow(SW_SHOW);
        m_treeViews.push_back(dlg);
        // TBD this is just so they all get shown for now
        m_visibleTrees.push_back(m_visibleTrees.size());
    }
    UnlockWindowUpdate();
    // reposition and show the windows (handled in OnSize)
    CRect rctWnd;
    GetClientRect(&rctWnd);
    OnSize(SIZE_RESTORED, rctWnd.Width(), rctWnd.Height());
}

void CReaperEnhancementsView::DestroyEnhancementWindows()
{
    LockWindowUpdate();
    // the user has changed either a race or class and the available enhancement trees
    // has changed
    for (size_t i = 0; i < m_treeViews.size(); ++i)
    {
        m_treeViews[i]->DestroyWindow();
        delete m_treeViews[i];
        m_treeViews[i] = NULL;
    }
    m_treeViews.clear();
    m_visibleTrees.clear();
    UnlockWindowUpdate();
}

void CReaperEnhancementsView::UpdateWindowTitle()
{
    CString text;
    if (m_pCharacter != NULL)
    {
        // total up the total number of spent reaper points
        size_t totalReaperPoints = 0;
        for (size_t i = 0; i < m_availableTrees.size(); ++i)
        {
            totalReaperPoints += m_pCharacter->APSpentInTree(m_availableTrees[i].Name());
        }
        // calculate the required reaper xp
        size_t reaperXp = 0;
        for (size_t i = 0; i < totalReaperPoints; ++i)
        {
            reaperXp += (i * 2 + 1);
        }
        text.Format("Reaper Enhancements - %d RAPs, Requires %dk Reaper XP",
                totalReaperPoints,
                reaperXp);
    }
    else
    {
        text = "Reaper Enhancements";
    }
    GetParent()->SetWindowText(text);
}

void CReaperEnhancementsView::UpdateEnhancementTrained(
        Character * charData,
        const std::string & enhancementName,
        const std::string & selection,
        bool isTier5)
{
    UpdateWindowTitle();
}

void CReaperEnhancementsView::UpdateEnhancementRevoked(
        Character * charData,
        const std::string & enhancementName,
        const std::string & selection,
        bool isTier5)
{
    UpdateWindowTitle();
}

void CReaperEnhancementsView::UpdateEnhancementTreeReset(
        Character * charData)
{
    UpdateWindowTitle();
}
