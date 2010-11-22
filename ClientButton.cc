/**
 * coclient - coserver client file
 * @author Martin Lilleeng Sætra <martinls@met.no>
 *
 * $Id$
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <QDebug>
#include <QPushButton>
#include <QPixmap>
#include <QToolTip>

#include <conn.xpm>
#include <disconn.xpm>
#include <unconn.xpm>
#include <miMessage.h>
#include <ClientButton.h>


ClientButton::ClientButton(const QString& name, const QString& server, QWidget * parent)
    : QPushButton(name, parent) {

	uselabel = false;
	setIcon(QPixmap(disconn_xpm));
	setToolTip("Disconnected");
	setText("");

	coclient = new CoClient(name.toLocal8Bit().data(), "localhost", server.toLocal8Bit().data());

	connect(this, SIGNAL(clicked()), SLOT(connectToServer()));

	connect(coclient, SIGNAL(newClient(QString)), SLOT(setLabel(QString)));
	connect(coclient, SIGNAL(connected()), SLOT(connected()));
	connect(coclient, SIGNAL(unableToConnect()), SLOT(unableToConnect()));
	connect(coclient, SIGNAL(receivedMessage(miMessage&)), SIGNAL(receivedMessage(miMessage&)));
	connect(coclient, SIGNAL(addressListChanged()), SIGNAL(addressListChanged()));
}

void ClientButton::connectToServer() {

	qDebug() << __FUNCTION__;

	if (coclient->notConnected()) {
		qWarning() << "ClientButton::connectToServer(): notConnected";
		setToolTip("Connecting...");
		coclient->connectToServer();

	} else {
		qDebug() << "ClientButton::connectToServer(): connected";
		coclient->disconnectFromServer();
		setToolTip("Disconnected");
		setLabel("noClient");
		emit connectionClosed();
	}
}

void ClientButton::connected() {
	setIcon(QPixmap(conn_xpm));
	setToolTip("Connected");
}

void ClientButton::unableToConnect() {
	qWarning() << __FUNCTION__;
	setLabel("portBusy");
	setToolTip("Unable to connect");
}

void ClientButton::setLabel(const QString& name) {
	if (name == "noClient") {
		setIcon(QPixmap(disconn_xpm));
		setText("");
	} else if (name == "myself") {
		setIcon(QPixmap(conn_xpm));
		setText("");
	} else if (name == "portBusy") {
		setIcon(QPixmap(unconn_xpm));
		setText("");
	} else if (uselabel ) {
		setIcon(QPixmap(conn_xpm));

		/// not useful anymore.. needs refactoring
		setText("");
		//setText(name.c_str());
	}
}

void ClientButton::sendMessage(miMessage& msg) {
	coclient->sendMessage(msg);
}

QString ClientButton::getClientName(int id) {
	return coclient->getClientName(id);
}

bool ClientButton::clientTypeExist(const QString& type) {
	return coclient->clientTypeExist(type);
}

void ClientButton::useLabel(bool label) {
	uselabel = label;
}
