#include "filesytem.h"


bool savePlaylist(QSharedPointer<Playlist> playlist)
{
    QByteArray playlistJson = playlist.data()->toJson();
    QString dataPath = getDataPath();

    QFile playlist_file(dataPath+"/"+playlist.data()->getPlaylistId()+".json");
    if (!playlist_file.open(QIODevice::WriteOnly)) {
        qDebug() << "File Write error";
        return false;
    }
    playlist_file.write(playlistJson);
    return true;
}


void setupDataStore()
{
    QString dataPath = getDataPath();
    QDir().mkpath(dataPath);
}


Playlist *loadSavedPlaylist(const QString playlistId)
{
    QString dataPath = getDataPath();

    QFile playlist_file(dataPath+"/"+playlistId+".json");
    if (!playlist_file.open(QIODevice::ReadOnly)) {
        qDebug() << "File Open error";
        return NULL;
    }
    QByteArray playlistData(playlist_file.readAll());
    return new Playlist(playlistData);

}
bool saveMediaFile(const QByteArray &data,const QString filename) {
    QString dataPath = getDataPath();

    QFile file(dataPath+"/"+filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(data);
    file.close();
    return true;
}


QList<QString> missingMediaFiles(const QList<QString> fileList)
{
    QString dataPath = getDataPath();

    QList<QString> missingFiles;
    foreach (QString fileName, fileList) {
        if (!QFile(dataPath+"/"+fileName).exists())
            missingFiles << fileName;
    }
    return missingFiles;
}


QString getDataPath(const QString fileName)
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/"+fileName;
}
