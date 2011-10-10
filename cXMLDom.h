/**
*
*     Section: cXmlDom.h Dom Services.
* 
* Description: Defines the cXMLDom class that implements XMLDom objects and
*              provides underlying dom navigation support for all dom derivitive classes.
*
*      Author: Dennis McMahon
* 
*/

#include <msxml2.h>
#include <comutil.h>

namespace DOM
{
  // document loader...
  IXMLDOMNode *loadXMLFile (wchar_t *fileName,IXMLDOMDocument2 **domDocument);
  IXMLDOMNode *loadXMLStream (wchar_t *xmlString,IXMLDOMDocument2 **domDocument);
};

/**
*
* cDomDocEntry: contains the dom document interface and filename pair for a given loaded
*               xml document.
*/

class cDomDocEntry
{
public:
  cDomDocEntry (IXMLDOMDocument2 *xmlInterface,wchar_t *fileName);
  virtual ~cDomDocEntry ();
  // as properties...
  IXMLDOMDocument2 *xmlInterface;
  String fileName;
};

/**
*
* cDomDocs: contains the dom document load functionality and the list of loaded documents.
*
*/

class cDomDocList
{
public:
  cDomDocList ();
  virtual ~cDomDocList ();

  // document container...
  vector<cDomDocEntry *> documents;

  // document container methods...
  wchar_t *rootedName (wchar_t *fileName);
  IXMLDOMAttribute *createAttribute (wchar_t *name,_variant_t value);
};

/**
*
* cXMLDom: Implements the XMLDom interface and general dom document functionality.
*
*/

class cXMLDom : public XMLDom, public XMLAttribute
{
private:

  // all instances have the root doc list pointer...
  cDomDocList *docList;
  // the tree relationship...
  cXMLDom *_parent;
  String   _name;
  String   _text;
  // attribute references...
  BSTR     _attributeValue;
  list <wchar_t *> _attributeName;
  // your child node count(s)...
  long     _nodeCount;
  long     _importCount;

  // the xml interfaces...
  IXMLDOMNode           *xmlNode;
  IXMLDOMNodeList       *xmlNodeList;
  IXMLDOMNamedNodeMap   *xmlNodeAttributes;

  // the imported documents...
  vector<cXMLDom *> imports;
  // the inherited elements...
  cXMLDom *inheritedNode;

  void     insertImport (wchar_t *fileName);
  void     findImports (void);
  void     findInherits (void);
  bool     focusAttributeCalls (const wchar_t *childNode = 0);
  BSTR     readAttribute (wchar_t *attributeName);
  bool     getAttribute (void);
  bool     setAttribute (_variant_t value);
  cXMLDom *spawnNode (long index,cXMLDom *parent);
  cXMLDom *spawnNode (wchar_t *nodeSearch,cXMLDom *parent);
  void    _init (void);

public:

  cXMLDom ();
  cXMLDom (IXMLDOMNode *node,cXMLDom *parent,cDomDocList *docList = 0);
  virtual ~cXMLDom ();

  // base delete...
  void release (void);
  // to write back the document...
  bool save (wchar_t *fielName=0);
  // property queries for the node...
  String  &name (void);
  String  &textSection (void);
  long     nodeCount (void);
  // node creating query methods...
  XMLDom  *spawnChild (long index);
  XMLDom  *spawnChild (wchar_t *nodeSearch);
  XMLDom  *spawnAncestor (wchar_t *nodeSearch);

  // attribute access...
  bool setFocus (const wchar_t *childNode = 0);
  XMLAttribute &attribute (wchar_t *attributeName = 0);

  // typed int operators...
  operator int ();
  int operator() (const int value);
  int operator= (const int value);
  bool operator== (const int value);

  // typed unsigned int operators...
  operator unsigned int ();
  unsigned int operator() (const unsigned int value);
  unsigned int operator= (const unsigned int value);
  bool operator== (const unsigned int value);

  // typed long operators...
  operator long ();
  long operator() (const long value);
  long operator= (const long value);
  bool operator== (const long value);

  // typed unsigned long operators...
  operator unsigned long ();
  unsigned long operator() (const unsigned long value);
  unsigned long operator= (const unsigned long value);
  bool operator== (const unsigned long value);

  // typed bool operators...
  operator bool ();
  bool operator () (const bool value);
  bool operator= (const bool value);
  bool operator== (const bool value);

  // typed double operators...
  operator double ();
  double operator() (const double value);
  double operator= (const double value);
  bool operator== (const double value);

  // typed String operators...
  operator String ();
  String operator () (const String &value);
  String operator () (const wchar_t *value);
  String operator () (const char *value);
  String operator= (String &value);
  String operator= (wchar_t *value);
  String operator= (char *value);
  bool operator== (String &value);
  bool operator== (wchar_t *value);
  bool operator== (char *value);
};

/* eof */