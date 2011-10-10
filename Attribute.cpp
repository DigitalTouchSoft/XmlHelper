/** 
*    filename: Attribute.cpp 
* 
* description: Implements the the Xml::Attribute class.  This is a virtual
*              class that provides only default behavior.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"

/**
* property reference functions.


bool Xml::Attribute::x_existsRef ()
{
    return false;
}

_bstr_t Xml::Attribute::x_nameRef ()
{
    return _bstr_t ();
}


_variant_t Xml::Attribute::x_getValueRef ()
{
    return _variant_t ();
}

void Xml::Attribute::x_putValueRef (_variant_t valueRef)
{

}

/*
bool Xml::AttributeArray::add (_bstr_t attributeName,_variant_t attributeValue)
{
    return false;
}

bool Xml::AttributeArray::remove (_bstr_t attributeName)
{
    return false;
}

Xml::Attribute &Xml::AttributeArray::operator [] (long attributeIndex)
{
    return Xml::Default::attribute;
}

Xml::Attribute &Xml::AttributeArray::operator [] (_bstr_t &attributeName)
{
    return Xml::Default::attribute;
}

Xml::Attribute &Xml::AttributeArray::operator [] (const char *attributeName)
{
    return Xml::Default::attribute;
}

Xml::Attribute &Xml::AttributeArray::operator [] (const wchar_t *attributeName)
{
    return Xml::Default::attribute;
}

long Xml::AttributeArray::x_countRef ()
{
    return 0;
}

/* eof */