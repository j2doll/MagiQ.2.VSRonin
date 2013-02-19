#ifndef SignalerArrow_h__
#define SignalerArrow_h__
#include <QWidget>
class SignalerArrow : public QWidget{
	Q_OBJECT
private:
	enum{InitialWid=120};
	enum{InitialHei=40};
	QColor ArrowColor;
	QPoint From;
	QPoint To;
public:
	SignalerArrow(QWidget* parent);
	void SetFrom(const QPoint& a){From=a; UpdateGeometry();}
	void SetTo(const QPoint& a){To=a; UpdateGeometry();}
	void SetFromTo(const QPoint& a,const QPoint& b){From=a; To=b; UpdateGeometry();}
	const QPoint& GetFrom() const {return From;}
	const QPoint& GetTo() const {return To;}
	void UpdateGeometry();
protected:
	void paintEvent(QPaintEvent *event);
};

#endif // SignalerArrow_h__
