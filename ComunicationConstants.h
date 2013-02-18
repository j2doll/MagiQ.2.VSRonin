#ifndef COMUNICATIONCONSTANTS_H
#define COMUNICATIONCONSTANTS_H
//#define USE_SSL
namespace Comunications{
	enum {DefaultTCPport=7677};
	namespace TransmissionType{enum{
		SeverInformations
		,JoinRequest
		,ServerFull
		,PlayerJoins
		,YourNameAndColor
		,PlayerLeave
		,ReadyToPlay
		,InvalidDeck
		,GameStarted
		,PlayersOrder
		,PlayesNameAndAvatar
		,AllCards
		,YourHand
		,PlayerLibrary
		,OthersHand
		,Mulligan
		,HandAccepted
		,WaitingFor
		,StopWaitingFor
		,PhaseChanged
		,UntapCards
		,TapCards
		,YouDrawnCard
		,OtherDrawnCard
		,TimerFinished
		,TimerStopped
		,TimerResumed
		,StopTurnTimer
		,ResumeTurnTimer
		,ResumeStackTimer
		,StopTimers
		,ResumeTimers
		,EffectAddedStack
		,EffectResolved
		,PlayableCards
		,WantToPlayCard
		,PlayedCard
		,RemoveCardFromHand
		,PermanentResolved
		,ContinueToNextPhase
		,CardsThatCanAttack
		,AttackingCards
		,ChatMessage
	};}
}
#endif
