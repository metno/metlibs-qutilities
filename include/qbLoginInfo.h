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


#ifndef _qbLoginInfo_h
#define _qbLoginInfo_h


/* Created by met.no/FoU/PU 
   at Tue Feb  5 16:10:30 2002 */

#include <qwidget.h>
#include <q3frame.h>
#include <qlabel.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3HBoxLayout>

#include <puTools/miString.h>

using namespace std; 

/**
   @brief Login details info in statusbar

*/

class qbLoginInfo : public QWidget {
  Q_OBJECT
private:
  Q3HBoxLayout* top_hlayout; ///< top layout
  QLabel* databaselabel;
  QLabel* hostlabel;
  QLabel* userlabel;
  QLabel* connlabel;

public:
  qbLoginInfo(QWidget* parent);

public slots:
  void setInfo(const miString host,
	       const miString user,
	       bool loggedin,
	       bool testbase);  
};

#endif
