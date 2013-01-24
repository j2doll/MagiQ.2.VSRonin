#ifndef TESTWIDGET_H
#define TESTWIDGET_H
#include <QtGui>
#include "LanClient.h"
#include "LanServer.h"
#include "BattleGround.h"
#include "ChatWidget.h"
class TestWidget : public QWidget{
	Q_OBJECT
private:
	QPushButton* ConnectButton;
	QPushButton* JoinButton;
	QPushButton* DisconnectButton;
	QLineEdit* IPEditor;
	LanServer* Server;
	LanClient* Client;
	ChatWidget* chat;
	BattleGround* board;
public:
	TestWidget(QWidget* parent=0);
};
#endif