#ifndef LANSERVERTHREAD_H
#define LANSERVERTHREAD_H
#include <QThread>
#include <QAbstractSocket>
class LanServerSocket;
class LanServerThread : public QThread{
	Q_OBJECT
private:
	int SocketID;
	LanServerSocket* tcpSocket;
public:
	LanServerThread(int SockDesc,QObject* parent=0);
	void run(){exec();}
	void stop(){exit(0);}
signals:
	void ChatMessageRecieved(QString msg);
	void SendMessage(QString msg);
	void ReachedEnd(int SocID);
private slots:
	void SendReachedEnd(){emit ReachedEnd(SocketID);}
};
#endif
