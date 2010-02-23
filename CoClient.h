/** @mainpage qUtilities - coserver client file
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

#ifndef _COCLIENT
#define _COCLIENT


// Qt-includes
#include <qapplication.h>
#include <qstring.h>
#include <QTcpSocket>
#include <QProcess>
#include <QDialog>

#include <vector>
#include <map>

#include "miMessage.h"

class CoClient : public QDialog {
	Q_OBJECT

protected:

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
   * Send message to all CoServer client
   */
  void setBroadcastClient();

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
        std::string getClientName(int);
        bool clientTypeExist(const std::string &type);

private:
	QTcpSocket *tcpSocket;

	miutil::miString clientType;
	miutil::miString lockFile, serverCommand, host;

	quint32 blockSize;
	quint16 port;
	QProcess *server;
	miutil::miString userid;

        bool noCoserver4;
	int nrOfAttempts;

        std::vector<miMessage> inbox;
        std::map<int, std::string> clients;

	/**
	 * Adds or removes entries in the list of clients as clients
	 * connects and disconnects.
	 * @param msg Message containing client info
	 */
	void editClients(miMessage msg);

  	/**
  	 * Read port from file. File has to be located in ~/.diana/diana.port and only contain the port number.
  	 */
  	int readPortFromFile();

  	/**
  	 * Parses /etc/services and extracts portnumber based on username. (env variable USER)
  	 * Format in /etc/services
  	 * diana-<username>    <port>/tcp    # comment
  	 */
  	int readPortFromFile_Services();

signals:
  	void receivedMessage(miMessage &);
  	void addressListChanged();
  	void connected();
  	void newClient(miutil::miString);
  	void unableToConnect();

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
