#include "PlayerInfoDisplayer.h"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QBitmap>
#include <QProgressBar>
#include "MagiQPlayer.h"
#include "StyleSheets.h"
#include "RoundedCornersLabel.h"
PlayerInfoDisplayer::PlayerInfoDisplayer(QWidget* parent)
	:QWidget(parent)
	,InfosToDisplay(NULL)
{
	BackGround=new QFrame(this);
	BackGround->setObjectName("BackGround");

	NameLabel=new QLabel(this);
	NameLabel->setObjectName("NameLabel");
	NameLabel->setAlignment(Qt::AlignCenter);
	AvatarLabel=new RoundedCornersLabel(this);
	AvatarLabel->setObjectName("AvatarLabel");
	AvatarLabel->setScaledContents(true);
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
		LifeLabel->setText(QString("%1").arg(InfosToDisplay->GetLife()));
		int Lifer=InfosToDisplay->GetLife();
		while (Lifer>20) Lifer-=20;
		LifeBar->setValue(Lifer);
		AvatarLabel->SetImageToShow(InfosToDisplay->GetAvatar());
	}
	else{
		HandSizeLabel->setText(QString(""));
		GraveyardLabel->setText(QString(""));
		ExileLabel->setText(QString(""));
		QPixmap VoidPix(90,90);
		VoidPix.fill(Qt::gray);
		AvatarLabel->setPixmap(VoidPix);
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
	int Lifer=InfosToDisplay->GetLife();
	while (Lifer>20){
		Lifer-=20;
		Result++;
	}
	return Result;
}