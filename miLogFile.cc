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


/*!
    ------------------------------------
    Created by:
    met.no/FoU/pu/
    at Mon Jun 13 12:26:44 2005
    $Revision$
    at $Date$
    ------------------------------------
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <miLogFile.h>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QRegExp>

using namespace std;


map<QString,miLogFile::xy >  miLogFile::pos;
map<QString,miLogFile::xy >  miLogFile::size;
map<QString,QString >       miLogFile::tokens;
map<QString,QString>        miLogFile::sections;
QString                      miLogFile::filename="unnamed.log";
int                           miLogFile::xmax    = 1280;
int                           miLogFile::ymax    = 1024;

void miLogFile::setMaxXY(int xm, int ym)
{
  xmax = xm;
  ymax = ym;
}

bool miLogFile::hasSize(const QString& key) const
{
  if(!size.count(key))
    return false;

  xy s = size[key];
  return (s.x<=xmax && s.y<=ymax);
}

bool miLogFile::hasPos(const QString& key) const
{
  if(!pos.count(key))
    return false;

  xy p = pos[key];
  return (p.x<xmax-20 && p.y<ymax-20);
}


int miLogFile::posx(const QString& key)
{
  return ( pos.count(key) ? pos[key].x : 0 );
}

int miLogFile::posy(const QString& key)
{
  return ( pos.count(key) ? pos[key].y : 0 );
}

int miLogFile::sizex(const QString& key)
{
  return ( size.count(key) ? size[key].x : 0 );
}

int miLogFile::sizey(const QString& key)
{
  return ( size.count(key) ? size[key].y : 0 );
}

void miLogFile::addPos(const QString& key, int x, int y)
{
  if(x > 0 && y > 0 )
    pos[key] = xy(x,y);
  if(!section.isEmpty())
    sections[key+".pos"] = section;
}

void miLogFile::addSize(const QString& key, int x, int y)
{
  if (x>20 && y>20 )
    size[key] = xy(x,y);
  if(!section.isEmpty())
    sections[key+".size"] = section;
}

void miLogFile::addToken(const QString& key, const QString& tok)
{
  tokens[key] = tok;
  if(!section.isEmpty())
      sections[key] = section;
}

void miLogFile::addToken(const QString& key, bool tok)
{
  tokens[key] = (tok ? "TRUE" : "FALSE");
  if(!section.isEmpty())
      sections[key] = section;
}

void miLogFile::addToken(const QString& key, int token)
{
  tokens[key] = token;
  if(!section.isEmpty())
       sections[key] = section;
}



QString miLogFile::token(const QString& key) const
{
  return ( tokens.count(key) ? tokens[key] : "" );
}

bool miLogFile::booleanToken(const QString& key) const
{
  if(tokens.count(key))
    if(tokens[key] == "TRUE") return true;
  return false;
}

bool miLogFile::hasBooleanToken(const QString& key) const
{
  if(tokens.count(key))
    if(tokens[key] == "TRUE" || tokens[key] == "FALSE" )
      return true;

  return false;
}

bool miLogFile::hasIntToken(const QString& key) const
{
	bool hasInt = false;
	if(tokens.count(key))
		tokens[key].toInt(&hasInt);
	return hasInt;
}

int miLogFile::intToken(const QString& key) const
{
  if(tokens.count(key))
    return  tokens[key].toInt();
  return 0;
}




bool miLogFile::read(const QString& f )
{
  if(!f.isEmpty())
    filename=f;

  QFile log(filename);

  if (!log.open(QFile::ReadOnly | QIODevice::Text)) {
	  qWarning() << "could not open log file: " << filename;
	  return false;
  }

  QString         line;
  vector<QString> lines;

  QTextStream in(&log);

  while(!in.atEnd()) {
	  line = in.readLine();
	  lines.push_back(line);
  }

  return readStrings(lines);
}

bool miLogFile::readStrings(vector<QString> lines, const QString& section)
{
  QString         line;
  QString         key,tok;
  QStringList     words;

  for (int i = 0; i < lines.size(); ++i) {
    line=lines[i];
    if(line.contains("#"))
      continue;
    if(line.contains("==="))
      continue;

    line = line.trimmed();
    if(line.isEmpty())
      continue;

    int c = line.indexOf(" ",0);
    if(c > line.size())
      continue;
    int k = line.length() -  c;

    key =  line.left(c);
    tok =  line.mid(c,k);
    tok = tok.trimmed();

    if(tok.isEmpty())
      continue;

    if(!section.isEmpty())
      sections[key]=section;


    if(key.contains(".size")) {
      key.replace(".size","");
      words = tok.split(QRegExp("\\s+"), QString::SkipEmptyParts);
      if(words.size() < 2)
        continue;

      bool isInt = false;
      int sx = words[0].toInt(&isInt);
      if(!isInt)
    	  continue;
      int sy = words[1].toInt(&isInt);
      if(!isInt)
    	  continue;

      if( sx > 20 && sy > 20 )
        size[key] = xy(sx,sy);

      continue;
    }


    if(key.contains(".pos")) {
      key.replace(".pos","");
      words = tok.split(QRegExp("\\s+"), QString::SkipEmptyParts);
      if(words.size() < 2)
        continue;

      bool isInt = false;
      int px = words[0].toInt(&isInt);
      if(!isInt)
    	  continue;
      int py = words[1].toInt(&isInt);
      if(!isInt)
    	  continue;

      if(px > 0 && py > 0 )
        pos[key] = xy(px,py);
      continue;

    }

    tokens[key] = tok;
  }

  return true;
}




bool miLogFile::write(const QString& filename )
{
	if(filename.isEmpty())
		return false;

	QFile file(filename);
	if(!file.open(QFile::ReadOnly | QIODevice::Text)) {
		qWarning() << "could not open log file: " << filename;
	    return false;
	} else {
		QTextStream log(&file);
		log << "# automatically generated file. DO NOT EDIT!\n\n";
	    log << writeString();
	    file.close();
		return true;
	}
}


QString  miLogFile::writeString(const QString& section)
{
  QString logData;
  QTextStream log(&logData, QIODevice::WriteOnly);
  QString key;
  map<QString,xy>::iterator       ipos  = pos.begin();
  map<QString,xy>::iterator       isize = size.begin();
  map<QString,QString>::iterator itok  = tokens.begin();


  for (;isize!=size.end();isize++) {
    key=isize->first + ".size";
    if(!section.isEmpty() && sections[key] != section)
        continue;
    log <<  key << " " << isize->second.x << " " << isize->second.y << endl;
  }

  log << "===========================" << endl;

  for (;ipos!=pos.end();ipos++) {
    key=ipos->first + ".pos";
    if(!section.isEmpty() && sections[key] != section)
      continue;
    log << key << " "  << ipos->second.x << " " << ipos->second.y << endl;
  }

  log << "===========================" << endl;

  for (;itok!=tokens.end();itok++) {
    key=itok->first;
    if(!section.isEmpty() && sections[key] != section)
      continue;
    log << key  << " " << itok->second << endl;
  }
  return logData;
}

void miLogFile::logSizeAndPos(QWidget* w, QString name)
{
  if(!w)             return;
  if(name.isEmpty()) name=w->objectName();
  if(name.isEmpty()) return;


  addSize(name, w->width(), w->height());
  addPos( name, w->x(),     w->y()     );

}

void miLogFile::restoreSizeAndPos(QWidget* w, QString name)
{
  if(!w)             return;
  if(name.isEmpty()) name=w->objectName();
  if(name.isEmpty()) return;

  if(hasSize(name))
    w->resize(sizex(name),sizey(name));
  if(hasPos(name))
    w->move(posx(name),posy(name));
}

void miLogFile::logVisibility(QWidget* w, QString name)
{
  if(!w)             return;
  if(name.isEmpty()) name=w->objectName();
  if(name.isEmpty()) return;

  name+="IsVisible";
  addToken(name,w->isVisible());

}

void miLogFile::restoreVisibility(QWidget* w, QString name)
{
  if(!w)             return;
  if(name.isEmpty()) name=w->objectName();
  if(name.isEmpty()) return;
  name+="IsVisible";

  if(hasBooleanToken(name)) {
    if(booleanToken(name))
      w->show();
    else
      w->hide();
  }
}
