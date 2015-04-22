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

bool VisualPlayer::play(const QSharedPointer<Playlist> playlist)
{
    //TODO check if qml returned by toQML is valid qml
    QByteArray qml = playlist->toQML();
    QString qmlFileName = playlist->getPlaylistId()+".qml";
    if ( saveMediaFile(qml,qmlFileName) ){
        qDebug() << "switching playlist";
        QString qmlFilePath = getDataPath(qmlFileName);
        view->engine()->clearComponentCache();
        view->setSource(QUrl(qmlFilePath));        
        return true;
    }
    return false;
}

