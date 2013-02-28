#ifndef SignalerArrow_h__
#define SignalerArrow_h__
#include <QWidget>
#include <QPixmap>
class SignalerArrow : public QWidget{
	Q_OBJECT
private:
	enum{InitialWid=120};
	enum{InitialHei=40};
	QColor ArrowColor;
	QPoint From;
	QPoint To;
	QPolygon ArrowPoligon;
	unsigned short ArrowAlphaComponent;
	QPixmap ArrowImage;
	qreal Modulus;
	qreal Phase;
public:
	enum{ArrowYellow,ArrowRed};
	SignalerArrow(QWidget* parent);
	void SetFrom(const QPoint& a){From=a; UpdateGeometry();}
	void SetTo(const QPoint& a){To=a; UpdateGeometry();}
	void SetFromTo(const QPoint& a,const QPoint& b){From=a; To=b; UpdateGeometry();}
	const QPoint& GetFrom() const {return From;}
	const QPoint& GetTo() const {return To;}
	const QColor& GetArrowColor() const {return ArrowColor;}
	void SetArrowColor(const QColor& a){ArrowColor=a; ArrowColor.setAlpha(ArrowAlphaComponent);}
	void SetArrowColor(const QString& a);
	void SetArrowColor(const int& a);
	void UpdateGeometry();
	unsigned short GetArrowAlphaComponent() const{return ArrowAlphaComponent;}
	void SetArrowAlphaComponent(unsigned short a);
protected:
	void paintEvent(QPaintEvent *event);
};

#endif // SignalerArrow_h__
