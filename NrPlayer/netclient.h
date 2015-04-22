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
#include "filesytem.h"

class NetClient : public QObject
{
    Q_OBJECT
public:
    explicit NetClient(QObject *parent = 0);
    ~NetClient();
    bool downloadMediaFile(const QString filename, const QString playerId);
    bool downloadFiles(const QList<QString> filesToDownload, const QString playerId);    

    QScopedPointer<QNetworkReply> requestServer();
signals:

public slots:
    QString authenticate(const QString pairingCode);
    QSharedPointer<Playlist> downloadPlaylist(const QString playerId);
private:
    QString serverUrl;
    QSettings settings;
    QByteArray requestServer(const QJsonObject jsonPostData, const QString resource);
};

#endif // NETCLIENT_H
