// IgnoredFeatsFile.h
//

#pragma once
#include "XmlLib\SaxContentElement.h"
#include "XmlLib\SaxWriter.h"

class IgnoredFeatsFile :
    public XmlLib::SaxContentElement
{
    public:
        IgnoredFeatsFile(const std::string & filename);
        ~IgnoredFeatsFile(void);

        void Read();
        const std::vector<std::string> & IgnoredFeats();
        void Save(std::vector<std::string> & featList);

    protected:
        XmlLib::SaxContentElementInterface * StartElement(
                const XmlLib::SaxString & name,
                const XmlLib::SaxAttributes & attributes);

        void EndElement();

        std::string m_filename;
        std::vector<std::string> m_loadedFeats;
        size_t m_loadTotal;
};

