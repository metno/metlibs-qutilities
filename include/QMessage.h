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



#ifndef _QMessage_h
#define _QMessage_h

#include "miMessage.h"

#include <puTools/miString.h>

#include <qstring.h>
#include <qtextstream.h>
#include <qsocket.h>




/* Created by met.no/FoU/PU 
   at Fri Feb  1 17:03:26 2002 */

using namespace std; 

class QMessage  
{

private:
  int QMessageTo,QMessageFrom;
public:
QString content;
  //public:

  QMessage();
  QMessage(QSocket*);
  QMessage(miMessage,QString = "|");
  miMessage getMessage();
  void send(QSocket*);
  int to();
  int from();
  void setFrom(int);  
  void setTo(int);

  ~QMessage();

};

#endif













