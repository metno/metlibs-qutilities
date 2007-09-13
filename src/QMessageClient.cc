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


#include <QMessageClient.h>
#include <QLetterCommands.h>
#include <qstring.h>
#include <iostream>
#include <qfile.h>
#include <stdlib.h>
#include <qprocess.h>
#include <qtextstream.h>

/* Created by met.no/PU 
   at Thu Feb  7 09:47:40 2002 */

using namespace std; 

QMessageClient::QMessageClient()
{}



QMessageClient::QMessageClient(
			   QObject* parent, 
			   const char *name, 
			   const char *h,	
			   const char *sc, 
			   const char *lf,
			   Q_UINT16 p  
			   ) : QSocket(parent, name)
{
  port = qmstrings::port;
  clientType = name;

  lockFile = lf;
  serverCommand = sc ;
  host = h;

  connect(this, SIGNAL(connected()),SLOT(sendType()));
  connect(this, SIGNAL(readyRead()),SLOT(processLetters()));
  connect(this, SIGNAL(connectionClosed()),SLOT(connectionClosed()));
  connect(this, SIGNAL(error(int)), SLOT(socketError(int)));

//   if( connectToServer ){
//     newSession();
//   }
}



void QMessageClient::newSession()
{
  if (host == "localhost"){
    addressList.erase(addressList.begin(),addressList.end());
    emit addressListChanged();      
    connectToHost(host,port);
  }

}




void QMessageClient::slotWriteStderr()
{
  while (server->canReadLineStderr() )
    cerr << server->readLineStderr() << endl;
}


void QMessageClient::checkServer()
{
  if (server->readLineStdout() == "Yes, master") 
    newSession();  
}



bool QMessageClient::sendMessage(miMessage l, const char* sep )
{ 
   if (state() == Connected)
     {
       l.content();
       QMessage m(l, sep);
       m.send(this);
       return true;
     }
   else 
     return false;
}



void QMessageClient::closeConnection()
{
//   cerr <<"QMessageClient  socket: "<<socket()<<endl;
  close();
  connectionClosed();  
}



void QMessageClient::processLetters()
{
  QMessage *l;
  while (canReadLine())
    {
      l = new QMessage(this);
      if (l->from() == 0) 
	internal(*l);
      inBox.push_back(l->getMessage());
      emit receivedLetter();
    }
}



void QMessageClient::internal(QMessage l)
{ 
  miMessage m = l.getMessage() ;
  m.content();
  if (m.command ==qmstrings::newclient)
    {
      vector<miString> common = m.common.split(":");
      if(common.size()!=2) return;
      clientAddress ca;
      ca.id=atoi(common[0].cStr());
      ca.type=common[1].cStr();
      removeAddress(ca.id);
      addressList.push_back(ca);
      emit addressListChanged();
    }
  else if (m.command ==qmstrings::removeclient)
    {
      vector<miString> common = m.common.split(":");
      if(common.size()!=2) return;
      int i = atoi(common[0].cStr());;
      removeAddress(i);
      emit addressListChanged();      
    }
}



void QMessageClient::removeAddress(int j)
{
  //   cerr<<"remove address"<<endl;
  int i;
  for (i = 0; i < addressList.size();i++)
    {
      if (addressList[i].id == j)
	{
	  miMessage l;
	  l.command = qmstrings::remove;
	  l.description = "id";
	  l.from=0;
	  miString idStr(addressList[i].id);
	  l.data.push_back(idStr);
	  inBox.push_back(l);
	  emit receivedLetter();
	  addressList.erase(addressList.begin()+i);
          break;
	}
    }
}



void QMessageClient::sendType()
{
  miMessage l(0,0,"SETTYPE","INTERNAL");
  l.data.push_back(clientType);
  sendMessage(l);
}



void QMessageClient::connectionClosed()
{
//   cerr <<"QMessageClient::connectionClosed"<<endl;
  processLetters();

  int n=addressList.size();
  for(int i=0; i<n; i++){
    miMessage l;
    l.command = qmstrings::remove;
    l.description = "id";
    miString idStr(addressList[i].id);
    l.data.push_back(idStr);
    inBox.push_back(l);
  }
  //  cerr<<"receivedLetter"<<endl;
  emit receivedLetter();

  addressList.erase(addressList.begin(),addressList.end());
  emit addressListChanged();      

}

void QMessageClient::socketError(int e)
{
    cerr <<"Starting coserver... "<<endl;

    server  = new QProcess(serverCommand); 
    server->addArgument("-d");
    connect(server,SIGNAL(readyReadStdout()),SLOT(checkServer()));
    connect(server,SIGNAL(readyReadStderr()),SLOT(slotWriteStderr()));
    if( !server->start()){
      cerr <<"Couldn't start server"<<endl;
      server->kill();
      return;
    }

}














