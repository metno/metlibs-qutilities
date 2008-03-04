/**
 * coclient - coserver client file
 * @author Martin Lilleeng S�tra <martinls@met.no>
 * 
 * $Id: ClientButton.cc 3 2007-09-28 10:40:54Z martinls $
 * 
 * Copyright (C) 2007 met.no
 * 
 * Contact information:
 * Norwegian Meteorological Institute
 * Box 43 Blindern
 * 0313 OSLO
 * NORWAY
 * email: diana@met.no
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

// Qt-includes
#include <qpushbutton.h>
#include <QPixmap>
#include <qtooltip.h>

#include <conn.xpm>
#include <disconn.xpm>
#include <miMessage.h>
#include <ClientButton.h>

ClientButton::ClientButton(const QString & name, const QString & server,
		QWidget * parent) :
	QPushButton(name, parent) {
#ifdef  HAVE_LOG4CXX
	logger = log4cxx::Logger::getLogger("coclient.ClientButton"); ///< LOG4CXX init
#endif // HAVE_LOG4CXX

	uselabel = false;
	setIcon(QPixmap(disconn_xpm));
	setToolTip("Frakoblet");
	setText("");

	string sc = server.toStdString();
	string n = name.toStdString();
	coclient = new CoClient(parent, n.c_str(), "localhost", sc.c_str());

	connect(this, SIGNAL(clicked()), SLOT(connectToServer()));

	connect(coclient, SIGNAL(newClient(miString)), SLOT(setLabel(miString)));
	connect(coclient, SIGNAL(connected()), SLOT(connected()));
	connect(coclient, SIGNAL(receivedMessage(miMessage&)), SIGNAL(receivedMessage(miMessage&)));
	connect(coclient, SIGNAL(addressListChanged()), SIGNAL(addressListChanged()));
}

void ClientButton::connectToServer() {
	if (coclient->notConnected()) {
		coclient->connectToServer();
		setToolTip("Kobler til...");
	} else {
		coclient->disconnectFromServer();
		setToolTip("Frakoblet");
		setLabel("noClient");
		emit connectionClosed();
	}
}

void ClientButton::connected() {
	setIcon(QPixmap(conn_xpm));
	setToolTip("Tilkoblet");
}

void ClientButton::setLabel(miString name) {
	if (name == "noClient") {
		setIcon(QPixmap(disconn_xpm));
		setText("");
	} else if (name == "myself") {
		setIcon(QPixmap(conn_xpm));
		setText("");
	} else if (uselabel ) {
		setIcon(QPixmap(conn_xpm));
		setText(name.c_str());
	}
}

void ClientButton::sendMessage(miMessage& msg) {
	coclient->sendMessage(msg);
}

miString ClientButton::getClientName(int id) {
	return coclient->getClientName(id);
}

bool ClientButton::clientTypeExist(const string& type) {
	return coclient->clientTypeExist(type);
}

void ClientButton::useLabel(bool label) {
	uselabel = label;
}
