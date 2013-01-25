#ifndef PLAYERINFODISPLAYER_H
#define PLAYERINFODISPLAYER_H
#include <QWidget>
#include <QPixmap>
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
	QLabel* ManaPoolLabel;

	MagiQPlayer* InfosToDisplay;
public:
	PlayerInfoDisplayer(QWidget* parent=0);
	void SetInfosToDisplay(MagiQPlayer* a=NULL){InfosToDisplay=a;}
	MagiQPlayer* GetInfosToDisplay() const {return InfosToDisplay;}
	void UpdateAspect();
	int GetLifeLevel() const;
protected:
	void resizeEvent(QResizeEvent* event);
};
#endif