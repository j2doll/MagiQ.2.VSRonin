#include "PlayerInfoDisplayer.h"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QBitmap>
#include <QProgressBar>
#include <QPropertyAnimation>
#include "MagiQPlayer.h"
#include "StyleSheets.h"
#include "RoundedCornersLabel.h"
PlayerInfoDisplayer::PlayerInfoDisplayer(QWidget* parent)
	:QWidget(parent)
	,InfosToDisplay(NULL)
	,CurrentLife(0)
{
	BackGround=new QFrame(this);
	BackGround->setObjectName("BackGround");

	NameLabel=new QLabel(this);
	NameLabel->setObjectName("NameLabel");
	NameLabel->setAlignment(Qt::AlignCenter);
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
	ExileLabel=new QLabel(this);
	ExileLabel->setObjectName("ExileLabel");
	ExileLabel->setAlignment(Qt::AlignCenter);
	ExileLabel->setToolTip(tr("Exile"));
	ManaPoolLabel=new QLabel(this);
	ManaPoolLabel->setObjectName("ManaPoolLabel");
	ManaPoolLabel->setScaledContents(true);
	ManaPoolLabel->setToolTip(tr("Mana Pool"));
	ManaPoolLabel->setPixmap(QPixmap(":/Board/ManaPool.png"));

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
	MainLayout->addWidget(AvatarLabel,0,0,1,2);
	MainLayout->addWidget(HandSizeLabel,1,0);
	MainLayout->addWidget(GraveyardLabel,2,0);
	MainLayout->addWidget(ExileLabel,3,0);
	MainLayout->addLayout(LifeLay,1,1,3,1);
	MainLayout->addWidget(ManaPoolLabel,4,0,1,2);
}
void PlayerInfoDisplayer::UpdateAspect(){
	if (InfosToDisplay){
		HandSizeLabel->setText(QString("%1").arg(InfosToDisplay->GetHand().size()));
		GraveyardLabel->setText(QString("%1").arg(InfosToDisplay->GetGraveyard().size()));
		ExileLabel->setText(QString("%1").arg(InfosToDisplay->GetExile().size()));
		CurrentLife=LifeLabel->text().toInt();
		LifeLabel->setText(QString("%1").arg(InfosToDisplay->GetLife()));
		AvatarLabel->SetImageToShow(InfosToDisplay->GetAvatar());
	}
	else{
		HandSizeLabel->setText(QString(""));
		GraveyardLabel->setText(QString(""));
		ExileLabel->setText(QString(""));
		AvatarLabel->SetImageToShow(QPixmap());
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
	int TempCurr=CurrentLife;
	int TempNew=InfosToDisplay ? InfosToDisplay->GetLife() : 0;
	while (TempCurr>20) {TempCurr-=20; TempNew-=20;}
	if (TempNew>20) CurrentLife+=20-TempCurr;
	if (!Animations.isEmpty())
		Animations.takeAt(0)->start(QAbstractAnimation::DeleteWhenStopped);
	setStyleSheet(StyleSheets::PlayerInfoCSS);
}
void PlayerInfoDisplayer::AnimateLifeBar(int NewLife){
	if (!Animations.isEmpty()) return;
	int TempCurr=CurrentLife;
	int TempNew=NewLife;
	while (TempCurr>20) {TempCurr-=20; TempNew-=20;}
	while (TempNew>20){
		Animations.append(new QPropertyAnimation(LifeBar,"value",this));
		Animations.last()->setDuration(LifeAnimationDuration);
		Animations.last()->setEasingCurve(QEasingCurve::Linear);
		Animations.last()->setKeyValueAt(0.0,TempCurr);
		Animations.last()->setKeyValueAt(1.0,20);
		connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()));
		TempNew-=20;
		TempCurr=0;
	}
	Animations.append(new QPropertyAnimation(LifeBar,"value",this));
	Animations.last()->setDuration(LifeAnimationDuration);
	Animations.last()->setEasingCurve(QEasingCurve::Linear);
	Animations.last()->setKeyValueAt(0.0,TempCurr);
	Animations.last()->setKeyValueAt(1.0,TempNew);
	connect(Animations.last(),SIGNAL(finished()),this,SLOT(NextAnimation()));

	if (!Animations.isEmpty())
		Animations.takeAt(0)->start(QAbstractAnimation::DeleteWhenStopped);
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
}
