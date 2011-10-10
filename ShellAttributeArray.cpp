/** 
*    filename: ShellAttributeArray.cpp 
* 
* description: Implements the ShellAttributeArray class.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"

#define IndexedAttributeRange (wchar_t *)0xFFFF
#define UnknownAttributeName  (wchar_t *)0xC0C0C0C0

ShellAttributeArray::ShellAttributeArray ()
{
    this->domNode = 0;
    this->domNodeAttributes = 0;
}

ShellAttributeArray::~ShellAttributeArray ()
{
    // release the interfaces...
    if (this->domNode) this->domNode->Release ();
    if (this->domNodeAttributes) this->domNodeAttributes->Release ();
    // clear the atttribute name list...
    while (this->attributeNameStack.size ())
    {
        BSTR nameString = this->attributeNameStack.front ();
        this->attributeNameStack.pop_front ();
        if (nameString && (nameString > IndexedAttributeRange) && (nameString != UnknownAttributeName)) SysFreeString (nameString);
    }
}

void ShellAttributeArray::copy (IXMLDOMNode *sourceDomNode)
{
    // clear any existing...
    if (this->domNode) try
    {
        this->domNode->Release ();
        this->domNode = 0;
    }
    catch (...)
    {
        this->domNode = 0;
    }
    if (this->domNodeAttributes) try
    {
        this->domNodeAttributes->Release ();
        this->domNodeAttributes = 0;
    }
    catch (...)
    {
        this->domNodeAttributes = 0;
    }
    // copy the node...
    if (this->domNode = sourceDomNode) try
    {
        // increment usage...
        this->domNode->AddRef ();
        // retrieve its attributes...
        if (!SUCCEEDED (sourceDomNode->get_attributes (&this->domNodeAttributes)))
        {
            // ensure the pointer is null...
            this->domNodeAttributes = 0;
        }
    }
    catch (...) {}
}

bool ShellAttributeArray::add (const char *attributeName,_variant_t attributeValue)
{
bool addedOk=false;

    if (attributeName && attributeName[0])
    {
        // add the specified attribute...
        if (this->domNode && this->domNodeAttributes) try
        {
            // get the owner document..
            IXMLDOMDocument *ownerDocument=0;
            if (SUCCEEDED (this->domNode->get_ownerDocument (&ownerDocument)))
            {
                // create the attribute node...
                IXMLDOMNode *newAttribute=0;
                if (SUCCEEDED (ownerDocument->createNode (_variant_t (2),_bstr_t (attributeName),0,&newAttribute)) && newAttribute)
                {
                    // set the value...
                    if (SUCCEEDED (newAttribute->put_nodeValue (attributeValue)))
                    {
                        // add the attribute item...
                        if (SUCCEEDED (this->domNodeAttributes->setNamedItem (newAttribute,0)))
                        {
                            // succeeded...
                            addedOk = true;
                        }
                    }
                    newAttribute->Release ();
                }
                ownerDocument->Release ();
            }
        }
        catch (...) {}
    }
    return addedOk;
}

bool ShellAttributeArray::remove (const char *attributeName)
{
    if (attributeName && attributeName[0])
    {
        // remove the specified attribute...
        if (this->domNodeAttributes) try
        {
            IXMLDOMNode *removedNode=0;
            // remove from the list...
            if (SUCCEEDED (this->domNodeAttributes->removeNamedItem (_bstr_t (attributeName),&removedNode)))
            {
                // succeeded...
                if (removedNode) removedNode->Release ();
                return true;
            }
        }
        catch (...) {}
    }
    return false;
}

long ShellAttributeArray::x_countRef ()
{
long length = 0;

    // return the count of attributes...
    if (this->domNodeAttributes) try
    {
        if (!SUCCEEDED (this->domNodeAttributes->get_length (&length)))
        {
            length = 0;
        }
    }
    catch (...) {}

    return length;
}

bool ShellAttributeArray::x_getIsNumeric ()
{
    // determine if the attribute named is numeric...
    BSTR value = this->getValue ();
    if (value)
    {
        bool numericType=false;
        // determine the chars...
        if ((value[0] >= '0') && (value[0] <= '9')) 
        {
            // starts with numbers...
            numericType = true;
        }
        else if ((value[0] == '-') || (value[0] == '+'))
        {
            // starts with a sign might be a number...
            if ((value[1] >= '0') && (value[1] <= '9')) 
            {
                // starts with sign then has numbers...
                numericType = true;
            }
        }
        SysFreeString (value);
        return numericType;
    }
    return false;
}

_bstr_t ShellAttributeArray::x_nameRef ()
{
    // return the read only name...
    if (this->attributeNameStack.size ())
    {
        // get the attribute name to use...
        BSTR theName (this->attributeNameStack.front ());
        this->attributeNameStack.pop_front ();

        if (theName != UnknownAttributeName)
        {
            if (theName > IndexedAttributeRange)
            {
                _bstr_t nameString;
                // this is the name to return...
                nameString.Attach (theName);
                return nameString;
            }
            else if (this->domNodeAttributes)
            {
                try
                {
                    // read the specified attribute...
                    IXMLDOMNode *xmlNode=NULL;
                    // get the node...
                    if (this->domNodeAttributes->get_item ((long)theName,&xmlNode) == S_OK)
                    {
                        BSTR bstrName=0;
                        // get the value...
                        if (SUCCEEDED (xmlNode->get_nodeName (&bstrName)) && bstrName[0])
                        {
                            _bstr_t returnName;
                            returnName.Attach (bstrName);
                            // return this name...
                            xmlNode->Release ();
                            return returnName;
                        }
                        xmlNode->Release ();
                    }
                }
                catch (...) {}
            }
        }
    }
    return _bstr_t ();
}

bool ShellAttributeArray::readAttribute (long attributeIndex,VARIANT *attributeValue)
{
    if (this->domNodeAttributes) try
    {
        long length=0;
        // make sure in range...
        if (SUCCEEDED (this->domNodeAttributes->get_length (&length)))
        {
            if (attributeIndex < length)
            {
                // read the specified attribute...
                IXMLDOMNode *xmlNode=NULL;
                // get the node...
                if (this->domNodeAttributes->get_item (attributeIndex,&xmlNode) == S_OK)
                {
                    attributeValue->vt = VT_BSTR;
                    // get the value...
                    HRESULT hr = xmlNode->get_nodeValue (attributeValue);
                    xmlNode->Release ();
                    return SUCCEEDED (hr);
                }
            }
        }
    }
    catch (...) {}

    return false;
}

bool ShellAttributeArray::readAttribute (wchar_t *attributeName,VARIANT *attributeValue)
{
    if (this->domNodeAttributes) try
    {
        // read the specified attribute...
        IXMLDOMNode *xmlNode=NULL;
        // get the node...
        if (this->domNodeAttributes->getNamedItem (attributeName,&xmlNode) == S_OK)
        {
            attributeValue->vt = VT_BSTR;
            // get the value...
            HRESULT hr = xmlNode->get_nodeValue (attributeValue);
            xmlNode->Release ();
            return SUCCEEDED (hr);
        }
    }
    catch (...) {}

    return false;
}

bool isPositiveIntent (const wchar_t *value)
{
    // looking for a positive intent value...
    if (value)
    {
        if ((value[0] == 'T') || (value[0] == 't')) 
        {
            if ((value[1] == 'R') || (value[1] == 'r'))
            {
                if ((value[2] == 'U') || (value[2] == 'u')) return true;
            }
        }
        else if ((value[0] == 'Y') || (value[0] == 'y')) 
        {
            if ((value[1] == 'E') || (value[1] == 'e'))
            {
                if ((value[2] == 'S') || (value[2] == 's')) return true;
            }
        }
        else if ((value[0] == 'E') || (value[0] == 'e'))
        {
            if ((value[1] == 'N') || (value[1] == 'n'))
            {
                if ((value[2] == 'A') || (value[2] == 'a')) return true;
            }
        }
    }
    return false;
}

int smart_wtoi (const wchar_t *value)
{
    // this returns one for the following positive intents, else wtoi...
    // done to allow boolean values to be represented numerically...
    if ((value[0] == 'T') || (value[0] == 't')) 
    {
        if ((value[1] == 'R') || (value[1] == 'r'))
        {
            if ((value[2] == 'U') || (value[2] == 'u')) return 1;
        }
    }
    else if ((value[0] == 'Y') || (value[0] == 'y')) 
    {
        if ((value[1] == 'E') || (value[1] == 'e'))
        {
            if ((value[2] == 'S') || (value[2] == 's')) return 1;
        }
    }
    else if ((value[0] == 'E') || (value[0] == 'e'))
    {
        if ((value[1] == 'N') || (value[1] == 'n'))
        {
            if ((value[2] == 'A') || (value[2] == 'a')) return 1;
        }
    }
    // return wtoi result...
    return _wtoi (value);
}

long smart_wtol (const wchar_t *value)
{
    // this returns one for the following positive intents, else wtol...
    // done to allow boolean values to be represented numerically...
    if ((value[0] == 'T') || (value[0] == 't')) 
    {
        if ((value[1] == 'R') || (value[1] == 'r'))
        {
            if ((value[2] == 'U') || (value[2] == 'u')) return 1;
        }
    }
    else if ((value[0] == 'Y') || (value[0] == 'y')) 
    {
        if ((value[1] == 'E') || (value[1] == 'e'))
        {
            if ((value[2] == 'S') || (value[2] == 's')) return 1;
        }
    }
    else if ((value[0] == 'E') || (value[0] == 'e'))
    {
        if ((value[1] == 'N') || (value[1] == 'n'))
        {
            if ((value[2] == 'A') || (value[2] == 'a')) return 1;
        }
    }
    // return wtol result...
    return _wtol (value);
}

ShellAttributeArray::operator bool ()
{
bool result = false;

    BSTR value = this->getValue ();
    if (value)
    {
        if (value[0] == '1') result = true;
        else result = isPositiveIntent (value);
        // free the string...
        SysFreeString (value);
    }
    return result;
}

ShellAttributeArray::operator char ()
{
char result = 0;

    BSTR value = this->getValue ();
    if (value)
    {
        result = (char)smart_wtoi (value);
        SysFreeString (value);
    }
    return result;
}

ShellAttributeArray::operator unsigned char ()
{
unsigned char result = 0;

    BSTR value = this->getValue ();
    if (value)
    {
        result = (unsigned char)smart_wtoi (value);
        SysFreeString (value);
    }
    return result;
}

ShellAttributeArray::operator short ()
{
short result = 0;

    BSTR value = this->getValue ();
    if (value)
    {
        result = (short)smart_wtoi (value);
        SysFreeString (value);
    }
    return result;
}

ShellAttributeArray::operator unsigned short ()
{
unsigned short result = 0;

    BSTR value = this->getValue ();
    if (value)
    {
        result = (unsigned short)smart_wtoi (value);
        SysFreeString (value);
    }
    return result;
}

ShellAttributeArray::operator int ()
{
int result = 0;

    BSTR value = this->getValue ();
    if (value)
    {
        result = (int)smart_wtoi (value);
        SysFreeString (value);
    }
    return result;
}

ShellAttributeArray::operator unsigned int ()
{
unsigned int result = 0;

    BSTR value = this->getValue ();
    if (value)
    {
        result = (unsigned int)smart_wtoi (value);
        SysFreeString (value);
    }
    return result;
}

ShellAttributeArray::operator long ()
{
long result = 0;

    BSTR value = this->getValue ();
    if (value)
    {
        result = smart_wtol (value);
        SysFreeString (value);
    }
    return result;
}

ShellAttributeArray::operator unsigned long ()
{
unsigned long result = 0;

    BSTR value = this->getValue ();
    if (value)
    {
        result = (unsigned long)smart_wtol (value);
        SysFreeString (value);
    }
    return result;
}

const char *ShellAttributeArray::x_getPtr ()
{
    BSTR value = this->getValue ();
    if (value)
    {
        // keep the const in the ref bstr...
        this->attributeRef = value;
        SysFreeString (value);
        return this->attributeRef.operator const char * ();
    }
    // always safe...
    this->attributeRef = " ";
    return this->attributeRef.operator const char * ();
}

_bstr_t ShellAttributeArray::x_getBstr ()
{
    return _bstr_t (this->x_getPtr ());
}

ShellAttributeArray::operator _bstr_t ()
{
    BSTR value = this->getValue ();
    if (value)
    {
        _bstr_t result (value);
        SysFreeString (value);
        return result;
    }
    return _bstr_t ();
}

bool ShellAttributeArray::x_existsRef ()
{
    // determine if the attribute named exists...
    BSTR value = this->getValue ();
    if (value)
    {
        SysFreeString (value);
        return true;
    }
    return false;
}

BSTR ShellAttributeArray::getValue ()
{
    // retrieve the attribute...it can be in this node or in the
    VARIANT attributeValue;
    attributeValue.vt = VT_BSTR;
    attributeValue.bstrVal = 0;
    // inherited node of this node...name specification required...
    if (this->attributeNameStack.size ())
    {
        // get the attribute name to use...
        BSTR theName = this->attributeNameStack.front ();
        this->attributeNameStack.pop_front ();

        if (theName != UnknownAttributeName)
        {
            if (theName > IndexedAttributeRange)
            {
                // read the attribute by name...
                this->readAttribute (theName,&attributeValue);
                SysFreeString (theName);
            }
            else
            {
                // read the attribute by index...
                this->readAttribute ((long)theName,&attributeValue);
            }
        }
    }
    return attributeValue.bstrVal;
}

bool ShellAttributeArray::writeAttribute (long attributeIndex,_variant_t &attributeValue)
{
    if (this->domNodeAttributes) try
    {
        long length=0;
        // make sure in range...
        if (SUCCEEDED (this->domNodeAttributes->get_length (&length)))
        {
            if (attributeIndex < length)
            {
                // write the specified attribute...
                IXMLDOMNode *xmlNode=NULL;
                // get the node...
                if (this->domNodeAttributes->get_item (attributeIndex,&xmlNode) == S_OK)
                {
                    // get the value...
                    HRESULT hr = xmlNode->put_nodeValue (attributeValue);
                    xmlNode->Release ();
                    return SUCCEEDED (hr);
                }
            }
        }
    }
    catch (...) {}

    return false;
}

bool ShellAttributeArray::writeAttribute (wchar_t *attributeName,_variant_t &attributeValue)
{
    if (this->domNodeAttributes) try
    {
        // write the specified attribute...
        IXMLDOMNode *xmlNode=NULL;
        // get the node...
        if (this->domNodeAttributes->getNamedItem (attributeName,&xmlNode) == S_OK)
        {
            // get the value...
            HRESULT hr = xmlNode->put_nodeValue (attributeValue);
            xmlNode->Release ();
            return SUCCEEDED (hr);
        }
    }
    catch (...) {}

    return false;
}

Xml::Attribute &ShellAttributeArray::operator = (Xml::Attribute &value)
{
    this->putValue (_variant_t (value.bstr));
    return *this;
}

bool ShellAttributeArray::operator = (bool value)
{
    this->putValue (_variant_t (value));
    return value;
}

char ShellAttributeArray::operator = (char value)
{
    this->putValue (_variant_t (value));
    return value;
}

unsigned char ShellAttributeArray::operator = (unsigned char value)
{
    this->putValue (_variant_t (value));
    return value;
}

short ShellAttributeArray::operator = (short value)
{
    this->putValue (_variant_t (value));
    return value;
}

unsigned short ShellAttributeArray::operator = (unsigned short value)
{
    this->putValue (_variant_t (value));
    return value;
}

int ShellAttributeArray::operator = (int value)
{
    this->putValue (_variant_t (value));
    return value;
}

unsigned int ShellAttributeArray::operator = (unsigned int value)
{
    this->putValue (_variant_t (value));
    return value;
}

long ShellAttributeArray::operator = (long value)
{
    this->putValue (_variant_t (value));
    return value;
}

unsigned long ShellAttributeArray::operator = (unsigned long value)
{
    this->putValue (_variant_t (value));
    return value;
}

_bstr_t ShellAttributeArray::operator = (char *value)
{
    this->putValue (_variant_t (value));
    return _bstr_t (value);
}

_bstr_t ShellAttributeArray::operator = (wchar_t *value)
{
    this->putValue (_variant_t (value));
    return _bstr_t (value);
}

void ShellAttributeArray::putValue (_variant_t valueRef)
{
    // write the attribute...
    if (this->attributeNameStack.size ())
    {
        // get the attribute name to use...
        BSTR theName = this->attributeNameStack.front ();
        this->attributeNameStack.pop_front ();

        if (theName != UnknownAttributeName)
        {
            if (theName > IndexedAttributeRange)
            {
                // write the attribute by name...
                this->writeAttribute (theName,valueRef);
                SysFreeString (theName);
            }
            else
            {
                // write the attribute by index...
                this->writeAttribute ((long)theName,valueRef);
            }
        }
    }
}

Xml::Attribute &ShellAttributeArray::operator [] (long attributeIndex)
{
    // push the index onto stack...
    this->attributeNameStack.push_back ((BSTR)attributeIndex);
    // return the operator interface...
    return *this;
}

Xml::Attribute &ShellAttributeArray::operator [] (const char *attributeName)
{
    // query the attribute...
    if (attributeName && attributeName[0])
    {
        _bstr_t copiedName (attributeName);
        // assign the next name to use...
        this->attributeNameStack.push_back (copiedName.Detach ());
    }
    else
    {
        // assign a fault name...
        this->attributeNameStack.push_back (UnknownAttributeName);
    }
    // return the operator interface...
    return *this;
}

/* eof */