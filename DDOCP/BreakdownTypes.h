// BreakdownTypes.h
//
#pragma once

enum BreakdownType
{
    Breakdown_Unknown = 0,
    // abilities
    Breakdown_Strength,
    Breakdown_Dexterity,
    Breakdown_Constitution,
    Breakdown_Intelligence,
    Breakdown_Wisdom,
    Breakdown_Charisma,
    // saves
    Breakdown_SaveFortitude,
    Breakdown_SaveReflex,
    Breakdown_SaveWill,
    Breakdown_SaveEnchantment,
    Breakdown_SaveFear,
    Breakdown_SaveIllusion,
    Breakdown_SavePoison,
    Breakdown_SaveDisease,
    Breakdown_SaveSpell,
    Breakdown_SaveMagic,
    Breakdown_SaveTraps,
    // physical
    Breakdown_ArmorCheckPenalty,
    Breakdown_BAB,
    Breakdown_MovementSpeed,
    Breakdown_Hitpoints,
    Breakdown_UnconsciousRange,
    Breakdown_PRR,
    Breakdown_MRR,
    Breakdown_AC,
    Breakdown_Dodge,
    Breakdown_DodgeCap,
    Breakdown_OffHandAttackBonus,
    Breakdown_DoubleStrike,
    Breakdown_OffHandDoubleStrike,
    Breakdown_DoubleShot,
    Breakdown_Fortification,
    Breakdown_HealingAmplification,
    Breakdown_NegativeHealingAmplification,
    Breakdown_RepairAmplification,
    Breakdown_Threat,
    Breakdown_DodgeBypass,
    Breakdown_FortificationBypass,
    Breakdown_Assassinate,
    Breakdown_TacticalStunningBlow,
    Breakdown_TacticalStunningFist,
    Breakdown_TacticalSunder,
    Breakdown_TacticalTrip,
    Breakdown_TacticalStunningShield,
    Breakdown_MissileDeflection,
    Breakdown_MissileDeflectionBypass,
    Breakdown_GlancingBlowsChance,
    Breakdown_GlancingBlowsDamage,
    Breakdown_GlancingBlowEffects,
    // skills
    Breakdown_SkillBalance,
    Breakdown_SkillBluff,
    Breakdown_SkillConcentration,
    Breakdown_SkillDiplomacy,
    Breakdown_SkillDisableDevice,
    Breakdown_SkillHaggle,
    Breakdown_SkillHeal,
    Breakdown_SkillHide,
    Breakdown_SkillIntimidate,
    Breakdown_SkillJump,
    Breakdown_SkillListen,
    Breakdown_SkillMoveSilently,
    Breakdown_SkillOpenLock,
    Breakdown_SkillPerform,
    Breakdown_SkillRepair,
    Breakdown_SkillSearch,
    Breakdown_SkillSpellCraft,
    Breakdown_SkillSpot,
    Breakdown_SkillSwim,
    Breakdown_SkillTumble,
    Breakdown_SkillUMD,
    // magical
    Breakdown_Spellpoints,
    Breakdown_ArcaneSpellfailure,
    Breakdown_ArcaneSpellfailureShields,
    Breakdown_SpellResistance,
    Breakdown_CasterLevel_Artificer,
    Breakdown_CasterLevel_Cleric,
    Breakdown_CasterLevel_Druid,
    Breakdown_CasterLevel_FavoredSoul,
    Breakdown_CasterLevel_Paladin,
    Breakdown_CasterLevel_Sorcerer,
    Breakdown_CasterLevel_Ranger,
    Breakdown_CasterLevel_Warlock,
    Breakdown_CasterLevel_Wizard,
    Breakdown_SpellPenetration,
    Breakdown_SpellPowerAcid,
    Breakdown_SpellPowerAlignment,
    Breakdown_SpellPowerCold,
    Breakdown_SpellPowerEarth,
    Breakdown_SpellPowerElectric,
    Breakdown_SpellPowerFire,
    Breakdown_SpellPowerForce,
    Breakdown_SpellPowerLight,
    Breakdown_SpellPowerNegative,
    Breakdown_SpellPowerPhysical,
    Breakdown_SpellPowerPositive,
    Breakdown_SpellPowerRepair,
    Breakdown_SpellPowerRust,
    Breakdown_SpellPowerSonic,
    Breakdown_SpellPowerUntyped,
    Breakdown_SpellPowerWater,
    Breakdown_SpellCriticalChanceAcid,
    Breakdown_SpellCriticalChanceAlignment,
    Breakdown_SpellCriticalChanceCold,
    Breakdown_SpellCriticalChanceEarth,
    Breakdown_SpellCriticalChanceElectric,
    Breakdown_SpellCriticalChanceFire,
    Breakdown_SpellCriticalChanceForce,
    Breakdown_SpellCriticalChanceLight,
    Breakdown_SpellCriticalChanceNegative,
    Breakdown_SpellCriticalChancePhysical,
    Breakdown_SpellCriticalChancePositive,
    Breakdown_SpellCriticalChanceRepair,
    Breakdown_SpellCriticalChanceRust,
    Breakdown_SpellCriticalChanceSonic,
    Breakdown_SpellCriticalChanceUntyped,
    Breakdown_SpellCriticalChanceWater,
    Breakdown_SpellCriticalMultiplierAcid,
    Breakdown_SpellCriticalMultiplierAlignment,
    Breakdown_SpellCriticalMultiplierCold,
    Breakdown_SpellCriticalMultiplierEarth,
    Breakdown_SpellCriticalMultiplierElectric,
    Breakdown_SpellCriticalMultiplierFire,
    Breakdown_SpellCriticalMultiplierForce,
    Breakdown_SpellCriticalMultiplierLight,
    Breakdown_SpellCriticalMultiplierNegative,
    Breakdown_SpellCriticalMultiplierPhysical,
    Breakdown_SpellCriticalMultiplierPositive,
    Breakdown_SpellCriticalMultiplierRepair,
    Breakdown_SpellCriticalMultiplierRust,
    Breakdown_SpellCriticalMultiplierSonic,
    Breakdown_SpellCriticalMultiplierUntyped,
    Breakdown_SpellCriticalMultiplierWater,
    Breakdown_SpellSchoolAbjuration,
    Breakdown_SpellSchoolConjuration,
    Breakdown_SpellSchoolDivination,
    Breakdown_SpellSchoolEnchantment,
    Breakdown_SpellSchoolEvocation,
    Breakdown_SpellSchoolIllusion,
    Breakdown_SpellSchoolNecromancy,
    Breakdown_SpellSchoolTransmutation,
    // other
    Breakdown_EnergyResistanceAcid,
    Breakdown_EnergyResistanceCold,
    Breakdown_EnergyResistanceElectric,
    Breakdown_EnergyResistanceFire,
    Breakdown_EnergyResistanceForce,
    Breakdown_EnergyResistanceLight,
    Breakdown_EnergyResistanceNegative,
    Breakdown_EnergyResistancePoison,
    Breakdown_EnergyResistancePositive,
    Breakdown_EnergyResistanceRepair,
    Breakdown_EnergyResistanceRust,
    Breakdown_EnergyResistanceSonic,
    Breakdown_EnergyAbsorptionAcid,
    Breakdown_EnergyAbsorptionCold,
    Breakdown_EnergyAbsorptionElectric,
    Breakdown_EnergyAbsorptionFire,
    Breakdown_EnergyAbsorptionForce,
    Breakdown_EnergyAbsorptionLight,
    Breakdown_EnergyAbsorptionNegative,
    Breakdown_EnergyAbsorptionPoison,
    Breakdown_EnergyAbsorptionPositive,
    Breakdown_EnergyAbsorptionRepair,
    Breakdown_EnergyAbsorptionRust,
    Breakdown_EnergyAbsorptionSonic,
    // weapon breakdowns
    Breakdown_Weapon,
    Breakdown_WeaponBaseDamage,
    Breakdown_WeaponAttackBonus,
    Breakdown_WeaponDamageBonus,
    Breakdown_WeaponCriticalThreatRange,
    Breakdown_WeaponCriticalAttackBonus,
    Breakdown_WeaponCriticalDamageBonus,
    Breakdown_WeaponCriticalMultiplier,
    Breakdown_WeaponSpecialMultiplier,
    Breakdown_WeaponAttackSpeed,
    // melee power
    Breakdown_MeleePower,
    Breakdown_RangedPower,
    // hirelings
    Breakdown_HirelingAbilityBonus,
    Breakdown_HirelingHitpoints,
    Breakdown_HirelingFortification,
    Breakdown_HirelingPRR,
    Breakdown_HirelingMRR,
    Breakdown_HirelingDodge,
    Breakdown_HirelingMeleePower,
    Breakdown_HirelingRangedPower,
    Breakdown_HirelingSpellPower,
    // Turn Undead
    Breakdown_TurnUndeadLevel,
    Breakdown_TurnUndeadHitDice,
};
