#include "playercontroller.h"
#include <QDebug>

PlayerController::PlayerController(QObject *parent) : QObject(parent)
{
    if (!settings.contains("Main/UpdateInterval"))
        settings.setValue("Main/UpdateInterval",10000); //10s

    if (!settings.contains("Main/playerId"))
        controllerState = NOT_AUTHENTICATED;

    if (!settings.contains("Main/playlistId"))
        controllerState = WITHOUT_PLAYLIST;

    setupDataStore();

    updateTimer.setInterval(settings.value("Main/UpdateInterval",10000).toUInt());
    updateTimer.setSingleShot(true);
    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(update()));
    updateTimer.start();
    update();
}

PlayerController::~PlayerController()
{

}
void PlayerController::update()
{
    qDebug() << "Update Tick";
    switch(controllerState){
    case NOT_AUTHENTICATED:
        authenticate();
        break;
    case WITHOUT_PLAYLIST:
        requestPlaylist();
            break;
        default:
            break;
    }
    updateTimer.start();
}

void PlayerController::requestPlaylist(){
    playlist = QSharedPointer<Playlist>(netClient.donwloadPlaylist(settings.value("Main/playerId","").toString()));
    if (playlist.data()->playlistIsValid()){
        controllerState = NEW_PLAYLIST;
        settings.setValue("Main/playlistId",QString(playlist.data()->getPlaylistId()));
        savePlaylist(playlist);
    }
}

void PlayerController::authenticate()
{
    QString pairingCode("21121");
    player.showAuthCode(pairingCode);
    QString pairingId = netClient.authenticate(pairingCode);
    if (pairingId != ""){
        settings.setValue("Main/playerId",pairingId);
        controllerState = WITHOUT_PLAYLIST;
    }
}

