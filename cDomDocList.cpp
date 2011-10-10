/** 
*    filename: cDomDocList.cpp 
* 
* description: Implements the cDomDocList class.  Also implements the
*              cDomDocEntry class contained by cDomDocList.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"
#include "cXMLDom.h"

cDomDocList::cDomDocList ()
{
    
}

cDomDocList::~cDomDocList ()
{
    long count = (long)this->documents.size ();
    if (count)
    {
        // empty the documents list...
        vector<cDomDocEntry *>::iterator document = this->documents.begin ();
        // save these documents...
        for (long index=0; index < count; ++index)
        {
            // release the doc info...
            delete document[index];
        }
    }
}

wchar_t *cDomDocList::rootedName (wchar_t *name)
{
    if (!_path::is::qualified (name))
    {
        // need to build a qualified path name...
        if (this->documents.size ())
        {
            // use entry zero as relative path...
            cDomDocEntry *doc = this->documents.at (0);
            if (doc)
            {
                String path = _path::get::path (doc->fileName);
                if (path)
                {
                    _path::make::pathName (path,name).detach ();
                }
            }
        }
    }
    // returning the qualified path...
    return String (name).detach ();
}

IXMLDOMAttribute *cDomDocList::createAttribute (BSTR attributeName,_variant_t value)
{
IXMLDOMAttribute *attribute = NULL;

    // need a document...
    if (this->documents.size ())
    {
        // grab the first document...
        cDomDocEntry *doc = this->documents.at (0);
        if (doc)
        {
            // use it to create the attribute...
            if (SUCCEEDED (doc->xmlInterface->createAttribute (attributeName,&attribute)))
            {
                // put the value in it...
                if (SUCCEEDED (attribute->put_value (value)))
                {
                    // return this...
                    return attribute;
                }
                // failed...
                attribute->Release ();
            }
        }
    }
    return NULL;
}

/**
*
* the XML document container...
*
*/

cDomDocEntry::cDomDocEntry (IXMLDOMDocument2 *xmlInterface,wchar_t *fileName)
{
    // we take ownership of the add-ref.
    this->xmlInterface = xmlInterface;
    this->fileName = fileName;
}

cDomDocEntry::~cDomDocEntry ()
{
    // release resources...
    if (this->xmlInterface) this->xmlInterface->Release ();
}

/* eof */