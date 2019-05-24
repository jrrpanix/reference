
#include <mysql/mysql.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

struct Location
{
  Location() : person_id(0),tracker_id(0),date(0),time(0),latitude(0), longitude(0) {}

  int    persion_id;
  int    tracker_id;
  int    date;
  int    time;
  double latitude;
  double longitude;
};
typedef boost::shared_ptr<Location> LocationPtr;
typedef std::vector<LocationPtr>    LocationVec;

std::ostream  &operator<<( std::ostream &stream , const LocationPtr &loc )
{
  stream << loc->person_id
	 << ","
	 << loc->tracker_id
	 << ","
	 << loc->date
	 << ","
	 << loc->time
	 << ","
	 << loc->latitude
	 << ","
	 << loc->longitude;
  return stream;
}

MYSQL *get_connection( const std::string &Uid , const std::string &Pwd , const std::string &Db)
{

  MYSQL *conn = mysql_init(NULL);
  const char *host = "localhost";
  const char *user = Uid.c_str();
  const char *pwd  = Pwd.c_str();
  const char *db   = Db.c_str();

  if ( mysql_real_connect( conn , host , user, pwd , db , 0 , NULL , 0 ) == NULL )
    {
      std::cout << mysql_error(conn) << std::endl;
      return 0;
    }
  return conn;
}

int get_locations( MYSQL *conn , int person_id , LocationVec &locV )
{
  std::stringstream strm;
  strm << "select tracker_id,date,time,latitude,longitude "
       << "from tracking_table where "
       << "person_id=" << person_id;
  int rv = mysql_query( conn , strm.str().c_str());
  if ( rv )
    {
      std::cerr << mysql_error(conn) << std::endl;
      return rv;
    }
  MYSQL_RES *result = mysql_store_result(conn );
  MYSQL_ROW row;
  while ( ( row = mysql_fetch_row( result ) ) )
    {
      size_t i = 0;
      LocationPtr loc( new Location() );
      loc->person_id = person_id;
      loc->tracker_id = boost::lexical_cast<int>(row[i++]);
      loc->date = boost::lexical_cast<int>(row[i++]);
      loc->time = boost::lexical_cast<int>(row[i++]);
      loc->latitude = boost::lexical_cast<double>(row[i++]);
      loc->longitude = boost::lexical_cast<double>(row[i++]);
      locV.push_back(loc);
    }
  return rv;
}


int main ( int argc , char **argv )
{
  if ( argc != 5 )
    {
      std::cerr << "usage " << argv[0] << " <person_id> <uid> <pwd> <db>" << std::endl;
      return -1;
    }
  int person_id = boost::lexical_cast<int>(argv[1]);
  std::string uid = argv[2];
  std::string pwd = argv[3];
  std::string db = argv[4];
  MYSQL *conn = get_connection(uid,pwd,db);
  if ( !conn )
    {
      return -1;
    }
  LocationVec locV;
  get_locations( conn , person_id , locV );
  for( size_t i = 0; i < locV.size() ; i++ ) std::cout << locV[i] << std::endl;
  mysql_close(conn);
  return 0;
}
