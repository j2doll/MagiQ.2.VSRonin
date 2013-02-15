#ifndef JUDGE_H
#define JUDGE_H
#include <QObject>
#include <QMap>
#include <QList>
#include "Deck.h"
#include "CostantsDefinition.h"
#include "MagiQPlayer.h"
#include <QStack>
class Judge : public QObject{
	Q_OBJECT
private:
	unsigned int CardIDCounter;
	unsigned int EffectsIDCounter;
	int TurnNumber;
	bool GameStarted;
	QString ServerName;
	int GameMode;
	int DecksFormat;
	int MinPlayer;
	int MaxPlayer;
	QMap<int,MagiQPlayer*> PlayersList;
	bool EverybodyReady() const;
	bool EverybodyAcceptedHand() const;
	bool EverybodyFinishedTimer() const;
	void StartMatch();
	int CurrentPhase;
	int NextPhase;
	bool Precombat;
	QList<int> PlayersOrder;
	void SetCurrentPhase(int a);
	int PhaseTimeLimit;
	int ResponseTimeLimit;
	int TurnTimeLimit;
	int WhoStoppedTheTimer;
	bool PhaseTimerRunning;
	bool StackTimerRunning;
	int TimerTypeStopped;
	enum{NoneT,PhaseT,StackT,BothT};
	QStack<EffectData*> EffectsStack;
	QStack<CardData> CardsStack;
	QStack<bool> EffectInStack;
	void AddToStack(EffectData* eff);
	void AddToStack(CardData crd);
	void ResolveEffect(EffectData* eff);
	void ResolveCard(CardData crd);
	void CheckPlayableCards();
	QMap<int,int> ManaAvailable(int PlayerCode) const;
	bool CanPlayCard(const CardData& crd, int PlayerCode, const QMap<int,int>& ManaAvai) const ;
	bool CanPlayCard(const CardData& crd, int PlayerCode) const {return CanPlayCard(crd,PlayerCode,ManaAvailable(PlayerCode));}
public:
	Judge(QObject* parent=0);
	bool GetGameStarted() const {return GameStarted;}
	int GetCurrentPhase() const {return CurrentPhase;}
	const QString& GetServerName() const {return ServerName;}
	void SetServerName(const QString& a){ServerName=a;}
	int GetGameMode() const {return GameMode;}
	void SetGameMode(int a){if(a>=0 && a<Constants::GameMode::END) GameMode=a;}
	int GetDecksFormat() const {return DecksFormat;}
	void SetDecksFormat(int a){if(a>=0 && a<Constants::Legality::END) DecksFormat=a;}
	int GetMinPlayer() const {return MinPlayer;}
	void SetMinPlayer(int a){if(a>1 && a<5) MinPlayer=a;}
	int GetMaxPlayer() const {return MaxPlayer;}
	void SetMaxPlayer(int a){if(a>1 && a<5) MaxPlayer=a;}
	int GetNumPlayers() const{return PlayersList.size();}
signals:
	void ServerInfos(QString,int,int,int,int,int);
	void LeftTheGame(QString);
	void Joined(QString);
	void YourNameColor(int,QString,QColor);
	void InvalidDeck(int);
	void AllCards(QList<CardData>);
	void PlayerHand(int,QList<int>);
	void PlayerLibrary(int,int);
	void PlayOrder(QList<int>);
	void PlayersNameAvatar(QMap<int,QString>,QMap<int,QPixmap>);
	void GameHasStarted();
	void WaitingFor(int,QString);
	void StopWaitingFor();
	void CurrentPhaseChanged(int);
	void CardsToUntap(QList<int>);
	void CardsToTap(QList<int>);
	void CardDrawn(int,int);
	void StopTimers();
	void ResumeTimers();
	void StopTurnTimer();
	void ResumeTurnTimer();
	void ResumeStackTimer();
	void EffectAddedToStack(quint32,const EffectData&);
	void EffectResolved();
	void PlayableCards(int,QList<int>);
	void PlayedCard(int Who,int crd);
	void RemoveFromHand(int who,int crdID);
	void PermanentResolved(int,int);
public slots:
	void SendServerInfos(){emit ServerInfos(ServerName,GameMode,DecksFormat,MinPlayer,MaxPlayer,PlayersList.size());}
	void IncomingJoinRequest(int a,QString nam,QPixmap avat);
	void SomebodyDisconnected(int a);
	void SendLeftTheGame(QString a){emit LeftTheGame(a);}
	void IsReadyToPlay(int a,bool ready);
	void DeckSetUp(int socID,CardDeck deck);
	void DoMulligan(int socID);
	void AcceptedHand(int SocID);
	void NextTurn();
	void UpkeepStep();
	void DrawStep();
	void MainStep();
	void TimerFinished(int SocID);
	void TimerStopped(int SocID);
	void ResumeTimer(int SocID);
	void WantsToPlayCard(int who,int CrdID,QList<int> UsedToPayIDs);
};
#endif