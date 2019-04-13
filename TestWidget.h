#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QtGlobal>
#include <QList>
#include <QtGui>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>

#include "LanClient.h"
#include "LanServer.h"
//#include "TestWidgetThread.h"
#include "BattleGround.h"
#include "ChatWidget.h"
#include "CardData.h"
#include "StackDisplayer.h"

class TestWidget : public QWidget
{
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
	//TestWidgetThread* ServerThread;
public:
	TestWidget(QWidget* parent=0);
};
#endif
