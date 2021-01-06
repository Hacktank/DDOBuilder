// AboutDlg.cpp
//
#include "stdafx.h"
#include "AboutDlg.h"
#include "BuildInfo.h"
#include "GlobalSupportFunctions.h"

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_VERSION, m_staticVersion);
    DDX_Control(pDX, IDC_BUG_REPORTERS, m_bugReporters);

    CString text;
    text.Format("DDOBuilder, Version %s", BUILDINFO_VERSION);
    m_staticVersion.SetWindowText(text);

    int count = AllFeats().size();
    text.Format("Feat Count: %d", count);
    SetDlgItemText(IDC_STATIC_FEATCOUNT, text);

    count = Augments().size();
    text.Format("Augment Count: %d", count);
    SetDlgItemText(IDC_STATIC_AUGMENTCOUNT, text);

    count = Items().size();
    text.Format("Item Count: %d", count);
    SetDlgItemText(IDC_STATIC_ITEMCOUNT, text);

    count = Spells().size();
    text.Format("Spell Count: %d", count);
    SetDlgItemText(IDC_STATIC_SPELLCOUNT, text);

    count = EnhancementTrees().size();
    text.Format("Enhancement Tree Count: %d", count);
    SetDlgItemText(IDC_STATIC_TREECOUNT, text);

    count = OptionalBuffs().size();
    text.Format("Self and Party Buff Count: %d", count);
    SetDlgItemText(IDC_STATIC_PARTYBUFF_COUNT, text);

    m_bugReporters.SetWindowText(
            "In alphabetical order:\r\n"
            "\r\n"
            "aaron250, "
            "Aganthor, "
            "Amastris, "
            "Amberlink, "
            "awsome-dudes, "
            "Aymerick55, "
            "AzrielAngelus, "
            "Bjond, "
            "Brakkart, "
            "Caarb, "
            "Cardo, "
            "Carpone, "
            "Civet, "
            "crcabanillas, "
            "cswob, "
            "Cybree, "
            "DaWiner, "
            "diquintino, "
            "DougSK, "
            "DwarvemHack, "
            "Erofen, "
            "Fanaval, "
            "fognozzle, "
            "Frinkky, "
            "FullPlayer20, "
            "Gemini-Dragon357, "
            "Gerkenstein, "
            "Gizah10, "
            "Gizmo0815, "
            "Gudlaugsson, "
            "Guntharm, "
            "hallowedone, "
            "Havader, "
            "Hawkwier, "
            "Hobgoblin, "
            "iAntiquary, "
            "Jalak21, "
            "Jokersir2, "
            "Kamdragon, "
            "Kaustics "
            "Kentpb, "
            "Kiriitto, "
            "Laur, "
            "LittleTealeaf, "
            "Lupito, "
            "outlawless, "
            "Madja, "
            "Maxxcore, "
            "MaxPaxy, "
            "mikameow, "
            "Mystickal413, "
            "nadia72295, "
            "NaughtyNancy, "
            "Neith, "
            "Ntoukis, "
            "nymcraian, "
            "pastorman1974, "
            "pcloke123, "
            "peng, "
            "Potnia, "
            "Question2, "
            "Rauven, "
            "Rawel, "
            "RetroDark, "
            "rnett, "
            "Seljuck, "
            "Selvera, "
            "ShintaoMonk, "
            "Strambotica, "
            "Spodi, "
            "Swimms, "
            "TeamscorpioRI, "
            "ValariusK, "
            "ValkyrieDuskfire, "
            "Vilhena, "
            "Vychos, "
            "warioman91, "
            "wolvemd, "
            "Xycona, "
            "Zretch");
    m_bugReporters.SetSel(-1);    // nothing starts selected
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


BOOL CAboutDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    GetDlgItem(IDOK)->SetFocus();

    return FALSE;
}
