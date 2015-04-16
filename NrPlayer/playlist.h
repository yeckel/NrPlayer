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
    QByteArray toQML() const;

    QList<QString> listMedia(const QList<QString> mediaTypes, const QString attribute = "id") const;
    bool isDifferent(const Playlist *b) const;
signals:

public slots:
private:
    QJsonDocument playlistFull;
    QJsonDocument playlistInfo;
    QByteArray playlistId;
    uint version;
    bool isValid;
    QByteArray addQmlHeader(QByteArray &qml) const;
    QByteArray addQmlImages(QByteArray &qml) const;
    QByteArray addQmlWebs(QByteArray &qml) const;
    QByteArray addQmlVideos(QByteArray &qml) const;
    QByteArray addQmlStates(QByteArray &qml) const;
    QByteArray addQmlTimer(QByteArray &qml) const;

    QString loadTemplate(const QString templateFileName) const;
    int findPreviousResource(const QJsonArray &resourcesArray,const int levelIndex) const;
    QList<QString> known_media;
    QString getTargetId(const QJsonObject &mediaResource) const;
};

#endif // PLAYLIST_H
