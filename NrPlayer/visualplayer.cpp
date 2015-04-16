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

void VisualPlayer::showSyncMessage()
{
    view->setSource(QUrl(QStringLiteral("qrc:/synchronizing.qml")));

}

bool VisualPlayer::play(const Playlist *playlist)
{
    QByteArray qml = playlist->toQML();
    QString qmlFileName = playlist->getPlaylistId()+".qml";
    if ( saveMediaFile(qml,qmlFileName) ){
        QString qmlFilePath = getDataPath(qmlFileName);
        view->setSource(QUrl(qmlFilePath));
        return true;
    }
    return false;
}

