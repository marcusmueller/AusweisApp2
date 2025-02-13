/**
 * Copyright (c) 2017-2023 Governikus GmbH & Co. KG, Germany
 */

#include "LocalIfdServer.h"

#include "AppSettings.h"
#include "Env.h"
#include "PortFile.h"

using namespace governikus;


LocalIfdServer::LocalIfdServer()
	: IfdServer()
	, mWebSocketServer(Env::create<LocalWebSocketServer*>())
{
	connect(mWebSocketServer.data(), &LocalWebSocketServer::fireMessageHandlerAdded, this, &IfdServer::fireMessageHandlerAdded);
	connect(mWebSocketServer.data(), &LocalWebSocketServer::fireConnectedChanged, this, &IfdServer::fireConnectedChanged);
}


void LocalIfdServer::setPsk(const QByteArray& pPsk)
{
	mWebSocketServer->setPsk(pPsk);
}


bool LocalIfdServer::isRunning() const
{
	return mWebSocketServer->isListening();
}


bool LocalIfdServer::start(const QString& pServerName)
{
	bool success = mWebSocketServer->listen(pServerName, PortFile::cDefaultPort);
	Q_EMIT fireIsRunningChanged();
	return success;
}


void LocalIfdServer::stop()
{
	mWebSocketServer->close();
	Q_EMIT fireIsRunningChanged();
}


void LocalIfdServer::setPairing(bool pEnable)
{
	Q_UNUSED(pEnable)
}


bool LocalIfdServer::isConnected() const
{
	return mWebSocketServer->isConnected();
}


bool LocalIfdServer::isPairingConnection() const
{
	return false;
}


QSslCertificate LocalIfdServer::getCurrentCertificate() const
{
	return QSslCertificate();
}


const QSharedPointer<ServerMessageHandler>& LocalIfdServer::getMessageHandler() const
{
	Q_ASSERT(mWebSocketServer);
	return mWebSocketServer->getMessageHandler();
}


bool LocalIfdServer::isLocal() const
{
	return true;
}
