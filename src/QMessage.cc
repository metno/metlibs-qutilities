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


#include <QMessage.h>
#include <sstream>
#include <qsocket.h>
#include <iostream>
#include <ctype.h>

/* Created by met.no/PU 
   at Fri Feb  1 17:11:14 2002 */

using namespace std; 

QMessage::QMessage()
{}


QMessage::QMessage(QSocket *sPtr)
{
  QTextStream s(sPtr);
  int n = 0;
  int i;
  char c;
  QMessageTo = 0;
  QMessageFrom = 0;
  s >> n;
  s >> QMessageTo >> QMessageFrom;

  for(i = 0; i < n; i++) content += sPtr->getch();
  
  if(!sPtr->atEnd())
    {
      c = sPtr->getch();
      while(!(isdigit(c) || sPtr->atEnd()))
	c = sPtr->getch();
      if(!sPtr->atEnd()) sPtr->ungetch(c);
    }
}



QMessage::QMessage(miMessage m,QString separator)
{
  QString dta;
  content = "";
  if (m.data.size())
    {
      int i;
      QTextOStream ds(&dta);
      ds << m.data[0].cStr(); 
      for (i = 1; i < m.data.size();i++) 
	ds << separator << m.data[i].cStr();
    }

  QTextOStream cs(&content);
  cs << m.to << ' ' 
     << m.from << ' '
     << m.commondesc.length() << ' '
     << m.common.length() << ' '
     << m.description.length() << ' '
     << m.command.length() << ' '
     << separator.length() << ' '
     << dta.length() << ' '
     << m.commondesc.cStr()
     << m.common.cStr()
     << m.description.cStr()
     << m.command.cStr()
     << separator
     << dta;

//   cerr <<"QMessage to: "<<m.to<<endl;
//   cerr <<"QMessage from: "<<m.from<<endl;
//   cerr <<"QMessage command: "<<m.command<<endl;
//   cerr <<"QMessage common: "<<m.common<<endl;
  QMessageTo = m.to;
  QMessageFrom = m.from;
}

int QMessage::to()
{ 
  return QMessageTo;
}



int QMessage::from()
{
  return QMessageFrom;
}


void QMessage::setFrom(int id)
{
  QMessageFrom = id;
}

void QMessage::setTo(int t)
{
  QMessageTo = t;
}


void QMessage::send(QSocket* sPtr)
{
  QTextStream s(sPtr);
  s << content.length() << ' ' 
    << QMessageTo << ' '
    << QMessageFrom << ' '
    << content << endl;

}


miMessage QMessage::getMessage()
{
  miMessage m;
  miString tmp, separator;
  string s = (const char*)content;
  istringstream is(s);
  int i,codeLen,comLen,deLen,coLen,seLen,daLen;
  
  is >> m.to >> m.from >> codeLen >> comLen >> deLen >> coLen >> seLen >> daLen;

  m.to = QMessageTo;
  m.from = QMessageFrom;
  is.get();
  for(i = 0;i<codeLen;i++) m.commondesc += is.get();
  for(i = 0;i<comLen;i++) m.common += is.get();
  for(i = 0;i<deLen;i++) m.description += is.get();
  for(i = 0;i<coLen;i++) m.command += is.get();  
  for(i = 0;i<seLen;i++) separator += is.get();  
  for(i = 0;i<daLen;i++) tmp += is.get();  
  
  if (seLen) m.data = tmp.split(separator);
  else m.data.push_back(tmp);

  return m;
}


QMessage::~QMessage()
{
}










