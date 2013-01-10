#ifndef CAMPODIGIOCO_H
#define CAMPODIGIOCO_H
#include <QWidget>
class BattleGround : public QWidget
{
	Q_OBJECT
private:
//Visual Elements
//Game Properties
	int NumOfPlayers;
public:
	BattleGround(QWidget* parent=0);
	
	void SetNumOfPlayers(int a){if(a<2) NumOfPlayers=2; else NumOfPlayers=a;}

	int GetNumOfPlayers() const {return NumOfPlayers;}
public slots:
	void StartGame();
};
#endif

