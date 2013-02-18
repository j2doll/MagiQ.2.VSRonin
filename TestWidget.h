#ifndef TESTWIDGET_H
#define TESTWIDGET_H
#include <QtGui>
#include "LanClient.h"
#include "LanServer.h"
#include "BattleGround.h"
#include "ChatWidget.h"
#include "CardData.h"
#include "StackDisplayer.h"
#include <QList>
class TestWidget : public QWidget{
	Q_OBJECT
private:
	QPushButton* ConnectButton;
	QPushButton* JoinButton;
	QPushButton* DisconnectButton;
	QPushButton* ReadyButton;
	QLineEdit* IPEditor;
	LanClient* Client;
	ChatWidget* chat;
	BattleGround* board;
	QFrame* BattleFrame;
	StackDispalyer* StackDisp;
	LanServer* Server;
public:
	TestWidget(QWidget* parent=0);
};
#endif