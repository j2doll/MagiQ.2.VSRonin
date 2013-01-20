#include "TestWidget.h"
TestWidget::TestWidget(QWidget* parent)
	:QWidget(parent)
{
	setMinimumSize(200,200);
	Server=new LanServer(this);
	Server->StartListening();
	Client=new LanClient(this);
	connect(Client,SIGNAL(ChatMessageRecieved(QString)),this,SLOT(RiceviMex(QString)));
	connect(Client,SIGNAL(Disconnected()),this,SLOT(Disconnesso()));
	connect(Client,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(MostraErrori(QAbstractSocket::SocketError)));
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
	MessageDisplay->append(a);
}
void TestWidget::InviaMex(){
	Client->SendChatMessage(MessageWrite->text());
	MessageWrite->setText("");
}
void TestWidget::Disconnesso(){
	MessageDisplay->append("Client Disconnesso");
}

void TestWidget::MostraErrori(QAbstractSocket::SocketError socketError)
{
	switch (socketError) {
	case QAbstractSocket::RemoteHostClosedError:
		break;
	case QAbstractSocket::HostNotFoundError:
		MessageDisplay->append("The host was not found. Please check the "
			"host name and port settings.");
		break;
	case QAbstractSocket::ConnectionRefusedError:
		MessageDisplay->append("The connection was refused by the peer. "
			"Make sure the fortune server is running, "
			"and check that the host name and port "
			"settings are correct.");
		break;
	default:
		MessageDisplay->append(QString("The following error occurred: %1.").arg(Client->GetSocketErrorString()));
	}
}