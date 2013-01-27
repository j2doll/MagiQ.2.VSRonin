#ifndef PLAYERINFODISPLAYER_H
#define PLAYERINFODISPLAYER_H
#include <QWidget>
#include <QPixmap>
#include <QList>
class QPropertyAnimation;
class QFrame;
class QLabel;
class QProgressBar;
class MagiQPlayer;
class RoundedCornersLabel;
class PlayerInfoDisplayer :public QWidget{
	Q_OBJECT
	Q_PROPERTY(int LifeLevel READ GetLifeLevel)
private:
	QFrame* BackGround;
	RoundedCornersLabel* AvatarLabel;
	QLabel* NameLabel;
	QLabel* HandSizeLabel;
	QLabel* GraveyardLabel;
	QLabel* ExileLabel;
	QLabel* LifeLabel;
	QProgressBar* LifeBar;
	QFrame* ManaPoolFrame;
	QLabel* WManaPoolLabel;
	QLabel* UManaPoolLabel;
	QLabel* BManaPoolLabel;
	QLabel* RManaPoolLabel;
	QLabel* GManaPoolLabel;
	QLabel* CManaPoolLabel;

	MagiQPlayer* InfosToDisplay;

	enum{LifeAnimationDuration=2500};
	QList<QPropertyAnimation*> Animations;
	int CurrentLife;
	int TargetLife;
private slots:
	void AnimateLifeBar(int NewLife);
	void NextAnimation();
public:
	PlayerInfoDisplayer(QWidget* parent=0);
	void SetInfosToDisplay(MagiQPlayer* a=NULL);
	MagiQPlayer* GetInfosToDisplay() const {return InfosToDisplay;}
	int GetLifeLevel() const;
public slots:
	void UpdateAspect();
protected:
	void resizeEvent(QResizeEvent* event);
};
#endif