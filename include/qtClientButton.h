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


#ifndef _clientbutton_h
#define _clientbutton_h

#include <puTools/miString.h>

#include <qwidget.h>
#include <qpushbutton.h>



using namespace std; 

class miMessage;
class ClientDialog;

class ClientButton : public QPushButton
{
  Q_OBJECT
  
private:
  bool uselabel;

public: 
  
  ClientButton(  const QString & text,
		 const QString & server,
		 QWidget * parent, 
		 const char * name = 0 );

  ClientDialog *clientm;
  void sendMessage(miMessage&);
  void findClients(const QString& type, vector<int>&);
  miString getClientName(int id);
  bool connectClient(const QString&);
  void useLabel(bool);

public slots:
  void openConnection();
  void newClient(miString);
  void connected();

signals:
  void receivedLetter(miMessage&);
  void addressListChanged();
  void connectionClosed();
};



#endif  
