#ifndef SMILESSELECTOR_H
#define SMILESSELECTOR_H
#include <QWidget>
#include "CostantsDefinition.h"
class SmileButton;
class QFrame;
class SmileSelector : public QWidget
{
	Q_OBJECT
public:
	SmileSelector(QWidget* parent=0,int SmilePR=8);
	void SetSmilesPerRow(const int& spr){SmilesPerRow=spr;}
signals:
	void selected(int iden);
private:
	QFrame* Sfondo;
	unsigned int SmilesPerRow;
	SmileButton* emotes[Constants::Num_Smiles];
public slots:
	void show_toggle();
protected:
	void resizeEvent(QResizeEvent* event);
};
#endif
