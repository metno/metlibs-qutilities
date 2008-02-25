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


#include <miTimeSpinBox.h>

#include <QFrame>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

miSpinBox::miSpinBox( int mi, int ma, int st, QWidget* p, const char* n )
    : QAbstractSpinBox(p) 
{
	max = ma;
	min = mi;
	
	lineEdit()->setReadOnly(true);
}

void miSpinBox::stepBy(int steps)
{
	//value += steps;
  emit newValue( value + steps );
}

void miSpinBox::setValue(int value)
{
	cerr << "setting value of a spinbox to " << value << endl;
	cerr << "________________________________________" << endl; ///< DEBUG
  this->value = value;
}

// MITIMESPINBOX


miTimeSpinBox::miTimeSpinBox( const char* name,QWidget* parent,
			      miutil::miString title,
			      miTimeSpinBox::Dayname dname, 
			      miTimeSpinBox::DisplayUntil displayuntil)
  : QWidget(parent,name)
{ 
  min=miutil::miTime(1900,1,1,0,0,0);
  max=miutil::miTime::nowTime();

  
  daynamelang = ( dname == NOR ? miutil::miDate::Norwegian : 
		  miutil::miDate::English );
  

  QVBoxLayout* topvl = new QVBoxLayout(this,1,1, "topvl");
  QFrame*      frame = new QFrame( this, "mframe" );
  frame->setFrameStyle( QFrame::Sunken | QFrame::Panel );
  topvl->addWidget( frame, 0 );

  QHBoxLayout* hlayout = new QHBoxLayout(frame, 2, 2, "mainlayout");
   

  dayname     = NULL;
  second      = NULL;
  minute      = NULL;
  hour        = NULL;
  QLabel* tit = NULL;
  
  weekday = -1;



  if(title.exists()) {
    tit = new QLabel(title.cStr(),frame,"name");
    hlayout->addWidget(tit, 4);
  }

  if(dname != NONAME) {
    dayname = new QLabel("    ",frame,"weekday");
    hlayout->addWidget(dayname, 4);
  }

  hlayout->addStretch();

  year     = new miSpinBox(0 ,3000,1,frame,"spinyear" );
  month    = new miSpinBox(0 ,13  ,1,frame,"spinmonth");
  day      = new miSpinBox(0 ,32  ,1,frame,"spinday"  );
  
 
  connect(year,   SIGNAL(newValue(int)),this,SLOT(changeYear(int)  ));
  connect(month,  SIGNAL(newValue(int)),this,SLOT(changeMonth(int) ));
  connect(day,    SIGNAL(newValue(int)),this,SLOT(changeDay(int)   ));

  hlayout->addWidget(year,  2);
  hlayout->addWidget(month, 1);
  hlayout->addWidget(day,   1);





  if(displayuntil <= HOUR ) {
    hour          = new miSpinBox(-1,24  ,1,frame,"spinhour" );
    QLabel* delim = new QLabel(" : ",frame,"delimiter");

    connect(hour,   SIGNAL(newValue(int)),this,SLOT(changeHour(int)  ));

    hlayout->addWidget(delim, 1);
    hlayout->addWidget(hour,  1);
  }

  if(displayuntil <= MINUTE) {
    minute = new miSpinBox(-1,60  ,1,frame,"spinmin"  );

    connect(minute, SIGNAL(newValue(int)),this,SLOT(changeMinute(int)));

    hlayout->addWidget(minute,1);
  }

  
  if(displayuntil == SECOND) {
    second = new miSpinBox(-1,60  ,1,frame,"spinsec"  );

    connect(second, SIGNAL(newValue(int)),this,SLOT(changeSecond(int)));

    hlayout->addWidget(second,1);
  }
      
  hlayout->setResizeMode(QLayout::SetFixedSize);
  
  setTime( max  );
  cerr << "???????????????" << endl; ///< DEBUG

  resetWeekdayName();
  checkMaxThisMonth();
}

void miTimeSpinBox::setDate(const miutil::miDate& d)
{
  yy = d.year();
  mm = d.month();
  dd = d.day();  
  setTime(miutil::miTime(yy,mm,dd,0,0,0));
}

void miTimeSpinBox::setTime(const miutil::miTime& t)
{
  yy = t.year();
  mm = t.month();
  dd = t.day();  
  h  = 0;
  m  = 0;
  s  = 0;

  year->setValue(  yy );  
  month->setValue( mm );
  day->setValue(   dd );
  
  if(hour) {
    h = t.hour(); 
    hour->setValue(h);
  }

  if(minute) {
    m  = t.min();  
    minute->setValue(m);
  }

  if(second) {
    s=t.sec();
    second->setValue(s);
  }


  ref  = t;
  checkMaxThisMonth();
}

void miTimeSpinBox::setMax(const miutil::miTime& mx)
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


void miTimeSpinBox::setMin(const miutil::miTime& mx)
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

void miTimeSpinBox::changeSecond(int v)
{
  newSecond(v);
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

void miTimeSpinBox::changeMinute(int v)
{
  newMinute(v);
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

void miTimeSpinBox::changeHour(int v)
{
  newHour(v);
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


void miTimeSpinBox::changeDay(int v)
{
  newDay(v);
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

void miTimeSpinBox::changeMonth(int v)
{
  newMonth(v);
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

void miTimeSpinBox::changeYear(int v )
{
  yy = v;
  checkMaxThisMonth();
  changeTime();
}


// TIME

void miTimeSpinBox::changeTime()
{
  ref.setTime(yy,mm,dd,h,m,s);

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

  int tmp = ref.dayOfWeek();

  if(tmp == weekday)
    return;

  weekday=tmp;

  miutil::miString t = ref.format("%a ",daynamelang );
  dayname->setText(t.cStr());
    
  
  if(weekday == 0 || weekday == 6)
    dayname->setPaletteForegroundColor (Qt::red);
  else
    dayname->setPaletteForegroundColor (Qt::black);
}
