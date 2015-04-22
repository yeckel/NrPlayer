#include "netclient.h"

NetClient::NetClient(QObject *parent) : QObject(parent)
{
    serverUrl = settings.value("Main/ServerURL","https://www.nuvelar.com/remote/index.php/").toString();
    settings.setValue("Main/ServerURL",serverUrl);
}

NetClient::~NetClient()
{

}

QByteArray NetClient::requestServer(const QJsonObject jsonPostData,const QString resource)
{
    QByteArray responseData;
    QEventLoop eventLoop;
    QNetworkAccessManager manager;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req;
    req.setUrl(QUrl(serverUrl+"/"+resource));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    QByteArray data = QJsonDocument(jsonPostData).toJson();
    QByteArray base64Data = data.toBase64();
    QByteArray requestHash = calcHmac(data);
    QByteArray postData("data=" + base64Data+"&requestHash="+requestHash);
    QScopedPointer<QNetworkReply> reply(manager.post(req,postData));

    eventLoop.exec();

    if (reply.data()->error() == QNetworkReply::NoError) {
        responseData = reply.data()->readAll();
        return responseData;
    }
    else {
        responseData = QByteArray::number(reply.data()->error());
        qDebug() << "Failure" <<reply.data()->errorString() << "data:" << responseData;
    }

    return responseData;
}

QString NetClient::authenticate(const QString pairingCode)
{
    QJsonObject jsonPostData
    {
        {"pairingId", encondeId(pairingCode)},
    };

    QByteArray reply = requestServer(jsonPostData,"connect");
    QJsonDocument loadDoc(QJsonDocument::fromJson(reply));
    QJsonObject json = loadDoc.object();
    return json["playerId"].toString();
}

QSharedPointer<Playlist> NetClient::downloadPlaylist(const QString playerId)
{
    //qDebug() << Q_FUNC_INFO;
    QByteArray jsonData = "{\"media\":[],\"playlists\":[],\"resources\":[]}";

    QJsonDocument jd = QJsonDocument::fromJson(jsonData);
    QJsonObject jsonPostData
    {
        {"playerId",playerId},
        {"fileList",jd.object()}
    };

    QByteArray responseData = requestServer(jsonPostData,"getState");

    return QSharedPointer<Playlist>(new Playlist(responseData));

}

bool NetClient::downloadMediaFile(const QString filename, const QString playerId) {

    QJsonObject jsonPostData
    {
        {"playerId", playerId},
        {"fileName", filename},
        {"fileType","media"}
    };

    QByteArray responseData = requestServer(jsonPostData,"downloadFile");
    if (saveMediaFile(responseData,filename))
        return true;
    return false;
}

bool NetClient::downloadFiles(const QList<QString> filesToDownload,const QString playerId) {
    //qDebug() << Q_FUNC_INFO;
    foreach (QString fileName, filesToDownload) {
        if (!downloadMediaFile(fileName,playerId)) {
            qDebug() << "Failed downloading:" << fileName;
            return false;
        }
    }
    return true;
}

