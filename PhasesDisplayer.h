#ifndef PHASESDISPLAYER_H
#define PHASESDISPLAYER_H
#include <QWidget>
class QProgressBar;
class QLabel;
class QFrame;
class PhasesDisplayer : public QWidget{
	Q_OBJECT
private:
	enum{AnimationDuration=500};
	QProgressBar* PhaseTimer;
	QProgressBar* TurnTimer;
	QFrame* Displayer;
	QLabel* CurrentPhaseIcon;
	QLabel* NextPhaseIcon;
	QFrame* Background;
	int CurrentPhase;
public:
	PhasesDisplayer(QWidget* parent=0);
	void SetPhaseTimeLimit(int a);
	void SetTurnTimeLimit(int a);
	void SetPhaseTime(int a);
	void SetTurnTime(int a);
	void SetPhaseTime();
	void SetTurnTime();
	void SetPhase(int a);
protected:
	void resizeEvent(QResizeEvent* event);
private slots:
	void AnimationFinished();
};
#endif