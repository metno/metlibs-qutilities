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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <miTimeSpinBox.h>

#include <QFrame>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

using namespace miutil;

miSpinBox::miSpinBox( int mi, int ma, int st, QWidget* p, const char* n )
    : QSpinBox(p)
{
	setRange(mi, ma);
	setSingleStep(st);
	lineEdit()->setReadOnly(true);
}

// MITIMESPINBOX


miTimeSpinBox::miTimeSpinBox( const char* name,QWidget* parent,
			      QString title,
			      miTimeSpinBox::Dayname dname,
			      miTimeSpinBox::DisplayUntil displayuntil)
  : QWidget(parent)
{
  min = QDateTime(QDate(1900,1,1));
  max = QDateTime::currentDateTime();


  daynamelang = ( dname == NOR ? miutil::miDate::Norwegian :
		  miutil::miDate::English );


  QVBoxLayout* topvl = new QVBoxLayout(this);
  topvl->setObjectName("topvl");
  QFrame*      frame = new QFrame(this);
  frame->setObjectName("mframe");
  frame->setFrameStyle( QFrame::Sunken | QFrame::Panel );
  topvl->addWidget( frame, 0 );

  QHBoxLayout* hlayout = new QHBoxLayout(frame);
  hlayout->setObjectName("mainlayout");


  dayname     = NULL;
  second      = NULL;
  minute      = NULL;
  hour        = NULL;
  QLabel* tit = NULL;

  weekday = -1;



  if(!title.isEmpty()) {
    tit = new QLabel(title,frame);
    tit->setObjectName("name");
    hlayout->addWidget(tit, 4);
  }

  if(dname != NONAME) {
    dayname = new QLabel("    ",frame);
    dayname->setObjectName("weekday");
    hlayout->addWidget(dayname, 4);
  }

  hlayout->addStretch();

  year     = new miSpinBox(0 ,3000,1,frame,"spinyear" );
  month    = new miSpinBox(0 ,13  ,1,frame,"spinmonth");
  day      = new miSpinBox(0 ,32  ,1,frame,"spinday"  );


  connect(year,   SIGNAL(valueChanged(QString)),this,SLOT(changeYear(QString)  ));
  connect(month,  SIGNAL(valueChanged(QString)),this,SLOT(changeMonth(QString) ));
  connect(day,    SIGNAL(valueChanged(QString)),this,SLOT(changeDay(QString)   ));

  hlayout->addWidget(year,  2);
  hlayout->addWidget(month, 1);
  hlayout->addWidget(day,   1);





  if(displayuntil <= HOUR ) {
    hour          = new miSpinBox(-1,24  ,1,frame,"spinhour" );
    QLabel* delim = new QLabel(" : ",frame);
    delim->setObjectName("delimiter");

    connect(hour,   SIGNAL(valueChanged(QString)),this,SLOT(changeHour(QString)  ));

    hlayout->addWidget(delim, 1);
    hlayout->addWidget(hour,  1);
  }

  if(displayuntil <= MINUTE) {
    minute = new miSpinBox(-1,60  ,1,frame,"spinmin"  );

    connect(minute, SIGNAL(valueChanged(QString)),this,SLOT(changeMinute(QString)));

    hlayout->addWidget(minute,1);
  }


  if(displayuntil == SECOND) {
    second = new miSpinBox(-1,60  ,1,frame,"spinsec"  );

    connect(second, SIGNAL(valueChanged(QString)),this,SLOT(changeSecond(QString)));

    hlayout->addWidget(second,1);
  }

  hlayout->setSizeConstraint(QLayout::SetFixedSize);

  setTime( max  );

  resetWeekdayName();
  checkMaxThisMonth();
}

void miTimeSpinBox::setDate(const QDate& d)
{
  yy = d.year();
  mm = d.month();
  dd = d.day();
  setTime(QDateTime(QDate(yy,mm,dd)));
}

void miTimeSpinBox::setTime(const QDateTime& t)
{
  yy = t.date().year();
  mm = t.date().month();
  dd = t.date().day();
  h  = 0;
  m  = 0;
  s  = 0;

  year->blockSignals(true);
  year->setValue(  yy );
  year->blockSignals(false);

  month->blockSignals(true);
  month->setValue( mm );
  month->blockSignals(false);

  day->blockSignals(true);
  day->setValue(   dd );
  day->blockSignals(false);

  if(hour) {
    h = t.time().hour();
    hour->blockSignals(true);
    hour->setValue(h);
    hour->blockSignals(false);
  }

  if(minute) {
    m = t.time().minute();
    minute->blockSignals(true);
    minute->setValue(m);
    minute->blockSignals(false);
  }

  if(second) {
    s = t.time().second();
    second->blockSignals(true);
    second->setValue(s);
    second->blockSignals(false);
  }


  ref  = t;
  checkMaxThisMonth();
}

void miTimeSpinBox::setMax(const QDateTime& mx)
{
  max = mx;
  if(checkMax()) {
    setTime(max);
    emit valueChanged(ref);
  }
}

bool miTimeSpinBox::checkMax()
{
   return (ref > max);
}

void miTimeSpinBox::checkMaxThisMonth()
{
  miutil::miDate tmp(yy,mm,1);
  maxday=tmp.daysInMonth();

  if(dd > maxday)
    dd=maxday;
}


void miTimeSpinBox::setMin(const QDateTime& mx)
{
  min = mx;
  if(checkMin()) {
    setTime(min);
    emit valueChanged(ref);
  }
}

bool miTimeSpinBox::checkMin()
{
  return (ref < min);
}

// SECOND

void miTimeSpinBox::changeSecond(QString v)
{
	bool ok;
  newSecond(v.toInt(&ok, 10));
  changeTime();
}

void miTimeSpinBox::newSecond(int v )
{
  s=v;

  if(s>59) {
    s=1;
    newMinute(++m);
  }
  if(s < 1) {
    s=59;
    newMinute(--m);
  }
}

// MINUTE

void miTimeSpinBox::changeMinute(QString v)
{
	bool ok;
  newMinute(v.toInt(&ok, 10));
  changeTime();
}

void miTimeSpinBox::newMinute(int v )
{
  m=v;

  if(m>59) {
    m=0;
    newHour(++h);
  }
  if(m < 0) {
    m=59;
    newHour(--h);
  }
}

// HOUR

void miTimeSpinBox::changeHour(QString v)
{
	bool ok;
  newHour(v.toInt(&ok, 10));
  changeTime();
}

void miTimeSpinBox::newHour(int v )
{
  h=v;

  if(h>23) {
    h=0;
    newDay(++dd);
  }
  if(h < 0) {
    h=23;
    newDay(--dd);
  }
}

// DAY

void miTimeSpinBox::changeDay(QString v)
{
	bool ok;
  newDay(v.toInt(&ok, 10));
  changeTime();
}

void miTimeSpinBox::newDay( int v)
{
 dd = v;

 if(dd > maxday) {
   dd=1;
   newMonth(++mm);
 }
 else if ( dd < 1) {
   dd=31;
   newMonth(--mm);
 }
}

// MONTH

void miTimeSpinBox::changeMonth(QString v)
{
	bool ok;
  newMonth(v.toInt(&ok, 10));
  changeTime();
}

void miTimeSpinBox::newMonth(int v)
{
  mm = v;
  if(mm > 12){
    yy++;
    mm=1;
  }
  else if ( mm < 1) {
    yy--;
    mm=12;
  }
  checkMaxThisMonth();
}


// YEAR

void miTimeSpinBox::changeYear(QString v )
{
	bool ok;
  yy = v.toInt(&ok, 10);
  checkMaxThisMonth();
  changeTime();
}

// TIME

void miTimeSpinBox::changeTime()
{
  ref.setDate(QDate(yy,mm,dd));
  ref.setTime(QTime(h,m,s));

  if(checkMax())
    ref = max;

  if ( checkMin() )
    ref = min;

  setTime(ref);
  resetWeekdayName();

  emit valueChanged(ref);
  emit valueChanged(ref.date());
}


// WEEKDAY


void miTimeSpinBox::resetWeekdayName()
{
  if(!dayname)
    return;

  int tmp = ref.date().dayOfWeek();

  if(tmp == weekday)
    return;

  weekday=tmp;

  //QString t = ref.format("%a ",daynamelang );
  QString t = mapDayOfWeekToShortDayName(ref.date().dayOfWeek(), daynamelang);
  dayname->setText(t);

  if(weekday == 0 || weekday == 6) {
  	QPalette qp;
  	qp.setColor(QPalette::WindowText, Qt::red);
    dayname->setPalette(qp);
  } else {
  	QPalette qp;
  	qp.setColor(QPalette::WindowText, Qt::black);
    dayname->setPalette(qp);
  }
}

QString miTimeSpinBox::mapDayOfWeekToShortDayName(const int dayOfWeek, const miutil::miDate::lang l) const
{
  QString language = l== miDate::Norwegian ? QString("no") : QString(miDate::getDefaultLanguage().cStr());

  QString la = language.toLower();

  int a=dayOfWeek - 1; // as QDate counts 1..7

  static QString nameEN[]={ "Sun",
			     "Mon",
			     "Tue",
			     "Wed",
			     "Thu",
			     "Fri",
			     "Sat" };

  static QString nameNO[]={ "Søn",
			     "Man",
			     "Tir",
			     "Ons",
			     "Tor",
			     "Fre",
			     "Lør" };

  static QString nameNN[]={ "Søn",
			     "Mån",
			     "Tys",
			     "Ons",
			     "Tor",
			     "Fre",
			     "Lau" };

  static QString nameDE[]={ "So",
			     "Mo",
			     "Di",
			     "Mi",
			     "Do",
			     "Fr",
			     "Sa" };

  if (la=="no" || la=="nb")
    return nameNO[a];
  if (la=="nn")
    return nameNN[a];
  if (la=="de")
    return nameDE[a];

  return nameEN[a];
}
