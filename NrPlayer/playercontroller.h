#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>

class PlayerController : public QObject
{
    Q_OBJECT
public:
    explicit PlayerController(QObject *parent = 0);
    ~PlayerController();

signals:

public slots:
};

#endif // PLAYERCONTROLLER_H
