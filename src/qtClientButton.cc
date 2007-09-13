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


#include <qtClientButton.h>
#include <qtClientDialog.h>
#include <qpushbutton.h>
#include <miMessage.h>
#include <qtooltip.h>


#include <conn.xpm>
#include <disconn.xpm>
// #include <circle_green.xpm>
// #include <circle_red.xpm>
// #include <circle_yellow.xpm>

ClientButton::ClientButton( const QString & text, 
			    const QString & server,
			    QWidget * parent, 
			    const char * name  )
  :QPushButton( text,parent,name)
{

  uselabel = false;
  setIconSet(QPixmap(disconn_xpm));
  QToolTip::add(this,"Frakoblet");
  setText("");  

  
  clientm = new ClientDialog(text, server, this);
  clientm->hide();

  connect( this,SIGNAL(clicked()), SLOT( openConnection() ));
  connect( clientm, SIGNAL(newClient(miString)), SLOT(newClient(miString)));
  connect( clientm, SIGNAL(connected()), SLOT(connected()));
  connect( clientm,SIGNAL(receivedLetter(miMessage&)),SIGNAL(receivedLetter(miMessage&)));
  connect( clientm,SIGNAL(addressListChanged()),SIGNAL(addressListChanged()));

}

void ClientButton::openConnection()
{
//   cerr << "ClientButton::openConnection()"<<endl;
  if( clientm->isIdle() ){ 
//     cerr<<"koble til"<<endl;
    clientm->openConnection();
    QToolTip::add(this,"Kobler til...");
  } else {
    //    cerr<<"koble fra"<<endl;
    clientm->closeConnection();
    setIconSet(QPixmap(disconn_xpm));
    QToolTip::add(this,"Frakoblet");
    setIconSet(QPixmap(disconn_xpm));
    setText("");  
    emit connectionClosed();
  }
}


void ClientButton::connected()
{
//   cerr <<"Connected :-)"<<endl;
  setIconSet(QPixmap(conn_xpm));
  QToolTip::add(this,"Tilkoblet");
}

void ClientButton::newClient(miString name)
{
  //   cerr <<"newClient: "<<name<<endl;
  if( name == "noClient" ){
    setIconSet(QPixmap(disconn_xpm));
    setText("");  
  } else if( name == "myself" ){
    setIconSet(QPixmap(conn_xpm));
    setText("");
  } else if( uselabel ){
    setIconSet(QPixmap(conn_xpm));
    setText(name.c_str());
  }  
}

void ClientButton::sendMessage(miMessage& letter)
{
//    cerr <<"ClientButton: sending letter"<<endl;
   clientm->sendMessage(letter);
}

void ClientButton::findClients(const QString& type, vector<int>& vc)
{
  clientm->findClients(type,vc);
}

miString ClientButton::getClientName(int id)
{
return clientm->getClientName(id);
}

bool ClientButton::connectClient(const QString& type)
{
  return clientm->connectClient(type);
}

void ClientButton::useLabel(bool label)
{
  uselabel = label;
}



