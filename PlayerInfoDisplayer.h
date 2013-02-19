#ifndef PLAYERINFODISPLAYER_H
#define PLAYERINFODISPLAYER_H
#include <QWidget>
#include <QPixmap>
#include <QList>
#include <QMap>
class QPropertyAnimation;
class QFrame;
class QLabel;
class QProgressBar;
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

	int PlayerID;
	int LifeValue;
	int HandSize;
	int LibrarySize;
	int GraveyardSize;
	int ExileSize;
	QPixmap PlayerAvatar;
	QString PlayerName;
	QMap<int,int> ManaPool;

	enum{LifeAnimationDuration=1000};
	enum{LifeBarMultiply=100}; //The higher, the smoother the animation is
	QList<QPropertyAnimation*> Animations;
	int CurrentLife;
	int TargetLife;
private slots:
	void AnimateLifeBar(int NewLife);
	void NextAnimation();
public:
	PlayerInfoDisplayer(QWidget* parent=0);
	int GetLifeLevel() const;
	int GetLifeValue() const{return LifeValue;}
	int GetHandSize() const{return HandSize;}
	int GetLibrarySize() const{return LibrarySize;}
	int GetGraveyardSize() const{return GraveyardSize;}
	int GetExileSize() const{return ExileSize;}
	const QPixmap& GetPlayerAvatar() const{return PlayerAvatar;}
	const QString& GetPlayerName() const{return PlayerName;}
	const QMap<int,int>& GetManaPool() const{return ManaPool;}
	int GetPlayerID() const {return PlayerID;}
	void SetLifeValue(int a){if(a==LifeValue) return; LifeValue=a; AnimateLifeBar(a);}
	void SetHandSize(int a){HandSize=a;}
	void SetLibrarySize(int a){LibrarySize=a;}
	void SetGraveyardSize(int a){GraveyardSize=a;}
	void SetExileSize(int a){ExileSize=a;}
	void SetPlayerAvatar(const QPixmap& a){PlayerAvatar=a;}
	void SetPlayerName(const QString& a){PlayerName=a;}
	void SetManaPool(const QMap<int,int>& a){ManaPool=a;}
	void SetPlayerID(int a){PlayerID= a<0 ? a:-a;}
public slots:
	void UpdateAspect();
protected:
	void resizeEvent(QResizeEvent* event);
	bool eventFilter(QObject *target, QEvent *event);
signals:
	void WManaPoolClicked(int);
	void UManaPoolClicked(int);
	void BManaPoolClicked(int);
	void RManaPoolClicked(int);
	void GManaPoolClicked(int);
	void CManaPoolClicked(int);
	void GraveyardClicked(int);
	void ExileClicked(int);
	void AvatarClicked(int);
};
#endif