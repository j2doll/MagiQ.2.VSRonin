#ifndef CAMPODIGIOCO_H
#define CAMPODIGIOCO_H
#include <QWidget>
#include <QList>
#include <QMap>
#include <QPixmap>
#include <QHash>
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
class HandLayout;
class ControlledLayout;
class ManaCostLabel;
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
	QFrame* StackCardsFrame;
	HandLayout* StackCardsFrameLay;
	QMap<int,QFrame*> HandFrames;
	QMap<int,PlayerInfoDisplayer*> PlayesInfos;
	QMap<int,HandLayout*> HandsLay;
	QMap<int,QFrame*> LandsContainer;
	QMap<int,ControlledLayout*> LandsContainerLay; //TODO change Layout
	QMap<int,QLabel*> DeckLabels;
	QMap<int,QLabel*> GraveyardLabels;
	CardViewer* AnimationCard;
	QHash<int,QString> PlayerNames;
	QHash<int,QPixmap> PlayerAvatars;
	QHash<int,int> PlayerLife;
	QMap<int,QList<CardViewer*>> CardsInHandView;
	QMap<int,QList<CardViewer*>> LandsControlledView;
	QMap<int,QList<CardViewer*>> CardsControlledView;
	QList<CardViewer*> CardsInStackView;
	QMap<int,QList<Card*>> CardsInHand;
	QMap<int,QList<Card*>> CardsControlled;
	QMap<int,QList<Card*>> CardsInGraveyard;
	QMap<int,QList<Card*>> CardsExiled;
	QList<Card*> CardsInStack;
	Card* GenericCard;
	Card* ZoomedCard;
	QFrame* QuestionFrame;
	QLabel* QuestionText;
	QPushButton* QuestionButton1;
	QPushButton* QuestionButton2;
	QFrame* QuestionLowFrame;
	QLabel* QuestionLowText;
	QPushButton* QuestionLowButton1;
	QFrame* RequiredCostFrame;
	QLabel* RequiredCostText;
	ManaCostLabel* RequiredCostLabel;
	ManaCostLabel* PayedCostLabel;
	QFrame* PayedCostFrame;
	QLabel* PayedCostText;
	PhasesDisplayer* PhaseDisp;
//Game Properties
	QMap<int,Card*> AllCards;
	QHash<int,int> LibrarySizes;
	//QMap<int,MagiQPlayer*> Players;
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
	int HandPrimarySorting;
	int HandSecondarySorting;
	Card* CardRequiringMana;
	QMap<int,int> ManaToTap;
	QList<int> CardsUsedToPay;
	bool ManaSelectionModeON;
//Functions
	void SortCardsInHand();
	void SortCardsControlled();
	void AnimateDraw(int whos);
	void AnimatePlay(int whos,Card* ToShow);
	QMap<int,int> AskManaToTap();
	int NumberOfLands(int who);
	bool CheckPayedCard(const QMap<int,int>& ManaCost,const QMap<int,int>& ManaPay) const;
	void ManaSelectionMode(Card* const& TheCard);
private slots:
	void UpdateAspect();
	void ResetHandOrder();
	void ResetLandOrder();
	void ResetStackLayOrder();
	void ClearQuestion();
	void ClearLowerQuestion();
	void SizePosFrames();
	void SizePosAllCards();
	void SizePosCosts();
	void ZoomAnimate(Card* crd);
	void TurnTimeUpdate();
	void PhaseTimeUpdate();
	void NewManaPayed(int crdID);
	void CancelManaSelectionMode();
public slots:
	void AskMulligan();
	void SetPlayersOrder(QList<int> ord);
	void SetPlayersNameAvatar(QMap<int,QString> nam,QMap<int,QPixmap> avt);
	void SetMyHand(QList<int> hnd);
	void SetOtherHand(int whos,int numcards);
	void SetPlayerLibrary(int whos,int numcards);
	void DispalyWaitingFor(QString a);
	void HideWaitingFor();
	void UntapCards(QList<int> crds);
	void TapCards(QList<int> crds);
	void SetCurrentPhase(int ph);
	void DrawCard(int crd);
	void OtherDraw(int who);
	void StopTimer();
	void StopTurnTimer();
	void ResumeTurnTimer();
	void ResumeStackTimer();
	void EffectAddedToStack(quint32 crd,EffectData eff);
	void SetPlayableCards(QList<int> IDs);
	void PlayedCard(int crd,int Who);
	void WantToPlayCard(int crdID);
	void RemoveCardHand(int who,int crdID);
	void ResolveCard(int Who, int crd);
	void SetAllCards(QList<CardData> a);
public:
	BattleGround(QWidget* parent=0);
	int GetNumOfPlayers() const {return PlayersOrder.size();}
protected:
	void resizeEvent(QResizeEvent* event);
signals:
	void WantPlayCard(int crdID, QList<int> PayedWithIDs);
	void NeedResizeFrames();
	void NeedResizeCards();
	void Mulligan();
	void KeepHand();
	void TimerFinished();
	void TimerStopped();
	void TimerResumed();
};
#endif

