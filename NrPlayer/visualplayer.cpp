#include "visualplayer.h"

VisualPlayer::VisualPlayer(QObject *parent) : QObject(parent)
{
    view = new QQuickView;
    view->showFullScreen();
}

VisualPlayer::~VisualPlayer()
{

}

void VisualPlayer::showAuthCode(const QString pairingCode)
{
    view->rootContext()->setContextProperty("pairingNummer",pairingCode);
    view->setSource(QUrl(QStringLiteral("qrc:/authentication.qml")));

}

