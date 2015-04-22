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

void PlayerController::getUpdates()
{
    {
        try{
            auto newPlaylist = netClient.downloadPlaylist(playerId);
            if (playlist->isDifferent(newPlaylist)){
                emit (stateUpdated(WITHOUT_PLAYLIST));
            }else
                updateTimer.start();
        }catch(AuthExeption &e){
            emit (stateUpdated(NOT_AUTHENTICATED));
        }
    }
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
        requestPlaylistAndMedia();
        break;
    case INIT_FROM_FS:
        if (loadPlaylistFromFS())
            emit(stateUpdated(READY_TO_PLAY));
        else
            emit(stateUpdated(WITHOUT_PLAYLIST));
        break;
    case READY_TO_PLAY:
        if (!playlist.isNull() && player.play(playlist))
             emit(stateUpdated(PLAYING));
        break;
    case PLAYING:
        getUpdates();
        break;
    default:
        break;
    }
}

void PlayerController::requestPlaylistAndMedia() {
    try{
        playlist = netClient.downloadPlaylist(settings.value("Main/playerId","").toString());
    }catch(AuthExeption &e){
        emit (stateUpdated(NOT_AUTHENTICATED));
        return;
    }
    if (playlist->playlistIsValid()) {
        settings.setValue("Main/playlistId",QString(playlist->getPlaylistId()));
        savePlaylist(playlist);
        if (makeMediaFilesReady()){
            emit(stateUpdated(READY_TO_PLAY));
            return;
        }
    }
    updateTimer.start();
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
    playlist = loadSavedPlaylist(currentPlaylistId);
    if (!playlist.isNull() && playlist->playlistIsValid()){
        if (makeMediaFilesReady())
            return true;
    }
    return false;
}

bool PlayerController::makeMediaFilesReady() {
    //qDebug() << Q_FUNC_INFO;
    QList<QString> fileList = playlist->listMediaFiles();
    QList<QString> missingFiles = missingMediaFiles(fileList);
    if ( netClient.downloadFiles(missingFiles,settings.value("Main/playerId").toString()) ) {
        return true;
    }
    return false;
}
