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

#include <QString>
#include <QWidget>
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

  static std::map<QString, xy> pos;
  static std::map<QString, xy> size;
  static std::map<QString, QString> tokens;
  static std::map<QString, QString> sections; // which section for each key
  static QString filename;
  static int xmax;
  static int ymax;

  QString section; // current section of a single instance
public:
  miLogFile()
  {
  }
  miLogFile(const QString& f)
  {
    read(f);
  }

  void setSection(const QString& s)
  {
    section = s;
  }

  int posx(const QString& key);
  int posy(const QString& key);

  int sizex(const QString& key);
  int sizey(const QString& key);

  QString token(const QString& key) const;
  bool booleanToken(const QString& key) const;
  int intToken(const QString& key) const;

  bool hasSize(const QString& key) const;
  bool hasPos(const QString& key) const;
  bool hasToken(const QString& key) const
  {
    return bool(tokens.count(key));
  }
  bool hasBooleanToken(const QString& key) const;
  bool hasIntToken(const QString& key) const;

  void addPos(const QString& key, int x, int y);
  void addSize(const QString& key, int x, int y);
  void addToken(const QString& key, const QString& token);
  void addToken(const QString& key, bool token);
  void addToken(const QString& key, int token);

  bool readStrings(vector<QString> lines, const QString& section = QString());
  QString writeString(const QString& section = QString());

  bool read(const QString& fname = QString()); /// none == filenname
  bool write(const QString& fname = QString()); /// none == filenname

  void setMaxXY(int xm, int ym);

  void logSizeAndPos(QWidget* w, QString name = QString()); // if name ="" w->name() is used
  void restoreSizeAndPos(QWidget* w, QString name = QString());
  void logVisibility(QWidget* w, QString name = QString());
  void restoreVisibility(QWidget* w, QString name = QString());

};

#endif
