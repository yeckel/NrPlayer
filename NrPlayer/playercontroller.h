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
    enum ControllerStatus {NOT_AUTHENTICATED, WITHOUT_PLAYLIST, NEW_PLAYLIST};
public:
    explicit PlayerController(QObject *parent = 0);    
    ~PlayerController();

signals:

public slots:
    void update();
private:
    QTimer updateTimer;
    QSettings settings;
    NetClient netClient;
    ControllerStatus controllerState;
    VisualPlayer player;
    //Playlist *playlist;
    QSharedPointer<Playlist> playlist;

    void authenticate();


    void requestPlaylist();
};

#endif // PLAYERCONTROLLER_H
