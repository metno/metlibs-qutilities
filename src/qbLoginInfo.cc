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


#include <qbLoginInfo.h>
#include <qpixmap.h>

#include <QLabel>
#include <QHBoxLayout>

#include <circle_green.xpm>
#include <circle_red.xpm>

qbLoginInfo::qbLoginInfo(QWidget* parent)
  : QWidget(parent,"LoginInfo")
{
  top_hlayout= new QHBoxLayout(this, 0, 4);
  
  databaselabel= new QLabel(this);
  userlabel    = new QLabel(this);

  QLabel* atl= new QLabel("@",this);
  hostlabel  = new QLabel(this);
  connlabel  = new QLabel(this);

  top_hlayout->addWidget(databaselabel);
  top_hlayout->addWidget(connlabel);
  top_hlayout->addWidget(userlabel);
  top_hlayout->addWidget(atl);
  top_hlayout->addWidget(hostlabel);
  
  top_hlayout->activate();
}

void qbLoginInfo::setInfo(const miString host,
			  const miString user,
			  bool loggedin,
			  bool testbase)
{
  miString h= host;
  miString u= user;
  hostlabel->setText(h.cStr());
  userlabel->setText(u.cStr());

  if (loggedin){
    connlabel->setPixmap(circle_green_xpm);
    if (testbase){
      databaselabel->setText("<font color=red>"+tr("Testing")+"</font>");
    } else {
      databaselabel->setText(tr("Operational"));
    }
  } else {
    connlabel->setPixmap(circle_red_xpm);
    databaselabel->clear();
  }
}


