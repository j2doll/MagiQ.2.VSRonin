#ifndef PHASESDISPLAYER_H
#define PHASESDISPLAYER_H
#include <QWidget>
class QProgressBar;
class QLabel;
class QFrame;
class PhasesDisplayer : public QWidget{
	Q_OBJECT
	Q_PROPERTY(bool StackTimerActivated READ GetStackTimerActivated WRITE SetStackTimerActivated)
private:
	enum{AnimationDuration=500};
	QProgressBar* PhaseTimer;
	QProgressBar* TurnTimer;
	QFrame* Displayer;
	QLabel* CurrentPhaseIcon;
	QLabel* NextPhaseIcon;
	QLabel* PausedLabel;
	QFrame* Background;
	int CurrentPhase;
	bool StackTimerActivated;
public:
	PhasesDisplayer(QWidget* parent=0);
	void SetPhaseTimeLimit(int a);
	void SetTurnTimeLimit(int a);
	void SetPhaseTime(int a);
	void SetTurnTime(int a);
	void SetPhaseTime();
	void SetTurnTime();
	void SetPhase(int a);
	void PausePhaseTimer();
	bool GetStackTimerActivated() const {return StackTimerActivated;}
	void SetStackTimerActivated(bool a);
protected:
	void resizeEvent(QResizeEvent* event);
private slots:
	void AnimationFinished();
};
#endif