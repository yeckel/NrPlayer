#include "filesytem.h"


bool savePlaylist(QSharedPointer<Playlist> playlist)
{
    QByteArray playlistJson = playlist->toJson();
    QString dataPath = getDataPath();

    QFile playlist_file(dataPath+"/"+playlist->getPlaylistId()+".json");
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


QSharedPointer<Playlist> loadSavedPlaylist(const QString playlistId)
{
    QString dataPath = getDataPath();

    QFile playlist_file(dataPath+"/"+playlistId+".json");
    if (!playlist_file.open(QIODevice::ReadOnly)) {
        qDebug() << "File Open error";
        return QSharedPointer<Playlist>(NULL);
    }
    QByteArray playlistData(playlist_file.readAll());
    return QSharedPointer<Playlist>(new Playlist(playlistData));

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
