#include "filesytem.h"


bool savePlaylist(QSharedPointer<Playlist> playlist)
{
    QByteArray playlistJson = playlist.data()->toJson();
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QFile playlit_file(dataPath+"/"+playlist.data()->getPlaylistId()+".json");
    if (!playlit_file.open(QIODevice::WriteOnly)){
        qDebug() << "File Write error";
        return false;
    }
    playlit_file.write(playlistJson);
    return true;
}


void setupDataStore()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
}
