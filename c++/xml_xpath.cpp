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

const char *sample_complex_xml()
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
  strm << "       <ip>  10.0.1.8 </ip>" << std::endl;
  strm << "       <port>  2500 </port>" << std::endl;
  strm << "    </engine>"    << std::endl;
  strm << "    <log mode=\"async\"  >"    << std::endl;
  strm << "       <file> /home/log/log_file </file>" << std::endl;
  strm << "    </log>"    << std::endl;
  strm << "  </parameters>" << std::endl;
  strm << "</application>"  << std::endl;
  doc_str = strm.str();
  return doc_str.c_str();
}

//---------------------------------------------------------------------
// XML Convience Functions
//---------------------------------------------------------------------
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

bool is_root_node( xmlNodePtr node )
{
  return node && node->parent == (xmlNodePtr)node->doc;
}

void get_attributes ( xmlNodePtr node , std::map<std::string,std::string> &attr_map )
{
  attr_map.clear();
  if ( !node->properties ) return;
  for(  xmlAttrPtr prop = node->properties ; prop ; prop = prop->next )
    if ( prop->children ) attr_map[ (char *)prop->name ] = (char *)prop->children->content;
}

void print_attr_map( std::ostream &stream ,const std::map<std::string,std::string> &attr_map )
{
  for( std::map<std::string,std::string>::const_iterator i = attr_map.begin() ; i != attr_map.end() ; ++i )
    stream << (*i).first << ":" <<  (*i).second << std::endl;
}

xmlNodePtr get_node( xmlDocPtr doc , xmlChar *path )
{
  xmlXPathContextPtr xpath_context = xmlXPathNewContext(doc);
  xmlXPathObjectPtr  xpath_obj = xmlXPathEvalExpression( path , xpath_context);
  return (xpath_obj && !xmlXPathNodeSetIsEmpty( xpath_obj->nodesetval)) ?xpath_obj->nodesetval->nodeTab[0] : 0 ;
}

bool is_empty_text_node( xmlNodePtr node )
{
  return node && node->type == XML_TEXT_NODE && node->content && node->content[0] == (const xmlChar)'\n' ;
}

std::string construct_path( xmlNodePtr node )
{
  std::string path;
  if ( !node ) return path;
  std::string tmp;
  xmlNodePtr curr = node->parent , first = curr ;
  while( curr && curr->name )
    {
      tmp = "/";
      tmp += (char *)curr->name;
      if ( curr == first )
	path = tmp ;
      else
	path = tmp + path;
      curr = curr->parent;
    }
  return path;
}


void xpath_print( xmlDocPtr doc , xmlChar *path )
{
  xmlXPathContextPtr xpath_context = xmlXPathNewContext(doc);
  xmlXPathObjectPtr  xpath_obj = xmlXPathEvalExpression( path , xpath_context);
  int nodes = ( !xpath_obj ||  xmlXPathNodeSetIsEmpty( xpath_obj->nodesetval) ) ? 0 : 
    xpath_obj->nodesetval->nodeNr;

  std::cout << "xpath=" << (char *) path << ":nodes=" << nodes ;

  if ( nodes == 0 ) 
    {
      std::cout << std::endl;
      return;
    }
  xmlNodeSetPtr nodeset = xpath_obj->nodesetval;
  for( int i = 0; i < nodeset->nodeNr; i++ )
    {
      xmlNodePtr node = nodeset->nodeTab[i];
      xmlChar *key = xmlNodeListGetString( doc ,node->xmlChildrenNode , 1 );
      const char *ks = key ? (const char *)key : "" ;
      const char *key_str = (ks && strlen(ks) > 0 && (int)ks[0] == 10 ) ? "" : ks;
      std::cout << ":node" << i+ 1
		<< " name=" << (char *) node->name 
		<< " type=" << node->type
		<< " key=" << key_str;
    }
  std::cout << std::endl;
}

void xpath_example()
{
  const char *text = sample_complex_xml();

  xmlDocPtr doc = xmlReadMemory( text, strlen(text), "mytext.xml" , NULL ,0 );

  xmlDocDump( stdout , doc );

  xpath_print( doc , (xmlChar *)"//application" );
  xpath_print( doc , (xmlChar *)"//application/parameters" );
  xpath_print( doc , (xmlChar *)"//application/parameters/engine/ip" );
  xpath_print( doc , (xmlChar *)"//application/*/*/*[(name()='port')]" );
  xpath_print( doc , (xmlChar *)"//*/db" );
  xpath_print( doc , (xmlChar *)"//*/db/*" );
  xpath_print( doc , (xmlChar *)"//*/log/*" );
  xpath_print( doc , (xmlChar *)"//*/stop/*" );
}

void attribute_example()
{
  const char *text = sample_complex_xml();

  xmlDocPtr doc = xmlReadMemory( text, strlen(text), "mytext.xml" , NULL ,0 );

  std::map<std::string,std::string> attr_map;
  
  get_attributes( get_node( doc, (xmlChar *)"//*/db/login" ) , attr_map );
  std::cout << "attribute map for " << (char *)"//*/db/login" << std::endl;
  print_attr_map( std::cout , attr_map );

  get_attributes( get_node( doc, (xmlChar *)"//*/parameters" ) , attr_map );
  std::cout << "attribute map for " << (char *)"//*/parameters" << std::endl;
  print_attr_map( std::cout , attr_map );
		  
  get_attributes( get_node( doc, (xmlChar *)"//*/parameters/engine" ) , attr_map );
  std::cout << "attribute map for " << (char *)"//*/parameters/engine" << std::endl;
  print_attr_map( std::cout , attr_map );


}


int main( int , char ** )
{
  std::cout << "-----------------  xpath examples      -------------------" << std::endl;
  xpath_example();
  std::cout << "----------------  attribute examples  -------------------" << std::endl;
  attribute_example();
}

