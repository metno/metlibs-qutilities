/**
 * coclient - coserver client file
 * @author Martin Lilleeng Sætra <martinls@met.no>
 * 
 * $Id: CoClient.cc 3 2007-09-28 10:40:54Z martinls $
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

// TODO: Add support for multiple servers active on different ports (on the same node)
// TODO: Add support for multiple clients per server

// Qt-includes
#include <QtGui>
#include <QtNetwork>
#include <qstring.h>
#include <qfile.h>

#include <iostream>
#include <stdlib.h>

#include <QLetterCommands.h>
#include <CoClient.h>

CoClient::CoClient(QWidget* parent, const char *name, const char *h,
		const char *sc, const char *lf, quint16 p) :
	QDialog(parent) {
#ifdef HAVE_LOG4CXX
	logger = log4cxx::Logger::getLogger("coclient.CoClient"); ///< LOG4CXX init
#endif	// HAVE_LOG4CXX

	port = qmstrings::port;
	clientType = name;

	lockFile = lf;
	serverCommand = sc ;
	host = h;

	blockSize = 0;

	tcpSocket = new QTcpSocket(this);
	
	noCoserver4 = false;

	// connected to coserver
	connect(tcpSocket, SIGNAL(connected()), this, SLOT(sendType()));
	// anything new to read?
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readNew()));
	// connection to server closed
	connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
	// socket error
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

#ifdef _DEBUG
	connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(printBytesWritten(qint64)));
#endif
}

void CoClient::printBytesWritten(qint64 written) {
	cout << "Written "<< written << " bytes to socket"<< endl;
}

bool CoClient::notConnected() {
	return (QAbstractSocket::UnconnectedState == tcpSocket->state());
}

void CoClient::connectionClosed() {
	LOG4CXX_INFO(logger, "Disconnected from server");
}

void CoClient::connectToServer() {
	noCoserver4 = false;
	tcpSocket->connectToHost(QString(host.cStr()), port);
}

void CoClient::disconnectFromServer() {
	tcpSocket->disconnectFromHost();
}

void CoClient::readNew() {
	QDataStream in(tcpSocket);
	in.setVersion(QDataStream::Qt_4_0);

	// make sure that the whole message has been written
	if (blockSize == 0) {
		if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
			return;

		in >> blockSize;
	}

	if (tcpSocket->bytesAvailable() < blockSize)
		return;

	// read incoming message
	miMessage msg;
	QString tmpcommand, tmpdescription, tmpcommondesc, tmpcommon,
			tmpclientType, tmpco, tmpdata;
	int size = 0;

	in >> msg.to;
	in >> msg.from;
	in >> tmpcommand;
	msg.command = tmpcommand.toStdString();
	in >> tmpdescription;
	msg.description = tmpdescription.toStdString();
	in >> tmpcommondesc;
	msg.commondesc = tmpcommondesc.toStdString();
	in >> tmpcommon;
	msg.common = tmpcommon.toStdString();
	in >> tmpclientType;
	msg.clientType = tmpclientType.toStdString();
	in >> tmpco;
	msg.co = tmpco.toStdString();
	in >> size; // NOT A FIELD IN MIMESSAGE (METADATA ONLY)
	for (int i = 0; i < size; i++) {
		in >> tmpdata;
		msg.data.push_back(tmpdata.toStdString());
	}

#ifdef _DEBUG
	cout << "miMessage in CoClient::readNew() (RECV)"<< endl;
	cout << msg.content() << endl;
#endif

	// if origin is the server itself, then it will always be a request to
	// change (add/delete) entries in the list of clients
	if (msg.from == 0)
		editClients(msg);

emit 						receivedMessage(msg);

	blockSize = 0;

	// more unread messages on socket?
	if (tcpSocket->bytesAvailable() > 0)
		readNew();
}

void CoClient::editClients(miMessage msg) {
	vector<miString> common = msg.common.split(":");
	if (common.size()!=2)
		return;

	int id = atoi(common[0].cStr());
	string type = common[1].cStr();

	if (msg.command == qmstrings::newclient) {
		clients.erase(id);
		clients[id] = type;
		LOG4CXX_INFO(logger, "Added new client of type " << type << " and id " << id << " to the list of clients");

		emit newClient(type);
		emit addressListChanged();
	} else if (msg.command == qmstrings::removeclient) {
		clients.erase(id);
		LOG4CXX_INFO(logger, "Removed client of type " << type << " and id " << id << " from the list of clients");

		emit newClient("myself");
		emit addressListChanged();
	} else {
		LOG4CXX_ERROR(logger, "Error editing client list");
	}
}

void CoClient::sendType() {
	LOG4CXX_INFO(logger, "Connected to server"); ///< sendType() triggered by connected()-signal

	miMessage msg(0, 0, "SETTYPE", "INTERNAL");
	msg.data.push_back(clientType);
	sendMessage(msg);
	emit connected();
}

bool CoClient::sendMessage(miMessage &msg, const char* sep) {
	bool hasReciever = false;
	
	if (tcpSocket->state() == QTcpSocket::ConnectedState) {
		map<int, string>::iterator it;
		if(msg.to != 0 && msg.to != -1) { ///< if the message is for the server or is a broadcast, do not do anything
			
			for(it = clients.begin(); it != clients.end(); ++it) {
				if((int)it->first == msg.to) {
					hasReciever = true;
					break;
				}
			}
			
		}

		/// if msg does not contain a valid receiver address, broadcast it to all clients
		if(!hasReciever)
			msg.to = -1;
		
#ifdef _DEBUG
		cout << "miMessage in CoClient::sendMessage() (SEND)"<< endl;
		cout << msg.content() << endl;
#endif

		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);

		// send message to server
		out << (quint32)0;

		out << msg.to;
		// msg.from is set by server-side socket
		out << msg.command.cStr();
		out << msg.description.cStr();
		out << msg.commondesc.cStr();
		out << msg.common.cStr();
		out << msg.clientType.cStr();
		out << msg.co.cStr();
		out << quint32(msg.data.size()); // NOT A FIELD IN MIMESSAGE (TEMP ONLY)
#ifdef _DEBUG
		cout << "Size of data in last sent msg: " << msg.data.size() << endl;
#endif
		for (int i = 0; i < msg.data.size(); i++) {
			out << msg.data[i].cStr();
		}

		out.device()->seek(0);
		out << (quint32)(block.size() - sizeof(quint32));

		tcpSocket->write(block);
		tcpSocket->waitForBytesWritten();
		return true;
	} else {
		LOG4CXX_ERROR(logger, "Error sending message");
		return false;
	}
}

string CoClient::getClientName(int id) {
	return clients[id];
}

bool CoClient::clientTypeExist(const string& type) {
	map<int, string>::iterator it;
	for (it = clients.begin(); it != clients.end(); it++)
		if ((*it).second == type)
			return true;

	return false;
}

void CoClient::checkServer() {
	if (server->readAllStandardOutput() == "Started")
		connectToServer();
}

void CoClient::slotWriteStandardError() {
	LOG4CXX_ERROR(logger, server->readAllStandardError().data());
}

void CoClient::socketError(QAbstractSocket::SocketError e) {
	/// try this only once
	if(noCoserver4) return;
	
	LOG4CXX_INFO(logger, "Starting coserver...");

	server = new QProcess();
	QString cmd = QString(serverCommand.cStr());
	QStringList args = QStringList("-d"); ///< -d for dynamicMode

	connect(server, SIGNAL(readyReadStandardOutput()), SLOT(checkServer()));
	connect(server, SIGNAL(readyReadStandardError()), SLOT(slotWriteStandardError()));

	server->start(cmd, args);

	// make sure that coserver has time to start before checking its state
	sleep(1);

	if (server->state() == QProcess::NotRunning) {
		LOG4CXX_ERROR(logger, "Couldn't start server. Make sure the path of coserver4 is correctly set in the setup of your client, and try again.");
		server->kill();
		noCoserver4 = true;
		return;
	}

	LOG4CXX_INFO(logger, "coserver started");
	
	tcpSocket->connectToHost(QString(host.cStr()), port);
}
