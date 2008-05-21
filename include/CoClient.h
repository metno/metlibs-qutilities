/** @mainpage qUtilities - coserver client file
 * @author Martin Lilleeng Sætra <martinls@met.no>
 * 
 * $Id: CoClient.h 3 2007-09-28 10:40:54Z martinls $
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

#ifndef _COCLIENT
#define _COCLIENT


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif	/* HAVE_CONFIG_H */

// Qt-includes
#include <qapplication.h>
#include <qstring.h>
#include <QTcpSocket>
#include <QProcess>
#include <QDialog>

#include <vector>
#include <map>

#ifdef HAVE_LOG4CXX		/* Defined in config.h */
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/level.h>
#else  /* ! HAVE_LOG4CXX */
#include <miLogger/logger.h>
#endif /* HAVE_LOG4CXX */

#include "miMessage.h"

using namespace std;

class CoClient : public QDialog {
	Q_OBJECT

protected:
#ifdef HAVE_LOG4CXX
    log4cxx::LoggerPtr logger;
#endif  

public:
	/**
	 * CoClient.
	 */
	CoClient(QWidget* parent,
			const char * name, 
			const char * h, 
			const char * sc, 
			const char * lf = "/tmp/.serverlock",
			quint16 p = 0);

	/**
	 * Connects to the server.
	 */
	void connectToServer(void);
  
	/**
	 * Disconnects from the server.
	 */
	void disconnectFromServer(void);
  
	/**
	 * Sends a message to the server.
	 * @param msg The message
	 * @param sep Seperator character
	 * @return Returns true upon successful sending, false otherwise
	 */
	bool sendMessage(miMessage &msg, const char *sep = "|");
  
	bool notConnected(void);
	string getClientName(int);
	bool clientTypeExist(const string &type);
	
private:
	QTcpSocket *tcpSocket;

	miString clientType;
	miString lockFile, serverCommand, host;

	quint32 blockSize;
	quint16 port;
	QProcess *server;
	
	bool noCoserver4;

	vector<miMessage> inbox;
	map<int, string> clients;
  
	/**
	 * Adds or removes entries in the list of clients as clients
	 * connects and disconnects.
	 * @param msg Message containing client info
	 */
	void editClients(miMessage msg);
  
signals:
  	void receivedMessage(miMessage &);
  	void addressListChanged();
  	void connected();
  	void newClient(miString);
  
private slots:
	/**
 	* Read new incoming message.
 	*/
  	void readNew();
  
  	/**
  	 * Identifies which type of client this is to the server (by sending a message).
  	 */
  	void sendType();
  
  	/**
  	 * Starts new session if coserver outputs "Started".
  	 */
  	void checkServer();
  
  	/**
  	 * Called when disconnected from server.
  	 */
  	void connectionClosed();
  
  	/**
  	 * Logs new errors when they are available on coserver's stderr. (DEPRECATED?)
  	 */
  	void slotWriteStandardError();
  
  	/**
  	 * Starts coserver if not already running.
  	 * @param e Error signal (not used)
  	 */
  	void socketError(QAbstractSocket::SocketError e);
  
  	/**
  	 * Debugging function.
  	 * Only used if _DEBUG is defined.
  	 * @param written The number of bytes written to socket
  	 */
  	void printBytesWritten(qint64 written);
};

#endif
