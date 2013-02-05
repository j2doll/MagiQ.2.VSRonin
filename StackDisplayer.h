#ifndef STACKDISPLAYER_H
#define STACKDISPLAYER_H
#include <QWidget>
#include <QStack>
#include "EffectData.h"
class QTableWidget;
class Card;
class StackDispalyer : public QWidget{
	Q_OBJECT
private:
	QTableWidget* StackList;
	QStack<bool> IsCard;
	QStack<Card*> CardsInSTack;
public:
	StackDispalyer(QWidget* parent=0);
public slots:
	void AddEffect(const EffectData& a);
	void AddCard(const CardData& a);
	void Resolve();
	void EmptyStack();
};
#endif