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
	,CurrentLife(0)
	,TargetLife(0)
	,LifeValue(0)
	,HandSize(0)
	,LibrarySize(0)
	,ExileSize(0)
	,GraveyardSize(0)
	,PlayerName(tr("Player"))
	,PlayerAvatar(":/Board/DefaultAvatar.png")
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
	LifeBar->setMaximum(20*LifeBarMultiply);
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
	NameLabel->setText(PlayerName);
	HandSizeLabel->setText(QString("%1").arg(HandSize));
	GraveyardLabel->setText(QString("%1").arg(GraveyardSize));
	ExileLabel->setText(QString("%1").arg(ExileSize));
	AvatarLabel->SetImageToShow(PlayerAvatar);
	LifeLabel->setText(QString("%1").arg(LifeValue));
	WManaPoolLabel->setText(QString("%1").arg(ManaPool.value(Constants::ManaCosts::W,0)));
	UManaPoolLabel->setText(QString("%1").arg(ManaPool.value(Constants::ManaCosts::U,0)));
	BManaPoolLabel->setText(QString("%1").arg(ManaPool.value(Constants::ManaCosts::B,0)));
	RManaPoolLabel->setText(QString("%1").arg(ManaPool.value(Constants::ManaCosts::R,0)));
	GManaPoolLabel->setText(QString("%1").arg(ManaPool.value(Constants::ManaCosts::G,0)));
	CManaPoolLabel->setText(QString("%1").arg(ManaPool.value(Constants::ManaCosts::Colorless,0)));
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
			(CurrentLife>=60 && CurrentLife+(tempPoint->keyValueAt(1.0).toInt()/LifeBarMultiply)-(tempPoint->keyValueAt(0.0).toInt()/LifeBarMultiply)>=60)
			||
			(CurrentLife<=0 && CurrentLife+(tempPoint->keyValueAt(1.0).toInt()/LifeBarMultiply)-(tempPoint->keyValueAt(0.0).toInt()/LifeBarMultiply)<=0)
		)
		{
			CurrentLife+=(tempPoint->keyValueAt(1.0).toInt()/LifeBarMultiply)-(tempPoint->keyValueAt(0.0).toInt()/LifeBarMultiply);
			tempPoint->deleteLater();
			return NextAnimation();
		}
		else {
			CurrentLife+=(tempPoint->keyValueAt(1.0).toInt()/LifeBarMultiply)-(tempPoint->keyValueAt(0.0).toInt()/LifeBarMultiply);
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
				Animations.last()->setKeyValueAt(0.0,TempCurr*LifeBarMultiply);
				Animations.last()->setKeyValueAt(1.0,20*LifeBarMultiply);
				connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
				TempNew-=20;
				TempCurr=0;
			}
			Animations.append(new QPropertyAnimation(LifeBar,"value",this));
			Animations.last()->setEasingCurve(QEasingCurve::Linear);
			Animations.last()->setKeyValueAt(0.0,TempCurr*LifeBarMultiply);
			Animations.last()->setKeyValueAt(1.0,TempNew*LifeBarMultiply);
			connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
		}
		else if (TargetLife>NewLife){
			while (TempCurr>20) {TempCurr-=20; TempNew-=20;}
			while (TempNew<0){
				Animations.append(new QPropertyAnimation(LifeBar,"value",this));
				Animations.last()->setEasingCurve(QEasingCurve::Linear);
				Animations.last()->setKeyValueAt(0.0,TempCurr*LifeBarMultiply);
				Animations.last()->setKeyValueAt(1.0,0*LifeBarMultiply);
				connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
				TempNew+=20;
				TempCurr=20;
			}
			Animations.append(new QPropertyAnimation(LifeBar,"value",this));
			Animations.last()->setEasingCurve(QEasingCurve::Linear);
			Animations.last()->setKeyValueAt(0.0,TempCurr*LifeBarMultiply);
			Animations.last()->setKeyValueAt(1.0,TempNew*LifeBarMultiply);
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
			Animations.last()->setKeyValueAt(0.0,TempCurr*LifeBarMultiply);
			Animations.last()->setKeyValueAt(1.0,20*LifeBarMultiply);
			connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
			TempNew-=20;
			TempCurr=0;
		}
		Animations.append(new QPropertyAnimation(LifeBar,"value",this));
		Animations.last()->setEasingCurve(QEasingCurve::Linear);
		Animations.last()->setKeyValueAt(0.0,TempCurr*LifeBarMultiply);
		Animations.last()->setKeyValueAt(1.0,TempNew*LifeBarMultiply);
		connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
	}
	else if (CurrentLife>NewLife){
		while (TempCurr>20) {TempCurr-=20; TempNew-=20;}
		while (TempNew<0){
			Animations.append(new QPropertyAnimation(LifeBar,"value",this));
			Animations.last()->setEasingCurve(QEasingCurve::Linear);
			Animations.last()->setKeyValueAt(0.0,TempCurr*LifeBarMultiply);
			Animations.last()->setKeyValueAt(1.0,0);
			connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
			TempNew+=20;
			TempCurr=20;
		}
		Animations.append(new QPropertyAnimation(LifeBar,"value",this));
		Animations.last()->setEasingCurve(QEasingCurve::Linear);
		Animations.last()->setKeyValueAt(0.0,TempCurr*LifeBarMultiply);
		Animations.last()->setKeyValueAt(1.0,TempNew*LifeBarMultiply);
		connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()),Qt::QueuedConnection);
	}
	else return;

	foreach(tempPoint,Animations)
		tempPoint->setDuration(LifeAnimationDuration/Animations.size());
	if (!Animations.isEmpty()){
		tempPoint=Animations.takeAt(0);
		CurrentLife=tempPoint->keyValueAt(1.0).toInt()/LifeBarMultiply;
		tempPoint->start(QAbstractAnimation::DeleteWhenStopped);
	}
}
bool PlayerInfoDisplayer::eventFilter(QObject *target, QEvent *event){
	if (event->type()==QEvent::MouseButtonPress)
	{
		QMouseEvent* mouseevent=static_cast<QMouseEvent*>(event);
		if (mouseevent->button()==Qt::LeftButton){
			if(target==WManaPoolLabel){emit WManaPoolClicked(PlayerID);}
			else if(target==UManaPoolLabel){emit UManaPoolClicked(PlayerID);}
			else if(target==BManaPoolLabel){emit BManaPoolClicked(PlayerID);}
			else if(target==RManaPoolLabel){emit RManaPoolClicked(PlayerID);}
			else if(target==GManaPoolLabel){emit GManaPoolClicked(PlayerID);}
			else if(target==CManaPoolLabel){emit CManaPoolClicked(PlayerID);}
			else if(target==ExileLabel){emit ExileClicked(PlayerID);}
			else if(target==GraveyardLabel){emit GraveyardClicked(PlayerID);}
			else if(target==AvatarLabel){emit AvatarClicked(PlayerID);}
		}
	}
	return QWidget::eventFilter(target,event);
}
QPoint PlayerInfoDisplayer::PlayerAvatarCenter() const{
	QPoint Result(AvatarLabel->pos());
	Result.rx()+=AvatarLabel->width()/2;
	Result.ry()+=AvatarLabel->height()/2;
	return Result;
}
