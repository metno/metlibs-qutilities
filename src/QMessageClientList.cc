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


/* Created by met.no/PU 
   at Fri Feb 22 13:35:31 2002 */

//using namespace std; 


#include <QMessageClientList.h>


QMessageClientList::QMessageClientList()
{}


QMessageClientList::QMessageClientList(QWidget* parent, QMessageClient* c)
  : QListBox(parent)
{
  client = c;
  connect(client,SIGNAL(addressListChanged()),SLOT(readClientList()));
  connect(client,SIGNAL(connected()),SLOT(clear()));
  connect(this,SIGNAL(selectionChanged()),SLOT(updateCurrentAddress()));
}


void QMessageClientList::readClientList()
{
  int i,h;
  bool oldCurrent = false;

  if (currentItem() != -1) 
    h = client->addressList[currentItem()].id;
  else 
    h = 0;

  clear();

  for (i = 0; i < client->addressList.size();i++)
    {
      insertItem(client->addressList[i].type);  
      if (client->addressList[i].id==h)
	{
	  setCurrentItem(i);
	  oldCurrent = true;
	}
    }

  if (!oldCurrent) setCurrentItem(0);
}


void QMessageClientList::updateCurrentAddress()
{
  cAdr =  client->addressList[currentItem()].id;
}

int QMessageClientList::currentAddress()
{
  return cAdr;
}





