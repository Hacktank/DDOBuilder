// BreakdownItemSave.cpp
//
#include "stdafx.h"
#include "BreakdownItemSave.h"

#include "BreakdownsView.h"
#include "GlobalSupportFunctions.h"

BreakdownItemSave::BreakdownItemSave(
        BreakdownType type,
        SaveType st, 
        MfcControls::CTreeListCtrl * treeList,
        HTREEITEM hItem,
        AbilityType ability,
        BreakdownItemSave * pBaseItem) :
    BreakdownItem(type, treeList, hItem),
    m_saveType(st),
    m_ability(ability),
    m_pBaseBreakdown(pBaseItem),
    m_noFailOnOne(type, Effect_SaveNoFailOn1, "", NULL, hItem)
{
    // need to know when base total value changes
    if (ability != Ability_Unknown)
    {
        AddAbility(ability);
    }
    if (m_pBaseBreakdown != NULL)
    {
        // need to know changes in our base save type
        m_pBaseBreakdown->AttachObserver(this);
        m_pBaseBreakdown->m_noFailOnOne.AttachObserver(this);
    }
    m_noFailOnOne.AttachObserver(this);
    m_baseAbility = m_mainAbility;
}

BreakdownItemSave::~BreakdownItemSave()
{
}

void BreakdownItemSave::SetCharacter(Character * charData, bool observe)
{
    // pass through to the base class
    BreakdownItem::SetCharacter(charData, observe);
    // and set on our owned element
    m_noFailOnOne.SetCharacter(charData, false);
}

// required overrides
CString BreakdownItemSave::Title() const
{
    CString text;
    text = EnumEntryText(m_saveType, saveTypeMap);
    if (m_saveType != Save_Fortitude
            && m_saveType != Save_Reflex
            && m_saveType != Save_Will)
    {
        text = "vs " + text;
    }
    return text;
}

CString BreakdownItemSave::Value() const
{
    CString value;

    value.Format(
            "%4d",
            (int)Total());
    if (HasNoFailOn1())
    {
        value += " (No fail on 1)";
    }
    return value;
}

void BreakdownItemSave::CreateOtherEffects()
{
    // add class/ability saves for the generic saves types of Fortitude, Reflex and Will
    if (m_pCharacter != NULL)
    {
        m_otherEffects.clear();
        if (m_saveType == Save_Fortitude
                || m_saveType == Save_Reflex
                || m_saveType == Save_Will)
        {
            std::vector<size_t> classLevels = m_pCharacter->ClassLevels(m_pCharacter->MaxLevel());
            for (size_t ci = Class_Unknown; ci < Class_Count; ++ci)
            {
                if (classLevels[ci] > 0
                        && ci != Class_Epic)
                {
                    std::string className = EnumEntryText((ClassType)ci, classTypeMap);
                    className += " Levels";
                    ActiveEffect classBonus(
                            Bonus_class,
                            className,
                            1,
                            ClassSave(m_saveType, (ClassType)ci, classLevels[ci]),
                            "");        // no tree
                    AddOtherEffect(classBonus);
                }
            }
            // special case, look for Divine Grace feat
            size_t count = TrainedCount(m_pCharacter->CurrentFeats(m_pCharacter->MaxLevel()), "Divine Grace");
            if (count > 0)
            {
                // character has divine grace feat trained. Add charisma bonus for this save
                // Divine Grace is capped at 2+(3 x paladin level).
                // For multiclassing, this means 2 levels of Paladin would at best
                // grant a max of +8 to saves (2 + {3x2}). 
                int maxBonus = ((classLevels[Class_Paladin] + classLevels[Class_PaladinSacredFist])* 3) + 2;
                BreakdownItem * pBI = FindBreakdown(StatToBreakdown(Ability_Charisma));
                ASSERT(pBI != NULL);
                pBI->AttachObserver(this); // watch for any changes
                int bonus = BaseStatToBonus(pBI->Total());
                bonus = min(bonus, maxBonus);
                if (bonus > 0) // only add to list if not negative
                {
                    // should now have the best option
                    CString text;
                    text.Format(
                            "Divine Grace (Charisma) (Capped @ %d)",
                            maxBonus);
                    ActiveEffect feat(
                            Bonus_ability,
                            (LPCTSTR)text,
                            1,
                            bonus,
                            "");        // no tree
                    AddOtherEffect(feat);
                }
            }
            else
            {
                // now check for the half elf: Paladin Dilettante feat
                count = TrainedCount(m_pCharacter->CurrentFeats(m_pCharacter->MaxLevel()), "Half-Elf Dilettante: Paladin");
                if (count > 0)
                {
                    // yes, they have it. Work out what the max capped charisma bonus to saves is
                    // its 2 + any enhancement upgrades
                    int maxBonus = 2; // default max bonus is 2
                    if (m_pCharacter->IsEnhancementTrained(
                            "HalfElfImprovedDilettanteI",
                            "Improved Dilettante: Paladin",
                            TT_enhancement))
                    {
                        maxBonus++;
                    }
                    if (m_pCharacter->IsEnhancementTrained(
                            "HalfElfImprovedDilettanteII",
                            "Improved Dilettante: Paladin",
                            TT_enhancement))
                    {
                        maxBonus++;
                    }
                    if (m_pCharacter->IsEnhancementTrained(
                            "HalfElfImprovedDilettanteIII",
                            "Improved Dilettante: Paladin",
                            TT_enhancement))
                    {
                        maxBonus++;
                    }
                    BreakdownItem * pBI = FindBreakdown(StatToBreakdown(Ability_Charisma));
                    ASSERT(pBI != NULL);
                    pBI->AttachObserver(this); // watch for any changes
                    int bonus = BaseStatToBonus(pBI->Total());
                    // cap it if required
                    bonus = min(bonus, maxBonus);
                    if (bonus != 0) // only add to list if non zero
                    {
                        CString text;
                        text.Format(
                                "Lesser Divine Grace (Charisma) (Capped @ %d)",
                                maxBonus);
                        // should now have the best option
                        ActiveEffect feat(
                                Bonus_ability,
                                (LPCTSTR)text,
                                1,
                                bonus,
                                "");        // no tree
                        AddOtherEffect(feat);
                    }
                }
            }
        }
        if (m_ability != Ability_Unknown)
        {
            // Base ability bonus to save
            AbilityType ability = LargestStatBonus();
            BreakdownItem * pBI = FindBreakdown(StatToBreakdown(ability));
            ASSERT(pBI != NULL);
            int bonus = BaseStatToBonus(pBI->Total());
            if (bonus != 0) // only add to list if non zero
            {
                // should now have the best option
                std::string bonusName = "Ability bonus (" + EnumEntryText(ability, abilityTypeMap) + ")";
                ActiveEffect feat(
                        Bonus_ability,
                        bonusName,
                        1,
                        bonus,
                        "");        // no tree
                feat.SetBreakdownDependency(StatToBreakdown(ability)); // so we know which effect to update
                feat.SetDivider(1, DT_statBonus);
                AddOtherEffect(feat);
            }
        }
        if (m_pBaseBreakdown != NULL)
        {
            // show the base save value used in our total
            double bonus = m_pBaseBreakdown->Total();
            ActiveEffect statBonus(
                    Bonus_base,
                    (LPCTSTR)(m_pBaseBreakdown->Title() + " Save"),
                    1,
                    bonus,
                    "");        // no tree
            AddOtherEffect(statBonus);
        }
    }
}

bool BreakdownItemSave::AffectsUs(const Effect & effect) const
{
    // return true if the effect applies to this save
    // note that effect that apply to "All" only apply to Fortitude, Reflex and Will
    // as the sub-save types use the total from the main category as a part of their total
    // so we do not want to count the bonus twice
    bool isUs = false;
    if (effect.Type() == Effect_SaveBonus)
    {
        isUs = IsOurSaveType(effect.Save());
    }
    return isUs;
}

bool BreakdownItemSave::IsOurSaveType(SaveType st) const
{
    bool isUs = false;
    if (m_saveType == Save_Fortitude
            || m_saveType == Save_Reflex
            || m_saveType == Save_Will)
    {
        isUs = (st == Save_All
                || st == m_saveType);
    }
    else
    {
        // for a sub-type, it must be the specific save type only
        isUs = (st == m_saveType);
    }
    return isUs;
}

void BreakdownItemSave::UpdateClassChanged(
        Character * charData,
        ClassType classFrom,
        ClassType classTo,
        size_t level)
{
    BreakdownItem::UpdateClassChanged(charData, classFrom, classTo, level);
    // need to re-create other effects list
    CreateOtherEffects();
    Populate();
}

void BreakdownItemSave::UpdateEnhancementTrained(
        Character * charData,
        const std::string & enhancementName,
        const std::string & selection,
        bool isTier5)
{
    BreakdownItem::UpdateEnhancementTrained(charData, enhancementName, selection, isTier5);
    if (enhancementName == "HalfElfImprovedDilettanteI"
            || enhancementName == "HalfElfImprovedDilettanteII"
            || enhancementName == "HalfElfImprovedDilettanteIII")
    {
        // need to re-create other effects list
        CreateOtherEffects();
        Populate();
    }
}

void BreakdownItemSave::UpdateEnhancementRevoked(
        Character * charData,
        const std::string & enhancementName,
        const std::string & selection,
        bool isTier5)
{
    BreakdownItem::UpdateEnhancementTrained(charData, enhancementName, selection, isTier5);
    if (enhancementName == "HalfElfImprovedDilettanteI"
            || enhancementName == "HalfElfImprovedDilettanteII"
            || enhancementName == "HalfElfImprovedDilettanteIII")
    {
        // need to re-create other effects list
        CreateOtherEffects();
        Populate();
    }
}

void BreakdownItemSave::UpdateTotalChanged(
        BreakdownItem * item,
        BreakdownType type)
{
    // Stat bonus has changed
    CreateOtherEffects();
    // do base class stuff also
    BreakdownItem::UpdateTotalChanged(item, type);
    // a no fail on a 1 may have changed, ensure updates
    Populate();
}

void BreakdownItemSave::UpdateFeatEffect(
        Character * pCharacter,
        const std::string & featName,
        const Effect & effect)
{
    if (effect.Type() == Effect_SaveNoFailOn1
            && IsOurSaveType(effect.Save()))
    {
        m_noFailOnOne.UpdateFeatEffect(pCharacter, featName, effect);
    }
    // handle special affects that change our list of available stats
    if (AffectsUs(effect))
    {
        if (effect.HasAbility())
        {
            // add to the list of available stats for this save
            ASSERT(effect.HasAbility());
            AddAbility(effect.Ability());  // duplicates are fine
            CreateOtherEffects();
        }
        else
        {
            // pass through to the base class
            BreakdownItem::UpdateFeatEffect(pCharacter, featName, effect);
        }
    }
}

void BreakdownItemSave::UpdateFeatEffectRevoked(
        Character * pCharacter,
        const std::string & featName,
        const Effect & effect)
{
    if (effect.Type() == Effect_SaveNoFailOn1
            && IsOurSaveType(effect.Save()))
    {
        m_noFailOnOne.UpdateFeatEffectRevoked(pCharacter, featName, effect);
    }
    // handle special affects that change our list of available stats
    if (AffectsUs(effect))
    {
        if (effect.HasAbility())
        {
            ASSERT(effect.HasAbility());
            RemoveFirstAbility(effect.Ability());
            CreateOtherEffects();
        }
        else
        {
            // pass through to the base class
            BreakdownItem::UpdateFeatEffectRevoked(pCharacter, featName, effect);
        }
    }
}

void BreakdownItemSave::UpdateItemEffect(
        Character * pCharacter,
        const std::string & itemName,
        const Effect & effect)
{
    if (effect.Type() == Effect_SaveNoFailOn1
            && IsOurSaveType(effect.Save()))
    {
        m_noFailOnOne.UpdateItemEffect(pCharacter, itemName, effect);
    }
    // handle special affects that change our list of available stats
    if (AffectsUs(effect))
    {
        if (effect.HasAbility())
        {
            // add to the list of available stats for this save
            ASSERT(effect.HasAbility());
            AddAbility(effect.Ability());  // duplicates are fine
            CreateOtherEffects();
        }
        else
        {
            // pass through to the base class
            BreakdownItem::UpdateItemEffect(pCharacter, itemName, effect);
        }
    }
}

void BreakdownItemSave::UpdateItemEffectRevoked(
        Character * pCharacter,
        const std::string & itemName,
        const Effect & effect)
{
    if (effect.Type() == Effect_SaveNoFailOn1
            && IsOurSaveType(effect.Save()))
    {
        m_noFailOnOne.UpdateItemEffectRevoked(pCharacter, itemName, effect);
    }
    // handle special affects that change our list of available stats
    if (AffectsUs(effect))
    {
        if (effect.HasAbility())
        {
            ASSERT(effect.HasAbility());
            RemoveFirstAbility(effect.Ability());
            CreateOtherEffects();
        }
        else
        {
            // pass through to the base class
            BreakdownItem::UpdateItemEffectRevoked(pCharacter, itemName, effect);
        }
    }
}

void BreakdownItemSave::UpdateEnhancementEffect(
        Character * pCharacter,
        const std::string & enhancementName,
        const EffectTier & effect)
{
    if (effect.m_effect.Type() == Effect_SaveNoFailOn1
            && IsOurSaveType(effect.m_effect.Save()))
    {
        m_noFailOnOne.UpdateEnhancementEffect(pCharacter, enhancementName, effect);
    }
    // handle special affects that change our list of available stats
    if (AffectsUs(effect.m_effect))
    {
        if (effect.m_effect.HasAbility())
        {
            // add to the list of available stats for this save
            ASSERT(effect.m_effect.HasAbility());
            AddAbility(effect.m_effect.Ability());  // duplicates are fine
            CreateOtherEffects();
        }
        else
        {
            // pass through to the base class
            BreakdownItem::UpdateEnhancementEffect(pCharacter, enhancementName, effect);
        }
    }
}

void BreakdownItemSave::UpdateEnhancementEffectRevoked(
        Character * pCharacter,
        const std::string & enhancementName,
        const EffectTier & effect)
{
    if (effect.m_effect.Type() == Effect_SaveNoFailOn1
            && IsOurSaveType(effect.m_effect.Save()))
    {
        m_noFailOnOne.UpdateEnhancementEffectRevoked(pCharacter, enhancementName, effect);
    }
    // handle special affects that change our list of available stats
    if (AffectsUs(effect.m_effect))
    {
        if (effect.m_effect.HasAbility())
        {
            ASSERT(effect.m_effect.HasAbility());
            RemoveFirstAbility(effect.m_effect.Ability());
            CreateOtherEffects();
        }
        else
        {
            // pass through to the base class
            BreakdownItem::UpdateEnhancementEffectRevoked(pCharacter, enhancementName, effect);
        }
    }
}

bool BreakdownItemSave::HasNoFailOn1() const
{
    if (m_pBaseBreakdown != NULL)
    {
        // saves which are based on a main save type (Will, Reflex and Fortitude)
        // will have a no fail on a 1 in their base save also has a no fail on a 1
        BreakdownItemSave * pBB = dynamic_cast<BreakdownItemSave*>(m_pBaseBreakdown);
        if (pBB->HasNoFailOn1())
        {
            return true;
        }
    }
    return (m_noFailOnOne.Total() > 0);
}

void BreakdownItemSave::AppendItems(CListCtrl * pControl)
{
    if (m_noFailOnOne.Total() != 0)
    {
        // add the stuff for max caster level below the caster level stuff
        size_t start = pControl->GetItemCount();
        pControl->InsertItem(start, "");
        pControl->InsertItem(start + 1, "No Fail on a 1");
        m_noFailOnOne.AddItems(pControl);
    }
}
