#include "PhasesDisplayer.h"
#include "CostantsDefinition.h"
#include <QProgressBar>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "StyleSheets.h"
PhasesDisplayer::PhasesDisplayer(QWidget* parent)
	:QWidget(parent)
	,CurrentPhase(Constants::Phases::Untap)
	,StackTimerActivated(false)
{
	Background=new QFrame(this);
	Background->setObjectName("Background");
	Displayer=new QFrame(this);
	Displayer->setObjectName("Displayer");
	Displayer->setMinimumSize(20,20);
	CurrentPhaseIcon=new QLabel(Displayer);
	CurrentPhaseIcon->setObjectName("CurrentPhaseIcon");
	CurrentPhaseIcon->setScaledContents(true);
	CurrentPhaseIcon->setPixmap(QPixmap(Constants::PhaseImages[0]));
	CurrentPhaseIcon->setToolTip(Constants::PhasesNames[0]);
	NextPhaseIcon=new QLabel(Displayer);
	NextPhaseIcon->setObjectName("NextPhaseIcon");
	NextPhaseIcon->setScaledContents(true);

	PhaseTimer=new QProgressBar(this);
	PhaseTimer->setObjectName("PhaseTimer");
	PhaseTimer->setMinimum(0);
	PhaseTimer->setOrientation(Qt::Horizontal);
	PhaseTimer->setToolTip(tr("Phase Time"));
	PhaseTimer->setTextVisible(false);
	PhaseTimer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	QGridLayout* PhaseTimerLay=new QGridLayout(PhaseTimer);
	PhaseTimerLay->setMargin(2);
	PhaseTimerLay->setSpacing(0);
	PausedLabel=new QLabel(PhaseTimer);
	PausedLabel->setObjectName("PausedLabel");
	PausedLabel->setAlignment(Qt::AlignCenter);
	PausedLabel->setText(tr("Timer Stopped"));
	PhaseTimerLay->addWidget(PausedLabel);
	PausedLabel->hide();
	TurnTimer=new QProgressBar(this);
	TurnTimer->setObjectName("TurnTimer");
	TurnTimer->setMinimum(0);
	TurnTimer->setOrientation(Qt::Horizontal);
	TurnTimer->setTextVisible(false);
	TurnTimer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

	QGridLayout* MainLay=new QGridLayout(this);
	MainLay->setMargin(3);
	MainLay->setSpacing(2);
	MainLay->addWidget(Displayer,0,0);
	MainLay->addWidget(PhaseTimer,0,1);
	MainLay->addWidget(TurnTimer,1,0,1,2);
	CurrentPhaseIcon->move(0,0);
	NextPhaseIcon->hide();
	SetStackTimerActivated(false);
}
void PhasesDisplayer::resizeEvent(QResizeEvent* event){
	Background->setGeometry(0,0,width(),height());
	CurrentPhaseIcon->resize(Displayer->size());
	NextPhaseIcon->resize(Displayer->size());
}
void PhasesDisplayer::SetPhase(int a){
	if (a<Constants::Phases::Untap || a>Constants::Phases::TurnEnd || CurrentPhase==a) return;
	CurrentPhase=a;
	NextPhaseIcon->setPixmap(QPixmap(Constants::PhaseImages[a]));
	NextPhaseIcon->setToolTip(Constants::PhasesNames[a]);
	NextPhaseIcon->show();
	QParallelAnimationGroup* Animazioni=new QParallelAnimationGroup;
	QPropertyAnimation* AnimCurrent=new QPropertyAnimation(CurrentPhaseIcon,"pos",Displayer);
	AnimCurrent->setDuration(AnimationDuration);
	AnimCurrent->setEasingCurve(QEasingCurve::Linear);
	AnimCurrent->setKeyValueAt(0.0,QPoint(0,0));
	AnimCurrent->setKeyValueAt(1.0,QPoint(-Displayer->width()-2,0));
	Animazioni->addAnimation(AnimCurrent);
	QPropertyAnimation* AnimNext=new QPropertyAnimation(NextPhaseIcon,"pos",Displayer);
	AnimNext->setDuration(AnimationDuration);
	AnimNext->setEasingCurve(QEasingCurve::Linear);
	AnimNext->setKeyValueAt(1.0,QPoint(0,0));
	AnimNext->setKeyValueAt(0.0,QPoint(Displayer->width()+2,0));
	Animazioni->addAnimation(AnimNext);
	connect(Animazioni,SIGNAL(finished()),this,SLOT(AnimationFinished()));
	Animazioni->start(QAbstractAnimation::DeleteWhenStopped);
}
void PhasesDisplayer::AnimationFinished(){
	CurrentPhaseIcon->move(0,0);
	CurrentPhaseIcon->setPixmap(QPixmap(Constants::PhaseImages[CurrentPhase]));
	CurrentPhaseIcon->setToolTip(Constants::PhasesNames[CurrentPhase]);
	NextPhaseIcon->hide();
}
void PhasesDisplayer::SetPhaseTimeLimit(int a) {PhaseTimer->setMaximum(a);}
void PhasesDisplayer::SetTurnTimeLimit(int a) {TurnTimer->setMaximum(a);}
void PhasesDisplayer::SetPhaseTime(int a){PausedLabel->hide(); PhaseTimer->setValue(PhaseTimer->maximum()-a);}
void PhasesDisplayer::SetTurnTime(int a){TurnTimer->setValue(TurnTimer->maximum()-a);}
void PhasesDisplayer::SetPhaseTime(){PausedLabel->hide(); PhaseTimer->setValue(0);}
void PhasesDisplayer::SetTurnTime(){TurnTimer->setValue(0);}
void PhasesDisplayer::PausePhaseTimer(){PausedLabel->show(); PhaseTimer->setValue(0);}
void PhasesDisplayer::SetStackTimerActivated(bool a){
	StackTimerActivated=a;
	PhaseTimer->setToolTip(a ? tr("Response Time"):tr("Phase Time"));
	setStyleSheet(StyleSheets::PhasesDisplayerCSS);
}