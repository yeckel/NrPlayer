#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QJsonObject>
#include <QStandardPaths>
#include <QList>

class Playlist : public QObject
{
    Q_OBJECT
public:
    explicit Playlist(QObject *parent = 0);
    Playlist(const QByteArray &jsonData, QObject *parent = 0);
    bool playlistIsValid() {
        return isValid;
    }
    ~Playlist();

    QByteArray getPlaylistId() const {
        return playlistId;
    }
    uint getVersion() const {
        return version;
    }
    QByteArray toJson() const;
    QList<QString> listMediaFiles();

signals:

public slots:
private:
    QJsonDocument playlistFull;
    QJsonDocument playlistInfo;
    QByteArray playlistId;
    uint version;
    bool isValid;
};

#endif // PLAYLIST_H
