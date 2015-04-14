#include "auth.h"

QByteArray calcHmac(QByteArray message) {
    QByteArray key = "cKUTvJ8OXQRK49IvTaW94pqnTdPJ5l6f";
    QByteArray hmac = hmacSha256(key,message).toBase64();

    return hmac;
}

QByteArray hmacSha256(QByteArray secretKey, const QByteArray& message) {

    QCryptographicHash::Algorithm alg = QCryptographicHash::Sha256;

    int blockSize = 64; // HMAC-SHA-1 & SHA-256 Blocksize

    if (secretKey.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
        secretKey = QCryptographicHash::hash(secretKey, alg);
    }


    if (secretKey.length() < blockSize) {
        int padding = blockSize - secretKey.length();
        secretKey.append(QByteArray(padding, char(0x00)));
    }

    QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
    QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "\"



    for (int i = 0; i < secretKey.length(); i++) {
        innerPadding[i] = innerPadding[i] ^ secretKey.at(i); // XOR operation between every byte in key and innerpadding, of key length
        outerPadding[i] = outerPadding[i] ^ secretKey.at(i); // XOR operation between every byte in key and outerpadding, of key length
    }


    QByteArray total = outerPadding;
    QByteArray part = innerPadding;

    part.append(message);
    total.append(QCryptographicHash::hash(part, alg));

    QByteArray hashed = QCryptographicHash::hash(total, alg);

    QByteArray replaced = hashed.toHex();

    return replaced;
}

QString encondeId(const QString id) {
    QByteArray hashedId = QCryptographicHash::hash(id.toLatin1(), QCryptographicHash::Sha256);
    return hashedId.toHex();
}

uint createRandomPlayerId() {
    const int LOW = 11111;
    const int HIGH = 99999;
    return (qrand() % ((HIGH + 1) - LOW) + LOW);
}
