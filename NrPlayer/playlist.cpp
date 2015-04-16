#include "playlist.h"
#include "filesytem.h"
#include <QCryptographicHash>

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
    known_media = {"Image","Video","WWW"};
    isValid = false;
    playlistFull = QJsonDocument::fromJson(jsonData);
    QJsonObject json = playlistFull.object();

    QByteArray playlistInfoStr = json["playlistInfo"].toString().toUtf8();
    QJsonParseError error;
    playlistInfo = QJsonDocument::fromJson(playlistInfoStr,&error);
    //qDebug() << error.errorString();
    //qDebug() << playlistInfo.toJson();
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
    //qDebug() << "Playlist Deleted";
}


QByteArray Playlist::toJson() const
{
    return playlistFull.toJson();
}

QList<QString> Playlist::listMediaFiles()
{

    QList<QString> mediaTypes = {"Image","Video"};
    return listMedia(mediaTypes);
}

QByteArray Playlist::toQML() const
{
    QByteArray finalQml;
    addQmlHeader(finalQml);
    addQmlImages(finalQml);
    addQmlWebs(finalQml);
    addQmlVideos(finalQml);
    addQmlStates(finalQml);
    addQmlTimer(finalQml);
    finalQml.append("\n}");
    return finalQml;
}

QByteArray Playlist::addQmlHeader(QByteArray &qml) const
{
    QString header = loadTemplate(":/qml_stubs/playlist_header.txt");
    //header.replace(QString("%playlistId%"),QString(this->playlistId));
    qml = qml + header.toUtf8();
    return qml;
}

QByteArray Playlist::addQmlImages(QByteArray &qml) const
{
    QString imageTemplate = loadTemplate(":/qml_stubs/image.txt");

    QList<QString> images = listMedia(QList<QString>({QString("Image")}));
    foreach (QString imageId, images) {
        QString it(imageTemplate);
        it.replace(QString("%imageId%"),imageId);
        it.replace(QString("%fileName%"),getDataPath()+imageId);
        qml = qml + it.toUtf8();
    }
    return qml;
}

QByteArray Playlist::addQmlWebs(QByteArray &qml) const
{
    QString webTemplate = loadTemplate(":/qml_stubs/webview.txt");
    QList<QString> webs = listMedia(QList<QString>({QString("WWW")}),"url");
    foreach (QString webUrl, webs) {
        QString it(webTemplate);
        QString webId = QCryptographicHash::hash((webUrl.toUtf8()),QCryptographicHash::Md5).toHex();
        it.replace(QString("%webId%"),webId);
        it.replace(QString("%webUrl%"),webUrl);
        qml = qml + it.toUtf8();
    }
    return qml;
}

QByteArray Playlist::addQmlVideos(QByteArray &qml) const
{
     QString videoTemplate = loadTemplate(":/qml_stubs/video.txt");
     QList<QString> images = listMedia(QList<QString>({QString("Video")}));
     foreach (QString imageId, images) {
         QString it(videoTemplate);
         it.replace(QString("%videoId%"),imageId);
         it.replace(QString("%videoFile%"),getDataPath()+imageId);
         qml = qml + it.toUtf8();
     }
     return qml;
}

QByteArray Playlist::addQmlStates(QByteArray &qml) const
{
    qml += "states: [\n";
    QJsonArray resourcesArray = playlistInfo.object()["resources"].toArray();
    for (int levelIndex = 0; levelIndex < resourcesArray.size(); ++levelIndex) {
        QJsonObject mediaResource = resourcesArray[levelIndex].toObject();

        if (!mediaResource["active"].toBool())
            continue;

        if (!known_media.contains(mediaResource["type"].toString()))
            continue;

        const QString propertyChangeTemplate("PropertyChanges {target: id%target%\n opacity: %op% }\n");
        const QString timerPropertyTempalte("PropertyChanges {target: timer \n interval: %toNextTick% }\n");
        const QString startVideoTemplate("StateChangeScript{ script: id%videoId%.start(); }\n");
        const QString stopVideoTemplate("StateChangeScript{ script: id%videoId%.stop(); }\n");


        QString state("State {\n");

            QString actualTargetChange(propertyChangeTemplate);
            QString targetId = getTargetId(mediaResource);

            actualTargetChange.replace("%target%",targetId);
            actualTargetChange.replace("%op%","1");
            state.append(actualTargetChange);

            if (mediaResource["type"].toString() == "Video"){
                QString startVideo(startVideoTemplate);
                startVideo.replace(QString("%videoId%"),targetId);
                state.append(startVideo);
            }

            int previewsMediaResourceIndex = findPreviousResource(resourcesArray,levelIndex);
            if (previewsMediaResourceIndex != levelIndex ){
                QJsonObject previewsMediaResource = resourcesArray[previewsMediaResourceIndex].toObject();
                QString targetId = getTargetId(previewsMediaResource);
                QString previewsTargetChange(propertyChangeTemplate);

                previewsTargetChange.replace("%target%",targetId);
                previewsTargetChange.replace("%op%","0");
                state.append(previewsTargetChange);

                if (previewsMediaResource["type"].toString() == "Video"){
                    QString stopVideo(stopVideoTemplate);
                    stopVideo.replace(QString("%videoId%"),targetId);
                    state.append(stopVideo);
                }
            }

            QString timerChange(timerPropertyTempalte);
            int duration = mediaResource["duration"].toInt();
            timerChange.replace("%toNextTick%",QString("%1").arg(duration)+"000");
            state.append(timerChange);

        state.append("},\n");
        qml.append(state.toUtf8());
    }
    qml.remove(qml.size()-2,1); //remove the trailing ","
    qml += "]\n";
    return qml;
}

QByteArray Playlist::addQmlTimer(QByteArray &qml) const
{
    QString header = loadTemplate(":/qml_stubs/timer.txt");
    qml = qml + header.toUtf8();
    return qml;
}

QString Playlist::loadTemplate(const QString templateFileName) const
{
    QFile templateFile(templateFileName);
    templateFile.open(QFile::ReadOnly);
    QByteArray fileData(templateFile.readAll());
    return QString(fileData);
}

int Playlist::findPreviousResource(const QJsonArray &resourcesArray, const int levelIndex) const
{
    int size = resourcesArray.size();
    int position = levelIndex-1;
    int count = 0;
    while(1){
        if (position < 0)
            position = size-1;
        QJsonObject previousObject = resourcesArray[position].toObject();
        if (known_media.contains(previousObject["type"].toString()))
            return position;
        count++;
        if (count == size)
            return levelIndex; //if no other proper media found, return the same index
        position--;
    }
}

QList<QString> Playlist::listMedia(const QList<QString> mediaTypes, const QString attribute) const
{
    QList<QString> files;
    QJsonArray resourcesArray = playlistInfo.object()["resources"].toArray();
    for (int levelIndex = 0; levelIndex < resourcesArray.size(); ++levelIndex) {
        QJsonObject resource = resourcesArray[levelIndex].toObject();
        QString resourceType = resource["type"].toString();
        if ( mediaTypes.contains(resourceType)) {
            files << resource[attribute].toString();
        }
    }
    return files;
}

bool Playlist::isDifferent(const Playlist *b) const
{
    if (this->version != b->version)
        return true;
    if (this->playlistId != b->playlistId)
        return true;
    return false;
}


QString Playlist::getTargetId(const QJsonObject &mediaResource) const
{
    QString targetId;
    if (mediaResource["type"].toString() == "WWW"){
        QString webUrl = mediaResource["url"].toString();
        targetId = QCryptographicHash::hash((webUrl.toUtf8()),QCryptographicHash::Md5).toHex();
    }
    else
        targetId = mediaResource["id"].toString();
    return targetId;
}
