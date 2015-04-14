#ifndef NETCLIENT_H
#define NETCLIENT_H

#include <QObject>

class NetClient : public QObject
{
    Q_OBJECT
public:
    explicit NetClient(QObject *parent = 0);
    ~NetClient();

signals:

public slots:
};

#endif // NETCLIENT_H
