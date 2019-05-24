
//
// this file shows how to use the basic features of libxml2
//
//

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xpath.h>
#include <libxml/xmlIO.h>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string.h>


//----------------------------------------------------------------------------------------------
//
// this is a file of examples on using funcions in libxml2
// documentation : http://www.xmlsoft.org
// download      : ftp://www.xmlsoft.org/libxml2 
// version       : 2.7.8
// date          : 10/24/2011
// 
// to compile: g++ xml_basics.cpp -oxmlb -I/usr/local/include/libxml2 -L/usr/local/lib -lxml2
//
// note if xml2 is installed in a different location on your server use that location instead
//----------------------------------------------------------------------------------------------

// create an xml document
// creating an xmlDocPtr from a const char *

const char *example_of_easy_xml_doc()
{
  // not use of static doc_string so returning pointer to its internals will
  // be valid
  static std::string doc_str;
  std::stringstream strm;
  strm << "<letter>" << std::endl;
  strm << "  <heading>" << std::endl;
  strm << "     <to> John </to>" << std::endl;
  strm << "     <from> Tracy </from>" << std::endl;
  strm << "     <subject> Reminder </subject>" << std::endl;
  strm << "  </heading>" << std::endl;
  strm << "  <body>" << std::endl;
  strm << " Hi Tracy, this is a reminder to do your homework" << std::endl;
  strm << "  </body> " << std::endl;
  strm << "</letter>" << std::endl;
  doc_str = strm.str();
  return doc_str.c_str();
}


void example1()
{
  // Example1 : create an xmlDocPtr using xmlReadMemory and then dump it to stdout

  const char *text = example_of_easy_xml_doc();

  // libxml2 function xmlReadMemory
  xmlDocPtr doc = xmlReadMemory( text , strlen(text) , "mytext.xml" , NULL , 0 );

  // libxml2 function xmlDocDump
  xmlDocDump( stdout , doc );
}


void walk_tree( xmlNodePtr node )
{
  // basic walking through an xml tree 
  if ( !node ) return;
  for( xmlNodePtr curr = node ; curr ; curr = curr->next )
    {
      if ( curr->type == XML_ELEMENT_NODE )
	{
	  std::cout << "element_node=" << curr->name << std::endl;
	}
      if ( curr->children )
	walk_tree( curr->children );
    }
}

void example2()
{
  // Example2 : create an xml document and walk through it

  const char *text = example_of_easy_xml_doc();

  // create the xmlDocPtr using libxml2 function xmlReadMemory
  xmlDocPtr doc = xmlReadMemory( text , strlen(text) , "mytext.xml" , NULL , 0 );

  // get the root node usign libxml2 function xmlDocGetRootElement(doc);
  xmlNodePtr root_node = xmlDocGetRootElement(doc);

  // walk the tree
  walk_tree(root_node);
}


const char *complex_xml()
{
  static std::string doc_str;
  std::stringstream strm;
  strm << "<application>"   << std::endl;
  strm << "   <db>"   << std::endl;
  strm << "   <login uid=\"math1\" pwd=\"0121\" server=\"MathServ\" db=\"math\" />" << std::endl;
  strm << "   <login uid=\"science1\" pwd=\"0121\" server=\"ScienceServ\" db=\"science\" />" << std::endl;
  strm << "   </db>"   << std::endl;
  strm << "   <parameters name=\"engine\" instance=\"static\" >"  << std::endl;
  strm << "    <engine model=\"linear\" version=\"1.1.1\" >"    << std::endl;
  strm << "       <start> 08:00:00 </start>" << std::endl;
  strm << "       <stop>  20:00:00 </stop>" << std::endl;
  strm << "    </engine>"    << std::endl;
  strm << "  </parameters>" << std::endl;
  strm << "</application>"  << std::endl;
  doc_str = strm.str();
  return doc_str.c_str();
}

int count_element_children( xmlNodePtr node )
{
  int n = 0;
  if ( !node || !node->children ) return n;
  xmlNodePtr child = node->children;
  for( xmlNodePtr curr = child ; curr ; curr = curr->next )
    if ( curr->type == XML_ELEMENT_NODE )
      n++;
  return n;
}


void detail_walk_tree( xmlNodePtr node )
{
  if (!node) return;
  for( xmlNodePtr curr = node ; curr ; curr = curr->next )
    {
      if ( curr->type == XML_ELEMENT_NODE )
	{
	  std::cout << "element_name=" << curr->name << ":";
	  
	  if( curr->parent == (xmlNodePtr)curr->doc )
	    std::cout << "root" << ":";
	  else
	    std::cout << "parent=" << curr->parent->name << ":";

	  int nchild = count_element_children(curr);
	  std::cout << ":children=" << nchild;
	  if ( curr->properties )
	    {
	      for(  xmlAttrPtr prop = curr->properties ; prop ; prop = prop->next )
		{
		  std::cout << ":attr_name=" << prop->name;
		  if ( prop->children )
		    {
		      std::cout << ":attr_value=" << prop->children->content;
		    }
		}
	    }
	  if ( curr->children && nchild == 0 )
	    {
	      std::cout << ":" << curr->children->content ;
	    }
	  std::cout << std::endl;
	}
      if ( curr->children )
	detail_walk_tree( curr->children );
    }
}


  
void example3()
{
  const char *text = complex_xml();

  // libxml2 function xmlReadMemory
  xmlDocPtr doc = xmlReadMemory( text , strlen(text) , "mytext.xml" , NULL , 0 );

  xmlNodePtr root_node = xmlDocGetRootElement(doc);

  xmlDocDump( stdout , doc );
  detail_walk_tree( root_node );
}


int main( int argc, char **argv )
{
  std::cout << "-------------starting example 1---------------" << std::endl;
  example1();
  std::cout << "-------------starting example 2---------------" << std::endl;
  example2();
  std::cout << "-------------starting example 3---------------" << std::endl;
  example3();
}
