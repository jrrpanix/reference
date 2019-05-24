#include <pion/net/HTTPResponseWriter.hpp>
#include <pion/net/WebService.hpp>
#include <pion/net/WebServer.hpp>
#include <pion/PionPlugin.hpp>
#include <sstream>

using namespace pion;
using namespace pion::net;

//--------------------------------------------------------------
// begin ShutdownManager.hpp
//
// Shutdown Manager - comes from pion-net included in this source
// so all the code is in one file, its a separate include
//
// pion-net: a C++ framework for building lightweight HTTP interfaces
// Copyright (C) 2007-2008 Atomic Labs, Inc.  (http://www.atomiclabs.com)
//
// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt
//
// #ifndef __PION_SHUTDOWNMANAGER_HEADER__
// #define __PION_SHUTDOWNMANAGER_HEADER__

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#ifndef PION_WIN32
#include <signal.h>
#endif


///
/// ShutdownManager: used to manage shutdown for the main thread
///
class ShutdownManager {
public:
  // default constructor & destructor
  ShutdownManager(void) : m_shutdown_now(false) {}
  ~ShutdownManager() {}

  /// signals the shutdown condition
  inline void shutdown(void) {
    boost::mutex::scoped_lock shutdown_lock(m_shutdown_mutex);
    m_shutdown_now = true;
    m_shutdown_cond.notify_all();
  }
       
  /// blocks until the shutdown condition has been signaled
  inline void wait(void) {
    boost::mutex::scoped_lock shutdown_lock(m_shutdown_mutex);
    while (! m_shutdown_now)
      m_shutdown_cond.wait(shutdown_lock);
  }
       
private:
  /// true if we should shutdown now
  bool                                    m_shutdown_now;
       
  /// used to protect the shutdown condition
  boost::mutex                    m_shutdown_mutex;
       
  /// triggered when it is time to shutdown
  boost::condition                m_shutdown_cond;
};

/// static shutdown manager instance used to control shutdown of main()
static ShutdownManager  main_shutdown_manager;


/// signal handlers that trigger the shutdown manager
#ifdef PION_WIN32
BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
  switch(ctrl_type) {
  case CTRL_C_EVENT:
  case CTRL_BREAK_EVENT:
  case CTRL_CLOSE_EVENT:
  case CTRL_SHUTDOWN_EVENT:
    main_shutdown_manager.shutdown();
    return TRUE;
  default:
    return FALSE;
  }
}
#else
void handle_signal(int sig)
{
  main_shutdown_manager.shutdown();
}
#endif
// #endif end ShutdownManager.hpp
//---------------------------------------------------------------

//--------------------------------------------------------------
// Hello Service
//--------------------------------------------------------------
class Hello : public WebService
{
public:
  virtual void operator()( HTTPRequestPtr &req , TCPConnectionPtr &tcp);
private:
  static std::string form_html();
};

std::string Hello::form_html()
{
  std::stringstream stream;
  stream << "<html>\n"
	 << "\t<body>\n" 
	 << "\t\t<div style=\"text-align:center;\">\n"
	 << "\t\t\t<p>\n"
	 << "\t\t\t\t</br>\n"
	 << "\t\t\t</p>\n"
	 << "\t\t\t<p>\n"
	 << "\t\t\t <a href=\"/goodbye\"> Hello Everyone!\n </a>"
	 << "\t\t\t</p>\n"
	 << "\t\t</div>\n"
	 << "\t</body>\n"
	 << "</html>" << std::endl;
  return stream.str();
}

void Hello::operator()( HTTPRequestPtr &req , TCPConnectionPtr &tcp)
{
  static const std::string HELLO_HTML = form_html();
  HTTPResponseWriterPtr writer( HTTPResponseWriter::create( tcp, *req , 
							    boost::bind( &TCPConnection::finish , tcp ) ) );
  writer->writeNoCopy( HELLO_HTML );
  writer->writeNoCopy( HTTPTypes::STRING_CRLF );
  writer->writeNoCopy( HTTPTypes::STRING_CRLF );
  writer->send();
}

//--------------------------------------------------------------
// GoodBye Service
//--------------------------------------------------------------
class GoodBye : public WebService
{
public:
  virtual void operator()( HTTPRequestPtr &req , TCPConnectionPtr &tcp);
private:
  static std::string form_html();
};

std::string GoodBye::form_html()
{
  std::stringstream stream;
  stream << "<html>\n"
	 << "\t<body>\n" 
	 << "\t\t<div style=\"text-align:center;\">\n"
	 << "\t\t\t<p>\n"
	 << "\t\t\t\t</br>\n"
	 << "\t\t\t</p>\n"
	 << "\t\t\t<p>\n"
	 << "\t\t\t<a href=\"/hello\"> GoodBye Everyone!\n </a>"
	 << "\t\t\t</p>\n"
	 << "\t\t</div>\n"
	 << "\t</body>\n"
	 << "</html>" << std::endl;
  return stream.str();
}

void GoodBye::operator()( HTTPRequestPtr &req , TCPConnectionPtr &tcp)
{
  static const std::string HELLO_HTML = form_html();
  HTTPResponseWriterPtr writer( HTTPResponseWriter::create( tcp, *req , 
							    boost::bind( &TCPConnection::finish , tcp ) ) );
  writer->writeNoCopy( HELLO_HTML );
  writer->writeNoCopy( HTTPTypes::STRING_CRLF );
  writer->writeNoCopy( HTTPTypes::STRING_CRLF );
  writer->send();
}

//--------------------------------------------------------------
// run services
//--------------------------------------------------------------
void run( void *arg=0 )
{
  static const unsigned int DEFAULT_PORT = 8080;
  int *portP = (int *)(arg);
  int port = portP ? *portP : DEFAULT_PORT;
  boost::asio::ip::tcp::endpoint cfg_endpoint(boost::asio::ip::tcp::v4(), port );
  WebServer  web_server(cfg_endpoint);

  WebService *hello = new Hello();
  WebService *goodbye = new GoodBye();

  web_server.addService( "/hello" , hello );
  web_server.addService( "/goodbye" , goodbye );
  web_server.start();
  main_shutdown_manager.wait();  
}

//--------------------------------------------------------------
// main
//--------------------------------------------------------------
int main( int argc , char **argv )
{
  // access url http://your-server-name:port/hello
  int port = argc > 1 ? atoi( argv[1] ) : 0;
  int *portP = port != 0 ? &port : 0 ;

  run( (void *) portP );
}
