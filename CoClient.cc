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

// TODO: Add support for multiple servers active on different ports (on the same node)
// TODO: Add support for multiple clients per server

#ifdef __WIN32__
// GetUserName()
#include <windows.h>
#include <lmcons.h>
#else
// uid_t and getpwnam()
#include <sys/types.h>
#include <pwd.h>
#endif

#include <unistd.h>

#include <fstream>
#include <iostream>

// Qt-includes
#include <QtGui>
#include <QtNetwork>
#include <QString>
#include <QFile>

#include <puCtools/sleep.h>

#include <QLetterCommands.h>
#include <CoClient.h>
#include <unistd.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif  /* HAVE_CONFIG_H */

#ifdef HAVE_LOG4CXX   /* Defined in config.h */
#include <log4cxx/logger.h>

static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("coclient.CoClient");

#else  /* ! HAVE_LOG4CXX */
#include <miLogger/logger.h>
#endif /* HAVE_LOG4CXX */

using namespace std;
using namespace miutil;

//#define _DEBUG

CoClient::CoClient(const char *name, const char *h,
    const char *sc, const char *lf, quint16 p)
{
  shell = NULL;
  server = NULL;
  nrOfAttempts = 0;
  coserverStarted = false;

  /*
   * Uncomment this and comment the one below to read from etc/services instead of ~/.diana/diana.port

	if ((readPortFromFile_Services() > 0) || (port == 0)) {
		port = qmstrings::port;
	}
   */

  if ((readPortFromFile() > 0) || (port == 0)) {
    port = qmstrings::port;
  }

  clientType = name;

  lockFile = lf;
  serverCommand = sc ;
  host = h;
  if (getenv("COSERVER_HOST") != NULL) {
    host = getenv("COSERVER_HOST");
  }
  if (getenv("USER") != NULL) {
    userid = QString(getenv("USER"));
  } else {
    userid = QString(getenv("USERNAME"));
  }
  if (getenv("HOSTNAME") != NULL) {
    userid += "@" + QString(getenv("HOSTNAME"));
  } else {
    userid += "@" + QString(getenv("COMPUTERNAME"));
  }
#ifdef __WIN32__
  {
    DWORD size = UNLEN + 1;
    CHAR name[size];
    if (!GetUserNameA(name, &size)) {
      cerr << "GetUserNameA() failed" << endl;
      userid = "UnknownWin32User";
    } else {
      userid = name;
    }
  }
#else
  {
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (pw) {
      userid = pw->pw_name;
    } else {
    	QString data;
    	data.append("UnknownUser");
        userid = data;
    }
  }
#endif

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

void CoClient::setBroadcastClient() {
  userid = "";
}

int CoClient::readPortFromFile() {
#ifdef _DEBUG
  cerr << "CoClient::readPortFromFile()" << endl;
#endif

  QString homePath = QString(getenv("HOME"));

#ifdef _DEBUG
  cerr << "homePath: " << homePath << endl;
#endif

  FILE *pfile;
  char fileContent[10];

  QString pFilePath = homePath + QString("/.coserver.port");

  pfile = fopen(pFilePath.toLatin1().constData(), "r");
  if (pfile == NULL) {
#ifdef _DEBUG
    cerr << "Could not read file " << endl;
#endif
    return 1;
  } else {
    fgets(fileContent, 10, pfile);
    puts(fileContent);
    fclose(pfile);
    port = QString(fileContent).toInt();

#ifdef _DEBUG
    cerr << "Port: " << port << " read from file." << endl;
#endif
  }
  return 0;
}

// etc/services: diana-<username>		<port>/tcp		# comment
int CoClient::readPortFromFile_Services() {

#ifdef _DEBUG
  cerr << "CoClient::readPortFromFile_Services()" << endl;
#endif

  QString filename = "/etc/services";
  QFile file(filename);
  QString line;
  QStringList tokens;
  QString user = QString(getenv("USER"));
  int port = 0;

  if (!file.open(QFile::ReadOnly |QIODevice::Text)) {
    cerr << "Could not open file: " << filename.toStdString() << endl;
    return 1;
  } else {
    QTextStream in(&file);
    while(!in.atEnd()) {
    	line = in.readLine();
      if (line.size() && line.at(0) != '#' && line.at(0) != '\n' && line.at(0) != ' ') {
        line.replace("\t", " ");
        line.replace("/", " ");
        tokens = line.split(" ", QString::SkipEmptyParts);
        if ((tokens.size() > 0)  && (tokens[1].size() > 0) && (tokens[0].contains("-")) &&
            (user == tokens[0].split("-", QString::SkipEmptyParts)[1])) {
          this->port = tokens[1].toInt(0);
        }
      }
    }
#ifdef _DEBUG
    cerr << "Port: " << port << " read from: " << filename << endl;
#endif
  }
  file.close();
  return 0;
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

#ifdef _DEBUG
  cerr << "connectToServer(): Connecting to port: " << port << endl;
#endif

  tcpSocket->connectToHost(host, port);

}

void CoClient::shellFinished( int exitCode, QProcess::ExitStatus exitStatus ) {
#ifdef _DEBUG
  cerr << "shellFinished( " << exitCode << ", " << exitStatus << " )" << endl;
#endif
  delete shell;
  shell = NULL;
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
  msg.command = tmpcommand;
  in >> tmpdescription;
  msg.description = tmpdescription;
  in >> tmpcommondesc;
  msg.commondesc = tmpcommondesc;
  in >> tmpcommon;
  msg.common = tmpcommon;
  in >> tmpclientType;
  msg.clientType = tmpclientType;
  in >> tmpco;
  msg.co = tmpco;
  in >> size; // NOT A FIELD IN MIMESSAGE (METADATA ONLY)
  for (int i = 0; i < size; i++) {
    in >> tmpdata;
    msg.data.push_back(tmpdata);
  }

#ifdef _DEBUG
  cout << "miMessage in CoClient::readNew() (RECV)"<< endl;
  cout << msg.content() << endl;
#endif

  // if origin is the server itself, then it will always be a request to
  // change (add/delete) entries in the list of clients
  if (msg.from == 0)
    editClients(msg);

  emit receivedMessage(msg);

  blockSize = 0;

  // more unread messages on socket?
  if (tcpSocket->bytesAvailable() > 0)
    readNew();
}

void CoClient::editClients(miMessage msg) {
  QStringList common = msg.common.split(":");
  if (common.size()!=2)
    return;

  int id = common[0].toInt();
  QString type = common[1];

  if (msg.command == qmstrings::newclient) {
    clients.erase(id);
    clients[id] = type;
    LOG4CXX_INFO(logger, "Added new client of type " << type.toStdString() << " and id " << id << " to the list of clients");

    emit newClient(type);
    emit addressListChanged();
  } else if (msg.command == qmstrings::removeclient) {
    clients.erase(id);
    LOG4CXX_INFO(logger, "Removed client of type " << type.toStdString() << " and id " << id << " from the list of clients");

    emit newClient(QString("myself"));
    emit newClient(miString("myself"));
    emit addressListChanged();
  } else {
    LOG4CXX_ERROR(logger, "Error editing client list");
  }
}

void CoClient::sendType() {
  LOG4CXX_INFO(logger, "Connected to server"); ///< sendType() triggered by connected()-signal

  miMessage msg(0, 0, "SETTYPE", "INTERNAL");
  msg.data.push_back(clientType);
  msg.commondesc = "userId";
  msg.common = userid;
  sendMessage(msg);
  emit connected();
}

bool CoClient::sendMessage(miMessage &msg, const char* sep) {
  bool hasReciever = false;

  if (tcpSocket->state() == QTcpSocket::ConnectedState) {
    map<int, QString>::iterator it;
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
    out << msg.command;
    out << msg.description;
    out << msg.commondesc;
    out << msg.common;
    out << msg.clientType;
    out << msg.co;
    out << quint32(msg.data.size()); // NOT A FIELD IN MIMESSAGE (TEMP ONLY)
#ifdef _DEBUG
    cout << "Size of data in last sent msg: " << msg.data.size() << endl;
#endif
    for (int i = 0; i < msg.data.size(); i++) {
      out << msg.data[i];
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

QString CoClient::getClientName(int id) {
  return clients[id];
}

bool CoClient::clientTypeExist(const QString& type) {
  map<int, QString>::iterator it;
  for (it = clients.begin(); it != clients.end(); it++)
    if ((*it).second == type)
      return true;

  return false;
}

void CoClient::checkServer() {
  if (server->readAllStandardOutput() == "Started")
    connectToServer();
}

void CoClient::checkServerRunning() {
#ifdef _DEBUG
  cerr << "checkServerRunning()" << endl;
#endif
  QByteArray result = shell->readAllStandardOutput();
  shellresult= shellresult + QString(result);
}

void CoClient::slotWriteStandardError() {
#ifdef _DEBUG
  cerr << "slotWriteStandardError()" << endl;
#endif
  if (server)
    LOG4CXX_ERROR(logger, server->readAllStandardError().data());
}

void CoClient::slotWriteCheckStandardError() {
#ifdef _DEBUG
  cerr << "slotWriteCheckStandardError()" << endl;
#endif
  if (shell)
  {
    LOG4CXX_ERROR(logger, shell->readAllStandardError().data());
    shell->terminate();
  }
}

void CoClient::socketError(QAbstractSocket::SocketError e) {
  /// try this only once
  if(noCoserver4) return;

  if (nrOfAttempts < 2) {

#ifdef _DEBUG
    cerr << "Starting coserver..." << endl;
#endif

    LOG4CXX_INFO(logger, "Starting coserver...");

    server = new QProcess();
    QString cmd(serverCommand);

    QStringList args = QStringList("-d"); ///< -d for dynamicMode

    QString strPort;
    strPort.setNum(port);

    if (readPortFromFile() == 0) {
      args << "-p" << strPort;
    } else {
      cerr << "Could not read port from file." << endl;
    }

    connect(server, SIGNAL(readyReadStandardOutput()), SLOT(checkServer()));
    connect(server, SIGNAL(readyReadStandardError()), SLOT(slotWriteStandardError()));

    server->start(cmd, args);

    // make sure that coserver has time to start before checking its state
    server->waitForStarted();
    pu_sleep(1);

    if (server->state() != QProcess::Running) {
      LOG4CXX_ERROR(logger, "Couldn't start server. Make sure the path of coserver4 is correctly set in the setup of your client, and try again.");

#ifdef _DEBUG
      cerr << "Couldn't start coserver4. Make sure the path of coserver4 is correctly set in the setup of your client, and try again." << endl;;
#endif

      server->kill();
      noCoserver4 = true;
      return;
    }

    LOG4CXX_INFO(logger, "coserver started");

#ifdef _DEBUG
    cerr << "Connect to host with port " << port << endl;
#endif

    tcpSocket->connectToHost(host, port);
    nrOfAttempts++;

  } else  {

    connect(server, SIGNAL(readyReadStandardOutput()), SLOT(checkServer()));

    cerr << "Connection attempt limit reached. Disconnecting..." << endl;
    tcpSocket->disconnectFromHost();
    server->kill();

    emit unableToConnect();

    noCoserver4 = true;
    return;
  }

}
