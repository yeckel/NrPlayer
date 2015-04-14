#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
#include <QTimer>
#include "visualplayer.h"
#include "playlist.h"
#include "netclient.h"

class PlayerController : public QObject
{
    Q_OBJECT
    enum ControllerStatus {PLAY, NOT_AUTHENTICATED, SWITCH, AUTH};
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
    ControllerStatus cStatus;
    VisualPlayer player;

};

#endif // PLAYERCONTROLLER_H
