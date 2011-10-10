/** 
*    filename: ShellDocument.cpp 
* 
* description: Implements the ShellDocument class.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"

// the MS-XML guids...
extern "C" const GUID __declspec(selectany) CLSID_DOMDocument =
  {0xf6d90f11,0x9c73,0x11d3,{0xb3,0x2e,0x00,0xc0,0x4f,0x99,0x0b,0xb4}};
extern "C" const GUID __declspec(selectany) IID_IXMLDOMDocument2 =
  {0x2933bf95,0x7b36,0x11d2,{0xb2,0x0e,0x00,0xc0,0x4f,0x98,0x3e,0x60}};

ShellDocument::ShellDocument () 
{ 
    this->status = Xml::Status::noDocument;
    this->domDocument = 0; 
    try
    {
        if (!SUCCEEDED (CoCreateInstance (CLSID_DOMDocument,NULL,CLSCTX_INPROC_SERVER,IID_IXMLDOMDocument2,(void **)&this->domDocument)))
        {
            // ensure the pointer is null...
            this->domDocument = 0;
        }
    }
    catch (...)
    {
        // ensure the pointer is null...
        this->domDocument = 0;
    }
}

ShellDocument::~ShellDocument ()
{
    try
    {
        // release the document...
        if (this->domDocument) this->domDocument->Release ();
    }
    catch (...) {}
}

IXMLDOMNode *ShellDocument::loadFile (_bstr_t &fileName)
{
IXMLDOMNode *domRootNode=0;

    if (this->domDocument) try
    {
    VARIANT_BOOL isSuccessful;

        // load the doc...
        this->domDocument->put_async (VARIANT_FALSE);
        this->domDocument->put_preserveWhiteSpace (VARIANT_FALSE);
        if (SUCCEEDED (this->domDocument->load (_variant_t (fileName),&isSuccessful)) && (isSuccessful == VARIANT_TRUE))
        {
            // get the root element...
            IXMLDOMElement *rootElement=NULL;
            if (SUCCEEDED (this->domDocument->get_documentElement (&rootElement)) && rootElement)
            {
                // get the root node...
                rootElement->QueryInterface (IID_IXMLDOMNode,(void **)&domRootNode);
                rootElement->Release ();
            }
        }
    }
    catch (...) {}

    return domRootNode;
}

IXMLDOMNode *ShellDocument::loadXML (_bstr_t &xmlString)
{
IXMLDOMNode *domRootNode=0;

    if (this->domDocument) try
    {
    VARIANT_BOOL isSuccessful;

        this->domDocument->put_async (VARIANT_FALSE);
        this->domDocument->put_preserveWhiteSpace (VARIANT_FALSE);
        if (SUCCEEDED (this->domDocument->loadXML (xmlString,&isSuccessful)) && (isSuccessful == VARIANT_TRUE))
        {
            // get the root element...
            IXMLDOMElement *rootElement=NULL;
            if (SUCCEEDED (this->domDocument->get_documentElement (&rootElement)) && rootElement)
            {
                // get the root node...
                rootElement->QueryInterface (IID_IXMLDOMNode,(void **)&domRootNode);
                rootElement->Release ();
            }
        }
    }
    catch (...) {}

    return domRootNode;
}

/* eof */