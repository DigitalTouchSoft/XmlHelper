/** 
*    filename: cXMLDom.cpp 
* 
* description: Implements the construction, destruction, and private methods
*              of the cXMLDom class.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"
#include "cXMLDom.h"

/**
* construction destruction...
*/

void cXMLDom::_init (void)
{
    // initialize a null dom...
    this->_parent = NULL;
    this->_attributeValue = NULL;
    this->_nodeCount = 0;
    this->_importCount = 0;
    // xml interfaces...
    this->docList = NULL;
    this->xmlNode = NULL;
    this->xmlNodeList = NULL;
    this->xmlNodeAttributes = NULL;
    // the inherited node...
    this->inheritedNode = NULL;
}

cXMLDom::cXMLDom ()
{
    // initialize properties...
    this->_init ();
}

cXMLDom::cXMLDom (IXMLDOMNode *node,cXMLDom *parent,cDomDocList *docList) 
{
    // initialize properties...
    this->_init ();
    // get the doc list...
    this->docList = docList;
    // assign your parent...
    if (this->_parent = parent)
    {
        // get a parental doc list reference...
        this->docList = this->_parent->docList;
    }
    // assign your xml node...
    if (this->xmlNode = node)
    {
        // get the node list...
        if (SUCCEEDED (this->xmlNode->get_childNodes (&this->xmlNodeList)))
        {
            // see if this imports...
            if ((this->xmlNodeList->get_length (&this->_nodeCount) == S_OK) && this->_nodeCount)
            {
                // find all imports...
                this->findImports ();
            }
            // see if this inherits...
            this->findInherits ();
        }
    }
}

cXMLDom::~cXMLDom ()
{
    // release all...xml interfaces...
    if (this->xmlNodeAttributes) this->xmlNodeAttributes->Release ();
    if (this->xmlNodeList) this->xmlNodeList->Release ();
    if (this->xmlNode) this->xmlNode->Release ();
    // inherited node...
    if (this->inheritedNode) this->inheritedNode->release ();
    // attributes...
    if (this->_attributeValue) SysFreeString (this->_attributeValue);
    // empty the name stack...
    while (this->_attributeName.size ())
    {
        delete this->_attributeName.back ();
        this->_attributeName.pop_back ();
    }
    // see if you are a root...
    if (!this->_parent)
    {
        // delete the doclist...
        if (this->docList) delete this->docList;
        // release com...
        CoUninitialize ();
    }
}

/**
* public methods...
*/

bool cXMLDom::save (wchar_t *fileName)
{
    // walk the list of loaded msxmldom documents and save them to file...
    long count = (long)this->docList->documents.size ();
    if (count)
    {
        vector<cDomDocEntry *>::iterator document = this->docList->documents.begin ();
        // save these documents...
        for (long index=0; index < count; ++index)
        {
            document[index]->xmlInterface->save (_variant_t ((wchar_t *)document[index]->fileName));
        }
        return true;
    }
    return false;
}

void cXMLDom::release (void)
{
    delete this;
}

String &cXMLDom::name (void)
{
    if (this->xmlNode)
    {
        if (!this->_name)
        {
            BSTR bstrNodeName=NULL;
            // return an allocated copy of the current node name...
            if (SUCCEEDED (this->xmlNode->get_nodeName (&bstrNodeName)))
            {
                if (bstrNodeName)
                {
                    // collect the name...
                    this->_name = (wchar_t *)bstrNodeName;
                    // and free the bstr...
                    SysFreeString (bstrNodeName);
                }
                
            }
        }
    }
    return this->_name;
}

String &cXMLDom::textSection (void)
{
    // get the text section of this node...
    if (this->xmlNode)
    {
        if (!this->_text)
        {
            // not read yet...
            IXMLDOMNode *text=NULL;
            if (SUCCEEDED (this->xmlNode->selectSingleNode (L"text()",&text)) && text)
            {
                // read its text value...
                VARIANT value;
                value.vt = 0;
                value.bstrVal = NULL;
                if (SUCCEEDED (text->get_nodeValue (&value)) && (value.vt == VT_BSTR))
                {
                    // keep the reference...
                    this->_text = value.bstrVal;
                    // dump the bstr...
                    SysFreeString (value.bstrVal);
                }
                text->Release ();
            }
        }
    }
    return this->_text;
}

long cXMLDom::nodeCount (void)
{
long totalNodes = this->_nodeCount;

    // return the count of nodes...
    if (this->_importCount)
    {
        // sum any import nodes...
        vector<cXMLDom *>::iterator import = this->imports.begin ();
        for (long index=0; index < this->_importCount; ++index)
        {
            totalNodes += import[index]->nodeCount ();
        }
    }
    if (this->inheritedNode)
    {
        // sum inherited nodes...
        totalNodes += this->inheritedNode->nodeCount ();
    }
    return totalNodes;
}

XMLDom *cXMLDom::spawnChild (long index)
{
    // spawn a child node by index...
    if (this->_nodeCount > index)
    {
        // it should be in this nodes list...
        return spawnNode (index,this);
    }
    // adjust for out of range...
    index -= this->_nodeCount;
    // check inheritence...
    if (this->inheritedNode)
    {
        if (this->inheritedNode->nodeCount () > index)
        {
            // it should be in this nodes list...
            return this->inheritedNode->spawnNode (index,this);
        }
        // adjust for out of range...
        index -= this->inheritedNode->nodeCount ();
    }
    // see if you have imported nodes...
    if (this->_importCount)
    {
        // check the imported nodes...
        vector<cXMLDom *>::iterator import = this->imports.begin ();
        for (long pos=0; pos < this->_importCount; ++pos)
        {
            if (import[pos]->nodeCount () > index)
            {
                // index ranges in this import...
                return import[pos]->spawnNode (index,this);
            }
            // adjust for out of range...
            index -= import[pos]->nodeCount ();
        }
    }
    return NULL;
}

XMLDom *cXMLDom::spawnChild (wchar_t *nodeSearch)
{
XMLDom *dom=NULL;

    // spawn a child node that matches the search criteria...
    if (dom = this->spawnNode (nodeSearch,this))
    {
        // found in the main node...
        return dom;
    }
    // see if you have an inherited node...
    if (this->inheritedNode)
    {
        if (dom = this->inheritedNode->spawnNode (nodeSearch,this))
        {
            // found in inherited node...
            return dom;
        }
    }
    // see if you have imported nodes...
    if (this->_importCount)
    {
        // check the imported nodes...
        vector<cXMLDom *>::iterator import = this->imports.begin ();
        for (long pos=0; pos < this->_importCount; ++pos)
        {
            if (dom = import[pos]->spawnNode (nodeSearch,this))
            {
                // found in an imported node...
                return dom;
            }
        }
    }
    return NULL;
}

XMLDom *cXMLDom::spawnAncestor (wchar_t *nodeSearch)
{
    cXMLDom *parent = this->_parent;
    if (parent) do
    {
        // try to find the node in the parent...
        XMLDom *dom = parent->spawnChild (nodeSearch);
        if (dom) 
        {
            // return this...
            return dom;
        }
    }
    while (parent = parent->_parent);
    // never found...
    return NULL;
}

bool cXMLDom::setFocus (const wchar_t *childNode)
{
    // set attribute reading focus...get or set uses...
    if (this->xmlNodeAttributes)
    {
        // release the current attributes...
        this->xmlNodeAttributes->Release ();
        this->xmlNodeAttributes = NULL;
    }
    // focus on your self and your inherited node...
    bool found = this->focusAttributeCalls (childNode);
    // see if you have an inherited node...
    if (this->inheritedNode)
    {
        // focus to inherited node...
        found = this->inheritedNode->focusAttributeCalls (childNode) ? true : found;
    }
    return found;
}

XMLAttribute &cXMLDom::attribute (wchar_t *attributeName)
{
    // clear existing attributes and set new focus...
    // see if there is new assignment...
    if (attributeName)
    {
        // assign the next name to use...
        this->_attributeName.push_back (String (attributeName).detach ());
    }
    else
    {
        // assign a fault name...
        this->_attributeName.push_back (String ("unknown").detach ());
    }
    // return the operator interface...
    return *this;
}

/* eof */