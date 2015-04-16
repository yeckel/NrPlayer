#ifndef VISUALPLAYER_H
#define VISUALPLAYER_H

#include <QObject>
#include <QtQuick>
#include "playlist.h"
#include "filesytem.h"

class VisualPlayer : public QObject
{
    Q_OBJECT
public:
    explicit VisualPlayer(QObject *parent = 0);
    ~VisualPlayer();

signals:

public slots:
    void showAuthCode(const QString pairingCode);
    bool play(const Playlist *playlist);
private:
    QQuickView *view;
};

#endif // VISUALPLAYER_H
