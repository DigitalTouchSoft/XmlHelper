/**
*
*    Filename: ShellXml.h
*
* Description: Declares the shell'd aspects of xml nodes.
*
*      Author: Dennis McMahon
*
*/

#pragma once

#include <msxml2.h>

class ShellAttributeArray : public Xml::AttributeArray, public Xml::Attribute
{
private:
    _bstr_t attributeRef;
    std::list<BSTR> attributeNameStack;
    bool readAttribute (long attributeIndex,VARIANT *attributeValue);
    bool readAttribute (wchar_t *attributeName,VARIANT *attributeValue);
    bool getAttribute (_variant_t *attributeValue);
    bool writeAttribute (long attributeIndex,_variant_t &attributeValue);
    bool writeAttribute (wchar_t *attributeName,_variant_t &attributeValue);
    // the xml dom interfaces...
    IXMLDOMNode *domNode;
    IXMLDOMNamedNodeMap *domNodeAttributes;

public:
    ShellAttributeArray ();
    virtual ~ShellAttributeArray ();

    // copy another...
    void copy (IXMLDOMNode *sourceDomNode);

    // AttributeArray class implementation...
    bool add (const char *attributeName,_variant_t attributeValue);
    bool remove (const char *attributeName);
    Xml::Attribute &operator [] (long attributeIndex);
    Xml::Attribute &operator [] (const char *attributeName);
    long x_countRef ();
    // the attribute class...
    bool x_existsRef ();
    _bstr_t x_nameRef ();
    BSTR getValue ();
    void putValue (_variant_t valueRef);
    const char *x_getPtr ();
    _bstr_t x_getBstr ();
    bool x_getIsNumeric ();

    // retrieves the value of the attribute...
    operator bool ();
    operator char ();
    operator unsigned char ();
    operator short ();
    operator unsigned short ();
    operator int ();
    operator unsigned int ();
    operator long ();
    operator unsigned long ();
    operator _bstr_t ();
    // sets the value of the attribute...
    Xml::Attribute &operator = (Xml::Attribute &value);
    bool operator = (bool value);
    char operator = (char value);
    unsigned char operator = (unsigned char value);
    short operator = (short value);
    unsigned short operator = (unsigned short value);
    int operator = (int value);
    unsigned int operator = (unsigned int value);
    long operator = (long value);
    unsigned long operator = (unsigned long value);
    _bstr_t operator = (char *value);
    _bstr_t operator = (wchar_t *value);
};

typedef std::map<IXMLDOMNode *,Xml::Node *> XMLNodeMap;

class ShellNodeArray : public Xml::NodeArray
{
private:
    XMLNodeMap nodeMap;
    IXMLDOMNode *domNode;
    IXMLDOMNodeList *domNodeList;
    void clear ();

public:
    ShellNodeArray ();
    virtual ~ShellNodeArray ();

    // copy another...
    void copy (IXMLDOMNode *sourceDomNode);
    // NodeArray class implementation...
    Xml::Node &add (const char *nodeName,long insertPosition = -1);
    bool remove (const char *nodeName);
    Xml::Node &operator[] (long nodeIndex);
    Xml::Node &operator [] (const char *searchString);
    // ...reference functions...
    long x_countRef ();
};

class ShellNode 
{
public:
    Xml::Status::code status;
    IXMLDOMNode *domNode;

    ShellNode ();
    virtual ~ShellNode ();

    // copy another...
    void copy (IXMLDOMNode *sourceDomNode);
    // get the node name...
    BSTR getName ();
    // get set text section...
    BSTR getText ();
    bool setText (_bstr_t &textString);
};

class ShellDocument
{
public:
    Xml::Status::code status;
    IXMLDOMDocument2 *domDocument;

    ShellDocument ();
    virtual ~ShellDocument ();
    IXMLDOMNode *loadFile (_bstr_t &fileName);
    IXMLDOMNode *loadXML (_bstr_t &xmlString);
};

class DOMInterfaceCollections
{
public: static bool copyNode (Xml::Node *subjectNode,IXMLDOMNode *domNode);
};

/* eof */