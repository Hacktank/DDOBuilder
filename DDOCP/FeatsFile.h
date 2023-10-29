// FeatsFile.h
//

#pragma once
#include "XmlLib\SaxContentElement.h"
#include "XmlLib\SaxWriter.h"
#include "Feat.h"

class FeatsFile :
    public XmlLib::SaxContentElement
{
    public:
        FeatsFile(const std::string & filename);
        ~FeatsFile(void);

        void Read();
        const std::vector<Feat> & Feats() const;

    protected:
        XmlLib::SaxContentElementInterface * StartElement(
                const XmlLib::SaxString & name,
                const XmlLib::SaxAttributes & attributes);

        void EndElement();

        std::string m_filename;
        std::vector<Feat> m_loadedFeats;
        size_t m_loadTotal;
};

