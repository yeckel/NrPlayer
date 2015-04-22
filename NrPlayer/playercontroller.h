#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
#include <QTimer>
#include "visualplayer.h"
#include "playlist.h"
#include "netclient.h"
#include "filesytem.h"

class PlayerController : public QObject
{
    Q_OBJECT
    enum ControllerStatus {INIT_FROM_FS,NOT_AUTHENTICATED, WITHOUT_PLAYLIST, READY_TO_PLAY, PLAYING};
public:
    explicit PlayerController(QObject *parent = 0);
    ~PlayerController();

    void getUpdates();
signals:
    void stateUpdated(ControllerStatus status);
public slots:
    void timerUpdate();
private slots:
    void changeContollerState(ControllerStatus newStatus);
private:
    QTimer updateTimer;
    QSettings settings;
    NetClient netClient;
    ControllerStatus controllerState;
    VisualPlayer player;    
    QSharedPointer<Playlist> playlist;
    QString pairingId;
    QString playerId;
    QString pairingCode;

    bool authenticate();


    void requestPlaylistAndMedia();
    bool loadPlaylistFromFS();
    bool makeMediaFilesReady();    
};

#endif // PLAYERCONTROLLER_H
