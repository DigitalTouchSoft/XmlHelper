/** 
*    filename: XMLAttribute.cpp 
* 
* description: Implements the operators of the XMLAttribute class.
* 
*      author: Dennis McMahon
* 
*/

#include "stdafx.h"
#include "cXMLDom.h"

/**
* typed int...
*/

XMLAttribute::operator int () 
{ 
    return 0; 
}

int XMLAttribute::operator() (const int value) 
{ 
    return 0; 
}

int XMLAttribute::operator= (const int value) 
{ 
    return 0; 
}

bool XMLAttribute::operator== (const int value) 
{ 
    return false; 
}

/**
* typed unsigned int...
*/

XMLAttribute::operator unsigned int ()
{ 
    return 0; 
}

unsigned int XMLAttribute::operator() (const unsigned int value) 
{ 
    return 0; 
}

unsigned int XMLAttribute::operator= (const unsigned int value) 
{ 
    return 0; 
}

bool XMLAttribute::operator== (const unsigned int value) 
{ 
    return false; 
}

/**
* typed long....
*/

XMLAttribute::operator long () 
{ 
    return 0; 
}

long XMLAttribute::operator() (const long value) 
{ 
    return 0; 
}

long XMLAttribute::operator= (const long value) 
{ 
    return 0; 
}

bool XMLAttribute::operator== (const long value) 
{ 
    return false; 
}

/**
* typed unsigned long...
*/

XMLAttribute::operator unsigned long () 
{ 
    return 0; 
}

unsigned long XMLAttribute::operator() (const unsigned long value) 
{ 
    return 0; 
}

unsigned long XMLAttribute::operator= (const unsigned long value) 
{ 
    return 0; 
}

bool XMLAttribute::operator== (const unsigned long value) 
{ 
    return false; 
}

/**
* typed bool...
*/

XMLAttribute::operator bool () 
{ 
    return false; 
}

bool XMLAttribute::operator () (const bool value) 
{ 
    return false; 
}

bool XMLAttribute::operator= (const bool value) 
{ 
    return false; 
}

bool XMLAttribute::operator== (const bool value) 
{ 
    return false; 
}

/**
* typed double..
*/

XMLAttribute::operator double () 
{ 
    return 0.0; 
}

double XMLAttribute::operator() (const double value) 
{ 
    return 0.0; 
}

double XMLAttribute::operator= (const double value) 
{ 
    return 0.0; 
}

bool XMLAttribute::operator== (const double value) 
{ 
    return false; 
}

/**
* typed String....
*/

XMLAttribute::operator String ()
{
    return String ();
}

String XMLAttribute::operator () (const String &value)
{
    return String ();
}

String XMLAttribute::operator () (const wchar_t *value)
{
    return String ();
}

String XMLAttribute::operator () (const char *value)
{
    return String ();
}

String XMLAttribute::operator= (String &value)
{
    return String ();
}

String XMLAttribute::operator= (wchar_t *value)
{
    return String ();
}

String XMLAttribute::operator= (char *value)
{
    return String ();
}

bool XMLAttribute::operator== (String &value)
{
    return false;
}

bool XMLAttribute::operator== (wchar_t *value)
{
    return false;
}

bool XMLAttribute::operator== (char *value)
{
    return false;
}

/* eof */