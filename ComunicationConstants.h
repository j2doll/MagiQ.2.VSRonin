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
		,YourHand
		,YourLibrary
		,OthersHand
		,OthersLibrary
		,Mulligan
		,HandAccepted
		,WaitingFor
		,StopWaitingFor
		,PhaseChanged
		,UntapCards
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
		,ChatMessage
	};}
}
#endif
