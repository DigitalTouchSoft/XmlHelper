/** 
*    filename: ShellNode.cpp 
* 
* description: Implements the ShellNode class.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"

bool DOMInterfaceCollections::copyNode (Xml::Node *subjectNode,IXMLDOMNode *domNode)
{
    // validate the arguments...
    if (subjectNode && subjectNode->x_xmlNode)
    {
        // the primary interface is copied to the shell node...
        static_cast<ShellNode *>(subjectNode->x_xmlNode)->copy (domNode);
        // the sub element interface is copied to the node array class...
        static_cast<ShellNodeArray *>(&subjectNode->child)->copy (domNode);
        // the attribute interface is copied to the attribute array class...
        static_cast<ShellAttributeArray *>(&subjectNode->attribute)->copy (domNode);
        // report assigned...
        return true;
    }
    return false;
}

ShellNode::ShellNode ()
{
    this->status = Xml::Status::noNode;
    this->domNode = 0;
}

ShellNode::~ShellNode ()
{
    // release interfaces...
    if (this->domNode) this->domNode->Release ();
}

void ShellNode::copy (IXMLDOMNode *sourceDomNode)
{
    // clear any existing...
    if (this->domNode) 
    {
        this->domNode->Release ();
        this->domNode = 0;
    }
    // set status to no node...
    this->status = Xml::Status::noNode;
    // copy the node...
    if (this->domNode = sourceDomNode)
    {
        // increment the usage count...
        this->domNode->AddRef ();
        // set status ready...
        this->status = Xml::Status::ready;
    }
}

BSTR ShellNode::getName ()
{
BSTR nameString=0;

    // get the node name into a string...
    if (this->domNode) try
    {
        if (!SUCCEEDED (this->domNode->get_nodeName (&nameString)))
        {
            if (nameString)
            {
                // some failure, clear the name...
                SysFreeString (nameString);
                nameString = 0;
            }
        }
    }
    catch (...) {}

    return nameString;
}

BSTR ShellNode::getText ()
{
BSTR textString=0;

    // get the text section of this node...
    if (this->domNode) try
    {
        IXMLDOMNode *text=NULL;
        if (SUCCEEDED (this->domNode->selectSingleNode (L"text()",&text)) && text)
        {
            // read its text value...
            VARIANT value;
            value.vt = 0;
            value.bstrVal = NULL;
            if (SUCCEEDED (text->get_nodeValue (&value)) && (value.vt == VT_BSTR))
            {
                // return the reference...
                textString =  value.bstrVal;
            }
            text->Release ();
        }
    }
    catch (...) {}

    return textString;
}

bool ShellNode::setText (_bstr_t &textString)
{
bool setOk = false;

    // set the text section of this node...
    if (this->domNode && textString.length ()) try
    {
        // and the string is valid...
        IXMLDOMNode *text=NULL;
        if (SUCCEEDED (this->domNode->selectSingleNode (L"text()",&text)) && text)
        {
            // write its text value...
            _variant_t value (textString);
            if (SUCCEEDED (text->put_nodeValue (value)))
            {
                // keep the reference...
                setOk = true;
            }
            text->Release ();
        }
        else
        {
            // insert a text section node...
            IXMLDOMDocument *ownerDocument = 0;
            if (SUCCEEDED (this->domNode->get_ownerDocument (&ownerDocument)))
            {
                IXMLDOMText *textNode=0;
                if (SUCCEEDED (ownerDocument->createTextNode (textString,&textNode)))
                {
                    // insert this in the current node...
                    if (SUCCEEDED (this->domNode->appendChild (textNode,0)))
                    {
                        // finished...
                        setOk = true;
                    }
                    textNode->Release ();
                }
                ownerDocument->Release ();
            }
        }
    }
    catch (...) {}

    return setOk;
}

/* eof */