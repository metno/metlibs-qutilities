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


#ifndef _qbLoginDialog_h
#define _qbLoginDialog_h


/* Created by met.no/FoU/PU 
   at Tue Feb  5 14:10:48 2002 */

// QLineEdit(port) added by JS/ 07-2002

#include <QDialog>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QButtonGroup>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <puSQL/puSQLgate.h>
#include <puTools/miString.h>


using namespace std; 

/**
   @brief Login dialog

*/

class qbLoginDialog : public QDialog {
  Q_OBJECT
private:
  miString title_;

  puSQLgate* gate;
  /// current serverhost,user,database and portnumber
  miString host;
  miString user;
  miString base;
  unsigned int port;

  /// .. for operational use
  miString oper_label;
  miString oper_host;
  miString oper_user;
  miString oper_base;
  unsigned int oper_port;

  /// .. for testing
  miString test_label;
  miString test_host;
  miString test_user;
  miString test_base;
  unsigned int test_port;

  miString pass;
  bool loggedin;
  bool inTest;
  bool hasTest;
  bool hasOffline;
  bool isoffline;
 
  QVBoxLayout*   top_vlayout; ///< top layout
  QVBoxLayout*   f_vlayout;   ///< frame  layout
  QHBoxLayout*   h_hlayout;   ///< header layout
  QHBoxLayout*   b_hlayout;   ///< buttons layout
  QFrame* topframe;           ///< frame for label etc.
  QLabel* label;              ///< top label
  QButtonGroup* gtestoper;    ///< test/operational button group
  QFrame *ff;                 ///< main frame

  QLineEdit* portnumber;
  QLineEdit* dbserver;
  QLineEdit* username;
  QLineEdit* passwd;

  QColor oper_color;
  QColor test_color;
  QColor std_color;

  QLabel* warnings;
  
  QPushButton *b_oper;
  QPushButton *b_test;
  QPushButton *okb;
  QPushButton *quitb;
  QPushButton *offlineb;

  void makeWidget();
  void setInfo(const miString h,
	       const miString u,
	       const miString b,
	       const unsigned int p);


private slots:
  void okPushed();
  void offlinePushed();
  void cancelPushed();
  void testoperChanged(int b);

protected slots:
  void reject();

public:
  qbLoginDialog(puSQLgate* g, QWidget* parent);
  qbLoginDialog(puSQLgate* g,
		const miString title,
		const miString h,
		const miString u,
		const miString b,
		const unsigned int p,
		const miString th,
		const miString tu,
		const miString tb,
		const unsigned int tp,
		QWidget* parent,
		miString oplabel="Operational",
		miString testlabel="Test",
		bool     hastest=true,
		bool     hasoffline=false);

  
  miString Host() const {return host;}
  miString User() const {return user;}
  miString Base() const {return base;}
  unsigned int Port() const {return port;}
  miString Password() const {return pass;}
  bool LoggedIn() const { return loggedin;}
  bool Test() const { return inTest;}
  bool Offline() const { return isoffline;}
  void setOffline(const bool b) { isoffline= b;}

signals:
  void loginReport();
};

#endif
