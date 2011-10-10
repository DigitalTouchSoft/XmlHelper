/** 
*    filename: cXMLDomOperator.cpp 
* 
* description: Implements the operators of the cXMLDom class.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"
#include "cXMLDom.h"

/**
* int operators...
*/

cXMLDom::operator int ()
{
    // retrieve attribute value as an int...
    if (this->getAttribute ())
    {
        // convert the value to an int...
        return _wtoi (this->_attributeValue);
    }
    return 0;
}

int cXMLDom::operator () (const int value)
{
    // retrieve attribute value as an int...
    if (this->getAttribute ())
    {
        // convert the value to an int...
        return _wtoi (this->_attributeValue);
    }
    // return the value as default...
    return value;
}

int cXMLDom::operator= (const int value)
{
    // assign attribute value as an int...
    if (this->setAttribute (_variant_t (value)))
    {
        // return the value...
        return (int)value;
    }
    return 0;
}

bool cXMLDom::operator == (const int value)
{
    // return the comparison...
    return (this->operator int () == value);
}

/**
* unsigned int operators...
*/

cXMLDom::operator unsigned int ()
{
    // retrieve the attribute as an unsigned int...
    if (this->getAttribute ())
    {
        // convert the value to an unsigned int...
        return (unsigned int)_wtoi (this->_attributeValue);
    }
    return 0;
}

unsigned int cXMLDom::operator() (const unsigned int value)
{
    // retrieve the attribute as an unsigned int...
    if (this->getAttribute ())
    {
        // convert the value to an unsigned int...
        return (unsigned int)_wtoi (this->_attributeValue);
    }
    // return the value as a default...
    return value;
}

unsigned int cXMLDom::operator= (const unsigned int value)
{
    // assign attribute value as an unsigned int...
    if (this->setAttribute (_variant_t (value)))
    {
        // return the value...
        return (unsigned int)value;
    }
    return 0;
}

bool cXMLDom::operator== (const unsigned int value)
{
    // return the comparison...
    return (this->operator unsigned int () == value);
}

/**
* long operators...
*/

cXMLDom::operator long ()
{
    // retrieve attribute value...
    if (this->getAttribute ())
    {
        // convert the value to a long...
        return _wtol (this->_attributeValue);
    }
    return 0;
}

long cXMLDom::operator() (const long value)
{
    // retrieve attribute value...
    if (this->getAttribute ())
    {
        // convert the value to a long...
        return _wtol (this->_attributeValue);
    }
    // return the value as default...
    return value;
}

long cXMLDom::operator= (const long value)
{
    // assign attribute value...
    if (this->setAttribute (_variant_t (value)))
    {
        // return the value...
        return (long)value;
    }
    return 0;
}

bool cXMLDom::operator== (const long value)
{
    // return the comparison...
    return (this->operator long () == value);
}

/**
* unsigned long operators...
*/

cXMLDom::operator unsigned long ()
{
    // get the attribute as an unsigned long...
    if (this->getAttribute ())
    {
        // convert the value to an unsigned long...
        return (unsigned long)_wtol (this->_attributeValue);
    }
    return 0;
}

unsigned long cXMLDom::operator() (const unsigned long value)
{
    // get the attribute as an unsigned long...
    if (this->getAttribute ())
    {
        // convert the value to an unsigned long...
        return (unsigned long)_wtol (this->_attributeValue);
    }
    // return the value as default...
    return value;
}

unsigned long cXMLDom::operator= (const unsigned long value)
{
    // set attribute value...
    if (this->setAttribute (_variant_t (value)))
    {
        return (unsigned long)value;
    }
    return 0;
}

bool cXMLDom::operator== (const unsigned long value)
{
    // return the comparison...
    return (this->operator unsigned long () == value);
}

/**
* bool operators...
*/

cXMLDom::operator bool ()
{
    // get the attribute as a boolean...
    if (this->getAttribute ())
    {
        // boolean can be alphanumerical...
        if (this->_attributeValue[0] == 'T') return true;
        else if (this->_attributeValue[0] == 't') return true;
        else if (this->_attributeValue[0] == 'Y') return true;
        else if (this->_attributeValue[0] == 'y') return true;
        // boolean can be numerical...
        else if (_wtol (this->_attributeValue)) return true;
    }
    return false;
}

bool cXMLDom::operator () (const bool value)
{
    // get the attribute as a boolean...
    if (this->getAttribute ())
    {
        // boolean can be alphanumerical...
        if (this->_attributeValue[0] == 'T') return true;
        else if (this->_attributeValue[0] == 't') return true;
        else if (this->_attributeValue[0] == 'Y') return true;
        else if (this->_attributeValue[0] == 'y') return true;
        // boolean can be numerical...
        else if (_wtol (this->_attributeValue)) return true;
        // no truths...
        return false;
    }
    // return the value as default...
    return value;
}

bool cXMLDom::operator= (const bool value)
{
    // convert the value to a string...
    wchar_t *valueString = L"True";
    if (!value) valueString = L"False";
    // assign the value...
    if (this->setAttribute (_variant_t (valueString)))
    {
        return (bool)value;
    }
    return false;
}

bool cXMLDom::operator== (const bool value)
{
    // return the comparison...
    return (this->operator bool () == value);
}

/**
* double operators...
*/

cXMLDom::operator double ()
{
    // get the attribute as a floating double...
    if (this->getAttribute ())
    {
        // convert the value to double...
        return _wtof (this->_attributeValue);
    }
    return 0.0;
}

double cXMLDom::operator() (const double value)
{
    // get the attribute as a floating double...
    if (this->getAttribute ())
    {
        // convert the value to double...
        return _wtof (this->_attributeValue);
    }
    // return the value as a default...
    return value;
}

double cXMLDom::operator= (const double value)
{
    // set attribute value...
    if (this->setAttribute (_variant_t (value)))
    {
        return (double)value;
    }
    return 0.0;
}

bool cXMLDom::operator== (const double value)
{
    // return the comparison...
    return (this->operator double () == value);
}

/**
* wchar_t *operators...

cXMLDom::operator wchar_t* ()
{
    // retrieve the value as a string...
    if (this->getAttribute ())
    {
        // get the new value...
        return wide::newCopy (this->_attributeValue);
    }
    return 0;
}

wchar_t *cXMLDom::operator() (const wchar_t *value)
{
    // retrieve the value as a string...
    if (this->getAttribute ())
    {
        // get the new value...
        return wide::newCopy (this->_attributeValue);
    }
    // return the value as a default...
    return wide::newCopy (value);
}

wchar_t *cXMLDom::operator= (const wchar_t *value)
{
    // set the attribute as a string...
    if (!value)
    {
        // validated nulls, they remove the attribute...
    }
    // assign the value...
    if (this->setAttribute (_variant_t (value)))
    {
        return (wchar_t *)value;
    }
    return 0;
}

bool cXMLDom::operator== (const wchar_t *value)
{
    // return the comparison...
    return (wide::compare (this->operator wchar_t * (),value) == 0);
}

*/

/**
* char *operators...

cXMLDom::operator char* ()
{
    // retrieve the value as a string...
    if (this->getAttribute ())
    {
        char *value = new char [wide::length (this->_attributeValue) + 2];
        if (value)
        {
            // get the new value...
            for (int pos=0; this->_attributeValue[pos]; ++pos) value[pos] = (char)this->_attributeValue[pos];
            value[pos] = 0;
            return value;
        }
    }
    return 0;
}

char *cXMLDom::operator() (const char *value)
{
    // retrieve the value as a string...
    if (this->getAttribute ())
    {
        char *byteValue = new char [wide::length (this->_attributeValue) + 2];
        if (byteValue)
        {
            // get the new value...
            for (int pos=0; this->_attributeValue[pos]; ++pos) byteValue[pos] = (char)this->_attributeValue[pos];
            byteValue[pos] = 0;
            return byteValue;
        }
    }
    // return the value as a default...
    if (value)
    {
        for (long pos=0; value[pos]; ++pos);
        char *byteValue = new char [pos+2];
        if (byteValue)
        {
            for (pos=0; value[pos]; ++pos) byteValue[pos] = value[pos];
            byteValue[pos] = 0;
            return byteValue;
        }
    }
    return 0;
}

char *cXMLDom::operator= (const char *value)
{
    // set the attribute as a string...
    if (!value)
    {
        // validated nulls, they remove the attribute...
    }
    // assign the value...
    if (this->setAttribute (_variant_t (value)))
    {
        return (char *)value;
    }
    return 0;
}

bool cXMLDom::operator== (const char *value)
{
    // return the comparison...
    wchar_t *wideValue = wide::newCopy (value);
    if (wideValue)
    {
        bool result = (wide::compare (this->operator wchar_t * (),wideValue) == 0);
        delete wideValue;
        return result;
    }
    return false;
}

*/

/**
* String operators...
*/

cXMLDom::operator String ()
{
    if (this->getAttribute ())
    {
        // get the new value...
        return String (this->_attributeValue);
    }
    return String ();
}

String cXMLDom::operator () (const String &value)
{
    if (this->getAttribute ())
    {
        // get the new value...
        return String (this->_attributeValue);
    }
    // return the default...
    return String ((String &)value);
}

String cXMLDom::operator () (const wchar_t *value)
{
    if (this->getAttribute ())
    {
        // get the new value...
        return String (this->_attributeValue);
    }
    // return the default...
    return String (value);
}

String cXMLDom::operator () (const char *value)
{
    if (this->getAttribute ())
    {
        // get the new value...
        return String (this->_attributeValue);
    }
    // return the default...
    return String (value);
}

String cXMLDom::operator= (String &value)
{
    // set the attribute as a string...
    if (!value)
    {
        // validated nulls, they remove the attribute...
    }
    // assign the value...
    if (this->setAttribute (_variant_t ((wchar_t *)value)))
    {
        return String (value);
    }
    return String ();
}

String cXMLDom::operator= (wchar_t *value)
{
    // set the attribute as a string...
    if (!value)
    {
        // validated nulls, they remove the attribute...
    }
    // assign the value...
    if (this->setAttribute (_variant_t (value)))
    {
        return String (value);
    }
    return String ();
}

String cXMLDom::operator= (char *value)
{
    // set the attribute as a string...
    if (!value)
    {
        // validated nulls, they remove the attribute...
    }
    // assign the value...
    if (this->setAttribute (_variant_t (value)))
    {
        return String (value);
    }
    return String ();
}

bool cXMLDom::operator== (String &value)
{
    if (this->getAttribute ())
    {
        // equal if the same...
        return (value == this->_attributeValue);
    }
    // equal if null...
    return (!value);
}

bool cXMLDom::operator== (wchar_t *value)
{
    if (this->getAttribute ())
    {
        // equal if the same...
        return (String (value) == this->_attributeValue);
    }
    // equal if null...
    return (!value);
}

bool cXMLDom::operator== (char *value)
{
    if (this->getAttribute ())
    {
        // equal if the same...
        return (String (value) == this->_attributeValue);
    }
    // equal if null...
    return (!value);
}

/* eof */