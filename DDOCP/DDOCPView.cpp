// DDOCPView.cpp
//
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DDOCP.h"
#endif

#include "DDOCPDoc.h"
#include "DDOCPView.h"
#include "GlobalSupportFunctions.h"
#include "MainFrm.h"
#include "ActiveStanceDialog.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDDOCPView
namespace
{
    const int c_controlSpacing = 3;
    COLORREF f_abilityOverspendColour = RGB(0xE9, 0x96, 0x7A); // dark salmon
}

IMPLEMENT_DYNCREATE(CDDOCPView, CFormView)

#pragma warning(push)
#pragma warning(disable: 4407) // warning C4407: cast between different pointer to member representations, compiler may generate incorrect code
BEGIN_MESSAGE_MAP(CDDOCPView, CFormView)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_WM_ERASEBKGND()
    ON_BN_CLICKED(IDC_BUTTON_STR_PLUS, OnButtonStrPlus)
    ON_BN_CLICKED(IDC_BUTTON_STR_MINUS, OnButtonStrMinus)
    ON_BN_CLICKED(IDC_BUTTON_DEX_PLUS, OnButtonDexPlus)
    ON_BN_CLICKED(IDC_BUTTON_DEX_MINUS, OnButtonDexMinus)
    ON_BN_CLICKED(IDC_BUTTON_CON_PLUS, OnButtonConPlus)
    ON_BN_CLICKED(IDC_BUTTON_CON_MINUS, OnButtonConMinus)
    ON_BN_CLICKED(IDC_BUTTON_INT_PLUS, OnButtonIntPlus)
    ON_BN_CLICKED(IDC_BUTTON_INT_MINUS, OnButtonIntMinus)
    ON_BN_CLICKED(IDC_BUTTON_WIS_PLUS, OnButtonWisPlus)
    ON_BN_CLICKED(IDC_BUTTON_WIS_MINUS, OnButtonWisMinus)
    ON_BN_CLICKED(IDC_BUTTON_CHA_PLUS, OnButtonChaPlus)
    ON_BN_CLICKED(IDC_BUTTON_CHA_MINUS, OnButtonChaMinus)
    ON_CBN_SELENDOK(IDC_COMBO_RACE, &CDDOCPView::OnSelendokComboRace)
    ON_CBN_SELENDOK(IDC_COMBO_ALIGNMENT, &CDDOCPView::OnSelendokComboAlignment)
    ON_CBN_SELENDOK(IDC_COMBO_TOME_STR, OnSelendokComboTomeStr)
    ON_CBN_SELENDOK(IDC_COMBO_TOME_DEX, OnSelendokComboTomeDex)
    ON_CBN_SELENDOK(IDC_COMBO_TOME_CON, OnSelendokComboTomeCon)
    ON_CBN_SELENDOK(IDC_COMBO_TOME_INT, OnSelendokComboTomeInt)
    ON_CBN_SELENDOK(IDC_COMBO_TOME_WIS, OnSelendokComboTomeWis)
    ON_CBN_SELENDOK(IDC_COMBO_TOME_CHA, OnSelendokComboTomeCha)
    ON_EN_CHANGE(IDC_NAME, &CDDOCPView::OnChangeName)
    ON_CBN_SELENDOK(IDC_COMBO_LEVEL4_ABILITY, OnSelendokComboAbilityLevel4)
    ON_CBN_SELENDOK(IDC_COMBO_LEVEL8_ABILITY, OnSelendokComboAbilityLevel8)
    ON_CBN_SELENDOK(IDC_COMBO_LEVEL12_ABILITY, OnSelendokComboAbilityLevel12)
    ON_CBN_SELENDOK(IDC_COMBO_LEVEL16_ABILITY, OnSelendokComboAbilityLevel16)
    ON_CBN_SELENDOK(IDC_COMBO_LEVEL20_ABILITY, OnSelendokComboAbilityLevel20)
    ON_CBN_SELENDOK(IDC_COMBO_LEVEL24_ABILITY, OnSelendokComboAbilityLevel24)
    ON_CBN_SELENDOK(IDC_COMBO_LEVEL28_ABILITY, OnSelendokComboAbilityLevel28)
    ON_CBN_SELENDOK(IDC_COMBO_LEVEL32_ABILITY, OnSelendokComboAbilityLevel32)
    ON_CBN_SELENDOK(IDC_COMBO_LEVEL36_ABILITY, OnSelendokComboAbilityLevel36)
    ON_CBN_SELENDOK(IDC_COMBO_LEVEL40_ABILITY, OnSelendokComboAbilityLevel40)
    ON_WM_SIZE()
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_CHECK_GUILD_BUFFS, OnButtonGuildBuffs)
    ON_EN_KILLFOCUS(IDC_EDIT_GUILD_LEVEL, OnKillFocusGuildLevel)
    ON_BN_CLICKED(IDC_RADIO_28PT, &CDDOCPView::OnBnClickedRadio28pt)
    ON_BN_CLICKED(IDC_RADIO_32PT, &CDDOCPView::OnBnClickedRadio32pt)
    ON_COMMAND(ID_EDIT_RESETBUILD, &CDDOCPView::OnEditResetbuild)
    ON_UPDATE_COMMAND_UI(ID_EDIT_FEATS_EPICONLY, &CDDOCPView::OnEditFeatsUpdateEpicOnly)
    ON_UPDATE_COMMAND_UI(ID_EDIT_FEATS_SHOWUNAVAILABLE, &CDDOCPView::OnEditFeatsUpdateShowUnavailable)
    ON_COMMAND(ID_EDIT_FEATS_EPICONLY, &CDDOCPView::OnEditFeatsEpicOnly)
    ON_COMMAND(ID_EDIT_FEATS_SHOWUNAVAILABLE, &CDDOCPView::OnEditFeatsShowUnavailable)
    ON_UPDATE_COMMAND_UI(ID_EDIT_IGNORELIST_ACTIVE, &CDDOCPView::OnEditUpdateIgnoreListActive)
    ON_COMMAND(ID_EDIT_IGNORELIST_ACTIVE, &CDDOCPView::OnEditIgnoreListActive)
    ON_COMMAND(ID_LAMANNIA_PREVIEW, OnLamanniaPreview)
    ON_UPDATE_COMMAND_UI(ID_LAMANNIA_PREVIEW, OnUpdateLamanniaPreview)
END_MESSAGE_MAP()
#pragma warning(pop)

// CDDOCPView construction/destruction
CDDOCPView::CDDOCPView() :
    CFormView(CDDOCPView::IDD),
    m_pCharacter(NULL),
    m_bIgnoreFocus(false)
{
    // TODO: add construction code here
}

CDDOCPView::~CDDOCPView()
{
}

void CDDOCPView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_BUILD, m_staticBuildDescription);
    DDX_Control(pDX, IDC_RADIO_28PT, m_button28Pt);
    DDX_Control(pDX, IDC_RADIO_32PT, m_button32Pt);
    DDX_Control(pDX, IDC_RADIO_34PT, m_button34Pt);
    DDX_Control(pDX, IDC_RADIO_36PT, m_button36Pt);
    DDX_Control(pDX, IDC_NAME, m_editName);
    DDX_Control(pDX, IDC_COMBO_RACE, m_comboRace);
    DDX_Control(pDX, IDC_COMBO_ALIGNMENT, m_comboAlignment);
    DDX_Control(pDX, IDC_EDIT_STR, m_editStr);
    DDX_Control(pDX, IDC_EDIT_DEX, m_editDex);
    DDX_Control(pDX, IDC_EDIT_CON, m_editCon);
    DDX_Control(pDX, IDC_EDIT_INT, m_editInt);
    DDX_Control(pDX, IDC_EDIT_WIS, m_editWis);
    DDX_Control(pDX, IDC_EDIT_CHA, m_editCha);
    DDX_Control(pDX, IDC_EDIT_STR_COST, m_editCostStr);
    DDX_Control(pDX, IDC_EDIT_DEX_COST, m_editCostDex);
    DDX_Control(pDX, IDC_EDIT_CON_COST, m_editCostCon);
    DDX_Control(pDX, IDC_EDIT_INT_COST, m_editCostInt);
    DDX_Control(pDX, IDC_EDIT_WIS_COST, m_editCostWis);
    DDX_Control(pDX, IDC_EDIT_CHA_COST, m_editCostCha);
    DDX_Control(pDX, IDC_COMBO_TOME_STR, m_comboTomeStr);
    DDX_Control(pDX, IDC_COMBO_TOME_DEX, m_comboTomeDex);
    DDX_Control(pDX, IDC_COMBO_TOME_CON, m_comboTomeCon);
    DDX_Control(pDX, IDC_COMBO_TOME_INT, m_comboTomeInt);
    DDX_Control(pDX, IDC_COMBO_TOME_WIS, m_comboTomeWis);
    DDX_Control(pDX, IDC_COMBO_TOME_CHA, m_comboTomeCha);
    DDX_Control(pDX, IDC_BUTTON_STR_PLUS, m_buttonStrPlus);
    DDX_Control(pDX, IDC_BUTTON_STR_MINUS, m_buttonStrMinus);
    DDX_Control(pDX, IDC_BUTTON_DEX_PLUS, m_buttonDexPlus);
    DDX_Control(pDX, IDC_BUTTON_DEX_MINUS, m_buttonDexMinus);
    DDX_Control(pDX, IDC_BUTTON_CON_PLUS, m_buttonConPlus);
    DDX_Control(pDX, IDC_BUTTON_CON_MINUS, m_buttonConMinus);
    DDX_Control(pDX, IDC_BUTTON_INT_PLUS, m_buttonIntPlus);
    DDX_Control(pDX, IDC_BUTTON_INT_MINUS, m_buttonIntMinus);
    DDX_Control(pDX, IDC_BUTTON_WIS_PLUS, m_buttonWisPlus);
    DDX_Control(pDX, IDC_BUTTON_WIS_MINUS, m_buttonWisMinus);
    DDX_Control(pDX, IDC_BUTTON_CHA_PLUS, m_buttonChaPlus);
    DDX_Control(pDX, IDC_BUTTON_CHA_MINUS, m_buttonChaMinus);
    DDX_Control(pDX, IDC_CHECK_GUILD_BUFFS, m_buttonGuildBuffs);
    DDX_Control(pDX, IDC_EDIT_GUILD_LEVEL, m_editGuildLevel);
    DDX_Control(pDX, IDC_STATIC_AVAILABLE_POINTS, m_staticAvailableSpend);
    DDX_Control(pDX, IDC_COMBO_LEVEL4_ABILITY, m_comboAILevel4);
    DDX_Control(pDX, IDC_COMBO_LEVEL8_ABILITY, m_comboAILevel8);
    DDX_Control(pDX, IDC_COMBO_LEVEL12_ABILITY, m_comboAILevel12);
    DDX_Control(pDX, IDC_COMBO_LEVEL16_ABILITY, m_comboAILevel16);
    DDX_Control(pDX, IDC_COMBO_LEVEL20_ABILITY, m_comboAILevel20);
    DDX_Control(pDX, IDC_COMBO_LEVEL24_ABILITY, m_comboAILevel24);
    DDX_Control(pDX, IDC_COMBO_LEVEL28_ABILITY, m_comboAILevel28);
    DDX_Control(pDX, IDC_COMBO_LEVEL32_ABILITY, m_comboAILevel32);
    DDX_Control(pDX, IDC_COMBO_LEVEL36_ABILITY, m_comboAILevel36);
    DDX_Control(pDX, IDC_COMBO_LEVEL40_ABILITY, m_comboAILevel40);
}

void CDDOCPView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    m_pCharacter = GetDocument()->GetCharacter();
    m_pCharacter->AttachObserver(this);

    PopulateComboboxes();
    UpdateRadioPoints();

    RestoreControls();      // puts controls to values of loaded data
    EnableButtons();
    UpdateBuildDescription();

    GetParentFrame()->RecalcLayout();
    ResizeParentToFit();
}

void CDDOCPView::OnRButtonUp(UINT nFlags, CPoint point)
{
    UNREFERENCED_PARAMETER(nFlags);
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void CDDOCPView::OnContextMenu(CWnd * pWnd, CPoint point)
{
    UNREFERENCED_PARAMETER(pWnd);
    UNREFERENCED_PARAMETER(point);
#ifndef SHARED_HANDLERS
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

BOOL CDDOCPView::OnEraseBkgnd(CDC* pDC)
{
    static int controlsNotToBeErased[] =
    {
        IDC_RADIO_28PT,
        IDC_RADIO_32PT,
        IDC_RADIO_34PT,
        IDC_RADIO_36PT,
        IDC_NAME,
        IDC_COMBO_RACE,
        IDC_COMBO_ALIGNMENT,
        IDC_BUTTON_STR_PLUS,
        IDC_EDIT_STR,
        IDC_BUTTON_STR_MINUS,
        IDC_EDIT_STR_COST,
        IDC_COMBO_TOME_STR,
        IDC_BUTTON_DEX_PLUS,
        IDC_EDIT_DEX,
        IDC_BUTTON_DEX_MINUS,
        IDC_EDIT_DEX_COST,
        IDC_COMBO_TOME_DEX,
        IDC_BUTTON_CON_PLUS,
        IDC_EDIT_CON,
        IDC_BUTTON_CON_MINUS,
        IDC_EDIT_CON_COST,
        IDC_COMBO_TOME_CON,
        IDC_BUTTON_INT_PLUS,
        IDC_EDIT_INT,
        IDC_BUTTON_INT_MINUS,
        IDC_EDIT_INT_COST,
        IDC_COMBO_TOME_INT,
        IDC_BUTTON_WIS_PLUS,
        IDC_EDIT_WIS,
        IDC_BUTTON_WIS_MINUS,
        IDC_EDIT_WIS_COST,
        IDC_COMBO_TOME_WIS,
        IDC_BUTTON_CHA_PLUS,
        IDC_EDIT_CHA,
        IDC_BUTTON_CHA_MINUS,
        IDC_EDIT_CHA_COST,
        IDC_COMBO_TOME_CHA,
        IDC_CHECK_GUILD_BUFFS,
        IDC_EDIT_GUILD_LEVEL,
        IDC_COMBO_LEVEL4_ABILITY,
        IDC_COMBO_LEVEL8_ABILITY,
        IDC_COMBO_LEVEL12_ABILITY,
        IDC_COMBO_LEVEL16_ABILITY,
        IDC_COMBO_LEVEL20_ABILITY,
        IDC_COMBO_LEVEL24_ABILITY,
        IDC_COMBO_LEVEL28_ABILITY,
        0 // end marker
    };

    pDC->SaveDC();

    const int * pId = controlsNotToBeErased;
    while (*pId != 0)
    {
        // Get rectangle of the control.
        CWnd * pControl = GetDlgItem(*pId);
        if (pControl && pControl->IsWindowVisible())
        {
            CRect controlClip;
            pControl->GetWindowRect(&controlClip);
            ScreenToClient(&controlClip);
            if (pControl->IsKindOf(RUNTIME_CLASS(CComboBox)))
            {
                // combo boxes return the height of the whole control, including the drop rectangle
                // reduce the height to the control size without it shown
                controlClip.bottom = controlClip.top
                        + GetSystemMetrics(SM_CYHSCROLL)
                        + GetSystemMetrics(SM_CYEDGE) * 2;
                // special case for combo boxes with image lists
                CComboBoxEx * pCombo = dynamic_cast<CComboBoxEx*>(pControl);
                if (pCombo != NULL)
                {
                    CImageList * pImage = pCombo->GetImageList();
                    if (pImage != NULL)
                    {
                        IMAGEINFO info;
                        pImage->GetImageInfo(0, &info);
                        // limit to the the height of the selection combo
                        controlClip.bottom = controlClip.top
                                + info.rcImage.bottom
                                - info.rcImage.top
                                + GetSystemMetrics(SM_CYEDGE) * 2;
                    }
                }
            }
            pDC->ExcludeClipRect(&controlClip);
        }

        // next in list
        ++pId;
    }
    CRect rctClient;
    GetWindowRect(&rctClient);
    ScreenToClient(&rctClient);
    pDC->FillSolidRect(rctClient, GetSysColor(COLOR_BTNFACE));
    pDC->RestoreDC(-1);

    return TRUE;
}
// CDDOCPView diagnostics

#ifdef _DEBUG
void CDDOCPView::AssertValid() const
{
    CFormView::AssertValid();
}

void CDDOCPView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}

CDDOCPDoc* CDDOCPView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDDOCPDoc)));
    return (CDDOCPDoc*)m_pDocument;
}
#endif //_DEBUG

// CDDOCPView message handlers
void CDDOCPView::PopulateComboboxes()
{
    // setup the combo-boxes to have selectable content

    // Available races
    for (size_t ri = Race_Unknown + 1; ri < Race_Count; ++ri)
    {
        CString text = EnumEntryText(
                (RaceType)ri,
                raceTypeMap);
        size_t i = m_comboRace.AddString(text);
        m_comboRace.SetItemData(i, ri);
    }

    // Available alignments
    for (size_t ai = Alignment_Unknown + 1; ai < Alignment_Count; ++ai)
    {
        CString text = EnumEntryText(
                (AlignmentType)ai,
                alignmentTypeMap);
        size_t i = m_comboAlignment.AddString(text);
        m_comboAlignment.SetItemData(i, ai);
    }

    // ability tomes
    for (int ti = 0; ti <= MAX_ABILITY_TOME; ++ti)
    {
        CString text;
        if (ti > 0)
        {
            text.Format("%+d Tome", ti);
        }
        else
        {
            text = "No Tome";
        }
        // add to all 6 ability tome combo-boxes
        size_t i = m_comboTomeStr.InsertString(ti, text);
        m_comboTomeStr.SetItemData(i, ti);
        if (ti > 0)
        {
            // Supreme option on 1st stat
            CString supreme;
            supreme.Format("%+d Supreme Tome", ti);
            i = m_comboTomeStr.AddString(supreme);
            m_comboTomeStr.SetItemData(i, -ti); // negative tome values for supreme
        }
        i = m_comboTomeDex.AddString(text);
        m_comboTomeDex.SetItemData(i, ti);
        i = m_comboTomeCon.AddString(text);
        m_comboTomeCon.SetItemData(i, ti);
        i = m_comboTomeInt.AddString(text);
        m_comboTomeInt.SetItemData(i, ti);
        i = m_comboTomeWis.AddString(text);
        m_comboTomeWis.SetItemData(i, ti);
        i = m_comboTomeCha.AddString(text);
        m_comboTomeCha.SetItemData(i, ti);
    }

    // level up stats
    for (int ai = Ability_Unknown + 1; ai < Ability_Count; ++ai)
    {
        CString text = EnumEntryText(
                (AbilityType)ai,
                abilityTypeMap);
        CString textAll = "All ";
        textAll += text;
        size_t i = m_comboAILevel4.AddString(text);
        m_comboAILevel4.SetItemData(i, ai);
        i = m_comboAILevel4.AddString(textAll);
        m_comboAILevel4.SetItemData(i, -ai); // negative stat value for all
        i = m_comboAILevel8.AddString(text);
        m_comboAILevel8.SetItemData(i, ai);
        i = m_comboAILevel12.AddString(text);
        m_comboAILevel12.SetItemData(i, ai);
        i = m_comboAILevel16.AddString(text);
        m_comboAILevel16.SetItemData(i, ai);
        i = m_comboAILevel20.AddString(text);
        m_comboAILevel20.SetItemData(i, ai);
        i = m_comboAILevel24.AddString(text);
        m_comboAILevel24.SetItemData(i, ai);
        i = m_comboAILevel28.AddString(text);
        m_comboAILevel28.SetItemData(i, ai);
        i = m_comboAILevel32.AddString(text);
        m_comboAILevel32.SetItemData(i, ai);
        i = m_comboAILevel36.AddString(text);
        m_comboAILevel36.SetItemData(i, ai);
        i = m_comboAILevel40.AddString(text);
        m_comboAILevel40.SetItemData(i, ai);
    }
    m_comboAILevel4.EnableWindow(m_pCharacter->MaxLevel() >= 4);
    m_comboAILevel8.EnableWindow(m_pCharacter->MaxLevel() >= 8);
    m_comboAILevel12.EnableWindow(m_pCharacter->MaxLevel() >= 12);
    m_comboAILevel16.EnableWindow(m_pCharacter->MaxLevel() >= 16);
    m_comboAILevel20.EnableWindow(m_pCharacter->MaxLevel() >= 20);
    m_comboAILevel24.EnableWindow(m_pCharacter->MaxLevel() >= 24);
    m_comboAILevel28.EnableWindow(m_pCharacter->MaxLevel() >= 28);
    m_comboAILevel32.EnableWindow(m_pCharacter->MaxLevel() >= 32);
    m_comboAILevel36.EnableWindow(m_pCharacter->MaxLevel() >= 36);
    m_comboAILevel40.EnableWindow(m_pCharacter->MaxLevel() >= 40);
}

void CDDOCPView::RestoreControls()
{
    // restore loaded values
    // name
    m_editName.SetWindowText(m_pCharacter->Name().c_str());

    // Race
    SelectComboboxEntry(m_pCharacter->Race(), &m_comboRace);

    // alignment
    SelectComboboxEntry(m_pCharacter->Alignment(), &m_comboAlignment);

    // tomes
    SelectComboboxEntry(m_pCharacter->StrTome(), &m_comboTomeStr);
    SelectComboboxEntry(m_pCharacter->DexTome(), &m_comboTomeDex);
    SelectComboboxEntry(m_pCharacter->ConTome(), &m_comboTomeCon);
    SelectComboboxEntry(m_pCharacter->IntTome(), &m_comboTomeInt);
    SelectComboboxEntry(m_pCharacter->WisTome(), &m_comboTomeWis);
    SelectComboboxEntry(m_pCharacter->ChaTome(), &m_comboTomeCha);

    DisplayAbilityValue(Ability_Strength, &m_editStr);
    DisplayAbilityValue(Ability_Dexterity, &m_editDex);
    DisplayAbilityValue(Ability_Constitution, &m_editCon);
    DisplayAbilityValue(Ability_Intelligence, &m_editInt);
    DisplayAbilityValue(Ability_Wisdom, &m_editWis);
    DisplayAbilityValue(Ability_Charisma, &m_editCha);

    DisplaySpendCost(Ability_Strength, &m_editCostStr);
    DisplaySpendCost(Ability_Dexterity, &m_editCostDex);
    DisplaySpendCost(Ability_Constitution, &m_editCostCon);
    DisplaySpendCost(Ability_Intelligence, &m_editCostInt);
    DisplaySpendCost(Ability_Wisdom, &m_editCostWis);
    DisplaySpendCost(Ability_Charisma, &m_editCostCha);
    UpdateAvailableSpend();

    // ability increase selections
    SelectComboboxEntry(m_pCharacter->Level4(), &m_comboAILevel4);
    SelectComboboxEntry(m_pCharacter->Level8(), &m_comboAILevel8);
    SelectComboboxEntry(m_pCharacter->Level12(), &m_comboAILevel12);
    SelectComboboxEntry(m_pCharacter->Level16(), &m_comboAILevel16);
    SelectComboboxEntry(m_pCharacter->Level20(), &m_comboAILevel20);
    SelectComboboxEntry(m_pCharacter->Level24(), &m_comboAILevel24);
    SelectComboboxEntry(m_pCharacter->Level28(), &m_comboAILevel28);
    SelectComboboxEntry(m_pCharacter->Level32(), &m_comboAILevel32);
    SelectComboboxEntry(m_pCharacter->Level36(), &m_comboAILevel36);
    SelectComboboxEntry(m_pCharacter->Level40(), &m_comboAILevel40);

    // guild level
    CString level;
    level.Format("%d", m_pCharacter->GuildLevel());
    m_editGuildLevel.SetWindowText(level);
    m_buttonGuildBuffs.SetCheck(m_pCharacter->HasApplyGuildBuffs() ? BST_CHECKED : BST_UNCHECKED);
}

void CDDOCPView::EnableButtons()
{
    // depending on the selected build points, enable/disable
    // all the ability increase/decrease buttons based on current spends
    const AbilitySpend & as = m_pCharacter->BuildPoints();

    m_buttonStrPlus.EnableWindow(as.CanSpendOnAbility(Ability_Strength));
    m_buttonStrMinus.EnableWindow(as.CanRevokeSpend(Ability_Strength));
    m_buttonDexPlus.EnableWindow(as.CanSpendOnAbility(Ability_Dexterity));
    m_buttonDexMinus.EnableWindow(as.CanRevokeSpend(Ability_Dexterity));
    m_buttonConPlus.EnableWindow(as.CanSpendOnAbility(Ability_Constitution));
    m_buttonConMinus.EnableWindow(as.CanRevokeSpend(Ability_Constitution));
    m_buttonIntPlus.EnableWindow(as.CanSpendOnAbility(Ability_Intelligence));
    m_buttonIntMinus.EnableWindow(as.CanRevokeSpend(Ability_Intelligence));
    m_buttonWisPlus.EnableWindow(as.CanSpendOnAbility(Ability_Wisdom));
    m_buttonWisMinus.EnableWindow(as.CanRevokeSpend(Ability_Wisdom));
    m_buttonChaPlus.EnableWindow(as.CanSpendOnAbility(Ability_Charisma));
    m_buttonChaMinus.EnableWindow(as.CanRevokeSpend(Ability_Charisma));
}

void CDDOCPView::DisplayAbilityValue(
        AbilityType ability,
        CEdit * control)
{
    size_t value = m_pCharacter->BaseAbilityValue(ability);
    CString text;
    text.Format("%d", value);
    control->SetWindowText(text);
}

void CDDOCPView::DisplaySpendCost(
        AbilityType ability,
        CEdit * control)
{
    const AbilitySpend & as = m_pCharacter->BuildPoints();
    size_t nextCost = as.NextPointsSpentCost(ability);
    CString text;
    if (nextCost > 0)
    {
        // show how many build points the next increase will cost
        text.Format("%d", nextCost);
    }
    else
    {
        // max spend on this ability has been reached
        text = "MAX";
    }
    control->SetWindowTextA(text);
}

void CDDOCPView::UpdateAvailableSpend()
{
    // show how many build points are yet to be spent
    const AbilitySpend & as = m_pCharacter->BuildPoints();
    int availablePoints = (int)as.AvailableSpend() - (int)as.PointsSpent();
    CString text;
    text.Format("Available Build points: %d", availablePoints);
    m_staticAvailableSpend.SetWindowText(text);
}

HBRUSH CDDOCPView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
    // colour the control based on whether the user has over spent
    // the number of build points available. This can happen
    // if they adjust down the number of past lives
    bool setWarning = false;
    if (pWnd == &m_staticAvailableSpend)
    {
        if (m_pDocument != NULL)
        {
            const AbilitySpend & as = m_pCharacter->BuildPoints();
            int availablePoints = (int)as.AvailableSpend() - (int)as.PointsSpent();
            setWarning = (availablePoints < 0);
        }
    }
    if (setWarning)
    {
        pDC->SetTextColor(f_abilityOverspendColour);
    }

    return hbr;
}

void CDDOCPView::OnButtonStrPlus()
{
    m_pCharacter->SpendOnAbility(Ability_Strength);
    EnableButtons();
    DisplayAbilityValue(Ability_Strength, &m_editStr);
    DisplaySpendCost(Ability_Strength, &m_editCostStr);
    UpdateAvailableSpend();
}

void CDDOCPView::OnButtonStrMinus()
{
    m_pCharacter->RevokeSpendOnAbility(Ability_Strength);
    EnableButtons();
    DisplayAbilityValue(Ability_Strength, &m_editStr);
    DisplaySpendCost(Ability_Strength, &m_editCostStr);
    UpdateAvailableSpend();
}

void CDDOCPView::OnButtonDexPlus()
{
    m_pCharacter->SpendOnAbility(Ability_Dexterity);
    EnableButtons();
    DisplayAbilityValue(Ability_Dexterity, &m_editDex);
    DisplaySpendCost(Ability_Dexterity, &m_editCostDex);
    UpdateAvailableSpend();
}

void CDDOCPView::OnButtonDexMinus()
{
    m_pCharacter->RevokeSpendOnAbility(Ability_Dexterity);
    EnableButtons();
    DisplayAbilityValue(Ability_Dexterity, &m_editDex);
    DisplaySpendCost(Ability_Dexterity, &m_editCostDex);
    UpdateAvailableSpend();
}

void CDDOCPView::OnButtonConPlus()
{
    m_pCharacter->SpendOnAbility(Ability_Constitution);
    EnableButtons();
    DisplayAbilityValue(Ability_Constitution, &m_editCon);
    DisplaySpendCost(Ability_Constitution, &m_editCostCon);
    UpdateAvailableSpend();
}

void CDDOCPView::OnButtonConMinus()
{
    m_pCharacter->RevokeSpendOnAbility(Ability_Constitution);
    EnableButtons();
    DisplayAbilityValue(Ability_Constitution, &m_editCon);
    DisplaySpendCost(Ability_Constitution, &m_editCostCon);
    UpdateAvailableSpend();
}

void CDDOCPView::OnButtonIntPlus()
{
    m_pCharacter->SpendOnAbility(Ability_Intelligence);
    EnableButtons();
    DisplayAbilityValue(Ability_Intelligence, &m_editInt);
    DisplaySpendCost(Ability_Intelligence, &m_editCostInt);
    UpdateAvailableSpend();
}

void CDDOCPView::OnButtonIntMinus()
{
    m_pCharacter->RevokeSpendOnAbility(Ability_Intelligence);
    EnableButtons();
    DisplayAbilityValue(Ability_Intelligence, &m_editInt);
    DisplaySpendCost(Ability_Intelligence, &m_editCostInt);
    UpdateAvailableSpend();
}

void CDDOCPView::OnButtonWisPlus()
{
    m_pCharacter->SpendOnAbility(Ability_Wisdom);
    EnableButtons();
    DisplayAbilityValue(Ability_Wisdom, &m_editWis);
    DisplaySpendCost(Ability_Wisdom, &m_editCostWis);
    UpdateAvailableSpend();
}

void CDDOCPView::OnButtonWisMinus()
{
    m_pCharacter->RevokeSpendOnAbility(Ability_Wisdom);
    EnableButtons();
    DisplayAbilityValue(Ability_Wisdom, &m_editWis);
    DisplaySpendCost(Ability_Wisdom, &m_editCostWis);
    UpdateAvailableSpend();
}

void CDDOCPView::OnButtonChaPlus()
{
    m_pCharacter->SpendOnAbility(Ability_Charisma);
    EnableButtons();
    DisplayAbilityValue(Ability_Charisma, &m_editCha);
    DisplaySpendCost(Ability_Charisma, &m_editCostCha);
    UpdateAvailableSpend();
}

void CDDOCPView::OnButtonChaMinus()
{
    m_pCharacter->RevokeSpendOnAbility(Ability_Charisma);
    EnableButtons();
    DisplayAbilityValue(Ability_Charisma, &m_editCha);
    DisplaySpendCost(Ability_Charisma, &m_editCostCha);
    UpdateAvailableSpend();
}


void CDDOCPView::OnSelendokComboRace()
{
    // race has been chosen
    RaceType race = static_cast<RaceType>(GetComboboxSelection(&m_comboRace));
    m_pCharacter->SetRace(race);

    // race may affect any ability stat value
    DisplayAbilityValue(Ability_Strength, &m_editStr);
    DisplayAbilityValue(Ability_Dexterity, &m_editDex);
    DisplayAbilityValue(Ability_Constitution, &m_editCon);
    DisplayAbilityValue(Ability_Intelligence, &m_editInt);
    DisplayAbilityValue(Ability_Wisdom, &m_editWis);
    DisplayAbilityValue(Ability_Charisma, &m_editCha);
    m_comboRace.SetFocus(); // ensure it keeps the focus
}

void CDDOCPView::OnSelendokComboAlignment()
{
    AlignmentType alignment = static_cast<AlignmentType>(GetComboboxSelection(&m_comboAlignment));
    m_pCharacter->SetAlignment(alignment);
    // views affected by alignment changes update automatically
    m_comboAlignment.SetFocus(); // ensure it keeps the focus
}

void CDDOCPView::OnSelendokComboTomeStr()
{
    int value = GetComboboxSelection(&m_comboTomeStr);
    // negative values are supreme tomes that set for all abilities
    if (value < 0)
    {
        value = -value;     // make it positive
        m_pCharacter->SetAbilityTome(Ability_Strength, value);
        m_pCharacter->SetAbilityTome(Ability_Dexterity, value);
        m_pCharacter->SetAbilityTome(Ability_Constitution, value);
        m_pCharacter->SetAbilityTome(Ability_Intelligence, value);
        m_pCharacter->SetAbilityTome(Ability_Wisdom, value);
        m_pCharacter->SetAbilityTome(Ability_Charisma, value);
        // keep controls up to date also
        SelectComboboxEntry(m_pCharacter->StrTome(), &m_comboTomeStr);
        SelectComboboxEntry(m_pCharacter->DexTome(), &m_comboTomeDex);
        SelectComboboxEntry(m_pCharacter->ConTome(), &m_comboTomeCon);
        SelectComboboxEntry(m_pCharacter->IntTome(), &m_comboTomeInt);
        SelectComboboxEntry(m_pCharacter->WisTome(), &m_comboTomeWis);
        SelectComboboxEntry(m_pCharacter->ChaTome(), &m_comboTomeCha);
    }
    else
    {
        m_pCharacter->SetAbilityTome(Ability_Strength, value);
    }
}

void CDDOCPView::OnSelendokComboTomeDex()
{
    size_t value = GetComboboxSelection(&m_comboTomeDex);
    m_pCharacter->SetAbilityTome(Ability_Dexterity, value);
}

void CDDOCPView::OnSelendokComboTomeCon()
{
    size_t value = GetComboboxSelection(&m_comboTomeCon);
    m_pCharacter->SetAbilityTome(Ability_Constitution, value);
}

void CDDOCPView::OnSelendokComboTomeInt()
{
    size_t value = GetComboboxSelection(&m_comboTomeInt);
    m_pCharacter->SetAbilityTome(Ability_Intelligence, value);
}

void CDDOCPView::OnSelendokComboTomeWis()
{
    size_t value = GetComboboxSelection(&m_comboTomeWis);
    m_pCharacter->SetAbilityTome(Ability_Wisdom, value);
}

void CDDOCPView::OnSelendokComboTomeCha()
{
    size_t value = GetComboboxSelection(&m_comboTomeCha);
    m_pCharacter->SetAbilityTome(Ability_Charisma, value);
}

void CDDOCPView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
    m_bIgnoreFocus = true;
    if (bActivate == TRUE)
    {
        // notify the main frame that we are active so that it can keep all
        // the floating windows using the correct document also
        CWnd * pWnd = AfxGetMainWnd();
        CMainFrame * pMainFrame = dynamic_cast<CMainFrame*>(pWnd);
        if (pMainFrame != NULL)
        {
            pMainFrame->SetActiveDocumentAndCharacter(GetDocument(), m_pCharacter);
        }
    }

    CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
    m_bIgnoreFocus = false;
}

void CDDOCPView::OnChangeName()
{
    CString name;
    m_editName.GetWindowText(name);
    if (name != m_pCharacter->Name().c_str())
    {
        // name has changed
        m_pCharacter->SetCharacterName(name);
    }
}

void CDDOCPView::OnSelendokComboAbilityLevel4()
{
    int sel = GetComboboxSelection(&m_comboAILevel4);
    if (sel < 0)
    {
        // its an "All Ability" selection
        AbilityType value = (AbilityType)(-sel);
        m_pCharacter->SetAbilityLevelUp(4, value);
        m_pCharacter->SetAbilityLevelUp(8, value);
        m_pCharacter->SetAbilityLevelUp(12, value);
        m_pCharacter->SetAbilityLevelUp(16, value);
        m_pCharacter->SetAbilityLevelUp(20, value);
        m_pCharacter->SetAbilityLevelUp(24, value);
        m_pCharacter->SetAbilityLevelUp(28, value);
        m_pCharacter->SetAbilityLevelUp(32, value);
        m_pCharacter->SetAbilityLevelUp(36, value);
        m_pCharacter->SetAbilityLevelUp(40, value);
        // keep UI up to date also
        SelectComboboxEntry(m_pCharacter->Level4(), &m_comboAILevel4);
        SelectComboboxEntry(m_pCharacter->Level8(), &m_comboAILevel8);
        SelectComboboxEntry(m_pCharacter->Level12(), &m_comboAILevel12);
        SelectComboboxEntry(m_pCharacter->Level16(), &m_comboAILevel16);
        SelectComboboxEntry(m_pCharacter->Level20(), &m_comboAILevel20);
        SelectComboboxEntry(m_pCharacter->Level24(), &m_comboAILevel24);
        SelectComboboxEntry(m_pCharacter->Level28(), &m_comboAILevel28);
        SelectComboboxEntry(m_pCharacter->Level32(), &m_comboAILevel32);
        SelectComboboxEntry(m_pCharacter->Level36(), &m_comboAILevel36);
        SelectComboboxEntry(m_pCharacter->Level40(), &m_comboAILevel40);
    }
    else
    {
        AbilityType value = (AbilityType)sel;
        m_pCharacter->SetAbilityLevelUp(4, value);
    }
}

void CDDOCPView::OnSelendokComboAbilityLevel8()
{
    AbilityType value = (AbilityType)GetComboboxSelection(&m_comboAILevel8);
    m_pCharacter->SetAbilityLevelUp(8, value);
}

void CDDOCPView::OnSelendokComboAbilityLevel12()
{
    AbilityType value = (AbilityType)GetComboboxSelection(&m_comboAILevel12);
    m_pCharacter->SetAbilityLevelUp(12, value);
}

void CDDOCPView::OnSelendokComboAbilityLevel16()
{
    AbilityType value = (AbilityType)GetComboboxSelection(&m_comboAILevel16);
    m_pCharacter->SetAbilityLevelUp(16, value);
}

void CDDOCPView::OnSelendokComboAbilityLevel20()
{
    AbilityType value = (AbilityType)GetComboboxSelection(&m_comboAILevel20);
    m_pCharacter->SetAbilityLevelUp(20, value);
}

void CDDOCPView::OnSelendokComboAbilityLevel24()
{
    AbilityType value = (AbilityType)GetComboboxSelection(&m_comboAILevel24);
    m_pCharacter->SetAbilityLevelUp(24, value);
}

void CDDOCPView::OnSelendokComboAbilityLevel28()
{
    AbilityType value = (AbilityType)GetComboboxSelection(&m_comboAILevel28);
    m_pCharacter->SetAbilityLevelUp(28, value);
}

void CDDOCPView::OnSelendokComboAbilityLevel32()
{
    AbilityType value = (AbilityType)GetComboboxSelection(&m_comboAILevel32);
    m_pCharacter->SetAbilityLevelUp(32, value);
}

void CDDOCPView::OnSelendokComboAbilityLevel36()
{
    AbilityType value = (AbilityType)GetComboboxSelection(&m_comboAILevel36);
    m_pCharacter->SetAbilityLevelUp(36, value);
}

void CDDOCPView::OnSelendokComboAbilityLevel40()
{
    AbilityType value = (AbilityType)GetComboboxSelection(&m_comboAILevel40);
    m_pCharacter->SetAbilityLevelUp(40, value);
}

void CDDOCPView::UpdateBuildDescription()
{
    CString text = m_pCharacter->GetBuildDescription().c_str();
    m_staticBuildDescription.SetWindowText(text);
}

// Character overrides
void CDDOCPView::UpdateAvailableBuildPointsChanged(Character * pCharacter)
{
    // show the now available build points
    UpdateRadioPoints();
    UpdateAvailableSpend();
    EnableButtons();
}

void CDDOCPView::UpdateClassChanged(
        Character * pCharacter,
        ClassType classFrom,
        ClassType classTo,
        size_t level)
{
    // level up view changed a class selection, keep our overall build description
    // up to date.
    UpdateBuildDescription();
}

void CDDOCPView::UpdateRaceChanged(Character * charData, RaceType race)
{
    // build points can change when Drow selected
    UpdateRadioPoints();
}

void CDDOCPView::OnSize(UINT nType, int cx, int cy)
{
    __super::OnSize(nType, cx, cy);
}

void CDDOCPView::OnButtonGuildBuffs()
{
    // toggle the enabled guild buffs status
    m_pCharacter->ToggleApplyGuildBuffs();
}

void CDDOCPView::OnKillFocusGuildLevel()
{
    // change the guild level
    CString text;
    m_editGuildLevel.GetWindowText(text);
    size_t level = atoi(text);      // its ES_NUMBER so guaranteed to work
    m_pCharacter->SetGuildLevel(level);
}

void CDDOCPView::UpdateRadioPoints()
{
    // although these are a group of 4 radio controls for selecting the number
    // of builds points the user has to spend there are a couple of rules that
    // apply to them as follows:
    // For any race other than Drow the builds point labels are: 28, 32, 34 and 36
    // for Drow they are: 28, 28, 30 and 32

    // if the character has no past lives buttons 28 and 32 are available, 34/36 disabled
    // if the character has 1+ past life all buttons are disabled but the relevant number
    // of build points available is checked.
    size_t buildPoints = m_pCharacter->DetermineBuildPoints();
    if (m_pCharacter->Race() == Race_Drow)
    {
        // for a Drow these buttons are always disabled
        // as its either 28, 30 (1 past life) or 32 (2+ past lives)
        m_button28Pt.SetWindowText("28");   // adventurer
        m_button32Pt.SetWindowText("28");   // champion
        m_button34Pt.SetWindowText("30");   // hero
        m_button36Pt.SetWindowText("32");   // legend

        m_button28Pt.SetCheck(buildPoints == 28 ? BST_CHECKED : BST_UNCHECKED);
        m_button32Pt.SetCheck(BST_UNCHECKED);
        m_button34Pt.SetCheck(buildPoints == 30 ? BST_CHECKED : BST_UNCHECKED);
        m_button36Pt.SetCheck(buildPoints == 32 ? BST_CHECKED : BST_UNCHECKED);

        m_button28Pt.EnableWindow(FALSE);
        m_button32Pt.EnableWindow(FALSE);
        m_button34Pt.EnableWindow(FALSE);
        m_button36Pt.EnableWindow(FALSE);
    }
    else
    {
        m_button28Pt.SetWindowText("28");   // adventurer
        m_button32Pt.SetWindowText("32");   // champion
        m_button34Pt.SetWindowText("34");   // hero
        m_button36Pt.SetWindowText("36");   // legend

        m_button28Pt.SetCheck(buildPoints == 28 ? BST_CHECKED : BST_UNCHECKED);
        m_button32Pt.SetCheck(buildPoints == 32 ? BST_CHECKED : BST_UNCHECKED);
        m_button34Pt.SetCheck(buildPoints == 34 ? BST_CHECKED : BST_UNCHECKED);
        m_button36Pt.SetCheck(buildPoints == 36 ? BST_CHECKED : BST_UNCHECKED);

        if (buildPoints < 34)
        {
            // user can select between 28 or 32 points
            // if they want 34/36 they need to train past lives
            m_button28Pt.EnableWindow(TRUE);
            m_button32Pt.EnableWindow(TRUE);
        }
        else
        {
            m_button28Pt.EnableWindow(FALSE);
            m_button32Pt.EnableWindow(FALSE);
        }
        m_button34Pt.EnableWindow(FALSE);   // never selectable, controlled by past lives
        m_button36Pt.EnableWindow(FALSE);   // never selectable, controlled by past lives
    }
}

void CDDOCPView::OnBnClickedRadio28pt()
{
    if (!m_bIgnoreFocus)
    {
        // set the number of build points to 28
        m_pCharacter->SetBuildPoints(28);
    }
}

void CDDOCPView::OnBnClickedRadio32pt()
{
    // set the number of build points to 32
    m_pCharacter->SetBuildPoints(32);
}


void CDDOCPView::OnEditResetbuild()
{
    // make sure the user really wants to do this, as its a big change
    UINT ret = AfxMessageBox("Warning: This command will reset to default all of the following:\r\n"
            "Ability Point Spends\r\n"
            "Class Selections\r\n"
            "Feat Selections\r\n"
            "Skill Points\r\n"
            "Enhancement Tree Selections\r\n"
            "Notes\r\n"
            "\r\n"
            "It will not touch any selected Past Lives, Gear setups or Tomes,\r\n"
            "but may affect Epic Destinies where the selections now become\r\n"
            "invalid due to missing feats.\r\n"
            "\r\n"
            "Are you sure you wish to do this?",
            MB_ICONQUESTION | MB_YESNO);
    if (ret == IDYES)
    {
        CWaitCursor longOperation;
        CWnd * pWnd = AfxGetMainWnd();
        CMainFrame * pMainFrame = dynamic_cast<CMainFrame*>(pWnd);
        if (pMainFrame != NULL)
        {
            // reset the windows first
            pMainFrame->SetActiveDocumentAndCharacter(NULL, NULL);
            // reset the build with nothing displayed
            m_pCharacter->ResetBuild();
            // notify the main frame that we are active so that everything gets
            // reset correctly in the views
            pMainFrame->SetActiveDocumentAndCharacter(GetDocument(), m_pCharacter);
        }
        // ensure this view is up to date
        UpdateRadioPoints();

        RestoreControls();
        EnableButtons();
        UpdateBuildDescription();
    }
}

void CDDOCPView::OnEditFeatsUpdateEpicOnly(CCmdUI * pCmdUi)
{
    pCmdUi->Enable(!m_pCharacter->ShowUnavailable());
    pCmdUi->SetCheck(m_pCharacter->ShowEpicOnly());
}

void CDDOCPView::OnEditFeatsUpdateShowUnavailable(CCmdUI * pCmdUi)
{
    pCmdUi->SetCheck(m_pCharacter->ShowUnavailable());
}

void CDDOCPView::OnEditFeatsEpicOnly()
{
    m_pCharacter->ToggleShowEpicOnly();
}

void CDDOCPView::OnEditFeatsShowUnavailable()
{
    m_pCharacter->ToggleShowUnavailable();
}

void CDDOCPView::OnEditUpdateIgnoreListActive(CCmdUI * pCmdUi)
{
    pCmdUi->SetCheck(m_pCharacter->ShowIgnoredItems());
}

void CDDOCPView::OnEditIgnoreListActive()
{
    m_pCharacter->ToggleShowIgnoredItems();
}

void CDDOCPView::OnLamanniaPreview()
{
    m_pCharacter->SetLamanniaMode(!m_pCharacter->HasLamanniaMode());    // toggle
    // cause the UI to update by simulating a character change
    CWnd* pWnd = AfxGetMainWnd();
    CMainFrame* pFrame = dynamic_cast<CMainFrame*>(pWnd);
    if (pFrame != NULL)
    {
        pFrame->SetActiveDocumentAndCharacter(NULL, NULL);
        pFrame->SetActiveDocumentAndCharacter(GetDocument(), m_pCharacter);
        m_comboAILevel4.EnableWindow(m_pCharacter->MaxLevel() >= 4);
        m_comboAILevel8.EnableWindow(m_pCharacter->MaxLevel() >= 8);
        m_comboAILevel12.EnableWindow(m_pCharacter->MaxLevel() >= 12);
        m_comboAILevel16.EnableWindow(m_pCharacter->MaxLevel() >= 16);
        m_comboAILevel20.EnableWindow(m_pCharacter->MaxLevel() >= 20);
        m_comboAILevel24.EnableWindow(m_pCharacter->MaxLevel() >= 24);
        m_comboAILevel28.EnableWindow(m_pCharacter->MaxLevel() >= 28);
        m_comboAILevel32.EnableWindow(m_pCharacter->MaxLevel() >= 32);
        m_comboAILevel36.EnableWindow(m_pCharacter->MaxLevel() >= 36);
        m_comboAILevel40.EnableWindow(m_pCharacter->MaxLevel() >= 40);
        UpdateBuildDescription();
    }
}

void CDDOCPView::OnUpdateLamanniaPreview(CCmdUI * pCmdUi)
{
    pCmdUi->Enable(TRUE);
    pCmdUi->SetCheck(m_pCharacter->HasLamanniaMode());
}
