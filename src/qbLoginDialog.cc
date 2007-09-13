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


#include <qbLoginDialog.h>
#include <qradiobutton.h>

qbLoginDialog::qbLoginDialog(puSQLgate* g, QWidget* parent)
  : QDialog( parent, "login", true ), gate(g),
    port(0), loggedin(false), inTest(false)
{
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
  : QDialog( parent, "login", true ), gate(g),title_(title),
    host(h), user(u), base(b), port(p),
    oper_host(h), oper_user(u), oper_base(b), oper_port(p),
    test_host(th), test_user(tu), test_base(tb), test_port(tp),
    loggedin(false), inTest(false), oper_label(oplabel), 
    test_label(testlabel), hasTest(hastest), hasOffline(hasoffline),
    isoffline(false)
{
  makeWidget();
}
  
void qbLoginDialog::makeWidget()
{
  oper_color = QColor(181,211,72);
  test_color = QColor(255, 89, 0);

  top_vlayout = new QVBoxLayout(this, 10, 10, "top_vlayout");

  ff = new QFrame( this, "loginframe" );
  ff->setFrameStyle( QFrame::Sunken | QFrame::Panel );
  ff->setLineWidth( 1 );
  top_vlayout->addWidget( ff, 0 );

  f_vlayout=  new QVBoxLayout(ff, 10, 10, "f_vlayout");

  topframe= new QFrame( ff, "topframe" );
  topframe->setFrameStyle( QFrame::Panel | QFrame::Raised );
  topframe->setBackgroundColor( oper_color );
  
  // Create a layout manager for the label
  h_hlayout = new QHBoxLayout(topframe,2,0, "h_hlayout");
  label= new QLabel(title_.cStr(), topframe,"label");
  label->setFont(QFont( "Helvetica", 14, QFont::Normal, true ));
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label->setPalette( QPalette( oper_color ) );
  h_hlayout->addWidget(label,0);
  
  gtestoper= new QButtonGroup(2,Qt::Horizontal,topframe);
  gtestoper->setExclusive(true);
  gtestoper->setRadioButtonExclusive(true);
  gtestoper->setFrameStyle(QFrame::NoFrame);
  gtestoper->setBackgroundColor( oper_color );
  connect(gtestoper,SIGNAL(clicked(int)),this,SLOT(testoperChanged(int)));
  
  QPushButton*  b_oper= new QPushButton(oper_label.cStr(),gtestoper);
  QPushButton*  b_test= new QPushButton(test_label.cStr(),gtestoper);
  b_oper->setToggleButton(true);
  b_test->setToggleButton(true);

  h_hlayout->addWidget(gtestoper,0);
  
  f_vlayout->addWidget(topframe, 0);

  // gridlayout for the input-fields
  QGridLayout* glayout = new QGridLayout(3,2,5,"loglayout");
  
  int startwidget= 0;
  QLabel* server= new QLabel(tr("Database server:"), ff,"server"); 
  QLabel* portn = new QLabel(tr("Database port:"),ff,"portn");
  QLabel* name=   new QLabel(tr("Username:"), ff,"name"); 
  QLabel* pwd=    new QLabel(tr("Password:"), ff,"pwd");
  dbserver=       new QLineEdit(ff,"dbserver");
  dbserver->setMinimumWidth(200);
  if (host.length()>0){
    dbserver->setText(host.c_str());
    startwidget=1;
  }
  portnumber  = new QLineEdit(ff,"portnum");
  if(port > 0 ) {
    QString tmp;
    portnumber->setText(tmp.setNum(port));
  }
  username  = new QLineEdit(ff,"username");
  if (user.length()>0){
    username->setText(user.c_str());
    startwidget= 2;
  }
  passwd  = new QLineEdit(ff,"passwd");
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

  okb= new QPushButton(tr("Log in"),ff, "okb");
  okb->setDefault(true);
  connect(okb, SIGNAL(clicked()), SLOT(okPushed()));
  quitb= new QPushButton(tr("Cancel"),ff, "quitb");
  connect(quitb, SIGNAL(clicked()), SLOT(cancelPushed()));
  if (hasOffline){
    offlineb= new QPushButton(tr("Offline"),ff, "offlineb");
    connect(offlineb, SIGNAL(clicked()), SLOT(offlinePushed()));
  }

  // buttons layout
  b_hlayout = new QHBoxLayout(20, "b_hlayout");
  b_hlayout->addWidget(okb, 10);
  if (hasOffline) b_hlayout->addWidget(offlineb, 10);
  b_hlayout->addWidget(quitb, 10);

  f_vlayout->addLayout(b_hlayout,0);

  // Start the geometry management
  f_vlayout->activate();
  top_vlayout->activate();
  top_vlayout->freeze();

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
  
  b_oper->setOn(true);
}


void qbLoginDialog::testoperChanged(int b)
{
  if (b == 0){
    setInfo(oper_host,oper_user,oper_base,oper_port);
    topframe->setBackgroundColor( oper_color );
    label->setPalette( QPalette( oper_color ) );
    gtestoper->setBackgroundColor( oper_color );
    inTest= false;

  } else {
    setInfo(test_host,test_user,test_base,test_port);
    topframe->setBackgroundColor( test_color );
    label->setPalette( QPalette( test_color ) );
    gtestoper->setBackgroundColor( test_color );
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
    cerr << tr("qbLoginDialog WARNING: NO GATE!") << endl;
    return;
  }
  host= dbserver->text().latin1();
  user= username->text().latin1();
  pass= passwd->text().latin1();
  port= atoi(portnumber->text().latin1());

  puSQLwarning w;
  isoffline= false;

  if (gate->open(base,host,user,pass,w,port)){
    loggedin= true;
    warnings->clear();
    emit loginReport();
    hide();
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
  hide();
}


void qbLoginDialog::cancelPushed()
{
  emit loginReport();
  hide();
}

void qbLoginDialog::reject()
{
  cancelPushed();
}
