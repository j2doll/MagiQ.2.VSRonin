#include "PlayerInfoDisplayer.h"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QBitmap>
#include <QProgressBar>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include "MagiQPlayer.h"
#include "StyleSheets.h"
#include "RoundedCornersLabel.h"
PlayerInfoDisplayer::PlayerInfoDisplayer(QWidget* parent)
	:QWidget(parent)
	,InfosToDisplay(NULL)
	,CurrentLife(0)
	,TargetLife(0)
{
	BackGround=new QFrame(this);
	BackGround->setObjectName("BackGround");

	NameLabel=new QLabel(this);
	NameLabel->setObjectName("NameLabel");
	NameLabel->setAlignment(Qt::AlignHCenter);
	NameLabel->setWordWrap(true);
	NameLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
	AvatarLabel=new RoundedCornersLabel(this);
	AvatarLabel->setObjectName("AvatarLabel");
	AvatarLabel->SetRadious(12);
	HandSizeLabel=new QLabel(this);
	HandSizeLabel->setObjectName("HandSizeLabel");
	HandSizeLabel->setAlignment(Qt::AlignCenter);
	HandSizeLabel->setToolTip(tr("Hand Size"));
	GraveyardLabel=new QLabel(this);
	GraveyardLabel->setObjectName("GraveyardLabel");
	GraveyardLabel->setAlignment(Qt::AlignCenter);
	GraveyardLabel->setToolTip(tr("Graveyard"));
	GraveyardLabel->installEventFilter(this);
	ExileLabel=new QLabel(this);
	ExileLabel->setObjectName("ExileLabel");
	ExileLabel->setAlignment(Qt::AlignCenter);
	ExileLabel->setToolTip(tr("Exile"));
	ExileLabel->installEventFilter(this);
	ManaPoolFrame=new QFrame(this);
	ManaPoolFrame->setObjectName("ManaPoolLabel");
	ManaPoolFrame->setToolTip(tr("Mana Pool"));
	WManaPoolLabel=new QLabel(ManaPoolFrame);
	WManaPoolLabel->setObjectName("WManaPoolLabel");
	WManaPoolLabel->setAlignment(Qt::AlignCenter);
	WManaPoolLabel->setToolTip(tr("White Mana"));
	WManaPoolLabel->installEventFilter(this);
	UManaPoolLabel=new QLabel(ManaPoolFrame);
	UManaPoolLabel->setObjectName("UManaPoolLabel");
	UManaPoolLabel->setAlignment(Qt::AlignCenter);
	UManaPoolLabel->setToolTip(tr("Blue Mana"));
	UManaPoolLabel->installEventFilter(this);
	BManaPoolLabel=new QLabel(ManaPoolFrame);
	BManaPoolLabel->setObjectName("BManaPoolLabel");
	BManaPoolLabel->setAlignment(Qt::AlignCenter);
	BManaPoolLabel->setToolTip(tr("Black Mana"));
	BManaPoolLabel->installEventFilter(this);
	RManaPoolLabel=new QLabel(ManaPoolFrame);
	RManaPoolLabel->setObjectName("RManaPoolLabel");
	RManaPoolLabel->setAlignment(Qt::AlignCenter);
	RManaPoolLabel->setToolTip(tr("Red Mana"));
	RManaPoolLabel->installEventFilter(this);
	GManaPoolLabel=new QLabel(ManaPoolFrame);
	GManaPoolLabel->setObjectName("GManaPoolLabel");
	GManaPoolLabel->setAlignment(Qt::AlignCenter);
	GManaPoolLabel->setToolTip(tr("Green Mana"));
	GManaPoolLabel->installEventFilter(this);
	CManaPoolLabel=new QLabel(ManaPoolFrame);
	CManaPoolLabel->setObjectName("CManaPoolLabel");
	CManaPoolLabel->setAlignment(Qt::AlignCenter);
	CManaPoolLabel->setToolTip(tr("Colorless Mana"));
	CManaPoolLabel->installEventFilter(this);

	QVBoxLayout* LifeLay=new QVBoxLayout;
	LifeLay->setMargin(0);
	LifeLay->setSpacing(2);
	LifeLabel=new QLabel(this);
	LifeLabel->setObjectName("LifeLabel");
	LifeLabel->setAlignment(Qt::AlignCenter);
	LifeLabel->setToolTip(tr("Life"));
	LifeLay->addWidget(LifeLabel);
	LifeBar=new QProgressBar(this);
	LifeBar->setObjectName("LifeBar");
	LifeBar->setOrientation(Qt::Vertical);
	LifeBar->setToolTip(tr("Life"));
	LifeBar->setMaximum(20);
	LifeBar->setMinimum(0);
	LifeBar->setTextVisible(false);
	LifeLay->addWidget(LifeBar);

	QGridLayout* MainLayout=new QGridLayout(this);
	MainLayout->setMargin(5);
	MainLayout->setSpacing(2);
	MainLayout->addWidget(NameLabel,0,0,1,2);
	MainLayout->addWidget(AvatarLabel,1,0,1,2);
	MainLayout->addWidget(HandSizeLabel,2,0);
	MainLayout->addWidget(GraveyardLabel,3,0);
	MainLayout->addWidget(ExileLabel,4,0);
	MainLayout->addLayout(LifeLay,2,1,3,1);
	MainLayout->addWidget(ManaPoolFrame,5,0,1,2);
}
void PlayerInfoDisplayer::UpdateAspect(){
	if (InfosToDisplay){
		NameLabel->setText(InfosToDisplay->GetPlayerName());
		HandSizeLabel->setText(QString("%1").arg(InfosToDisplay->GetHand().size()));
		GraveyardLabel->setText(QString("%1").arg(InfosToDisplay->GetGraveyard().size()));
		ExileLabel->setText(QString("%1").arg(InfosToDisplay->GetExile().size()));
		AvatarLabel->SetImageToShow(InfosToDisplay->GetAvatar());
		LifeLabel->setText(QString("%1").arg(InfosToDisplay->GetLife()));
		WManaPoolLabel->setText(QString("%1").arg(InfosToDisplay->GetManaPool().value(Constants::ManaCosts::W,0)));
		UManaPoolLabel->setText(QString("%1").arg(InfosToDisplay->GetManaPool().value(Constants::ManaCosts::U,0)));
		BManaPoolLabel->setText(QString("%1").arg(InfosToDisplay->GetManaPool().value(Constants::ManaCosts::B,0)));
		RManaPoolLabel->setText(QString("%1").arg(InfosToDisplay->GetManaPool().value(Constants::ManaCosts::R,0)));
		GManaPoolLabel->setText(QString("%1").arg(InfosToDisplay->GetManaPool().value(Constants::ManaCosts::G,0)));
		CManaPoolLabel->setText(QString("%1").arg(InfosToDisplay->GetManaPool().value(Constants::ManaCosts::Colorless,0)));
	}
	else{
		HandSizeLabel->setText(QString(""));
		GraveyardLabel->setText(QString(""));
		ExileLabel->setText(QString(""));
		AvatarLabel->SetImageToShow(QPixmap());
		CurrentLife=0;
		LifeLabel->setText("0");
		LifeBar->setValue(0);
	}
	setStyleSheet(StyleSheets::PlayerInfoCSS);
}
void PlayerInfoDisplayer::resizeEvent(QResizeEvent* event){
	QWidget::resizeEvent(event);
	BackGround->setGeometry(0,0,width(),height());
	HandSizeLabel->setMask(QPixmap(":/Board/CardsInHand.png")
		.scaled(HandSizeLabel->size())
		.mask());
	GraveyardLabel->setMask(QPixmap(":/Board/Resources/Graveyard.png")
		.scaled(GraveyardLabel->size())
		.mask());
	ExileLabel->setMask(QPixmap(":/Board/Resources/Exile.png")
		.scaled(ExileLabel->size())
		.mask());
	AvatarLabel->setMask(QPixmap::grabWidget(AvatarLabel).mask());
	WManaPoolLabel->setGeometry(38*ManaPoolFrame->width()/102,0,26*ManaPoolFrame->width()/102,26*ManaPoolFrame->height()/98);
	UManaPoolLabel->setGeometry(76*ManaPoolFrame->width()/102,27*ManaPoolFrame->height()/98,26*ManaPoolFrame->width()/102,26*ManaPoolFrame->height()/98);
	BManaPoolLabel->setGeometry(61*ManaPoolFrame->width()/102,72*ManaPoolFrame->height()/98,26*ManaPoolFrame->width()/102,26*ManaPoolFrame->height()/98);
	RManaPoolLabel->setGeometry(14*ManaPoolFrame->width()/102,72*ManaPoolFrame->height()/98,26*ManaPoolFrame->width()/102,26*ManaPoolFrame->height()/98);
	GManaPoolLabel->setGeometry(0,27*ManaPoolFrame->height()/98,26*ManaPoolFrame->width()/102,26*ManaPoolFrame->height()/98);
	CManaPoolLabel->setGeometry((ManaPoolFrame->width()-((26*ManaPoolFrame->width()/102)))/2,(ManaPoolFrame->height()-((26*ManaPoolFrame->height()/102)))/2,26*ManaPoolFrame->width()/102,26*ManaPoolFrame->height()/98);
	WManaPoolLabel->setMask(QRegion(0,0,26*ManaPoolFrame->width()/102,26*ManaPoolFrame->height()/98,QRegion::Ellipse));
	UManaPoolLabel->setMask(QRegion(0,0,26*ManaPoolFrame->width()/102,26*ManaPoolFrame->height()/98,QRegion::Ellipse));
	BManaPoolLabel->setMask(QRegion(0,0,26*ManaPoolFrame->width()/102,26*ManaPoolFrame->height()/98,QRegion::Ellipse));
	RManaPoolLabel->setMask(QRegion(0,0,26*ManaPoolFrame->width()/102,26*ManaPoolFrame->height()/98,QRegion::Ellipse));
	GManaPoolLabel->setMask(QRegion(0,0,26*ManaPoolFrame->width()/102,26*ManaPoolFrame->height()/98,QRegion::Ellipse));
	CManaPoolLabel->setMask(QRegion(0,0,26*ManaPoolFrame->width()/102,26*ManaPoolFrame->height()/98,QRegion::Ellipse));
}
int PlayerInfoDisplayer::GetLifeLevel() const{
	if (!InfosToDisplay) return 0;
	int Result=0;
	int Lifer=CurrentLife;
	while (Lifer>20){
		Lifer-=20;
		Result++;
	}
	return Result;
}
void PlayerInfoDisplayer::NextAnimation(){
	if (!Animations.isEmpty()){
		QPropertyAnimation* tempPoint=Animations.takeAt(0);
		if (
			(CurrentLife>=60 && CurrentLife+tempPoint->keyValueAt(1.0).toInt()-tempPoint->keyValueAt(0.0).toInt()>=60)
			||
			(CurrentLife<=0 && CurrentLife+tempPoint->keyValueAt(1.0).toInt()-tempPoint->keyValueAt(0.0).toInt()<=0)
		)
		{
			CurrentLife+=tempPoint->keyValueAt(1.0).toInt()-tempPoint->keyValueAt(0.0).toInt();
			tempPoint->deleteLater();
			return NextAnimation();
		}
		else {
			CurrentLife+=tempPoint->keyValueAt(1.0).toInt()-tempPoint->keyValueAt(0.0).toInt();
			LifeBar->setValue(tempPoint->keyValueAt(0.0).toInt());
			tempPoint->start(QAbstractAnimation::DeleteWhenStopped);
		}
	}
	setStyleSheet(StyleSheets::PlayerInfoCSS);
}
void PlayerInfoDisplayer::AnimateLifeBar(int NewLife){
	QPropertyAnimation* tempPoint;
	int TempCurr=CurrentLife;
	int TempNew=NewLife;
	if (!Animations.isEmpty()){
		TempCurr=TargetLife;
		if (TargetLife<NewLife){
			while (TempCurr>20) {TempCurr-=20; TempNew-=20;}
			while (TempNew>20){
				Animations.append(new QPropertyAnimation(LifeBar,"value",this));
				Animations.last()->setEasingCurve(QEasingCurve::Linear);
				Animations.last()->setKeyValueAt(0.0,TempCurr);
				Animations.last()->setKeyValueAt(1.0,20);
				connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
				TempNew-=20;
				TempCurr=0;
			}
			Animations.append(new QPropertyAnimation(LifeBar,"value",this));
			Animations.last()->setEasingCurve(QEasingCurve::Linear);
			Animations.last()->setKeyValueAt(0.0,TempCurr);
			Animations.last()->setKeyValueAt(1.0,TempNew);
			connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
		}
		else if (TargetLife>NewLife){
			while (TempCurr>20) {TempCurr-=20; TempNew-=20;}
			while (TempNew<0){
				Animations.append(new QPropertyAnimation(LifeBar,"value",this));
				Animations.last()->setEasingCurve(QEasingCurve::Linear);
				Animations.last()->setKeyValueAt(0.0,TempCurr);
				Animations.last()->setKeyValueAt(1.0,0);
				connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
				TempNew+=20;
				TempCurr=20;
			}
			Animations.append(new QPropertyAnimation(LifeBar,"value",this));
			Animations.last()->setEasingCurve(QEasingCurve::Linear);
			Animations.last()->setKeyValueAt(0.0,TempCurr);
			Animations.last()->setKeyValueAt(1.0,TempNew);
			connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
		}
		foreach(tempPoint,Animations)
			tempPoint->setDuration(LifeAnimationDuration/Animations.size());
		return;
	}
	if (CurrentLife<NewLife){
		while (TempCurr>20) {TempCurr-=20; TempNew-=20;}
		while (TempNew>20){
			Animations.append(new QPropertyAnimation(LifeBar,"value",this));
			Animations.last()->setEasingCurve(QEasingCurve::Linear);
			Animations.last()->setKeyValueAt(0.0,TempCurr);
			Animations.last()->setKeyValueAt(1.0,20);
			connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
			TempNew-=20;
			TempCurr=0;
		}
		Animations.append(new QPropertyAnimation(LifeBar,"value",this));
		Animations.last()->setEasingCurve(QEasingCurve::Linear);
		Animations.last()->setKeyValueAt(0.0,TempCurr);
		Animations.last()->setKeyValueAt(1.0,TempNew);
		connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
	}
	else if (CurrentLife>NewLife){
		while (TempCurr>20) {TempCurr-=20; TempNew-=20;}
		while (TempNew<0){
			Animations.append(new QPropertyAnimation(LifeBar,"value",this));
			Animations.last()->setEasingCurve(QEasingCurve::Linear);
			Animations.last()->setKeyValueAt(0.0,TempCurr);
			Animations.last()->setKeyValueAt(1.0,0);
			connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
			TempNew+=20;
			TempCurr=20;
		}
		Animations.append(new QPropertyAnimation(LifeBar,"value",this));
		Animations.last()->setEasingCurve(QEasingCurve::Linear);
		Animations.last()->setKeyValueAt(0.0,TempCurr);
		Animations.last()->setKeyValueAt(1.0,TempNew);
		connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
	}
	else return;

	foreach(tempPoint,Animations)
		tempPoint->setDuration(LifeAnimationDuration/Animations.size());
	if (!Animations.isEmpty()){
		tempPoint=Animations.takeAt(0);
		CurrentLife=tempPoint->keyValueAt(1.0).toInt();
		tempPoint->start(QAbstractAnimation::DeleteWhenStopped);
	}
}
void PlayerInfoDisplayer::SetInfosToDisplay(MagiQPlayer* a){
	InfosToDisplay=a;
	if (!InfosToDisplay) return;
	CurrentLife=InfosToDisplay->GetLife();
	if (CurrentLife<0) CurrentLife=0;
	while(CurrentLife>20) CurrentLife-=20;
	LifeBar->setValue(CurrentLife);
	CurrentLife=InfosToDisplay->GetLife();
	connect(InfosToDisplay,SIGNAL(LifeChanged(int)),this,SLOT(AnimateLifeBar(int)),Qt::UniqueConnection);
	connect(InfosToDisplay,SIGNAL(LifeChanged(int)),this,SLOT(UpdateAspect()));
}
bool PlayerInfoDisplayer::eventFilter(QObject *target, QEvent *event){
	if (event->type()==QEvent::MouseButtonPress)
	{
		QMouseEvent* mouseevent=static_cast<QMouseEvent*>(event);
		if (mouseevent->button()==Qt::LeftButton){
			if(target==WManaPoolLabel){emit WManaPoolClicked();}
			if(target==UManaPoolLabel){emit UManaPoolClicked();}
			if(target==BManaPoolLabel){emit BManaPoolClicked();}
			if(target==RManaPoolLabel){emit RManaPoolClicked();}
			if(target==GManaPoolLabel){emit GManaPoolClicked();}
			if(target==CManaPoolLabel){emit CManaPoolClicked();}
			if(target==ExileLabel){emit ExileClicked();}
			if(target==GraveyardLabel){emit GraveyardClicked();}
		}
	}
	return QWidget::eventFilter(target,event);
}
