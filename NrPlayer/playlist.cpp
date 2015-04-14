#include "playlist.h"

Playlist::Playlist(QObject *parent)
{
    QByteArray emptyPlaylist = "{\
        \"filesToDelete\": {\
            \"mediaFiles\": [\
            ],\
            \"playlistFiles\": [],\
            \"resources\": []},\
        \"playlistInfo\": \"{}\"}";
    Playlist(emptyPlaylist,parent);
}

Playlist::Playlist(const QByteArray &jsonData, QObject *parent): QObject(parent)
{
//    qDebug() << Q_FUNC_INFO;
    isValid = false;
    playlistFull = QJsonDocument::fromJson(jsonData);
    QJsonObject json = playlistFull.object();

    QByteArray playlistInfoStr = json["playlistInfo"].toString().toLatin1();
    playlistInfo = QJsonDocument::fromJson(playlistInfoStr);

    if (playlistInfo.object().contains("id")) {
        playlistId = playlistInfo.object()["id"].toString().toLatin1();
        if (playlistId != "") {
            isValid = true;
            this->playlistId = playlistId;
        }
    } else {
        isValid = false;
    }
    if (playlistInfo.object().contains("version")) {
        version = playlistInfo.object()["version"].toInt();
    } else {
        isValid = false;
    }
//    qDebug() << playlistId << "  " << version;
}

Playlist::~Playlist()
{
    qDebug() << "Playlist Deleted";
}


QByteArray Playlist::toJson() const
{
    return playlistFull.toJson();
}

QList<QString> Playlist::listMediaFiles()
{
    QList<QString> files;
    QJsonArray resourcesArray = playlistInfo.object()["resources"].toArray();
    for (int levelIndex = 0; levelIndex < resourcesArray.size(); ++levelIndex) {
        QJsonObject resource = resourcesArray[levelIndex].toObject();
        QString resourceType = resource["type"].toString();
        if ( resourceType == "Image" || resourceType == "Video" ) {
            files << resource["id"].toString();
        } else {
            qDebug() << "Unhandled resource:" << resource["type"].toString();
        }

    }
    return files;
}


