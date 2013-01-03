#ifndef SMILEBUTTON_H
#define SMILEBUTTON_H
#include <QPushButton>
class SmileButton:public QPushButton
{
	Q_OBJECT
private:
	int Identit;
public:
	SmileButton(QWidget* parent, int id);
signals:
	void clicked(int id);
protected:
	void mousePressEvent(QMouseEvent *event);
};
#endif