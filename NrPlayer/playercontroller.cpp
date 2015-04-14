#include "playercontroller.h"
#include <QDebug>

PlayerController::PlayerController(QObject *parent) : QObject(parent)
{
    controllerState = INIT_FROM_FS;
    setupDataStore();

    if (!settings.contains("Main/UpdateInterval"))
        settings.setValue("Main/UpdateInterval",10000); //10s

    if (controllerState == INIT_FROM_FS && !settings.contains("Main/playerId"))
        controllerState = NOT_AUTHENTICATED;

    if (controllerState == INIT_FROM_FS && !settings.contains("Main/playlistId"))
        controllerState = WITHOUT_PLAYLIST;

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
    switch(controllerState) {
    case NOT_AUTHENTICATED:
        authenticate();
        break;
    case WITHOUT_PLAYLIST:
        requestPlaylist();
        break;
    case INIT_FROM_FS:
        loadPlaylistFromFS();
        break;
    case PLAY:
        break;
    default:
        break;
    }
    updateTimer.start();
}

void PlayerController::requestPlaylist() {
    playlist = QSharedPointer<Playlist>(netClient.downloadPlaylist(settings.value("Main/playerId","").toString()));
    if (playlist.data()->playlistIsValid()) {
        settings.setValue("Main/playlistId",QString(playlist.data()->getPlaylistId()));
        savePlaylist(playlist);
        if (makeMediaFilesReady())
            controllerState = PLAY;
    }
}

void PlayerController::authenticate() {
    QString pairingCode("21121");
    player.showAuthCode(pairingCode);
    QString pairingId = netClient.authenticate(pairingCode);
    if (pairingId != "") {
        settings.setValue("Main/playerId",pairingId);
        controllerState = WITHOUT_PLAYLIST;
    }
}

void PlayerController::loadPlaylistFromFS() {
    QString currentPlaylistId = settings.value("Main/playlistId").toString();
    playlist = QSharedPointer<Playlist>(loadSavedPlaylist(currentPlaylistId));
    if (playlist.data()->playlistIsValid())
        if (makeMediaFilesReady())
            controllerState = PLAY;
}

bool PlayerController::makeMediaFilesReady() {
    QList<QString> fileList = playlist.data()->listMediaFiles();
    QList<QString> missingFiles = missingMediaFiles(fileList);
    if ( netClient.downloadFiles(missingFiles,settings.value("Main/playerId").toString()) ) {
        return true;
    }
    return false;
}
