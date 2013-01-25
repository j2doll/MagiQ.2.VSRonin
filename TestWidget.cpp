#include "TestWidget.h"
TestWidget::TestWidget(QWidget* parent)
	:QWidget(parent)
{
	setMinimumSize(200,200);
	Server=new LanServer(this);
	Server->StartListening();
	Client=new LanClient(this);
	LanClient* WhoCares=new LanClient(this);
	board=new BattleGround;
	board->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

	CardData Montagna;
	Montagna.SetCardName("Mountain");
	Montagna.SetHasManaCost(false);
	Montagna.SetAvailableBackgrounds(Constants::CardBacksrounds::LandRed);
	Montagna.SetCardBackground(Constants::CardBacksrounds::LandRed);
	Montagna.AddAvailableEditions(Constants::Editions::AVR);
	Montagna.AddCardType(Constants::CardTypes::Basic);
	Montagna.AddCardType(Constants::CardTypes::Land);
	Montagna.AddCardSubType(Constants::CardSubTypes::Mountain);
	CardDeck Grimorio;
	for(int i=0;i<60;i++) Grimorio.AddCard(Montagna);
	Client->SetDeck(Grimorio);
	WhoCares->SetDeck(Grimorio);

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
	ReadyButton=new QPushButton(this);
	ReadyButton->setText("Ready");
	connect(ReadyButton,SIGNAL(clicked()),Client,SLOT(SendReady()));

	chat=new ChatWidget(this);

	//Chat Connections
	connect(chat,SIGNAL(OutgoingMessage(QString)),Client,SLOT(SendChatMessage(QString)));
	connect(Client,SIGNAL(ChatMessageRecieved(QString)),chat,SLOT(IncomingMesage(QString)));
	connect(Client,SIGNAL(Disconnected()),chat,SLOT(Disconnected()));
	connect(Client,SIGNAL(UserJoined(QString)),chat,SLOT(AnotherJoin(QString)));
	connect(Client,SIGNAL(UserLeft(QString)),chat,SLOT(AnotherLeave(QString)));
	connect(Client,SIGNAL(MyNameAndColor(QString,QColor)),chat,SLOT(Connected()));
	connect(Client,SIGNAL(MyNameAndColor(QString,QColor)),chat,SLOT(SetNameColor(QString,QColor)));
	connect(Client,SIGNAL(error(QAbstractSocket::SocketError)),chat,SLOT(DisplayNetworkErrors(QAbstractSocket::SocketError)));

	//BattleGroundConnection
	connect(Client,SIGNAL(PlayOrder(QList<int>)),board,SLOT(SetPlayersOrder(QList<int>)));
	connect(Client,SIGNAL(PlayOrder(QList<int>)),board,SLOT(show()));
	connect(Client,SIGNAL(MyHand(QList<CardData>)),board,SLOT(SetMyHand(QList<CardData>)));
	connect(Client,SIGNAL(OtherHand(int,int)),board,SLOT(SetOtherHand(int,int)));
	connect(Client,SIGNAL(MyLibrary(QList<CardData>)),board,SLOT(SetMyLibrary(QList<CardData>)));
	connect(Client,SIGNAL(OtherLibrary(int,int)),board,SLOT(SetOtherLibrary(int,int)));
	connect(Client,SIGNAL(MyHand(QList<CardData>)),board,SLOT(AskMulligan()));
	connect(board,SIGNAL(Mulligan()),Client,SLOT(SendMulligan()));
	connect(board,SIGNAL(KeepHand()),Client,SLOT(SendHandAccepted()));

	QGridLayout* MainLay=new QGridLayout(this);
	MainLay->addWidget(IPEditor,0,0);
	MainLay->addWidget(ConnectButton,1,0);
	MainLay->addWidget(JoinButton,2,0);
	MainLay->addWidget(ReadyButton,3,0);
	MainLay->addWidget(DisconnectButton,4,0);
	MainLay->addWidget(chat,0,1,5,1);

	WhoCares->ConnectToHost();
	WhoCares->SendJoinRequest();
	WhoCares->SendReady();
}