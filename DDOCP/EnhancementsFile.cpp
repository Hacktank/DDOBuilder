// EnhancementsFile.cpp
//
#include "StdAfx.h"
#include "EnhancementsFile.h"
#include "XmlLib\SaxReader.h"

namespace
{
    const XmlLib::SaxString f_saxElementName = L"Enhancements"; // root element name to look for
}

EnhancementsFile::EnhancementsFile(const std::string & filename) :
    SaxContentElement(f_saxElementName),
    m_filename(filename),
    m_loadTotal(0)
{
}


EnhancementsFile::~EnhancementsFile(void)
{
}

void EnhancementsFile::Read()
{
    // set up a reader with this as the expected root node
    XmlLib::SaxReader reader(this, f_saxElementName);
    // read in the xml from a file (fully qualified path)
    bool ok = reader.Open(m_filename);
    if (!ok)
    {
        std::string errorMessage = reader.ErrorMessage();
        // document has failed to load. Tell the user what we can about it
        CString text;
        text.Format("The document %s\n"
                "failed to load. The XML parser reported the following problem:\n"
                "\n", m_filename);
        text += errorMessage.c_str();
        AfxMessageBox(text, MB_ICONERROR);
    }
}

void EnhancementsFile::Save()
{
    bool ok = false;
    try
    {
        XmlLib::SaxWriter writer;
        // write the xml
        bool ok = writer.Open(m_filename);

        // write the entire document
        if (ok)
        {
            writer.StartDocument(f_saxElementName);
            std::vector<EnhancementTree>::iterator it = m_loadedTrees.begin();
            while (it != m_loadedTrees.end())
            {
                (*it).Write(&writer);
                ++it;
            }
            writer.EndDocument();
        }
        else
        {
            // save the message so our client can get it
            std::string errorMessage = writer.ErrorMessage();
            AfxMessageBox(errorMessage.c_str(), MB_ICONERROR);
        }
    }
    catch (const std::exception & e)
    {
        ok = false;
        std::string errorMessage = e.what();
        AfxMessageBox(errorMessage.c_str(), MB_ICONERROR);
    }
}

XmlLib::SaxContentElementInterface * EnhancementsFile::StartElement(
        const XmlLib::SaxString & name,
        const XmlLib::SaxAttributes & attributes)
{
    XmlLib::SaxContentElementInterface * subHandler =
            SaxContentElement::StartElement(name, attributes);
    if (subHandler == NULL)
    {
        EnhancementTree tree;
        if (tree.SaxElementIsSelf(name, attributes))
        {
            m_loadedTrees.push_back(tree);
            subHandler = &(m_loadedTrees.back());
        }
    }

    return subHandler;
}

void EnhancementsFile::EndElement()
{
    SaxContentElement::EndElement();
}

const std::vector<EnhancementTree> & EnhancementsFile::EnhancementTrees() const
{
    return m_loadedTrees;
}
