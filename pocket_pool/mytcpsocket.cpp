#include "mytcpsocket.h"
#include "game.h"
#include "QMutex"
#include "QThread"
#include "QTcpServer"
#include "QTcpSocket"
#include "QVector"
#include "QByteArray"
#include "QString"
extern Game *game;
MyTcpSocket::MyTcpSocket(QObject *parent) :
    QObject(parent)
{
}

void MyTcpSocket::doConnect()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    // this is not blocking call
    socket->connectToHost("192.168.50.1", 10000);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
    hasvalue = false;
    count = 0;
}

void MyTcpSocket::connected()
{
    qDebug() << "connected...";
    int ballx;
    int bally;
    ballx = game->balls[0]->x();
    bally = game->balls[0]->y();
    QString zuobiao;
    zuobiao.append(QString::number(ballx));
    zuobiao.append(" ");
    zuobiao.append(QString::number(bally));
    // Hey server, tell me about you.
    socket->waitForBytesWritten(1000);
    socket->write(zuobiao.toLatin1());
}

void MyTcpSocket::disconnected()
{
    qDebug() << "disconnected...";
}

void MyTcpSocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void MyTcpSocket::readyRead()
{
    qDebug() << "reading...";

    QString tcpzuobiao;
    tcpzuobiao = socket->readAll();//read data from socket
    qDebug() << "Received data:" << tcpzuobiao;
    if(tcpzuobiao.length() != 0){
        int x;
        int y;
        x = tcpzuobiao.split(" ")[0].toInt();
        y = tcpzuobiao.split(" ")[1].toInt();
        game->cursor->setPos(x,y);
        if(tcpzuobiao.split(" ")[2].toDouble()!=0.0 ||tcpzuobiao.split(" ")[3].toDouble()!=0.0){

            game->tracker.hitValue.first = tcpzuobiao.split(" ")[2].toDouble() - 20.0;
            game->tracker.hitValue.second = tcpzuobiao.split(" ")[3].toDouble() - 20.0;
            game->tracker.startTracking();
            hasvalue = true;
        }
    }
    // read the data from the socket
    //qDebug() << socket->readAll();
    //return socket->readAll();
}
