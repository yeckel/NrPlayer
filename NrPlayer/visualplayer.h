#ifndef VISUALPLAYER_H
#define VISUALPLAYER_H

#include <QObject>

class VisualPlayer : public QObject
{
    Q_OBJECT
public:
    explicit VisualPlayer(QObject *parent = 0);
    ~VisualPlayer();

signals:

public slots:
};

#endif // VISUALPLAYER_H
