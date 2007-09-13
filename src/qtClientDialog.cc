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


#include <qtClientDialog.h>
#include <QMessageClient.h>
#include <QMessageClientList.h>
#include <QMessage.h>
#include <QLetterCommands.h>
#include <qstring.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <puTools/miString.h>


ClientDialog::ClientDialog(const QString& name, 
			   const QString& server,
			   QWidget* parent)
  : QDialog(parent)
{ 

  clientName = name.latin1();

  client = new QMessageClient(this,
			      name,
			      "localhost",
			      server);

  clientList = new QMessageClientList(this,client);

  connect( clientList, SIGNAL(selectionChanged()), SLOT(selectionChanged()));
  connect(client,SIGNAL(receivedLetter()),SLOT(processLetter()));
  connect( client,SIGNAL(addressListChanged()),SIGNAL(addressListChanged()));
  connect( client,SIGNAL(connected()),SIGNAL(connected()));

  QVBoxLayout *vlayout = new QVBoxLayout( this, 5, 5);
  vlayout->addWidget( clientList );

}

void ClientDialog::openConnection()
{
  if( client->state() == QSocket::Idle ){
    client->newSession();
    selectionChanged();
  }
}

void ClientDialog::closeConnection()
{
  client->closeConnection();
}

void ClientDialog::processLetter()
{
  while (client->inBox.size())
    {
//       cerr << "qtClientDialog::processLetter"<<endl;
      emit receivedLetter(client->inBox[0]);
      client->inBox.erase(client->inBox.begin());
    }
}

void ClientDialog::selectionChanged()
{
  if(client->addressList.size()){
    miString name = 
      client->addressList[clientList->currentItem()].type;
    //should be changed when the client know its own id
   if(name == clientName )
      emit newClient("myself");
    else
      emit newClient(name);
  }
}


void ClientDialog::sendMessage(miMessage& letter)
{
//    cerr <<"sending letter"<<endl;
//    cerr <<"To: "<<letter.to<<endl;
  if( letter.to == qmstrings::default_id ){
    letter.to=clientList->currentAddress();
  }
   client->sendMessage(letter);
}


void ClientDialog::findClients(const QString& type, vector<int>& vc)
{
  if (client){
    int n= client->addressList.size();
    for (int i=0; i<n; i++){
      if (type==client->addressList[i].type)
	vc.push_back(client->addressList[i].id);
    }
  }
}

miString ClientDialog::getClientName(int id)
{
  if (client){
    int n= client->addressList.size();
    for (int i=0; i<n; i++){
      if (id==client->addressList[i].id)
	return client->addressList[i].type;
    }
  }
}

bool ClientDialog::connectClient(const QString& type)
{
  //  cerr <<"connectClient"<<endl;
  if (client){
    int n= client->addressList.size();
    for (int i=n-1; i>-1; i--){
      if (type==client->addressList[i].type){
	clientList->setCurrentItem(i);
	clientList->updateCurrentAddress();
	return true;
      }
    }
  }
  return false;
}











