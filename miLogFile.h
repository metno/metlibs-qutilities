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
#include <qwidget.h>
#include <map>
#include <fstream>

using namespace std;

class miLogFile {
private:

  typedef struct xy {
    int x;
    int y;
    xy() :
      x(0), y(0)
    {
    }
    xy(int x_, int y_) :
      x(x_), y(y_)
    {
    }
  } xy;

  static std::map<miutil::miString, xy> pos;
  static std::map<miutil::miString, xy> size;
  static std::map<miutil::miString, miutil::miString> tokens;
  static std::map<miutil::miString, miutil::miString> sections; // which section for each key
  static miutil::miString filename;
  static int xmax;
  static int ymax;

  miutil::miString section; // current section of a single instance
public:
  miLogFile()
  {
  }
  miLogFile(miutil::miString f)
  {
    read(f);
  }

  void setSection(miutil::miString s)
  {
    section = s;
  }

  int posx(miutil::miString key);
  int posy(miutil::miString key);

  int sizex(miutil::miString key);
  int sizey(miutil::miString key);

  miutil::miString token(miutil::miString key) const;
  bool booleanToken(miutil::miString key) const;
  int intToken(miutil::miString key) const;

  bool hasSize(miutil::miString key) const;
  bool hasPos(miutil::miString key) const;
  bool hasToken(miutil::miString key) const
  {
    return bool(tokens.count(key));
  }
  bool hasBooleanToken(miutil::miString key) const;
  bool hasIntToken(miutil::miString key) const;

  void addPos(miutil::miString key, int x, int y);
  void addSize(miutil::miString key, int x, int y);
  void addToken(miutil::miString key, miutil::miString token);
  void addToken(miutil::miString key, bool token);
  void addToken(miutil::miString key, int token);

  bool readStrings(vector<miutil::miString> lines, miutil::miString section =
      "");
  miutil::miString writeString(miutil::miString section = "");

  bool read(miutil::miString fname = ""); /// none == filenname
  bool write(miutil::miString fname = ""); /// none == filenname

  void setMaxXY(int xm, int ym);

  void logSizeAndPos(QWidget* w, miutil::miString name = ""); // if name ="" w->name() is used
  void restoreSizeAndPos(QWidget* w, miutil::miString name = "");
  void logVisibility(QWidget* w, miutil::miString name = "");
  void restoreVisibility(QWidget* w, miutil::miString name = "");

};

#endif
