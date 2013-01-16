#ifndef CAMPODIGIOCO_H
#define CAMPODIGIOCO_H
#include <QWidget>
#include <QList>
class HandLayout;
class MagiQPlayer;
class QFrame;
class QLabel;
class CardViewer;
class BattleGround : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int NumOfPlayers READ GetNumOfPlayers)
private:
//Visual Elements
	QFrame* Board;
	QList<QFrame*> HandFrames;
	QList<HandLayout*> HandsLay;
	QList<QLabel*> DeckLabels;

//Game Properties
	bool GameStarted;
	QList<MagiQPlayer*> Players;
	CardViewer* ScheduledRaise;
//Functions
	void UpdateAspect();

	void TestStuff();
public:
	BattleGround(QWidget* parent=0);
	
	void SetPlayers(const QList<MagiQPlayer*>& a);
	void SetPlayers();
	void SetPlayers(const MagiQPlayer& a){SetPlayers(); AddPlayer(a);}
	void SetPlayer(int PlNum, const MagiQPlayer& a);
	void AddPlayer(const MagiQPlayer& a);

	int GetNumOfPlayers() const {return Players.size();}
	bool GetGameStarted() const {return GameStarted;}
	const QList<MagiQPlayer*>& GetPlayers() const {return Players;}
	MagiQPlayer* GetPlayer(int index){return Players.value(index,NULL);}
public slots:
	void StartGame();
private slots:
	void ResetHandOrder();
protected:
	void resizeEvent(QResizeEvent* event);
};
#endif

