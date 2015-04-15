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
    QByteArray toQML();

    QList<QString> listMedia(const QList<QString> mediaTypes, const QString attribute = "id");
signals:

public slots:
private:
    QJsonDocument playlistFull;
    QJsonDocument playlistInfo;
    QByteArray playlistId;
    uint version;
    bool isValid;
    QByteArray addQmlHeader(QByteArray &qml);
    QByteArray addQmlImages(QByteArray &qml);
    QByteArray addQmlWebs(QByteArray &qml);
    QByteArray addQmlVideos(QByteArray &qml);
    QByteArray addQmlStates(QByteArray &qml);
    QByteArray addQmlTimer(QByteArray &qml);

    QString loadTemplate(const QString templateFileName);
    int findPreviousResource(const QJsonArray &resourcesArray,const int levelIndex);
    QList<QString> known_media;
    QString getTargetId(const QJsonObject &mediaResource);
};

#endif // PLAYLIST_H
