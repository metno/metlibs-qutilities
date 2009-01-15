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


#ifndef _miLogFile_h
#define _miLogFile_h

/*!
    ------------------------------------
    Created by:
    met.no/FoU/pu/
    at Mon Jun 13 12:26:44 2005
    $Revision$
    at $Date$
    ------------------------------------
*/

#include <puTools/miString.h>

#include <map>
#include <fstream>

#include <qwidget.h>

using namespace std;

class miLogFile {
private:

  typedef struct xy {
    int x;
    int y;
    xy() : x(0) , y(0) {}
    xy(int x_,int y_) : x(x_) , y(y_) {}
  } xy;


  static map<miString,xy>       pos;
  static map<miString,xy>       size;
  static map<miString,miString> tokens;
  static map<miString,miString> sections; // which section for each key
  static miString               filename;
  static int                    xmax;
  static int                    ymax;

  miString section;  // current section of a single instance
public:
  miLogFile() {}
  miLogFile(miString f)  { read(f);}

  void setSection(miString s) { section=s;}

  int posx(  miString key);
  int posy(  miString key);

  int sizex( miString key);
  int sizey( miString key);

  miString token(        miString key ) const;
  bool     booleanToken( miString key ) const;
  int      intToken(     miString key ) const;

  bool hasSize(  miString key) const;
  bool hasPos(   miString key) const;
  bool hasToken( miString key) const { return bool(tokens.count(key));}
  bool hasBooleanToken(miString key) const;
  bool hasIntToken(miString key) const;

  void addPos(   miString key, int x, int y );
  void addSize(  miString key, int x, int y);
  void addToken( miString key, miString token);
  void addToken( miString key, bool token);
  void addToken( miString key, int  token);


  bool      readStrings(vector<miString> lines,miString section="");
  miString  writeString(miString section="");

  bool read(  miString fname="" ); /// none == filenname
  bool write( miString fname="" ); /// none == filenname

  void setMaxXY(int xm, int ym);

  void logSizeAndPos(QWidget* w,miString name=""); // if name ="" w->name() is used
  void restoreSizeAndPos(QWidget* w,miString name="");
  void logVisibility(QWidget* w, miString name="");
  void restoreVisibility(QWidget* w, miString name="");


};

#endif
