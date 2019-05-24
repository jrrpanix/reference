
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xpath.h>
#include <libxml/xmlIO.h>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <cstring>
#include <boost/lexical_cast.hpp>

struct UST_XML
{
  long   date;
  double BC_1MONTH;
  double BC_3MONTH;
  double BC_6MONTH;
  double BC_1YEAR;
  double BC_2YEAR;
  double BC_3YEAR;
  double BC_5YEAR;
  double BC_7YEAR;
  double BC_10YEAR;
  double BC_20YEAR;
  double BC_30YEAR;
  double BC_30YEARDISPLAY;
  
};

struct UST_Cmp
{
  bool operator()( const UST_XML &x1 , const UST_XML &x2 ) { return x1.date < x2.date; }
};


std::ostream &operator<<( std::ostream &stream , const UST_XML &x )
{
  stream <<  x.date << ","
	 <<  std::setprecision(7) << std::fixed << x.BC_1MONTH << ","
	 <<  std::setprecision(7) << std::fixed << x.BC_3MONTH << ","
	 <<  std::setprecision(7) << std::fixed << x.BC_6MONTH << ","
	 <<  std::setprecision(7) << std::fixed << x.BC_1YEAR << ","
	 <<  std::setprecision(7) << std::fixed << x.BC_2YEAR << ","
	 <<  std::setprecision(7) << std::fixed << x.BC_3YEAR << ","
	 <<  std::setprecision(7) << std::fixed << x.BC_5YEAR << ","
	 <<  std::setprecision(7) << std::fixed << x.BC_7YEAR << ","
	 <<  std::setprecision(7) << std::fixed << x.BC_10YEAR << ","
	 <<  std::setprecision(7) << std::fixed << x.BC_20YEAR << ","
	 <<  std::setprecision(7) << std::fixed << x.BC_30YEAR << ","
	 <<  std::setprecision(7) << std::fixed << x.BC_30YEARDISPLAY ;
  return stream;
}

typedef double UST_XML::*UST_XML_DBL;
typedef std::vector<UST_XML> UST_XMLVec;

static long parse_date( const char *str )
{
  std::string s(str);

  int y = boost::lexical_cast<int>(s.substr(0,4));
  int m = boost::lexical_cast<int>(s.substr(5,2));
  int d = boost::lexical_cast<int>(s.substr(8,2));

  return y * 10000 + m*100 + d;
}


static void extract_data( xmlNodePtr node , UST_XML &ust )
{
  static struct MAP_S
  {
    xmlChar          *xml_str;
    UST_XML_DBL      field;
  } _mapS [] = 
      {
	{ (xmlChar *)"BC_1MONTH", &UST_XML::BC_1MONTH },
	{ (xmlChar *)"BC_3MONTH", &UST_XML::BC_3MONTH },
	{ (xmlChar *)"BC_6MONTH", &UST_XML::BC_6MONTH } ,
	{ (xmlChar *)"BC_1YEAR", &UST_XML::BC_1YEAR } ,
	{ (xmlChar *)"BC_2YEAR", &UST_XML::BC_2YEAR } ,
	{ (xmlChar *)"BC_3YEAR", &UST_XML::BC_3YEAR } ,
	{ (xmlChar *)"BC_5YEAR", &UST_XML::BC_5YEAR } ,
	{ (xmlChar *)"BC_7YEAR", &UST_XML::BC_7YEAR } ,
	{ (xmlChar *)"BC_10YEAR", &UST_XML::BC_10YEAR } ,
	{ (xmlChar *)"BC_20YEAR", &UST_XML::BC_20YEAR } ,
	{ (xmlChar *)"BC_30YEAR", &UST_XML::BC_30YEAR } ,
	{ (xmlChar *)"BC_30YEARDISPLAY", &UST_XML::BC_30YEARDISPLAY } ,
	{ (xmlChar *)0 , 0 }
      };

  if ( !node || !node->name )
    return ;

  if ( xmlStrEqual( (const xmlChar *)"NEW_DATE" , node->name ) )
    {
      if( node->children && node->children->content )
	{
	  ust.date = parse_date( (char *)node->children->content );
	}
      return;
    }

  for( int i = 0 ; _mapS[i].xml_str  ; i++ )
    if ( xmlStrEqual( _mapS[i].xml_str, node->name ) )
      {
	if( node->children && node->children->content )
	  {
	    double x = boost::lexical_cast<double>((char *)node->children->content);
	    ust.*(_mapS[i].field)=x;
	  }
      }
}


static xmlNodePtr find_child_node( xmlNodePtr node , const xmlChar *name )
{
  if ( !node ) return node;
  xmlNodePtr child = node->children;
  for( xmlNodePtr curr = child ; curr ; curr = curr->next )
    if ( curr->type == XML_ELEMENT_NODE && xmlStrEqual( curr->name , name ) )
      return curr;
  return 0;
}

static void parse_tsy( xmlNodePtr node )
{
  if ( !node || !node->type == XML_ELEMENT_NODE || !xmlStrEqual( node->name , (const xmlChar *)"feed" )  )
    return ;
  UST_XMLVec V;
  for( xmlNodePtr curr = node->children ; curr ; curr = curr->next )
    if ( curr->type == XML_ELEMENT_NODE && xmlStrEqual( curr->name , (const xmlChar *)"entry" ) )
      {
	xmlNodePtr content_node = find_child_node( curr , (const xmlChar *) "content" );
	if ( content_node )
	  {
	    xmlNodePtr property_node = find_child_node( content_node , (const xmlChar *)"properties" );
	    if ( property_node )
	      {
		// put the contents into UST_XML struct
		UST_XML ust; 
		for( xmlNodePtr pcurr = property_node->children ; pcurr ; pcurr = pcurr->next )
		  if ( pcurr && pcurr->name ) extract_data( pcurr , ust );
		V.push_back( ust );
	      }
	  }
      }
  std::sort(V.begin(),V.end(),UST_Cmp());
  std::copy( V.begin(),V.end() , std::ostream_iterator<UST_XML>( std::cout , "\n" ));
}

int main( int argc , char **argv )
{
  if ( argc < 2 )
    {
      std::cerr << "usage " << argv[0] << " file " << std::endl;
      return -1;
    }
  const char *file = argv[1]; 
  xmlDocPtr doc = xmlReadFile( file , file , 0 );
  parse_tsy( xmlDocGetRootElement(doc) );
  return 0;
}
