#include "playercontroller.h"
#include <QDebug>

PlayerController::PlayerController(QObject *parent) :
    QObject(parent),controllerState(INIT_FROM_FS)
{
    ControllerStatus initialState = INIT_FROM_FS;
    pairingCode = QString::number(createRandomPlayerId());

    setupDataStore();

    if (!settings.contains("Main/UpdateInterval"))
        settings.setValue("Main/UpdateInterval",10000); //10s

    if (initialState == INIT_FROM_FS && !settings.contains("Main/playerId"))
        initialState = NOT_AUTHENTICATED;

    playerId = settings.value("Main/playerId").toString();

    if (initialState == INIT_FROM_FS && !settings.contains("Main/playlistId"))
        initialState = WITHOUT_PLAYLIST;

    updateTimer.setInterval(settings.value("Main/UpdateInterval",10000).toUInt());
    updateTimer.setSingleShot(true);
    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(timerUpdate()));

    connect(this,SIGNAL(stateUpdated(ControllerStatus)),this,SLOT(changeContollerState(ControllerStatus)));
    emit(stateUpdated(initialState));
}

PlayerController::~PlayerController()
{

}
void PlayerController::timerUpdate()
{
    qDebug() << "Update Tick";    
    changeContollerState(controllerState);
}

void PlayerController::changeContollerState(PlayerController::ControllerStatus newStatus)
{
    qDebug() << "Old state:" << controllerState << " new state:" << newStatus;
    controllerState = newStatus;

    switch(newStatus) {
    case NOT_AUTHENTICATED:
        if (authenticate())
            emit(stateUpdated(WITHOUT_PLAYLIST));
        else
            updateTimer.start();
        break;
    case WITHOUT_PLAYLIST:
        player.showSyncMessage();
        if (requestPlaylistAndMedia())
            emit(stateUpdated(READY_TO_PLAY));
        else
            updateTimer.start();
        break;
    case INIT_FROM_FS:
        if (loadPlaylistFromFS())
            emit(stateUpdated(READY_TO_PLAY));
        else
            emit(stateUpdated(WITHOUT_PLAYLIST));
        break;
    case READY_TO_PLAY:
        if (!playlist.isNull() && player.play(playlist.data()))
             emit(stateUpdated(PLAYING));
        break;
    case PLAYING:
    {
        QScopedPointer<Playlist> newPlaylist(netClient.downloadPlaylist(playerId));
        if (playlist.data()->isDifferent(newPlaylist.data())){
            emit (stateUpdated(WITHOUT_PLAYLIST));
        }else
            updateTimer.start();
    }
        break;
    default:
        break;
    }
}

bool PlayerController::requestPlaylistAndMedia() {
    playlist = QSharedPointer<Playlist>(netClient.downloadPlaylist(settings.value("Main/playerId","").toString()));
    if (playlist.data()->playlistIsValid()) {
        settings.setValue("Main/playlistId",QString(playlist.data()->getPlaylistId()));
        savePlaylist(playlist);
        if (makeMediaFilesReady())
            return true;
    }
    return false;
}

bool PlayerController::authenticate() {    
    player.showAuthCode(pairingCode);
    QString pairingId = netClient.authenticate(pairingCode);
    if (pairingId != "") {
        settings.setValue("Main/playerId",pairingId);
        playerId = pairingId;
        return true;
    }
    return false;
}

bool PlayerController::loadPlaylistFromFS() {
    QString currentPlaylistId = settings.value("Main/playlistId").toString();
    playlist = QSharedPointer<Playlist>(loadSavedPlaylist(currentPlaylistId));
    if (!playlist.isNull() && playlist.data()->playlistIsValid()){
        if (makeMediaFilesReady())
            return true;
    }
    return false;
}

bool PlayerController::makeMediaFilesReady() {
    //qDebug() << Q_FUNC_INFO;
    QList<QString> fileList = playlist.data()->listMediaFiles();
    QList<QString> missingFiles = missingMediaFiles(fileList);
    if ( netClient.downloadFiles(missingFiles,settings.value("Main/playerId").toString()) ) {
        return true;
    }
    return false;
}
