
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>

#include "StackDisplayer.h"
#include "Card.h"
#include "CardViewer.h"

StackDispalyer::StackDispalyer(QWidget* parent)
	:QWidget(parent)
{
	StackList=new QTableWidget(this);
	StackList->setShowGrid(true);
	StackList->setObjectName("StackList");
	StackList->setColumnCount(1);
	StackList->setRowCount(1);
    QHeaderView* VerHea = StackList->verticalHeader();

    // VerHea->setResizeMode( QHeaderView::ResizeToContents );
    // VerHea->setResizeMode( 0, QHeaderView::Stretch );
    VerHea->setSectionResizeMode( QHeaderView::ResizeToContents );
    VerHea->setSectionResizeMode( 0, QHeaderView::Stretch );

	VerHea->setVisible(false);
	StackList->horizontalHeader()->setVisible(false);
	StackList->horizontalHeader()->setStretchLastSection(true);
	StackList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	StackList->setSortingEnabled(false);
	StackList->setSelectionMode(QAbstractItemView::NoSelection);
	StackList->setFocusPolicy(Qt::NoFocus);
	QVBoxLayout* MainLay=new QVBoxLayout(this);
	MainLay->setMargin(0);
	MainLay->setSpacing(2);
	MainLay->addWidget(StackList);
}
void StackDispalyer::AddEffect(const EffectData& a){
	IsCard.push(false);
	StackList->insertRow(1);
	StackList->setItem(1,0,
		new QTableWidgetItem(a.GetEffectText())
		);

}
void StackDispalyer::Resolve(){
	if (IsCard.isEmpty()) return;
	if(IsCard.pop()) CardsInSTack.pop()->deleteLater();
	StackList->removeRow(1);
}
void StackDispalyer::EmptyStack(){
	IsCard.clear();
	foreach(Card* crd,CardsInSTack)
		crd->deleteLater();
	CardsInSTack.clear();
	StackList->setRowCount(1);
}
void StackDispalyer::AddCard(const CardData& a){
	IsCard.push(true);
	CardsInSTack.push(new Card(a,this));
	CardsInSTack.top()->resize(60,84);
	CardsInSTack.top()->hide();
	StackList->insertRow(1);
	StackList->setItem(1,0,new QTableWidgetItem());
	CardViewer* NewViewer=new CardViewer(this);
	NewViewer->SetCanBeClick(false);
	NewViewer->SetCanBeZoom(true);
	NewViewer->SetShadable(false);
	NewViewer->SetCardToDisplay(CardsInSTack.top());
	NewViewer->UpdateAspect();
	StackList->setCellWidget(1,0,NewViewer);
}
