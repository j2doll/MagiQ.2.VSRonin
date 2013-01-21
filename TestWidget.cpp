#include "TestWidget.h"
#include "ChatWidget.h"
TestWidget::TestWidget(QWidget* parent)
	:QWidget(parent)
{
	setMinimumSize(200,200);
	Server=new LanServer(this);
	Server->StartListening();
	connect(Server,SIGNAL(LeftTheGame()),this,SLOT(StampaLeftGame()));
	Client=new LanClient(this);
	connect(Client,SIGNAL(ChatMessageRecieved(QString)),this,SLOT(RiceviMex(QString)));
	connect(Client,SIGNAL(Disconnected()),this,SLOT(Disconnesso()));
	connect(Client,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(MostraErrori(QAbstractSocket::SocketError)));
	connect(Client,SIGNAL(ServerInfos(QString,int,int,int,int,int)),this,SLOT(StampaServerInfo(QString,int,int,int,int,int)));
	ConnectButton=new QPushButton(this);
	ConnectButton->setText("Join");
	connect(ConnectButton,SIGNAL(clicked()),Client,SLOT(SendJoinRequest()));
	DisconnectButton=new QPushButton(this);
	DisconnectButton->setText("Leave");
	//DisconnectButton->hide();
	connect(DisconnectButton,SIGNAL(clicked()),Client,SLOT(disconnectFromHost()));
	//connect(ConnectButton,SIGNAL(clicked()),DisconnectButton,SLOT(show()));
	MessageDisplay=new QTextEdit(this);
	MessageWrite=new QLineEdit(this);
	MessageWrite->setText("");
	SendButton=new QPushButton(this);
	SendButton->setText("Send");
	connect(SendButton,SIGNAL(clicked()),this,SLOT(InviaMex()));
	QVBoxLayout* MainLay=new QVBoxLayout(this);
	MainLay->addWidget(ConnectButton);
	MainLay->addWidget(DisconnectButton);
	MainLay->addWidget(MessageDisplay);
	MainLay->addWidget(MessageWrite);
	MainLay->addWidget(SendButton);
	Client->ConnectToHost();
	ChatWidget* chat=new ChatWidget;
	connect(chat,SIGNAL(OutgoingMessage(QString)),Client,SLOT(SendChatMessage(QString)));
	connect(Client,SIGNAL(ChatMessageRecieved(QString)),chat,SLOT(IncomingMesage(QString)));
	connect(Client,SIGNAL(Disconnected()),chat,SLOT(Disconnected()));
	chat->show();
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
void TestWidget::StampaServerInfo(QString nam,int gamemode,int legal,int minp,int maxp,int currp){
	MessageDisplay->append(QString("Server Informations:<br>"
		"Name: %1<br>"
		"Game Mode: %2<br>"
		"Decks Format: %3<br>"
		"Minimum Players: %4<br>"
		"Maximum Players: %5<br>"
		"Current Players: %6<br>")
		.arg(nam)
		.arg(gamemode)
		.arg(legal)
		.arg(minp)
		.arg(maxp)
		.arg(currp));
}
void TestWidget::StampaLeftGame(){
	MessageDisplay->append("Un Giocatore ha lasciato la partita");
}