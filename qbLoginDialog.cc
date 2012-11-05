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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <qbLoginDialog.h>

#include <QRadioButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>

using namespace miutil;

qbLoginDialog::qbLoginDialog(puSQLgate* g, QWidget* parent)
  : QDialog(parent), gate(g),
    port(0), loggedin(false), inTest(false)
{
	setObjectName("login");
	setModal(true);
  makeWidget();
}

qbLoginDialog::qbLoginDialog(puSQLgate* g,
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
			     miString oplabel,
			     miString testlabel,
			     bool     hastest,
			     bool hasoffline)
  : QDialog(parent), title_(title), gate(g),
    host(h), user(u), base(b), port(p),
    oper_label(oplabel), oper_host(h), oper_user(u), oper_base(b), oper_port(p),
    test_label(testlabel), test_host(th), test_user(tu), test_base(tb), test_port(tp),
    loggedin(false), inTest(false),
    hasTest(hastest), hasOffline(hasoffline),
    isoffline(false)
{
	setObjectName("login");
	setModal(true);
  makeWidget();
}

void qbLoginDialog::makeWidget()
{
  oper_color = QColor(181,211,72);
  test_color = QColor(255, 89, 0);

  top_vlayout = new QVBoxLayout(this);
  top_vlayout->setObjectName("top_vlayout");

  ff = new QFrame(this);
  ff->setObjectName("loginframe");
  ff->setFrameStyle( QFrame::Sunken | QFrame::Panel );
  ff->setLineWidth( 1 );
  top_vlayout->addWidget( ff, 0 );

  f_vlayout=  new QVBoxLayout(ff);
  f_vlayout->setObjectName("f_vlayout");

  topframe= new QFrame(ff);
  topframe->setObjectName("topframe");
  topframe->setFrameStyle( QFrame::Panel | QFrame::Raised );
  topframe->setAutoFillBackground(true);
  topframe->setPalette( oper_color );

  // Create a layout manager for the label
  h_hlayout = new QHBoxLayout(topframe);
  h_hlayout->setObjectName("h_hlayout");
  label= new QLabel(title_.cStr(), topframe);
  label->setObjectName("label");
  label->setFont(QFont( "Helvetica", 14, QFont::Normal, true ));
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label->setPalette( QPalette( oper_color ) );
  h_hlayout->addWidget(label,0);

  gtestoper= new QButtonGroup(topframe);
  gtestoper->setExclusive(true);
  connect(gtestoper,SIGNAL(buttonClicked(int)),this,SLOT(testoperChanged(int)));

  b_oper= new QPushButton(oper_label.cStr());
  b_test= new QPushButton(test_label.cStr());
  gtestoper->addButton(b_oper, 0);
  gtestoper->addButton(b_test, 1);

  QGroupBox* opertest_box = new QGroupBox(topframe);
  opertest_box->setFlat(true);
  QHBoxLayout *hbox = new QHBoxLayout;
  hbox->addWidget(b_oper);
  hbox->addWidget(b_test);
  opertest_box->setLayout(hbox);

  h_hlayout->addWidget(opertest_box, 0);

  f_vlayout->addWidget(topframe, 0);

  // gridlayout for the input-fields
  QGridLayout* glayout = new QGridLayout();
  glayout->setObjectName("loglayout");

  int startwidget= 0;
  QLabel* server= new QLabel(tr("Database server:"), ff);
  server->setObjectName("server");
  QLabel* portn = new QLabel(tr("Database port:"),ff);
  portn->setObjectName("portn");
  QLabel* name=   new QLabel(tr("Username:"), ff);
  name->setObjectName("name");
  QLabel* pwd=    new QLabel(tr("Password:"), ff);
  pwd->setObjectName("pwd");
  dbserver=       new QLineEdit(ff);
  dbserver->setObjectName("dbserver");
  dbserver->setMinimumWidth(100);
  if (host.length()>0){
    dbserver->setText(host.c_str());
    startwidget=1;
  }
  portnumber  = new QLineEdit(ff);
  portnumber->setObjectName("portnum");
  if(port > 0 ) {
    QString tmp;
    portnumber->setText(tmp.setNum(port));
  }
  username  = new QLineEdit(ff);
  username->setObjectName("username");
  if (user.length()>0){
    username->setText(user.c_str());
    startwidget= 2;
  }
  passwd  = new QLineEdit(ff);
  passwd->setObjectName("passwd");
  passwd->setEchoMode(QLineEdit::Password);

  glayout->addWidget(server, 1,1);
  glayout->addWidget(portn,  2,1);
  glayout->addWidget(name,   3,1);
  glayout->addWidget(pwd,    4,1);

  glayout->addWidget(dbserver,  1,2);
  glayout->addWidget(portnumber,2,2);
  glayout->addWidget(username,  3,2);
  glayout->addWidget(passwd,    4,2);

  f_vlayout->addLayout(glayout, 0);

  warnings= new QLabel(ff);
  warnings->setTextFormat(Qt::RichText);
  warnings->setMinimumHeight(45);
  warnings->setFrameStyle(QFrame::Box | QFrame::Raised);
  f_vlayout->addWidget(warnings,10);

  okb= new QPushButton(tr("Log in"),ff);
  okb->setObjectName("okb");
  okb->setDefault(true);
  connect(okb, SIGNAL(clicked()), SLOT(okPushed()));
  quitb= new QPushButton(tr("Cancel"),ff);
  okb->setObjectName("quitb");
  connect(quitb, SIGNAL(clicked()), SLOT(cancelPushed()));
  if (hasOffline){
    offlineb= new QPushButton(tr("Offline"),ff);
    offlineb->setObjectName("offlineb");
    connect(offlineb, SIGNAL(clicked()), SLOT(offlinePushed()));
  }

  // buttons layout
  b_hlayout = new QHBoxLayout();
  b_hlayout->setObjectName("b_hlayout");
  b_hlayout->addWidget(okb, 10);
  if (hasOffline) b_hlayout->addWidget(offlineb, 10);
  b_hlayout->addWidget(quitb, 10);

  f_vlayout->addLayout(b_hlayout,0);

  // Start the geometry management
  f_vlayout->activate();
  top_vlayout->activate();

  if (startwidget==0){
    dbserver->end(true);
    dbserver->setFocus();
  } else if (startwidget==1){
    username->end(true);
    username->setFocus();
  } else {
    passwd->end(true);
    passwd->setFocus();
  }

  b_oper->setDown(true);
}


void qbLoginDialog::testoperChanged(int b)
{
  if (b == 0){
    setInfo(oper_host,oper_user,oper_base,oper_port);
    b_oper->setDown(true);
    b_test->setDown(false);
    topframe->setPalette( oper_color );
    label->setPalette( QPalette( oper_color ) );
    //gtestoper->setBackgroundColor( oper_color );
    inTest= false;

  } else {
    setInfo(test_host,test_user,test_base,test_port);
    b_test->setDown(true);
    b_oper->setDown(false);
    topframe->setPalette( test_color );
    label->setPalette( QPalette( test_color ) );
    //gtestoper->setBackgroundColor( test_color );
    inTest= hasTest;
  }
}

void qbLoginDialog::setInfo(const miString h,
			    const miString u,
			    const miString b,
			    const unsigned int p)
{
  host= h;
  user= u;
  base= b;
  port= p;

  QString tmp;
  portnumber->setText(tmp.setNum(port));

  dbserver->setText(host.cStr());
  username->setText(user.cStr());
}


void qbLoginDialog::okPushed()
{
  if (!gate){
    cerr << tr("qbLoginDialog WARNING: NO GATE!").toStdString() << endl;
    return;
  }
  host= dbserver->text().toStdString();
  user= username->text().toStdString();
  pass= passwd->text().toStdString();
  port= atoi(portnumber->text().toStdString().c_str());

  puSQLwarning w;
  isoffline= false;

  if (gate->open(base,host,user,pass,w,port)){
    loggedin= true;
    warnings->clear();
    emit loginReport();
    setVisible(false);
  } else {
    loggedin= false;
    warnings->setText(w.w2str().cStr());
  }
}

void qbLoginDialog::offlinePushed()
{
  loggedin= false;
  isoffline=true;
  emit loginReport();
  setVisible(false);
}


void qbLoginDialog::cancelPushed()
{
  emit loginReport();
  setVisible(false);
}

void qbLoginDialog::reject()
{
  cancelPushed();
}
