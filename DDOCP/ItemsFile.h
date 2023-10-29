// ItemsFile.h
//

#pragma once
#include "XmlLib\SaxContentElement.h"
#include "XmlLib\SaxWriter.h"
#include "Item.h"

class ItemsFile :
    public XmlLib::SaxContentElement
{
    public:
        ItemsFile(const std::string & path);
        ~ItemsFile(void);

        void ReadFiles();
        const std::vector<Item> & Items() const;

    protected:
        XmlLib::SaxContentElementInterface * StartElement(
                const XmlLib::SaxString & name,
                const XmlLib::SaxAttributes & attributes);

        void EndElement();

    private:
        bool ReadFile(const std::string & filename);
        std::string m_path;
        std::vector<Item> m_loadedItems;
        size_t m_loadTotal;
};

