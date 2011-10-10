/** 
*    filename: ShellNodeArray.cpp 
* 
* description: Implements the ShellNodeArray class.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"

//        if (SUCCEEDED (this->xmlNode->get_nodeType (&nodeType)) && (nodeType == NODE_ELEMENT))

ShellNodeArray::ShellNodeArray ()
{
    this->domNode = 0;
    this->domNodeList = 0;
}

ShellNodeArray::~ShellNodeArray ()
{
    this->clear ();
}

void ShellNodeArray::clear ()
{
    // release existing interfaces...
    if (this->domNode)
    {
        this->domNode->Release ();
        this->domNode = 0;
    }
    if (this->domNodeList) 
    {
        this->domNodeList->Release ();
        this->domNodeList = 0;
    }
    // clear the node map...
    for (XMLNodeMap::iterator mapList = this->nodeMap.begin (); mapList != this->nodeMap.end (); ++mapList)
    {
        if (mapList->second) delete mapList->second;
    }
    this->nodeMap.clear ();
}

void ShellNodeArray::copy (IXMLDOMNode *sourceDomNode)
{
    // release existing...
    this->clear ();
    // copy the node..
    if (this->domNode = sourceDomNode) try
    {
        // increment the usage count...
        this->domNode->AddRef ();
        // get the node list...
        if (!SUCCEEDED (sourceDomNode->get_childNodes (&this->domNodeList)))
        {
            // ensure the pointer is null...
            this->domNodeList = 0;
        }
    }
    catch (...) {}
}

Xml::Node &ShellNodeArray::add (const char *nodeName,long insertPosition)
{
    // add a node of name given...
    if (this->domNode && nodeName && nodeName[0]) try
    {
        IXMLDOMDocument *ownerDocument=0;
        if (SUCCEEDED (this->domNode->get_ownerDocument (&ownerDocument)))
        {
            IXMLDOMNode *newNode=0;
            // create an element node...
            if (SUCCEEDED (ownerDocument->createNode (_variant_t (1),_bstr_t (nodeName),_bstr_t ("\0"),&newNode)))
            {
                // join it at the specified location...
                if ((insertPosition < 0) || (insertPosition >= this->count))
                {
                    // append at the end...
                    if (SUCCEEDED (this->domNode->appendChild (newNode,0)))
                    {
                        // succeeded...
                        ownerDocument->Release ();
                        return this->operator [] (this->x_countRef () - 1);
                    }
                }
                else
                {
                    // get the element at offset...
                    IXMLDOMNode *beforeNode=0;
                    if (this->domNodeList)
                    {
                        // get the node...
                        if (!SUCCEEDED (this->domNodeList->get_item (insertPosition,&beforeNode)))
                        {
                            // make sure null if fails...
                            beforeNode = 0;
                            insertPosition = 0;
                        }
                    }
                    // insert at postion specified...
                    if (SUCCEEDED (this->domNode->insertBefore (newNode,_variant_t (beforeNode),0)))
                    {
                        // succeeded...
                        ownerDocument->Release ();
                        return this->operator [] (insertPosition);
                    }
                }
            }
            ownerDocument->Release ();
        }
    }
    catch (...) {}

    return Xml::Default::node;
}

bool ShellNodeArray::remove (const char *searchString)
{
bool removedOk=false;

    if (searchString && searchString[0]) try
    {
        // remove the specified node...
        if (this->domNode)
        {
            // get member matching search string...
            IXMLDOMNode *subDomNode=0;
            
            // query the dom...
            if (SUCCEEDED (this->domNode->selectSingleNode (_bstr_t (searchString),&subDomNode)) && subDomNode)
            {
                IXMLDOMNode *removedDomNode=0;
                // remove the node...
                if (SUCCEEDED (this->domNode->removeChild (subDomNode,&removedDomNode)))
                {
                    // the node is removed...
                    if (removedDomNode) removedDomNode->Release ();
                    // see if it's in the node map...
                    XMLNodeMap::iterator mapEntry;
                    if ((mapEntry = this->nodeMap.find (subDomNode)) != this->nodeMap.end ())
                    {
                        // remove it from the node map...
                        this->nodeMap.erase (mapEntry);
                    }
                    // flag as succeeded...
                    removedOk = true;
                }
                subDomNode->Release ();
            }
        }
    }
    catch (...) {}

    return removedOk;
}

Xml::Node &ShellNodeArray::operator[] (long nodeIndex)
{
    // access node by index...
    if (nodeIndex < this->x_countRef ()) try
    {
        // see if you can get the member from the dom...
        if (this->domNodeList)
        {
            IXMLDOMNode *subDomNode=0;
            // query the dom...
            if (SUCCEEDED (this->domNodeList->get_item (nodeIndex,&subDomNode)))
            {
                XMLNodeMap::iterator mapEntry;
                // see if it's in the map...
                if ((mapEntry = this->nodeMap.find (subDomNode)) != this->nodeMap.end ())
                {
                    // return this node...
                    return *mapEntry->second;
                }
                // put it in the map...
                Xml::Node *newNode = new Xml::Node ();
                if (newNode)
                {
                    // assign the node...
                    DOMInterfaceCollections::copyNode (newNode,subDomNode);
                    // put it in the map...
                    this->nodeMap[subDomNode] = newNode;
                    // release your instance...
                    subDomNode->Release ();
                    // return it...
                    return *newNode;
                }
            }
        }
    }
    catch (...) {}

    return Xml::Default::node;
}

Xml::Node &ShellNodeArray::operator [] (const char *searchString)
{
    if (searchString && searchString[0]) try
    {
        if (this->domNodeList)
        {
            // get member matching search string...
            IXMLDOMNode *subDomNode=0;
            
            // query the dom...
            if (SUCCEEDED (this->domNode->selectSingleNode (_bstr_t (searchString),&subDomNode)) && subDomNode)
            {
                XMLNodeMap::iterator mapEntry;
                // see if it's in the map...
                if ((mapEntry = this->nodeMap.find (subDomNode)) != this->nodeMap.end ())
                {
                    // return this node...
                    return *mapEntry->second;
                }
                // put it in the map...
                Xml::Node *newNode = new Xml::Node ();
                if (newNode)
                {
                    // assign the node...
                    DOMInterfaceCollections::copyNode (newNode,subDomNode);
                    // put it in the map...
                    this->nodeMap[subDomNode] = newNode;
                    // release your instance...
                    subDomNode->Release ();
                    // return it...
                    return *newNode;
                }
            }
        }
    }
    catch (...) {}

    return Xml::Default::node;
}

long ShellNodeArray::x_countRef ()
{
    // return the count of nodes...
    if (this->domNodeList) try
    {
        long length = 0;
        // retrieve the length...
        if (this->domNodeList->get_length (&length) == S_OK)
        {
            return length;
        }
    }
    catch (...) {}

    return 0;
}

/* eof */