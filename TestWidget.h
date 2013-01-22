#ifndef TESTWIDGET_H
#define TESTWIDGET_H
#include <QtGui>
#include "LanClient.h"
#include "LanServer.h"
class TestWidget : public QWidget{
	Q_OBJECT
private:
	QPushButton* ConnectButton;
	QPushButton* JoinButton;
	QPushButton* DisconnectButton;
	QPushButton* SendButton;
	QLineEdit* IPEditor;
	LanServer* Server;
	LanClient* Client;
	QLineEdit* MessageWrite;
	QTextEdit* MessageDisplay;
public:
	TestWidget(QWidget* parent=0);
private slots:
	void InviaMex();
	void RiceviMex(QString a);
	void Disconnesso();
	void MostraErrori(QAbstractSocket::SocketError socketError);
	void StampaServerInfo(QString nam,int gamemode,int legal,int minp,int maxp,int currp);
	void StampaLeftGame(QString a);
};
#endif