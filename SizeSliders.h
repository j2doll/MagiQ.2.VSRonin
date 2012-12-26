#ifndef SIZESLIDERS_H
#define SIZESLIDERS_H
#include <QWidget>
class QSpinBox;
class QLabel;
class SizeSliders: public QWidget{
	Q_OBJECT
private:
	int wid;
	int hei;
	int posx;
	int posy;
	QSpinBox* WidthSlider;
	QSpinBox* HeightSlider;
	QSpinBox* HorPosSlider;
	QSpinBox* VerPosSlider;
	QLabel* WidthLabel;
	QLabel* HeightLabel;
	QLabel* HorPosLabel;
	QLabel* VerPosLabel;
	void UpdateSliders() const;
private slots:
	void WidChange(int a){wid=a; emit SizeChanged(QSize(wid,hei)); emit GeometryChanged(posx,posy,wid,hei);}
	void HeiChange(int a){hei=a; emit SizeChanged(QSize(wid,hei)); emit GeometryChanged(posx,posy,wid,hei);}
	void PosXChange(int a){posx=a; emit PositionChanged(QPoint(posx,posy)); emit GeometryChanged(posx,posy,wid,hei);}
	void PosYChange(int a){posy=a; emit PositionChanged(QPoint(posx,posy)); emit GeometryChanged(posx,posy,wid,hei);}
public:
	SizeSliders(QWidget* parent=0);
	void SetOSize(const QSize& a){wid=a.width(); hei=a.height(); UpdateSliders();}
	void SetOSize(int w, int h){wid=w; hei=h; UpdateSliders();}
	void SetOPosition(const QPoint& a){posx=a.x(); posy=a.y(); UpdateSliders();}
	void SetOPosition(int x, int y){posx=x; posy=y; UpdateSliders();}
	void SetOGeometry(int x, int y, int w, int h){posx=x; posy=y; wid=w; hei=h; UpdateSliders();}
signals:
	void SizeChanged(QSize s);
	void PositionChanged(QPoint p);
	void GeometryChanged(int x,int y,int w,int h);

};
#endif