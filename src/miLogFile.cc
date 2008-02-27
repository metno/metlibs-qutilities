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


#include <miLogFile.h>

map<miString,miLogFile::xy >  miLogFile::pos;
map<miString,miLogFile::xy >  miLogFile::size;
map<miString,miString >       miLogFile::tokens;
miString                      miLogFile::filename="unnamed.log";
int                           miLogFile::xmax    = 1280;
int                           miLogFile::ymax    = 1024;

void miLogFile::setMaxXY(int xm, int ym)
{
  xmax = xm;
  ymax = ym;
}

bool miLogFile::hasSize(miString key) const
{
  if(!size.count(key))
    return false;
  
  xy s = size[key];  
  return (s.x<=xmax && s.y<=ymax);
}

bool miLogFile::hasPos(miString key) const
{
  if(!pos.count(key))
    return false;

  xy p = pos[key];
  return (p.x<xmax-20 && p.y<ymax-20);
}


int miLogFile::posx(miString key)
{
  return ( pos.count(key) ? pos[key].x : 0 );
}

int miLogFile::posy(miString key)
{
  return ( pos.count(key) ? pos[key].y : 0 );
}

int miLogFile::sizex(miString key)
{
  return ( size.count(key) ? size[key].x : 0 );
}

int miLogFile::sizey(miString key)
{
  return ( size.count(key) ? size[key].y : 0 );
}

void miLogFile::addPos( miString key,int x,int y)
{
  if(x > 0 && y > 0 )
    pos[key] = xy(x,y);
}

void miLogFile::addSize(miString key,int x,int y)
{
  if (x>20 && y>20 )
    size[key] = xy(x,y);
}

void miLogFile::addToken(miString key,miString tok )
{
  tokens[key] = tok;
}

void miLogFile::addToken(miString key,bool tok )
{
  tokens[key] = (tok ? "TRUE" : "FALSE");
}

void miLogFile::addToken(miString key,int token )
{
  tokens[key] = miString(token);
}



miString miLogFile::token(miString key) const
{
  return ( tokens.count(key) ? tokens[key] : "" );
}

bool miLogFile::booleanToken(miString key) const
{
  if(tokens.count(key))
    if(tokens[key] == "TRUE") return true;
  return false;
}

bool miLogFile::hasBooleanToken(miString key) const
{
  if(tokens.count(key))
    if(tokens[key] == "TRUE" || tokens[key] == "FALSE" )
      return true;

  return false;
}

bool miLogFile::hasIntToken(miString key) const
{
  if(tokens.count(key))
    if(tokens[key].isInt())
      return true;
}

int miLogFile::intToken(miString key) const
{
  if(tokens.count(key))
    return  tokens[key].toInt(0);
  return 0;
}


bool miLogFile::read( miString f )
{
  if(f.exists())
    filename=f;
 
  ifstream log(filename.cStr());
   
  if(!log) {
    cerr << "could not open log file: " << filename << endl;
    return false;
  }

  miString         key,tok;
  miString         line;
  vector<miString> words;

  miString section;

  while(log) {
    getline(log,line);
    if(line.contains("#"))
      continue;
    if(line.contains("==="))
      continue;
    
    line.trim();
    if(!line.exists())
      continue;
    
    int c = line.find_first_of(" ",0);
    if(c > line.size())
      continue;
    int k = line.length() -  c;
    
    key =  line.substr(0,c);
    tok =  line.substr(c,k);
    tok.trim();
    
    if(!tok.exists())
      continue;
    
    if(key.contains(".size")) {
      key.replace(".size","");
      
      words = tok.split();
      
      if(words.size() < 2) 
	continue;
     
      if ( !words[0].isNumber() || !words[1].isNumber() )
	continue;
    
      int sx = atoi(words[0].cStr());
      int sy = atoi(words[1].cStr());
 
      if( sx > 20 && sy > 20 )
	size[key] = xy(sx,sy);

      continue;
    }
    

    if(key.contains(".pos")) {
      key.replace(".pos","");
      words = tok.split();
      
      if(words.size() < 2) 
	continue;
      
      if ( !words[0].isNumber() || !words[1].isNumber() )
	continue;
      
      int px = atoi(words[0].cStr());
      int py = atoi(words[1].cStr());
 
      if(px > 0 && py > 0 )
	pos[key] = xy(px,py);
      continue;
      
    }

    tokens[key] = tok;
  }


  return true;
}




bool miLogFile::write( miString f )
{
  if(f.exists())
    filename=f;

  ofstream log(filename.cStr());

  if(!log) {
    cerr << "could not open log file: " << filename << endl;
    return false;
  }


  map<miString,xy>::iterator       ipos  = pos.begin();
  map<miString,xy>::iterator       isize = size.begin();
  map<miString,miString>::iterator itok  = tokens.begin();

  log << "# automatically generated file. DO NOT EDIT!\n\n";


  for(;isize!=size.end();isize++)
    log << isize->first    << ".size " 
	<< isize->second.x << " "
	<< isize->second.y << endl;

  log << "===========================" << endl;

  for(;ipos!=pos.end();ipos++)
    log << ipos->first    << ".pos " 
	<< ipos->second.x << " "
	<< ipos->second.y << endl;

  log << "===========================" << endl;
  
  for(;itok!=tokens.end();itok++)
    log << itok->first    << " " << itok->second << endl;

  return false;
}

void miLogFile::logSizeAndPos(QWidget* w,miString name)
{
  if(!w)             return;  
  if(!name.exists()) name=w->objectName().toStdString();
  if(!name.exists()) return;


  addSize(name, w->width(), w->height());
  addPos( name, w->x(),     w->y()     );
  
}

void miLogFile::restoreSizeAndPos(QWidget* w,miString name)
{
  if(!w)             return;
  if(!name.exists()) name=w->objectName().toStdString();
  if(!name.exists()) return;

  if(hasSize(name)) 
    w->resize(sizex(name),sizey(name));    
  if(hasPos("JoinDialog")) 
    w->move(posx(name),posy(name));
}

void miLogFile::logVisibility(QWidget* w,miString name)
{
  if(!w)             return;  
  if(!name.exists()) name=w->objectName().toStdString();
  if(!name.exists()) return;
  
  name+="IsVisible";
  addToken(name,w->isVisible());
  
}

void miLogFile::restoreVisibility(QWidget* w,miString name)
{
  if(!w)             return;  
  if(!name.exists()) name=w->objectName().toStdString();
  if(!name.exists()) return;
  name+="IsVisible";

  if(hasBooleanToken(name)) {
    if(booleanToken(name)) 
      w->show();
    else
      w->hide();
  }
}
