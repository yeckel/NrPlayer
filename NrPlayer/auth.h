#ifndef AUTH_H
#define AUTH_H

#include <QCryptographicHash>
#include <QtCore>

QByteArray calcHmac(QByteArray message);
QByteArray hmacSha256(QByteArray secretKey, const QByteArray& message);
QString encondeId(const QString id);
uint createRandomPlayerId();

#endif // AUTH_H
