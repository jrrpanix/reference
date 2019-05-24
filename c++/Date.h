#ifndef _calc_Date_h
#define _calc_Date_h

#include <iostream>
#include <iomanip>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/shared_ptr.hpp>

namespace calc
{
  typedef boost::gregorian::date          DateT;
  typedef boost::gregorian::date_duration DayT;
  typedef boost::gregorian::months        MonthT;
  typedef boost::gregorian::years         YearT;

  class Days
  {
  public:
  explicit Days( int d ) : d_(d) {}
  Days() : d_(0){}

  public:
    DayT days() const { return d_; }
    int  as_int() const { return d_.days(); }

  private:
    DayT d_;
  };

  class Months
  {
  public:
  Months() : m_(0) {}
  explicit Months( int m ) : m_(m) {}

  public:
    MonthT months() const { return m_; }
    int    as_int() const { return m_.number_of_months().as_number(); }

  private:
    MonthT m_;
  };

  class Years
  {
  public:
  Years() : y_(0) {}
  explicit Years( int y ) : y_(y) {}
    
  public:
    YearT years() const { return y_; }
    int   as_int() const { return y_.number_of_years().as_number(); }

  private:
    YearT y_;
  };

  class Date
  {
  public:
  Date():d_(0) {}
  Date( const DateT &d ) : d_(d) {}
  Date( int y , int m , int d ) : d_(y,m,d) {}

  public:
    
    int year()  const { return d_.year() ; }
    int month() const { return d_.month(); }
    int day()   const { return d_.day(); }
    int day_of_week() const { return d_.day_of_week(); }
    int week_number() const { return d_.week_number(); }

    long julian_day() const { return d_.julian_day(); }
    Date end_of_month() const { return d_.end_of_month(); }
    bool is_leap_year() const { int y=year(); return (!(y % 4))  && ((y % 100) || (!(y % 400))); }
    bool is_end_of_month() const
    {
      int d = day();
      if ( d < 28 ) return false;
      int m = month();
      switch ( m ) 
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12: return d==31;
	case 4:
	case 6:
	case 9:
	case 11: return d==30;
	case 2: return is_leap_year() ? d==29 : d==28;
	}
    }
	   
    Date &operator+= ( const Days &x ) { d_ += x.days(); return *this; }
    Date &operator+= ( const Months &x ) { d_ += x.months() ; return *this; }
    Date &operator+= ( const Years &x ) { d_ += x.years() ; return *this ; }

    Date &operator-= ( const Days &x ) { d_ -= x.days(); return *this; }
    Date &operator-= ( const Months &x ) { d_ -= x.months() ; return *this; }
    Date &operator-= ( const Years &x ) { d_ -= x.years() ; return *this ; }

    long as_yyyymmdd() const { return year() * 10000 + month()* 100 + day(); }

    static long days_diff( const Date &d1 , const Date &d2 ) { return d1.julian_day() - d2.julian_day(); }


    Date operator+( const Days &x ) const { DateT d(d_); d += x.days(); return Date(d); }
    Date operator+( const Months &x ) const { DateT d(d_); d += x.months(); return Date(d); }
    Date operator+( const Years &x ) const { DateT d(d_); d += x.years(); return Date(d); }

    Date operator-( const Days &x ) const { DateT d(d_); d -= x.days(); return Date(d); }
    Date operator-( const Months &x ) const { DateT d(d_); d -= x.months(); return Date(d); }
    Date operator-( const Years &x ) const { DateT d(d_); d -= x.years(); return Date(d); }

    bool operator==( const Date &d1 ) const { return d_ == d1.d_; }
    bool operator!=( const Date &d1 ) const { return d_ != d1.d_; }
    bool operator<( const Date &d1 ) const { return d_ < d1.d_; }
    bool operator<=( const Date &d1 ) const { return d_ <= d1.d_; }
    bool operator>( const Date &d1 ) const { return d_ > d1.d_; }
    bool operator>=( const Date &d1 ) const { return d_ >= d1.d_; }

  private:
    DateT d_;

  };

  inline std::ostream &operator<<( std::ostream &stream , const Date &x )
  {
    stream << std::setw(2) << std::setfill('0') << x.month()
	   << "/"
	   << std::setw(2) << std::setfill('0') << x.day()
	   << "/"
	   << std::setw(4) << std::setfill('0') << x.year()
	   << std::setfill( ' ' );
    return stream;
  }

  typedef std::vector<Date> DateVec;
  typedef boost::shared_ptr<DateVec> DateVecPtr;


}



#endif
