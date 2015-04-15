#include "netclient.h"

NetClient::NetClient(QObject *parent) : QObject(parent)
{

}

NetClient::~NetClient()
{

}

QString NetClient::authenticate(const QString pairingCode)
{
    QEventLoop eventLoop;
    QNetworkAccessManager manager;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req;
    req.setUrl(QUrl("https://www.nuvelar.com/remote/index.php/connect"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QJsonObject pairingData
    {
        {"pairingId", encondeId(pairingCode)},
    };
    QByteArray data = QJsonDocument(pairingData).toJson();
    QByteArray base64Data = data.toBase64();
    QByteArray requestHash = calcHmac(data);
    QByteArray postData("data=" + base64Data+"&requestHash="+requestHash);
    QScopedPointer<QNetworkReply> reply(manager.post(req,postData));

    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply.data()->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply.data()->readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(responseData));
        QJsonObject json = loadDoc.object();
        return json["playerId"].toString();
    }
    else {
        qDebug() << "Failure" <<reply.data()->errorString();
    }
    return QString();
}

Playlist* NetClient::downloadPlaylist(const QString playerId)
{
    qDebug() << Q_FUNC_INFO;
    QEventLoop eventLoop;
    QNetworkAccessManager manager;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req;
    req.setUrl(QUrl("https://www.nuvelar.com/remote/index.php/getState"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    QByteArray jsonData = "{\"media\":[],\"playlists\":[],\"resources\":[]}";

    QJsonDocument jd = QJsonDocument::fromJson(jsonData);
    QJsonObject fileData
    {
        {"playerId",playerId},
        {"fileList",jd.object()}
    };

    QByteArray data = QJsonDocument(fileData).toJson();
    QByteArray base64Data = data.toBase64();
    QByteArray requestHash = calcHmac(data);
    QByteArray postData("data=" + base64Data+"&requestHash="+requestHash);

    QScopedPointer<QNetworkReply> reply(manager.post(req,postData));

    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply.data()->error() == QNetworkReply::NoError) {

        QByteArray responseData = reply.data()->readAll();

        return new Playlist(responseData);
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
    }
    return new Playlist();
}

bool NetClient::downloadMediaFile(const QString filename, const QString playerId) {
    QEventLoop eventLoop;
    QNetworkAccessManager manager;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req;
    req.setUrl(QUrl("https://www.nuvelar.com/remote/index.php/downloadFile"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QJsonObject fileData
    {
        {"playerId", playerId},
        {"fileName", filename},
        {"fileType","media"}
    };

    QByteArray data = QJsonDocument(fileData).toJson();
    QByteArray base64Data = data.toBase64();
    QByteArray requestHash = calcHmac(data);
    QByteArray postData("data=" + base64Data+"&requestHash="+requestHash);
    QScopedPointer<QNetworkReply> reply(manager.post(req,postData));

    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply.data()->error() == QNetworkReply::NoError) {
        if (saveMediaFile(reply.data()->readAll(),filename))
            return true;
    }
    else {
        qDebug() << "Failure" <<reply.data()->errorString();
    }
    return false;
}

bool NetClient::downloadFiles(const QList<QString> filesToDownload,const QString playerId) {
    qDebug() << Q_FUNC_INFO;
    foreach (QString fileName, filesToDownload) {
        if (!downloadMediaFile(fileName,playerId)) {
            qDebug() << "Failed downloading:" << fileName;
            return false;
        }
    }
    return true;
}

