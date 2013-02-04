#ifndef CARDVIEWER_H
#define CARDVIEWER_H
#include <QWidget>
#include <QSize>
class Card;
class QLabel;
class QMouseEvent;
class QFrame;
class CardViewer :public QWidget{
	Q_OBJECT
	Q_PROPERTY(int CardRotation READ GetCardRotation WRITE SetCardRotation)
	Q_PROPERTY(bool CanBeZoom READ GetCanBeZoom WRITE SetCanBeZoom)
	Q_PROPERTY(bool CanBeClick READ GetCanBeClick WRITE SetCanBeClick)
public:
	CardViewer(QWidget* parent=0);
	int GetCardRotation() const {return CardRotation;}
	void SetCardRotation(int a);
	Card* GetCardToDisplay() const {return CardToDisplay;}
	void SetCardToDisplay(Card* c) {CardToDisplay=c;}
	void SetRepresentNumber(int a=1) {if(a<1) a=1; RepresentNumber=a;}
	QSize sizeHint() const {return PrefSize;}
	bool GetCanBeZoom() const {return CanBeZoom;}
	bool GetCanBeClick() const {return CanBeClick;}
	void SetCanBeZoom(bool a) {CanBeZoom=a;}
	void SetCanBeClick(bool a) {CanBeClick=a;}
	void SetShadable(bool a);
private:
	bool CanBeZoom;
	bool CanBeClick;
	int RepresentNumber;
	int CardRotation;
	QSize PrefSize;
	Card* CardToDisplay;
	QLabel* Displayer;
	QFrame* Shader;
protected:
	void enterEvent (QEvent* event){emit GainFocus();}
	void leaveEvent (QEvent* event){emit LeftFocus();}
	void mousePressEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* e);
	void resizeEvent(QResizeEvent* event);
public slots:
	void UpdateAspect();
	void TapAnimationStart();
signals:
	void LeftFocus();
	void GainFocus();
	void RequireZoom(Card*);
	void DoubleClicked(int);
};
#endif