#ifndef FILESYTEM_H
#define FILESYTEM_H
#include <QtCore>

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QList>

#include "playlist.h"

void setupDataStore();
bool savePlaylist(QSharedPointer<Playlist> playlist);
Playlist *loadSavedPlaylist(const QString playlistId);
bool saveMediaFile(const QByteArray &data, const QString filename);
QList<QString> missingMediaFiles(const QList<QString> fileList);
QString getDataPath();

#endif // FILESYTEM_H
