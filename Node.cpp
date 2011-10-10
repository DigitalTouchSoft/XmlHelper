/** 
*    filename: Node.cpp 
* 
* description: Implements the Xml::Node and Xml::NodeArray classes.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"

/**
* Xml::Node class
*/

Xml::Node::Node ()
:
child (*new ShellNodeArray ()),
attribute (*new ShellAttributeArray ())
{
    this->x_xmlNode = new ShellNode ();
}

Xml::Node::Node (Xml::Node &xmlNode)
:
child (*new ShellNodeArray ()),
attribute (*new ShellAttributeArray ())
{
    this->x_xmlNode = new ShellNode ();
    // copy the node...
    this->operator = (xmlNode);
}

Xml::Node::~Node ()
{
    // delete the node class...
    if (this->x_xmlNode) delete static_cast<ShellNode *>(this->x_xmlNode);
    // delete the node array...
    if (static_cast<ShellNodeArray *>(&this->child)) delete static_cast<ShellNodeArray *>(&this->child);
    if (static_cast<ShellAttributeArray *>(&this->attribute)) delete static_cast<ShellAttributeArray *>(&this->attribute);
}

Xml::Node &Xml::Node::operator= (Xml::Node &xmlNode)
{
IXMLDOMNode *copyNode=0;

    // copy the node given...
    if (xmlNode.x_xmlNode)
    {
        copyNode = static_cast<ShellNode *> (xmlNode.x_xmlNode)->domNode;
    }
    // assign the node...
    DOMInterfaceCollections::copyNode (this,copyNode);
    // return yourself...
    return *this;
}

Xml::Node &Xml::Node::find (const char *nodeName,const char *attributeName,const char *attributeValue)
{
    // search for a sub-node with the specified criteria...
    if (this->x_xmlNode)
    {
        // make sure min criteria is met...
        if (nodeName || (attributeName && attributeValue))
        {
            // walk the child nodes looking...
            for (long count=0; count < this->child.count; ++count)
            {
                // if name is given, use name...
                if (nodeName)
                {
                    if (_bstr_t (nodeName) != this->child[count].name) continue;
                }
                // if attribute is given, use attribute...
                if (attributeName && attributeValue)
                {
                    // the attribute must exist...
                    if (!this->child[count].attribute[attributeName].exists) continue;
                    // and it must match...
                    if (_bstr_t (attributeValue).operator != (this->child[count].attribute[attributeName])) continue;
                }
                // if both passed this node is a match...
                return this->child[count];
            }
            // not found so walk into child nodes...
            for (long count=0; count < this->child.count; ++count)
            {
                Xml::Node &findNode = this->child[count].find (nodeName,attributeName,attributeValue);
                // see if found...
                if (findNode.status == Xml::Status::ok)
                {
                    // return the node...
                    return findNode;
                }
            }
        }
    }
    // could not find...
    return Xml::Default::node;
}

_bstr_t Xml::Node::x_getNameRef ()
{
    if (this->x_xmlNode)
    {
        // return the node name...
        return _bstr_t ((wchar_t *)static_cast<ShellNode *>(this->x_xmlNode)->getName ());
    }
    return _bstr_t ();
}

_bstr_t Xml::Node::x_getTextRef ()
{
    if (this->x_xmlNode)
    {
        return _bstr_t ((wchar_t *)static_cast<ShellNode *>(this->x_xmlNode)->getText ());
    }
    return _bstr_t ();
}

void Xml::Node::x_putTextRef (_bstr_t textString)
{
    if (this->x_xmlNode)
    {
        static_cast<ShellNode *>(this->x_xmlNode)->setText (textString);
    }
}

Xml::Status::code Xml::Node::x_statusRef ()
{
    if (this->x_xmlNode)
    {
        // return the status...
        return static_cast<ShellNode *>(this->x_xmlNode)->status;
    }
    // default status...
    return Xml::Status::noNode;
}

/* eof */