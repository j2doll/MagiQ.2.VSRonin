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
	QMap<int,QList<CardViewer*>> CardsInHandView;
	CardViewer* AnimationCard;
	QMap<int,QList<Card*>> CardsInHand;
	Card* GenericCard;
	Card* ZoomedCard;
	QFrame* QuestionFrame;
	QLabel* QuestionText;
	QPushButton* QuestionButton1;
	QPushButton* QuestionButton2;
	PhasesDisplayer* PhaseDisp;
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
//Functions
	void SortCardsInHand();
	void AnimateDraw(int whos);
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
public:
	BattleGround(QWidget* parent=0);
	int GetNumOfPlayers() const {return Players.size();}
protected:
	void resizeEvent(QResizeEvent* event);
signals:
	void NeedResizeFrames();
	void Mulligan();
	void KeepHand();
	void TimerFinished();
	void TimerStopped();
	void TimerResumed();
};
#endif

