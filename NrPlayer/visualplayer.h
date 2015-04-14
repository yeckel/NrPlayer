#ifndef VISUALPLAYER_H
#define VISUALPLAYER_H

#include <QObject>
#include <QtQuick>

class VisualPlayer : public QObject
{
    Q_OBJECT
public:
    explicit VisualPlayer(QObject *parent = 0);
    ~VisualPlayer();

signals:

public slots:
    void showAuthCode(const QString pairingCode);
private:
    QQuickView *view;
};

#endif // VISUALPLAYER_H
