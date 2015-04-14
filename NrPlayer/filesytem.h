#ifndef FILESYTEM_H
#define FILESYTEM_H
#include <QtCore>

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

#include "playlist.h"
void setupDataStore();
bool savePlaylist(QSharedPointer<Playlist> playlist);

#endif // FILESYTEM_H
