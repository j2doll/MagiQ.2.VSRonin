#ifndef ROUNDEDCORNERSLABEL_H
#define ROUNDEDCORNERSLABEL_H
#include <QLabel>
class RoundedCornersLabel:public QLabel{
	Q_OBJECT
private:
	QPixmap ImageToShow;
	int Radious;
public:
	RoundedCornersLabel(QWidget* parent=0);
	void SetImageToShow(const QPixmap& a){ImageToShow=a;}
	const QPixmap& GetImageToShow() const {return ImageToShow;}
	int GetRadious() const {return Radious;}
	void SetRadious(int a){if(a<0) a=0; Radious=a;}
protected:
	void paintEvent(QPaintEvent *e);
};
#endif