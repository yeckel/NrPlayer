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

Playlist::Playlist(const QByteArray jsonData, QObject *parent): QObject(parent)
{
//    qDebug() << Q_FUNC_INFO;
    isValid = false;
    playlistDocument = QJsonDocument::fromJson(jsonData);
    QJsonObject json = playlistDocument.object();

    QByteArray playlistInfoStr = json["playlistInfo"].toString().toLatin1();
    playlistInfo = QJsonDocument::fromJson(playlistInfoStr);

    if (playlistInfo.object().contains("id")){
        playlistId = playlistInfo.object()["id"].toString().toLatin1();
        if (playlistId != ""){
            isValid = true;
            this->playlistId = playlistId;
        }        
    } else { isValid = false;}
    if (playlistInfo.object().contains("version")){
        version = playlistInfo.object()["version"].toInt();
    }else { isValid = false;}
//    qDebug() << playlistId << "  " << version;
}

Playlist::~Playlist()
{
    qDebug() << "Playlist Deleted";
}


QByteArray Playlist::toJson() const
{
    return playlistInfo.toJson();
}


