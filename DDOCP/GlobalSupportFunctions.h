// GlobalSupportFunctions.h
//
#include <string>
#include <list>
#include "XmlLib\SaxContentElement.h"
#include "Feat.h"
#include "TrainedFeat.h"
#include "DDOCP.h"
#include "AlignmentTypes.h"
#include "BreakdownTypes.h"

class BreakdownItem;
class Character;
class CMainFrame;
class MouseHook;

void LoadColumnWidthsByName(
        CListCtrl * control, 
        const std::string & fmtString);
void SaveColumnWidthsByName(
        CListCtrl * control, 
        const std::string & fmtString);
CSize RequiredControlSizeForContent(
        CListCtrl * control, 
        int maxItemsToShow,
        int requiredItems = 0); // non zero to force sizing to this number of items

template <typename T>
CString EnumEntryText(const T & t, const XmlLib::enumMapEntry<T> * m)
{
    const XmlLib::enumMapEntry<T> * p;
    for (p = m; p->name != NULL; ++p)
    {
        if (p->value == t)
        {
            break;
        }
    }
    if (p->name == NULL) // stopped at end of list
    {
        THROW("EnumEntryText failed to find entry");
    }
    XmlLib::SaxString entry = p->name;
    CString asAscii = ((std::string)entry).c_str();
    return asAscii;
}

template <typename T>
T TextToEnumEntry(const std::string & name, const XmlLib::enumMapEntry<T> * m)
{
    const XmlLib::enumMapEntry<T> * p;
    for (p = m; p->name != NULL; ++p)
    {
        XmlLib::SaxString entry = p->name;
        CString asAscii = ((std::string)entry).c_str();
        if (name == (LPCTSTR)asAscii)
        {
            break;
        }
    }
    if (p->name == NULL) // stopped at end of list
    {
        THROW("TextToEnumEntry failed to find entry");
    }
    return p->value;
}

BOOL OnEraseBackground(
        CWnd * pWnd,
        CDC * pDC,
        const int controlsNotToBeErased[]);

const std::list<Feat> & AllFeats();
const std::list<Feat> & StandardFeats();
const std::list<Feat> & HeroicPastLifeFeats();
const std::list<Feat> & RacialPastLifeFeats();
const std::list<Feat> & IconicPastLifeFeats();
const std::list<Feat> & EpicPastLifeFeats();
const std::list<Feat> & SpecialFeats();
const std::list<Feat> & FavorFeats();
const std::list<EnhancementTree> & EnhancementTrees();
const std::list<Spell> & Spells();
const std::list<Item> & Items();
const std::list<Augment> & Augments();
const std::list<GuildBuff> & GuildBuffs();
const std::list<OptionalBuff> & OptionalBuffs();
const std::list<SetBonus> & SetBonuses();
MouseHook * GetMouseHook();
std::vector<Spell> FilterSpells(ClassType ct, int level);
Spell FindSpellByName(const std::string & name);
const EnhancementTree & GetEnhancementTree(const std::string & treeName);
const EnhancementTreeItem * FindEnhancement(const std::string & internalName, std::string * treeName = NULL);
std::vector<TrainableFeatTypes> ClassSpecificFeatTypes(ClassType type);
std::vector<TrainableFeatTypes> RaceSpecificFeatTypes(RaceType type);
const Augment & FindAugmentByName(const std::string & name);
std::vector<Augment> CompatibleAugments(const std::string & name);
const SetBonus& FindSetBonus(const std::string& name);

AbilityType StatFromSkill(SkillType skill);
int ArmorCheckPenalty_Multiplier(SkillType skill);
int FindItemIndexByItemData(CListCtrl * pControl, DWORD itemData);
bool IsClassSkill(ClassType type, SkillType skill);
size_t SkillPoints(ClassType type, RaceType race, size_t intelligence, size_t level);
void SelectComboboxEntry(size_t itemData, CComboBox * pCombo);
size_t GetComboboxSelection(CComboBox * pCombo);
int RacialModifier(RaceType race, AbilityType ability);
bool CanTrainClass(ClassType type, AlignmentType alignment);
bool IsInGroup(TrainableFeatTypes type, const FeatGroup & group);
size_t TrainedCount(const std::list<TrainedFeat> & currentFeats, const std::string & featName);
const Feat & FindFeat(const std::string & featName);
const TrainedFeat * FindFeatInList(const std::list<TrainedFeat> & featList, const std::string & featName);
size_t ClassHitpoints(ClassType type);
size_t ClassSave(SaveType st, ClassType ct, size_t level);
std::vector<size_t> SpellSlotsForClass(ClassType ct, size_t level);
size_t ClassSpellPoints(ClassType ct, size_t level);
AbilityType ClassCastingStat(ClassType ct);
const Item & FindItem(const std::string & itemName);
size_t StanceStackCount(const std::string & stanceName);

enum ImageType
{
    IT_enhancement,
    IT_feat,
    IT_spell,
    IT_ui,
    IT_item,
    IT_augment,
};
HRESULT LoadImageFile(ImageType type, const std::string & name, CImage * pImage, bool loadDefaultOnFail = true);
bool ImageFileExists(ImageType type, const std::string & name);
CMainFrame * GetMainFrame();
void MakeGrayScale(CImage * pImage, COLORREF transparent = RGB(0, 0, 0));
CString TrainableFeatTypeLabel(TrainableFeatTypes type);
// support functions for breakdowns only
int BaseStatToBonus(double ability);
BreakdownType StatToBreakdown(AbilityType ability);
BreakdownType SkillToBreakdown(SkillType skill);
BreakdownType TacticalToBreakdown(TacticalType tactical);
BreakdownType SchoolToBreakdown(SpellSchoolType school);
BreakdownType ClassToCasterLevelsBreakdown(ClassType ct);
BreakdownType EnergyToCasterLevelsBreakdown(EnergyType energy);
BreakdownType SchoolToCasterLevelsBreakdown(SpellSchoolType school);
BreakdownItem * FindBreakdown(BreakdownType type);
double BAB(ClassType ct);
bool IsInWeaponClass(WeaponClassType wc, WeaponType wt);
bool IsMartialWeapon(WeaponType wt);
bool IsSimpleWeapon(WeaponType wt);
bool IsExoticWeapon(WeaponType wt);
bool IsRangedWeapon(WeaponType wt);
bool IsLightWeapon(WeaponType wt);
bool IsThrownWeapon(WeaponType wt);
bool IsFinesseableWeapon(WeaponType wt);
bool IsShield(WeaponType wt);
bool IsOneHandedWeapon(WeaponType wt);
bool IsTwoHandedWeapon(WeaponType wt);
bool IsAxe(WeaponType wt);
bool IsBow(WeaponType wt);
bool IsCrossbow(WeaponType wt);
bool IsRepeatingCrossbow(WeaponType wt);
bool IsMeleeWeapon(WeaponType wt);
bool CanEquipTo2ndWeapon(Character * pCharacter, const Item & item);
bool LimitToRuneArm(Character * pCharacter);

OptionalBuff FindOptionalBuff(const std::string & name);
void FormatExportData(std::string * exportData);
void FormatExportData(CString * exportData);
size_t CasterLevel(Character * pCharacter, ClassType c);

bool AddMenuItem(HMENU hTargetMenu, const CString& itemText, UINT itemID);
