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


#ifndef _ClientDialog_h
#define _ClientDialog_h

#include "QMessageClient.h"

#include <puTools/miString.h>

#include <qdialog.h>
#include <qstring.h>



class QMessageClientList;
class QPushButton;
class QLabel;

class ClientDialog : public QDialog
{
Q_OBJECT
private:
  QMessageClient *client;
  QMessageClientList* clientList;
  miString clientName;

public:
  ClientDialog(const QString& name, const QString& server, QWidget* parent);

  void sendMessage(miMessage&);
  void findClients(const QString& type, vector<int>&);
  miString getClientName(int id);
  bool connectClient(const QString&);
  bool isIdle(){return (client->state() == QSocket::Idle);}

private slots:
  void processLetter();
  void selectionChanged();

public slots:
  void openConnection();
  void closeConnection();

signals:
  void receivedLetter(miMessage&);
  void newClient(miString);
  void addressListChanged();
  void connected();
};

#endif


