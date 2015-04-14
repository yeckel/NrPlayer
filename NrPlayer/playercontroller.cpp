#include "playercontroller.h"
#include <QDebug>

PlayerController::PlayerController(QObject *parent) : QObject(parent)
{
    if (!settings.contains("Main/UpdateInterval"))
        settings.setValue("Main/UpdateInterval",10000); //10s

    if (!settings.contains("Main/playerId"))
        cStatus = NOT_AUTHENTICATED;

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
    switch(cStatus){
    case NOT_AUTHENTICATED:{
        QString pairingCode("21121");
        player.showAuthCode(pairingCode);
        QString pairingId = netClient.authenticate(pairingCode);
        if (pairingId != ""){
            settings.setValue("Main/playerId",pairingId);
            cStatus = AUTH;
        }
    }
        break;
    default:
        break;
    }
    updateTimer.start();
}

