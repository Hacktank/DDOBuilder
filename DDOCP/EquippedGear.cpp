// EquippedGear.cpp
//
#include "StdAfx.h"
#include "EquippedGear.h"
#include "XmlLib\SaxWriter.h"
#include "GlobalSupportFunctions.h"
#include "Character.h"

#define DL_ELEMENT EquippedGear

namespace
{
    const wchar_t f_saxElementName[] = L"EquippedGear";
    DL_DEFINE_NAMES(EquippedGear_PROPERTIES)

    const unsigned f_verCurrent = 1;
}

EquippedGear::EquippedGear(const std::string & name) :
    XmlLib::SaxContentElement(f_saxElementName, f_verCurrent),
    m_Helmet(L"Helmet"),
    m_Necklace(L"Necklace"),
    m_Trinket(L"Trinket"),
    m_Cloak(L"Cloak"),
    m_Belt(L"Belt"),
    m_Goggles(L"Goggles"),
    m_Gloves(L"Gloves"),
    m_Boots(L"Boots"),
    m_Bracers(L"Bracers"),
    m_Armor(L"Armor"),
    m_MainHand(L"MainHand"),
    m_OffHand(L"OffHand"),
    m_Quiver(L"Quiver"),
    m_Arrow(L"Arrow"),
    m_Ring1(L"Ring1"),
    m_Ring2(L"Ring2"),
    m_Trinket2(L"Trinket2"),
    m_Trinket3(L"Trinket3")
{
    DL_INIT(EquippedGear_PROPERTIES)
    m_Name = name;
}

DL_DEFINE_ACCESS(EquippedGear_PROPERTIES)

XmlLib::SaxContentElementInterface * EquippedGear::StartElement(
        const XmlLib::SaxString & name,
        const XmlLib::SaxAttributes & attributes)
{
    XmlLib::SaxContentElementInterface * subHandler =
            SaxContentElement::StartElement(name, attributes);

    DL_START(EquippedGear_PROPERTIES)

    return subHandler;
}

void EquippedGear::EndElement()
{
    // legacy file conversion
    // if the main hand weapon has a SentientJewel object, move it
    // to us instead
    if (m_hasMainHand)
    {
        if (m_MainHand.HasSentientIntelligence())
        {
            // copy it
            m_SentientIntelligence = m_MainHand.m_SentientIntelligence;
            m_hasSentientIntelligence = true;
            // and remove it from item
            m_MainHand.m_hasSentientIntelligence = false;
        }
    }
    // make sure gear sets without a sentient weapon load
    m_hasSentientIntelligence = true;
    SaxContentElement::EndElement();
    DL_END(EquippedGear_PROPERTIES)
}

void EquippedGear::Write(XmlLib::SaxWriter * writer) const
{
    writer->StartElement(ElementName());//, VersionAttributes());
    DL_WRITE(EquippedGear_PROPERTIES)
    writer->EndElement();
}

bool EquippedGear::HasItemInSlot(InventorySlotType slot) const
{
    switch (slot)
    {
    case Inventory_Arrows:  return HasArrow();
    case Inventory_Armor:   return HasArmor();
    case Inventory_Belt:    return HasBelt();
    case Inventory_Boots:   return HasBoots();
    case Inventory_Bracers: return HasBracers();
    case Inventory_Cloak:   return HasCloak();
    case Inventory_Gloves:  return HasGloves();
    case Inventory_Goggles: return HasGoggles();
    case Inventory_Helmet:  return HasHelmet();
    case Inventory_Necklace:return HasNecklace();
    case Inventory_Quiver:  return HasQuiver();
    case Inventory_Ring1:   return HasRing1();
    case Inventory_Ring2:   return HasRing2();
    case Inventory_Trinket: return HasTrinket();
    case Inventory_Weapon1: return HasMainHand();
    case Inventory_Weapon2: return HasOffHand();
    case Inventory_Trinket2:return HasTrinket2();
    case Inventory_Trinket3:return HasTrinket3();
    }
    return false;
}

Item EquippedGear::ItemInSlot(InventorySlotType slot) const
{
    Item noItem;
    switch (slot)
    {
    case Inventory_Arrows:
        if (HasArrow())
        {
            return Arrow();
        }
        break;
    case Inventory_Armor:
        if (HasArmor())
        {
            return Armor();
        }
        break;
    case Inventory_Belt:
        if (HasBelt())
        {
            return Belt();
        }
        break;
    case Inventory_Boots:
        if (HasBoots())
        {
            return Boots();
        }
        break;
    case Inventory_Bracers:
        if (HasBracers())
        {
            return Bracers();
        }
        break;
    case Inventory_Cloak:
        if (HasCloak())
        {
            return Cloak();
        }
        break;
    case Inventory_Gloves:
        if (HasGloves())
        {
            return Gloves();
        }
        break;
    case Inventory_Goggles:
        if (HasGoggles())
        {
            return Goggles();
        }
        break;
    case Inventory_Helmet:
        if (HasHelmet())
        {
            return Helmet();
        }
        break;
    case Inventory_Necklace:
        if (HasNecklace())
        {
            return Necklace();
        }
        break;
    case Inventory_Quiver:
        if (HasQuiver())
        {
            return Quiver();
        }
        break;
    case Inventory_Ring1:
        if (HasRing1())
        {
            return Ring1();
        }
        break;
    case Inventory_Ring2:
        if (HasRing2())
        {
            return Ring2();
        }
        break;
    case Inventory_Trinket:
        if (HasTrinket())
        {
            return Trinket();
        }
        break;
    case Inventory_Weapon1:
        if (HasMainHand())
        {
            return MainHand();
        }
        break;
    case Inventory_Weapon2:
        if (HasOffHand())
        {
            return OffHand();
        }
        break;
    case Inventory_Trinket2:
        if (HasTrinket2())
        {
            return Trinket2();
        }
        break;
    case Inventory_Trinket3:
        if (HasTrinket3())
        {
            return Trinket3();
        }
        break;
    }
    return noItem;
}

bool EquippedGear::IsSlotRestricted(
        InventorySlotType slot,
        Character * pChar) const
{
    bool bRestricted = false;

    if (slot == Inventory_Weapon2
            && HasItemInSlot(Inventory_Weapon1)
            && !CanEquipTo2ndWeapon(pChar, ItemInSlot(Inventory_Weapon1)))
    {
        // Weapon equipped in main hand that precludes weapon in off hand
        // do not permit selection of an item in the off hand slot
        bRestricted = true;
    }
    else
    {
        // check to see if this restricted by any item in any other slot
        for (size_t i = Inventory_Unknown + 1; i < Inventory_Count; ++i)
        {
            if (HasItemInSlot((InventorySlotType)i)
                    && ItemInSlot((InventorySlotType)i).HasRestrictedSlots()
                    && ItemInSlot((InventorySlotType)i).RestrictedSlots().HasSlot(slot))
            {
                bRestricted = true;
            }
        }
    }
    // Kobold gear restrictions
    if (pChar != NULL)
    {
        switch (pChar->Race())
        {
        case Race_Kobold:
        case Race_KoboldShamen:
            // kobold cannot wear boots or gloves
            if (slot == Inventory_Boots || slot == Inventory_Gloves)
            {
                bRestricted = true;
            }
            break;
        default:
            if (slot == Inventory_Trinket2 || slot == Inventory_Trinket3)
            {
                bRestricted = true;
            }
            break;
        }
    }
    return bRestricted;
}

void EquippedGear::SetItem(
        InventorySlotType slot,
        Character * pCharacter,
        const Item & item)
{
    bool itemsRemoved = false;
    std:: stringstream ss;
    switch (slot)
    {
    case Inventory_Arrows:  Set_Arrow(item); break;
    case Inventory_Armor:   Set_Armor(item); break;
    case Inventory_Belt:    Set_Belt(item); break;
    case Inventory_Boots:   Set_Boots(item); break;
    case Inventory_Bracers: Set_Bracers(item); break;
    case Inventory_Cloak:   Set_Cloak(item); break;
    case Inventory_Gloves:  Set_Gloves(item); break;
    case Inventory_Goggles: Set_Goggles(item); break;
    case Inventory_Helmet:  Set_Helmet(item); break;
    case Inventory_Necklace: Set_Necklace(item); break;
    case Inventory_Quiver:  Set_Quiver(item); break;
    case Inventory_Ring1:   Set_Ring1(item); break;
    case Inventory_Ring2:   Set_Ring2(item); break;
    case Inventory_Trinket: Set_Trinket(item); break;
    case Inventory_Trinket2:Set_Trinket2(item); break;
    case Inventory_Trinket3:Set_Trinket3(item); break;
    case Inventory_Weapon1: Set_MainHand(item); break;
    case Inventory_Weapon2: Set_OffHand(item); break;
    default: ASSERT(FALSE); break;
    }
     // if the item just equipped is a Minor Artifact, make sure
    // we do not have any other minor artifacts equipped.
    if (item.HasMinorArtifact())
    {
        // need to check all other slots
        for (size_t s = Inventory_Unknown + 1; s < Inventory_Count; ++s)
        {
            // make sure we do not remove the just equipped item
            if (s != slot)
            {
                if (HasItemInSlot((InventorySlotType)s))
                {
                    if (ItemInSlot((InventorySlotType)s).HasMinorArtifact())
                    {
                        // we do have at least 1 other minor artifact, remove it
                        ss << "You can only have a single Minor Artifact equipped at any time.\r\n"
                            "The following item was removed: ";
                        ss << ItemInSlot((InventorySlotType)s).Name();
                        ClearItem((InventorySlotType)s);
                        itemsRemoved = true;
                    }
                }
            }
        }
    }
   if (slot == Inventory_Weapon1
            && !CanEquipTo2ndWeapon(pCharacter, item)
            && HasItemInSlot(Inventory_Weapon2))
    {
        ss << "The following item was removed because you cannot have an item in your off hand:\r\n\r\n";
        ss << ItemInSlot(Inventory_Weapon2).Name();
        // item in this slot now stops an item in weapon slot 2
        ClearItem(Inventory_Weapon2);
        itemsRemoved = true;
    }
    if (itemsRemoved)
    {
        AfxMessageBox(ss.str().c_str(), MB_ICONEXCLAMATION);
    }
}

void EquippedGear::ClearItem(InventorySlotType slot)
{
    switch (slot)
    {
    case Inventory_Arrows:  Clear_Arrow(); break;
    case Inventory_Armor:   Clear_Armor(); break;
    case Inventory_Belt:    Clear_Belt(); break;
    case Inventory_Boots:   Clear_Boots(); break;
    case Inventory_Bracers: Clear_Bracers(); break;
    case Inventory_Cloak:   Clear_Cloak(); break;
    case Inventory_Gloves:  Clear_Gloves(); break;
    case Inventory_Goggles: Clear_Goggles(); break;
    case Inventory_Helmet:  Clear_Helmet(); break;
    case Inventory_Necklace: Clear_Necklace(); break;
    case Inventory_Quiver:  Clear_Quiver(); break;
    case Inventory_Ring1:   Clear_Ring1(); break;
    case Inventory_Ring2:   Clear_Ring2(); break;
    case Inventory_Trinket: Clear_Trinket(); break;
    case Inventory_Trinket2:Clear_Trinket2(); break;
    case Inventory_Trinket3:Clear_Trinket3(); break;
    case Inventory_Weapon1: Clear_MainHand(); break;
    case Inventory_Weapon2: Clear_OffHand(); break;
    default: ASSERT(FALSE); break;
    }
}

void EquippedGear::SetNumFiligrees(size_t count)
{
    m_SentientIntelligence.SetNumFiligrees(count);
}

bool EquippedGear::HasMinorArtifact() const
{
    bool bHas = false;
    // need to check all gear slots
    for (size_t s = Inventory_Unknown + 1; s < Inventory_Count; ++s)
    {
        if (HasItemInSlot((InventorySlotType)s))
        {
            if (ItemInSlot((InventorySlotType)s).HasMinorArtifact())
            {
                // we do have at least 1 other minor artifact, remove it
                bHas = true;
            }
        }
    }
    return bHas;
}
