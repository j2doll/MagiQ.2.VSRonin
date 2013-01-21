#ifndef SYMBOLSSELECTOR_H
#define SYMBOLSSELECTOR_H
#include <QWidget>
#include "EffectsConstants.h"
class SmileButton;
class QFrame;
class SymbolsSelector : public QWidget
{
	Q_OBJECT
	public:
		SymbolsSelector(QWidget* parent=0,int SmilePR=8);
		void SetSmilesPerRow(const int& spr){SmilesPerRow=spr;}
	signals:
		void selected(int iden);
	private:
		QFrame* Sfondo;
		unsigned int SmilesPerRow;
		SmileButton* emotes[Symbols::Num_Symbols];
	public slots:
		void show_toggle();
	protected:
		void resizeEvent(QResizeEvent* event);
};
#endif
