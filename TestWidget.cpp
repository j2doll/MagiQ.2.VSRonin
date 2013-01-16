#include "TestWidget.h"
TestWidget::TestWidget(QWidget* parent)
	:QWidget(parent)
{
	setMinimumSize(200,200);
	Server=new LanServer(this);
	Server->StartListening();
	Client=new LanClient(this);
	connect(Client,SIGNAL(ChatMessageRecieved(QString)),this,SLOT(RiceviMex(QString)));
	ConnectButton=new QPushButton(this);
	ConnectButton->setText("Connect");
	connect(ConnectButton,SIGNAL(clicked()),Client,SLOT(ConnectToHost()));
	MessageDisplay=new QTextEdit(this);
	MessageWrite=new QLineEdit(this);
	MessageWrite->setText("");
	SendButton=new QPushButton(this);
	SendButton->setText("Send");
	connect(SendButton,SIGNAL(clicked()),this,SLOT(InviaMex()));
	QVBoxLayout* MainLay=new QVBoxLayout(this);
	MainLay->addWidget(ConnectButton);
	MainLay->addWidget(MessageDisplay);
	MainLay->addWidget(MessageWrite);
	MainLay->addWidget(SendButton);
}
void TestWidget::RiceviMex(QString a){
	MessageWrite->insert("<br>"+a);
}
void TestWidget::InviaMex(){
	Client->SendChatMessage(MessageWrite->text());
	MessageWrite->setText("");
}