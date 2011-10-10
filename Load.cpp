/**
*    filename: Load.cpp 
* 
* description: Implements the load functionality of the cDom class.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"
#include "cXMLDom.h"

// the MS-XML guids...

extern "C" const GUID __declspec(selectany) CLSID_DOMDocument =
    {0xf6d90f11,0x9c73,0x11d3,{0xb3,0x2e,0x00,0xc0,0x4f,0x99,0x0b,0xb4}};
extern "C" const GUID __declspec(selectany) IID_IXMLDOMDocument2 =
    {0x2933bf95,0x7b36,0x11d2,{0xb2,0x0e,0x00,0xc0,0x4f,0x98,0x3e,0x60}};

// the global load methods...

IXMLDOMNode *DOM::loadXMLFile (wchar_t *fileName,IXMLDOMDocument2 **domDocument)
{
IXMLDOMNode *rootNode=NULL;

    // load an XML document into the DOM...
    *domDocument=NULL;
    // create the xml dom interface...
    if (SUCCEEDED (CoCreateInstance (CLSID_DOMDocument,NULL,CLSCTX_INPROC_SERVER,IID_IXMLDOMDocument2,(void **)domDocument)) && *domDocument)
      {
        VARIANT_BOOL isSuccessful;
        VARIANT xmlFile;
        xmlFile.bstrVal = SysAllocString (fileName);
        xmlFile.vt = VT_BSTR;
        if (xmlFile.bstrVal)
          {
            // load the doc...
            (*domDocument)->put_async (VARIANT_FALSE);
            if (SUCCEEDED ((*domDocument)->load (xmlFile,&isSuccessful)) && isSuccessful)
              {
                // get the root element...
                IXMLDOMElement *rootElement=NULL;
                if (SUCCEEDED ((*domDocument)->get_documentElement (&rootElement)) && rootElement)
                  {
                    // get the root node...
                    rootElement->QueryInterface (IID_IXMLDOMNode,(void **)&rootNode);
                    rootElement->Release ();
                  }
              }
            SysFreeString (xmlFile.bstrVal);
          }
      }
    // returns the root node of the xml file...
    return rootNode;
}
                        
IXMLDOMNode *DOM::loadXMLStream (wchar_t *xmlString,IXMLDOMDocument2 **domDocument)
{
IXMLDOMNode *rootNode=NULL;

    *domDocument=NULL;
    // create an xml dom interface...
    if (SUCCEEDED (CoCreateInstance (CLSID_DOMDocument,NULL,CLSCTX_INPROC_SERVER,IID_IXMLDOMDocument2,(void **)domDocument)) && *domDocument)
      {
        // load the doc...
        VARIANT_BOOL isSuccessful;
        (*domDocument)->put_async (VARIANT_FALSE);
        if (SUCCEEDED ((*domDocument)->loadXML (xmlString,&isSuccessful)) && isSuccessful)
          {
            // get the root element...
            IXMLDOMElement *rootElement=NULL;
            if (SUCCEEDED ((*domDocument)->get_documentElement (&rootElement)) && rootElement)
              {
                // get the root node...
                rootElement->QueryInterface (IID_IXMLDOMNode,(void **)&rootNode);
                rootElement->Release ();
              }
          }
      }
    return rootNode;
}

/* eof */