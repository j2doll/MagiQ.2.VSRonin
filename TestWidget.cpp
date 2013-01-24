#include "TestWidget.h"
#include "ChatWidget.h"
TestWidget::TestWidget(QWidget* parent)
	:QWidget(parent)
{
	setMinimumSize(200,200);
	Server=new LanServer(this);
	Server->StartListening();
	Client=new LanClient(this);

	board=new BattleGround(this);
	board->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);


	IPEditor=new QLineEdit(this);
	QRegExpValidator* validatore=new QRegExpValidator(QRegExp("^localhost|(([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5]))$"),IPEditor);
	IPEditor->setValidator(validatore);
	IPEditor->setText("localhost");
	connect(IPEditor,SIGNAL(textEdited(QString)),Client,SLOT(SetHostIP(QString)));
	ConnectButton=new QPushButton(this);
	ConnectButton->setText("Connect");
	connect(ConnectButton,SIGNAL(clicked()),Client,SLOT(ConnectToHost()));
	JoinButton=new QPushButton(this);
	JoinButton->setText("Join");
	connect(JoinButton,SIGNAL(clicked()),Client,SLOT(SendJoinRequest()));
	DisconnectButton=new QPushButton(this);
	DisconnectButton->setText("Leave");
	connect(DisconnectButton,SIGNAL(clicked()),Client,SLOT(disconnectFromHost()));

	chat=new ChatWidget(this);

	connect(chat,SIGNAL(OutgoingMessage(QString)),Client,SLOT(SendChatMessage(QString)));
	connect(Client,SIGNAL(ChatMessageRecieved(QString)),chat,SLOT(IncomingMesage(QString)));
	connect(Client,SIGNAL(Disconnected()),chat,SLOT(Disconnected()));
	connect(Client,SIGNAL(UserJoined(QString)),chat,SLOT(AnotherJoin(QString)));
	connect(JoinButton,SIGNAL(clicked()),chat,SLOT(Connected()));
	connect(Client,SIGNAL(UserLeft(QString)),chat,SLOT(AnotherLeave(QString)));
	connect(Client,SIGNAL(MyNameAndColor(QString,QColor)),chat,SLOT(SetNameColor(QString,QColor)));
	connect(Client,SIGNAL(error(QAbstractSocket::SocketError)),chat,SLOT(DisplayNetworkErrors(QAbstractSocket::SocketError)));

	QGridLayout* MainLay=new QGridLayout(this);
	MainLay->addWidget(board,0,0,1,2);
	MainLay->addWidget(IPEditor,1,0);
	MainLay->addWidget(ConnectButton,2,0);
	MainLay->addWidget(JoinButton,3,0);
	MainLay->addWidget(DisconnectButton,4,0);
	MainLay->addWidget(chat,1,1,4,1);
}