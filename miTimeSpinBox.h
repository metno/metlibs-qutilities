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

//#include <puTools/miTime.h>
#include <puTools/miDate.h>

#include <QString>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QSpinBox>
#include <QLayout>
#include <QLabel>

class miSpinBox : public QSpinBox {
  Q_OBJECT

public:
	/**
	 * Constructor.
	 * @deprecated n is deprecated after porting to Qt4.
	 */
  miSpinBox( int mi, int ma, int st=1, QWidget* p=0, const char* n=0 );
};


class miTimeSpinBox : public QWidget {
  Q_OBJECT
public:
  enum DisplayUntil { SECOND, MINUTE, HOUR, DAY };
  enum Dayname      { NONAME, NOR, ENG          };

private:
  miSpinBox * year;
  miSpinBox * month;
  miSpinBox * day;
  miSpinBox * hour;
  miSpinBox * minute;
  miSpinBox * second;

  QLabel    * dayname;

  QDateTime ref;
  QDateTime max;
  QDateTime min;

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
  QString mapDayOfWeekToShortDayName(const int dayOfWeek, const miutil::miDate::lang language) const;

public:
	/**
	 * Constructor.
	 * @deprecated title is no longer in use.
	 */
  miTimeSpinBox(const char *name,QWidget *parent, QString title="",
		miTimeSpinBox::Dayname      = miTimeSpinBox::NONAME,
		miTimeSpinBox::DisplayUntil = miTimeSpinBox::MINUTE );

  QDateTime   time()    const { return ref;          }
  QString isoTime() const { return ref.time().toString(Qt::ISODate);}
  QDate   date()    const { return ref.date();   }
  QString isoDate() const { return ref.date().toString(Qt::ISODate);}


  bool checkMax();
  bool checkMin();


public slots:
  void setTime(const QDateTime&);
  void setDate(const QDate&);
  void changeTime();

  void changeYear(QString v);
  void changeMonth(QString v);
  void changeDay(QString v);
  void changeHour(QString v);
  void changeMinute(QString v);
  void changeSecond(QString v);

  void setMax(const QDateTime&);
  void setMin(const QDateTime&);

signals:
  void valueChanged( const QDateTime& );
  void valueChanged( const QDate& );
};

#endif
