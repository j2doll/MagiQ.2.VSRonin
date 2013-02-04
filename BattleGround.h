#ifndef CAMPODIGIOCO_H
#define CAMPODIGIOCO_H
#include <QWidget>
#include <QList>
#include <QMap>
#include <QPixmap>
#include "CardData.h"
class HandLayout;
class MagiQPlayer;
class QFrame;
class QLabel;
class CardViewer;
class Card;
class QPushButton;
class PlayerInfoDisplayer;
class QTimer;
class PhasesDisplayer;
class QHBoxLayout;
class HandLayout; //TODO Change
class BattleGround : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int NumOfPlayers READ GetNumOfPlayers)
private:
	enum{AnimationDuration=500};
	enum{ZoommedCardWidth=200};
	enum{TimerUpdateIntervall=100};
//Visual Elements
	QFrame* Board;
	QMap<int,QFrame*> HandFrames;
	QMap<int,PlayerInfoDisplayer*> PlayesInfos;
	QMap<int,HandLayout*> HandsLay;
	QMap<int,QLabel*> DeckLabels;
	QMap<int,QLabel*> GraveyardLabels;
	CardViewer* AnimationCard;
	QMap<int,QList<CardViewer*>> CardsInHandView;
	QMap<int,QList<CardViewer*>> CardsControlledView;
	QMap<int,QList<Card*>> CardsInHand;
	QMap<int,QList<Card*>> CardsControlled;
	Card* GenericCard;
	Card* ZoomedCard;
	QFrame* QuestionFrame;
	QLabel* QuestionText;
	QPushButton* QuestionButton1;
	QPushButton* QuestionButton2;
	PhasesDisplayer* PhaseDisp;
	QMap<int,QFrame*> LandsContainer;
	QMap<int,QHBoxLayout*> LandsContainerLay;
	QMap<int,QMap<QString,QFrame*>> SameLandsFrames;
	QMap<int,QMap<QString,HandLayout*>> SameLandsFramesLay; /*TODO Change Layout Type*/
//Game Properties
	QList<Card*> AllCards;
	QMap<int,MagiQPlayer*> Players;
	QList<int> PlayersOrder;
	int CurrentPhase;
	QTimer* PhaseTimer;
	int PhaseTimeLimit;
	int CurrentPhaseTime;
	QTimer* ResponseTimer;
	int ResponseTimeLimit;
	int CurrentResponseTime;
	QTimer* TurnTimer;
	int TurnTimeLimit;
	int CurrentTurnTime;
	QMap<int,int> ManaToTap;
//Functions
	void SortCardsInHand();
	void AnimateDraw(int whos);
	void AnimatePlay(int whos,Card* ToShow);
	QMap<int,int> AskManaToTap();
private slots:
	void UpdateAspect();
	void ResetHandOrder();
	void ClearQuestion();
	void SizePosFrames();
	void ZoomAnimate(Card* crd);
	void TurnTimeUpdate();
	void PhaseTimeUpdate();
public slots:
	void AskMulligan();
	void SetPlayersOrder(QList<int> ord);
	void SetPlayersNameAvatar(QMap<int,QString> nam,QMap<int,QPixmap> avt);
	void SetMyHand(QList<CardData> hnd);
	void SetOtherHand(int whos,int numcards);
	void SetMyLibrary(QList<CardData> libr);
	void SetOtherLibrary(int whos,int numcards);
	void DispalyWaitingFor(QString a);
	void HideWaitingFor();
	void UntapCards(QList<int> crds);
	void SetCurrentPhase(int ph);
	void DrawCard(CardData crd);
	void OtherDraw(int who);
	void StopTimer();
	void StopTurnTimer();
	void ResumeTurnTimer();
	void ResumeStackTimer();
	void EffectAddedToStack(quint32 crd,EffectData eff);
	void SetPlayableCards(QList<int> IDs);
	void PlayedCard(int Who,CardData crd);
	void WantToPlayCard(int crdID);
public:
	BattleGround(QWidget* parent=0);
	int GetNumOfPlayers() const {return Players.size();}
protected:
	void resizeEvent(QResizeEvent* event);
signals:
	void WantPlayCard(int crdID);
	void NeedResizeFrames();
	void Mulligan();
	void KeepHand();
	void TimerFinished();
	void TimerStopped();
	void TimerResumed();
};
#endif

