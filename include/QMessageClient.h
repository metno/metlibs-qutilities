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


#ifndef _QMessageClient_h
#define _QMessageClient_h



#include "QMessage.h"

#include <vector>

#include <qstring.h>
#include <qapplication.h>
#include <qsocket.h>
#include <qprocess.h>


/* Created by met.no/FoU/PU 
   at Wed Feb  6 16:12:11 2002 */
using namespace std; 
class QMessageClient : public QSocket  
{
  Q_OBJECT

public:
  QMessageClient();
  QMessageClient(QObject* parent,
		 const char * name, 
		 const char * h, 
		 const char * sc, 
		 const char * lf = "/tmp/.serverlock",
		 Q_UINT16 p = 0
		 );

  bool sendMessage(miMessage, const char * = "|");
  
  struct clientAddress
  {
    miString type;
    int id;
  };

  vector<miMessage> inBox;
  vector<clientAddress> addressList;  

private:
  miString clientType;
  miString lockFile, serverCommand, host;

  Q_UINT16 port;
  QProcess *server;
  void internal(QMessage);
  void removeAddress(int);
  void connectServer();

signals:
  void receivedLetter();
  void addressListChanged();
  
public slots:
  void closeConnection();
  void newSession();

private slots:
  void processLetters();
  void sendType();
  void checkServer();
  void connectionClosed();
  void slotWriteStderr();
  void socketError(int);
};

#endif












