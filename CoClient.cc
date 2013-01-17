/**
 * coclient - coserver client file
 * @author Martin Lilleeng S�tra <martinls@met.no>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif  /* HAVE_CONFIG_H */

#include "CoClient.h"
#include "QLetterCommands.h"

#include <puCtools/sleep.h>

#ifdef __WIN32__
// GetUserName()
#include <windows.h>
#include <lmcons.h>
#else
// uid_t and getpwnam()
#include <sys/types.h>
#include <pwd.h>
#endif

// Qt-includes
#include <QtGui>
#include <QtNetwork>
#include <QTcpSocket>
#include <qstring.h>
#include <qfile.h>

#ifdef HAVE_LOG4CXX   /* Defined in config.h */
#include <log4cxx/logger.h>
static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("coclient.CoClient");
#else  /* ! HAVE_LOG4CXX */
#include <miLogger/logger.h>
#endif /* HAVE_LOG4CXX */

#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>

//#define _DEBUG

namespace /* anonymous */ {
std::string safe_getenv(const char* var)
{
    const char* v = getenv(var);
    if (not v)
        return std::string();
    return v;
}
} // namespace anonymous

CoClient::CoClient(const char *name, const char *h, const char *sc, const char *lf, quint16 p)
    : tcpSocket(new QTcpSocket(this))
    , server(0)
{
  nrOfAttempts = 0;
  coserverStarted = false;

  /*
   * Uncomment this and comment the one below to read from etc/services instead of ~/.diana/diana.port

	if ((readPortFromFile_Services() > 0) || (port == 0)) {
		port = qmstrings::port;
	}
   */
  port = p;

  if ((readPortFromFile() > 0) && (port == 0)) {
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
    userid = getenv("USER");
  } else {
    userid = safe_getenv("USERNAME");
  }
  if (getenv("HOSTNAME") != NULL) {
      userid += "@" + safe_getenv("HOSTNAME");
  } else {
      userid += "@" + safe_getenv("COMPUTERNAME");
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
        std::stringstream ss;
        ss << "UnknownUser" << uid;
        userid = ss.str();
    }
  }
#endif

  blockSize = 0;


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

  const std::string homePath = safe_getenv("HOME");

#ifdef _DEBUG
  cerr << "homePath: " << homePath << endl;
#endif

  std::ifstream pfile((homePath + "/.coserver.port").c_str());
  if (not pfile.is_open()) {
#ifdef _DEBUG
      std::cerr << "Could not read coserver.port from file" << std::endl;
#endif
      return 1;
  }
  pfile >> port;
  if (not pfile) {
#ifdef _DEBUG
      std::cerr << "Could not read port number from file" << std::endl;
#endif
      return 1;
  }

#ifdef _DEBUG
  std::cerr << "Port: " << port << " read from file." << std::endl;
#endif
  return 0;
}

// etc/services: diana-<username>		<port>/tcp		# comment
int CoClient::readPortFromFile_Services() {

#ifdef _DEBUG
    std::cerr << "CoClient::readPortFromFile_Services()" << std::endl;
#endif

    const char filename[] = "/etc/services";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return 1;
    }
    
    const char* user = getenv("USER");
    if (!user) {
        std::cerr << "Environment variable 'USER' not set." << std::endl;
        return 1;
    }

    std::string line;
    while(std::getline(file, line)) {
        if (line.empty() or line.at(0) == '#' or line.at(0) == '\n' or line.at(0) == ' ')
            continue;
        
        boost::replace_all(line, "\t", " ");
        boost::replace_all(line, "/", " ");
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(" "));
        if (tokens.size() < 2 or tokens[1].empty())
            continue;
        std::vector<std::string> tokens2;
        boost::split(tokens2, tokens[1], boost::is_any_of("-"));
        if (tokens2.size() != 2 or tokens2[1].empty())
            continue;
        if (tokens2[1] != user)
            continue;
        port = std::atoi(tokens2[1].c_str());
    }
#ifdef _DEBUG
    std::cerr << "Port: " << port << " read from '" << filename << "'" << std::endl;
#endif
    return 0;
}

void CoClient::printBytesWritten(qint64 written) {
    std::cout << "Written "<< written << " bytes to socket" << std::endl;
}

bool CoClient::notConnected() {
  return (QAbstractSocket::UnconnectedState == tcpSocket->state());
}

void CoClient::connectionClosed() {
  LOG4CXX_INFO(logger, "Disconnected from server");
  emit disconnected();

}

void CoClient::connectToServer() {
  noCoserver4 = false;

#ifdef _DEBUG
  std::cerr << "connectToServer(): Connecting to port: " << port << std::endl;
#endif

  tcpSocket->connectToHost(QString::fromStdString(host), port);

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

  emit receivedMessage(msg);

  blockSize = 0;

  // more unread messages on socket?
  if (tcpSocket->bytesAvailable() > 0)
    readNew();
}

void CoClient::editClients(const miMessage& msg) {
    std::vector<std::string> common;
    boost::split(common, msg.common, boost::is_any_of(":"));
    if (common.size() < 2)
        return;

    const int id = atoi(common[0].c_str());
    const std::string& type = common[1];

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
    msg.commondesc = "userId";
    msg.common = userid;
    sendMessage(msg);
    emit connected();
}

bool CoClient::sendMessage(miMessage &msg, const char* sep) {
    bool hasReciever = false;
    
    if (tcpSocket->state() == QTcpSocket::ConnectedState) {
        std::map<int, std::string>::iterator it;
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
        std::cout << "miMessage in CoClient::sendMessage() (SEND)"<< std::endl;
        std::cout << msg.content() << std::endl;
#endif

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);

        // send message to server
        out << (quint32)0;
        
        out << msg.to;
        // msg.from is set by server-side socket
        out << QString::fromStdString(msg.command);
        out << QString::fromStdString(msg.description);
        out << QString::fromStdString(msg.commondesc);
        out << QString::fromStdString(msg.common);
        out << QString::fromStdString(msg.clientType);
        out << QString::fromStdString(msg.co);
        out << quint32(msg.data.size()); // NOT A FIELD IN MIMESSAGE (TEMP ONLY)
#ifdef _DEBUG
        std::cout << "Size of data in last sent msg: " << msg.data.size() << std::endl;
#endif
        for (unsigned int i = 0; i < msg.data.size(); i++) {
            out << QString::fromStdString(msg.data[i]);
        }

        out.device()->seek(0);
        out << (quint32)(block.size() - sizeof(quint32));
        
        tcpSocket->write(block);
        tcpSocket->waitForBytesWritten(250);
        return true;
    } else {
        LOG4CXX_ERROR(logger, "Error sending message");
        return false;
    }
}

const std::string& CoClient::getClientName(int id) {
    std::map<int,std::string>::const_iterator it = clients.find(id);
    if (it == clients.end()) {
        static const std::string empty;
        return empty;
    }
    return it->second;
}

bool CoClient::clientTypeExist(const std::string& type) {
    std::map<int, std::string>::iterator it;
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
#ifdef _DEBUG
    std::cerr << "slotWriteStandardError()" << std::endl;
#endif
    if (server)
        LOG4CXX_ERROR(logger, server->readAllStandardError().data());
}

void CoClient::socketError(QAbstractSocket::SocketError e)
{
    if (QAbstractSocket::ConnectionRefusedError == e) {
        qDebug() << "Found no running coserver";
        startCoServer_();
    } else if (QAbstractSocket::RemoteHostClosedError == e) {
        qDebug() << "Coserver unexpected shutdown or crash";
        startCoServer_();
    } else {
        qDebug() << "Error when contacting coserver: " << e;
    }
}

void CoClient::startCoServer_()
{
    /// try this only once
    if(noCoserver4)
        return;

  if (nrOfAttempts < 2) {
#ifdef _DEBUG
      std::cerr << "Starting coserver..." << std::endl;
#endif

      LOG4CXX_INFO(logger, "Starting coserver...");
      
      server = new QProcess(this);
      QString cmd = QString::fromStdString(serverCommand);

      QStringList args = QStringList("-d"); ///< -d for dynamicMode
    
      if (readPortFromFile() == 0) {
          args << "-p" << QString::number(port);
      } else {
          std::cerr << "Could not read port from file." << std::endl;
      }

      connect(server, SIGNAL(readyReadStandardOutput()), SLOT(checkServer()));
      connect(server, SIGNAL(readyReadStandardError()), SLOT(slotWriteStandardError()));
      
      server->start(cmd, args);

      // make sure that coserver has time to start before checking its state
      server->waitForStarted();
      pu_sleep(1);

      if (server->state() != QProcess::Running) {
          LOG4CXX_ERROR(logger, "Couldn't start server. Make sure the path of coserver4"
                        " is correctly set in the setup of your client, and try again.");
          
#ifdef _DEBUG
          std::cerr << "Couldn't start coserver4. Make sure the path of coserver4"
              " is correctly set in the setup of your client, and try again." << std::endl;;
#endif
          
          server->kill();
          noCoserver4 = true;
          return;
      }

      LOG4CXX_INFO(logger, "coserver started");
      
#ifdef _DEBUG
      std::cerr << "Connect to host with port " << port << std::endl;
#endif
      
      tcpSocket->connectToHost(QString::fromStdString(host), port);
      nrOfAttempts++;
      
  } else  {
      
      connect(server, SIGNAL(readyReadStandardOutput()), SLOT(checkServer()));
      
      std::cerr << "Connection attempt limit reached. Disconnecting..." << std::endl;
      tcpSocket->disconnectFromHost();
      server->kill();
      
      emit unableToConnect();
      
      noCoserver4 = true;
      return;
  }

}
