/*
  libqUtilities - Diverse Qt-classes and coserver base
  
  $Id$

  Copyright (C) 2006 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef _miTimeSpinBox_h
#define _miTimeSpinBox_h

/* Created by met.no/PU 
   at Mon Mar 31 08:03:45 2003 */

#include <puTools/miTime>
#include <puTools/miDate>
#include <puTools/miString>

#include <qspinbox.h>
#include <qlayout.h>
#include <qlabel.h>

using namespace std; 

class miSpinBox : public QSpinBox {
  Q_OBJECT
public:
  miSpinBox( int mi, int ma, int st=1, QWidget* p=0, const char* n=0 );

public slots:
  void stepUp();
  void stepDown(); 

signals:
  void newValue(int);
};


class miTimeSpinBox : public QWidget {
  Q_OBJECT
public:
  typedef enum DisplayUntil { SECOND, MINUTE, HOUR, DAY };
  typedef enum Dayname      { NONAME, NOR, ENG          };

private:
  miSpinBox * year;
  miSpinBox * month;
  miSpinBox * day;
  miSpinBox * hour;
  miSpinBox * minute;
  miSpinBox * second;

  QLabel    * dayname;

  miutil::miTime ref;
  miutil::miTime max;
  miutil::miTime min;

  int yy,mm,dd,h,m,s;

  miutil::miDate::lang daynamelang;
    
  int weekday;
  int maxday;

  void newYear(int);
  void newMonth(int);
  void newDay(int);
  void newHour(int);
  void newMinute(int);
  void newSecond(int);
  void checkMaxThisMonth();
  void resetWeekdayName();

public:
  miTimeSpinBox(const char *name,QWidget *parent, miutil::miString title="", 
		miTimeSpinBox::Dayname      = miTimeSpinBox::NONAME,
		miTimeSpinBox::DisplayUntil = miTimeSpinBox::MINUTE );

  miutil::miTime   time()    const { return ref;          }
  miutil::miString isoTime() const { return ref.isoTime();}
  miutil::miDate   date()    const { return ref.date();   }
  miutil::miString isoDate() const { return ref.isoDate();}
 
  
  bool checkMax();
  bool checkMin();


public slots:
  void setTime(const miutil::miTime&);
  void setDate(const miutil::miDate&);
  void changeTime();

  void changeYear(int);
  void changeMonth(int);
  void changeDay(int);
  void changeHour(int);
  void changeMinute(int);
  void changeSecond(int);
  
  void setMax(const miutil::miTime&);
  void setMin(const miutil::miTime&);
  
signals:
  void valueChanged( const miutil::miTime& );
  void valueChanged( const miutil::miDate& );
};

#endif
