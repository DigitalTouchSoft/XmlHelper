/** 
*    filename: cXMLDomPrivate.cpp 
* 
* description: Implements the private methods of the cXMLDom class.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"
#include "cXMLDom.h"

void cXMLDom::insertImport (wchar_t *fileName)
{
    // load and merge this xml file...
    wchar_t *fullName = this->docList->rootedName (fileName);
    if (fullName)
      {
        IXMLDOMDocument2 *mergeDocument = NULL;
        // load this...
        IXMLDOMNode *importNode = DOM::loadXMLFile (fullName,&mergeDocument);
        if (importNode)
          {
            // give the ref-counts to the new dom and doc container...
            cXMLDom *dom = new cXMLDom (importNode,this);
            if (dom)
              {
                // add to the list of loaded docs...
                this->docList->documents.push_back (new cDomDocEntry (mergeDocument,fullName));
                // add to the imported nodes list...
                this->imports.push_back (dom);
                ++this->_importCount;
                // succeeded...
                return;
              }
            importNode->Release ();
          }
        delete fullName;
      }
}

void cXMLDom::findImports (void)
{
    if (this->xmlNodeList)
      {
        IXMLDOMNode *childNode=NULL;
        this->xmlNodeList->reset ();
        // walk your child nodes...
        while (SUCCEEDED (this->xmlNodeList->nextNode (&childNode)) && childNode)
          {
            // find all the include tags...
            BSTR name;
            if (SUCCEEDED (childNode->get_nodeName (&name)))
              {
                if (String (name) == "import")
                  {
                    // add this to the list...
                    IXMLDOMNamedNodeMap *nodeMap = NULL;
                    // get the attributes...
                    if (SUCCEEDED (childNode->get_attributes (&nodeMap)) && nodeMap)
                      {
                        IXMLDOMNode *namedNode = NULL;
                        if (SUCCEEDED (nodeMap->getNamedItem (L"name",&namedNode)) && namedNode)
                          {
                            // get the name attribute...
                            VARIANT value;
                            value.vt = VT_BSTR;
                            value.bstrVal = NULL;
                            if (SUCCEEDED (namedNode->get_nodeValue (&value)))
                              {
                                // merge this include...
                                this->insertImport (value.bstrVal);
                              }
                            namedNode->Release ();
                          }
                        nodeMap->Release ();
                      }
                  }
                SysFreeString (name);
              }
            // not an include node...
            childNode->Release ();
          }
      }
}

void cXMLDom::findInherits (void)
{
    String inheritedName = this->attribute (L"inherit");
    if (inheritedName.size ())
      {
        wchar_t *searchName = new wchar_t [this->name ().size () + inheritedName.size () + 32];
        if (searchName)
          {
            // find this inherited object...
            wsprintf (searchName,L"%s[@name = \"%s\"]",this->name (),(wchar_t *)inheritedName);
            // find this global element...
            cXMLDom *lookDom = this->_parent;
            if (lookDom) do
              {
                // search in this node...
                if (this->inheritedNode = lookDom->spawnNode (searchName,this)) break;
                // search the innerited node...
                if (lookDom->inheritedNode && (this->inheritedNode = lookDom->inheritedNode->spawnNode (searchName,this))) break;
                // search the imported nodes...
                if (lookDom->_importCount)
                  {
                    vector<cXMLDom *>::iterator import = lookDom->imports.begin ();
                    for (long pos=0; pos < lookDom->_importCount; ++pos)
                      {
                        // check each import...
                        if (this->inheritedNode = import[pos]->spawnNode (searchName,this)) break;
                      }
                  }                    
              }
            while (lookDom = lookDom->_parent);
            // delete the search string...
            delete searchName;
          }
      }
}

bool cXMLDom::focusAttributeCalls (const wchar_t *childNode)
{
    // focus to a child or to the root...
    if (childNode)
      {
        bool found = false;
        IXMLDOMNode *childDom=NULL;
        // to a child...
        if (SUCCEEDED (this->xmlNode->selectSingleNode ((BSTR)childNode,&childDom)) && childDom)
          {
            // get the child's attributes...
            found = SUCCEEDED (childDom->get_attributes (&this->xmlNodeAttributes));
            childDom->Release ();
          }
        return found;
      }
    // to the root...get your own attributes...
    return SUCCEEDED (this->xmlNode->get_attributes (&this->xmlNodeAttributes));
}

BSTR cXMLDom::readAttribute (wchar_t *attributeName)
{
    if (this->xmlNodeAttributes)
      {
        IXMLDOMNode *xmlNode=NULL;
        // get the node...
        if (this->xmlNodeAttributes->getNamedItem (attributeName,&xmlNode) == S_OK)
          {
            VARIANT value;
            value.vt = VT_BSTR;
            value.bstrVal = NULL;
            // get the value...
            xmlNode->get_nodeValue (&value);
            xmlNode->Release ();
            if (value.bstrVal)
              {
                // only return if it has value...
                if (value.bstrVal[0])
                  {
                    // return the attribute...
                    return value.bstrVal;
                  }
                else
                  {
                    // dump it...
                    SysFreeString (value.bstrVal);
                  }
              }
          }
      }
    // not found...
    return 0;
}

bool cXMLDom::getAttribute (void)
{
bool getResult=false;

    // retrieve the attribute...it can be in this node or in the
    // inherited node of this node...name specification required...
    if (this->_attributeName.size ())
      {
        // get the attribute name to use...
        wchar_t *theName = this->_attributeName.front ();
        this->_attributeName.pop_front ();

        if (theName)
          {
            DOMNodeType nodeType;
            // only allowed on element type nodes...
            if (SUCCEEDED (this->xmlNode->get_nodeType (&nodeType)) && (nodeType == NODE_ELEMENT))
              {
                // verify focus...
                if (this->xmlNodeAttributes || this->setFocus ())
                  {
                    // read the attribute...
                    getResult = (this->_attributeValue = this->readAttribute (theName)) != 0;
                  }
                if (!getResult)
                  {
                    // check for an inherited node...
                    if (this->inheritedNode && (this->inheritedNode->xmlNodeAttributes || this->inheritedNode->setFocus ()))
                      {
                        // try the inherited node...
                        getResult = (this->_attributeValue = this->inheritedNode->readAttribute (theName)) != 0;
                      }
                  }
              }
            delete theName;
          }
      }
    return getResult;
}

bool cXMLDom::setAttribute (_variant_t value)
{
bool assigned=false;

    // name specification required...
    if (this->_attributeName.size ())
      {
        // get the attribute name to use...
        wchar_t *theName = this->_attributeName.front ();
        this->_attributeName.pop_front ();

        if (theName)
          {
            // ensure focus is set...
            if (!this->xmlNodeAttributes) this->setFocus ();
            // only assign to this node, never to the inherited node...
            if (this->xmlNodeAttributes)
              {
                // create the new attribute...assign the value...
                IXMLDOMAttribute *attribute = this->docList->createAttribute (theName,value);
                if (attribute)
                  {
                    IXMLDOMNode *named = NULL;
                    // assign as a node...
                    if (SUCCEEDED (this->xmlNodeAttributes->setNamedItem (attribute,&named)))
                      {
                        // done...
                        assigned = true;
                        named->Release ();
                      }
                    // done...
                    attribute->Release ();
                  }
              }
            delete theName;
          }
      }
    return assigned;
}

cXMLDom *cXMLDom::spawnNode (long index,cXMLDom *parent)
{
    if (this->xmlNodeList)
      {
        // create a new DOM representing the specified node...
        IXMLDOMNode *child=NULL;
        if (SUCCEEDED (this->xmlNodeList->get_item (index,&child)) && child)
          { 
            // give the ref-count to the new dom...
            cXMLDom *dom = new cXMLDom (child,parent);
            if (dom)
              {
                // return...
                return dom;
              }
            // failed...
            child->Release ();
          }
      }
    return NULL;
}

cXMLDom *cXMLDom::spawnNode (wchar_t *nodeSearch,cXMLDom *parent)
{
    if (this->xmlNode)
      {
        _bstr_t bstrSearch = nodeSearch;
        // find the node in the root...
        IXMLDOMNode *child=NULL;
        if (SUCCEEDED (this->xmlNode->selectSingleNode (bstrSearch,&child)) && child)
          {
            // give the ref-count to the new dom...
            cXMLDom *dom = new cXMLDom (child,parent);
            if (dom)
              {
                // return...
                return dom;
              }
            // failed...
            child->Release ();
          }
      }
    return NULL;
}

/* eof */