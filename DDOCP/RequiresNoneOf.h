// RequiresNoneOf.h
//
#pragma once
#include "XmlLib\DLMacros.h"

class Character;
class Feat;
class TrainedFeat;
class Requirement;

class RequiresNoneOf :
    public XmlLib::SaxContentElement
{
    public:
        RequiresNoneOf(void);
        void Write(XmlLib::SaxWriter * writer) const;

        bool Met(
                const Character & charData, 
                const std::vector<size_t> & classLevels,
                size_t totalLevel,
                const std::vector<TrainedFeat> & currentFeats,
                bool includeTomes) const;
        bool CanTrainEnhancement(
            const Character & charData,
            size_t trainedRanks) const;
        bool CanTrainTree(
                const Character & charData) const;
        void CreateRequirementStrings(
                const Character & charData,
                std::vector<CString> * requirements,
                std::vector<bool> * met,
                size_t level) const;

        bool VerifyObject(
                std::stringstream * ss,
                const std::vector<Feat> & allFeats) const;

    protected:
        XmlLib::SaxContentElementInterface * StartElement(
                const XmlLib::SaxString & name,
                const XmlLib::SaxAttributes & attributes);

        virtual void EndElement();

        #define RequiresNoneOf_PROPERTIES(_) \
                DL_OPTIONAL_STRING(_, DisplayDescription) \
                DL_OBJECT_VECTOR(_, Requirement, Requirements)

        DL_DECLARE_ACCESS(RequiresNoneOf_PROPERTIES)
        DL_DECLARE_VARIABLES(RequiresNoneOf_PROPERTIES)
};
