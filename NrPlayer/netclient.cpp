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

