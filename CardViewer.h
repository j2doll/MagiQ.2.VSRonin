#ifndef CARDVIEWER_H
#define CARDVIEWER_H
#include <QWidget>
#include <QSize>
class Card;
class QLabel;
class QMouseEvent;
class CardViewer :public QWidget{
	Q_OBJECT
	Q_PROPERTY(int CardRotation READ GetCardRotation WRITE SetCardRotation)
public:
	CardViewer(QWidget* parent=0);
	int GetCardRotation() const {return CardRotation;}
	void SetCardRotation(int a);
	Card* GetCardToDisplay() const {return CardToDisplay;}
	void SetCardToDisplay(Card* c) {CardToDisplay=c;}
	void SetRepresentNumber(int a=1) {if(a<1) a=1; RepresentNumber=a;}
	QSize sizeHint() const {return PrefSize;}
private:
	int RepresentNumber;
	int CardRotation;
	QSize PrefSize;
	Card* CardToDisplay;
	QLabel* Displayer;
protected:
	void enterEvent (QEvent* event){emit GainFocus();}
	void leaveEvent (QEvent* event){emit LeftFocus();}
	void mousePressEvent(QMouseEvent* event);
public slots:
	void UpdateAspect();
	void TapAnimationStart();
signals:
	void LeftFocus();
	void GainFocus();
	void RequireZoom(Card*);
};
#endif