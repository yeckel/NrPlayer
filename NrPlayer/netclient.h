#ifndef NETCLIENT_H
#define NETCLIENT_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QJsonObject>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

#include "auth.h"
#include "playlist.h"

class NetClient : public QObject
{
    Q_OBJECT
public:
    explicit NetClient(QObject *parent = 0);
    ~NetClient();

signals:

public slots:
    QString authenticate(const QString pairingCode);
    Playlist *donwloadPlaylist(const QString playerId);
private:
};

#endif // NETCLIENT_H
