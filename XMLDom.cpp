/**
*    filename: XMLDom.cpp 
* 
* description: Implements the non-virtual parts of the XMLDom class.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"
#include "cXMLDom.h"

XMLDom *XMLDom::load (char *xmlDocument)
{
    // load from a single byte string...
    String docName = xmlDocument;
    // load from this...
    return XMLDom::load ((wchar_t *)docName);
}

XMLDom *XMLDom::load (wchar_t *xmlDocument)
{
    // load an XML document into the DOM...
    if (xmlDocument)
      {
        // start com...
        CoInitialize (0);
        // create the document...
        IXMLDOMDocument2 *xmlInterface = NULL;
        IXMLDOMNode *node = NULL;
        // check what kind of reference this is...
        if (xmlDocument[0] == '<')
          {
            // load as a stream...
            node = DOM::loadXMLStream (xmlDocument,&xmlInterface);
          }
        else
          {
            // load as a file...
            node = DOM::loadXMLFile (xmlDocument,&xmlInterface);
          }
        if (node && xmlInterface)
          {
            // create the dom docs container...
            cDomDocList *docList = new cDomDocList ();
            if (docList)
              {
                // add to the loaded docs...
                docList->documents.push_back (new cDomDocEntry (xmlInterface,xmlDocument));
                // give the ref-counts to the new dom and doc container...
                cXMLDom *dom = new cXMLDom (node,NULL,docList);
                if (dom)
                  {
                    // return this...
                    return (XMLDom *)dom;
                  }
              }
            // failed...
            node->Release ();
            xmlInterface->Release ();
          }
        // not loaded...
        CoUninitialize ();
      }
    return NULL;
}

/* eof */