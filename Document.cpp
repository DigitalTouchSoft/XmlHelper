/** 
*    filename: Document.cpp 
* 
* description: Implements the Xml::Document class.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"

// declares the default class container.
Xml::Document  Xml::Default::document;
Xml::Node      Xml::Default::node;

Xml::Document::Document ()
{
    CoInitialize (0);
    this->x_xmlDocument = 0;
}

Xml::Document::Document (_bstr_t &xmlDocumentFilename)
{
    CoInitialize (0);
    this->x_xmlDocument = 0;
    this->load (xmlDocumentFilename);
}

Xml::Document::~Document ()
{
    if (this->x_xmlDocument) 
    {
        // delete the document...
        delete static_cast<ShellDocument *>(this->x_xmlDocument);
    }
    CoUninitialize ();
}

Xml::Status::code Xml::Document::load (_bstr_t xmlDocumentFilename)
{
    // see if there's an existing...
    if (this->x_xmlDocument)
    {
        // delete the existing...
        delete static_cast<ShellDocument *>(this->x_xmlDocument);
        this->x_xmlDocument = 0;
    }
    if (xmlDocumentFilename.length () > 0)
    {
        // create a new dom shell...
        if (this->x_xmlDocument = new ShellDocument ())
        {
            // load the document...
            if (static_cast<ShellDocument *>(this->x_xmlDocument)->domDocument)
            {
                IXMLDOMNode *newDomNode;
                // load the file or stream...
                try
                {
                    if (((wchar_t *)xmlDocumentFilename)[0] == '<')
                    {
                        newDomNode = static_cast<ShellDocument *>(this->x_xmlDocument)->loadXML (xmlDocumentFilename);
                    }
                    else
                    {
                        newDomNode = static_cast<ShellDocument *>(this->x_xmlDocument)->loadFile (xmlDocumentFilename);
                    }
                }
                catch (...)
                {
                    // failed...
                }
                // validate the node...
                if (newDomNode)
                {
                    // copy to the root node...
                    if (DOMInterfaceCollections::copyNode (&this->rootNode,newDomNode))
                    {
                        // return success...
                        return static_cast<ShellDocument *>(this->x_xmlDocument)->status = Xml::Status::ready;
                    }
                }
                // failing load...
                return static_cast<ShellDocument *>(this->x_xmlDocument)->status = Xml::Status::invalidDocument;
            }
        }
    }
    return static_cast<ShellDocument *>(this->x_xmlDocument)->status = Xml::Status::failed;
}

Xml::Status::code Xml::Document::save (_bstr_t xmlDocumentFilename)
{
    if (xmlDocumentFilename.length () && this->x_xmlDocument) try
    {
        _variant_t saveName (xmlDocumentFilename);
        // call save...
        if (SUCCEEDED (static_cast<ShellDocument *>(this->x_xmlDocument)->domDocument->save (saveName)))
        {
            // return success...
            return static_cast<ShellDocument *>(this->x_xmlDocument)->status = Xml::Status::ok;
        }
        // save failed...
        return static_cast<ShellDocument *>(this->x_xmlDocument)->status = Xml::Status::failed;
    }
    catch (...) {}

    return Xml::Status::noDocument;
}

Xml::Status::code Xml::Document::x_statusRef ()
{
    if (this->x_xmlDocument)
    {
        // return the status...
        return static_cast<ShellDocument *>(this->x_xmlDocument)->status;
    }
    // default status...
    return Xml::Status::noDocument;
}

/* eof */