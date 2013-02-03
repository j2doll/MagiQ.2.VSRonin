#include "StackDisplayer.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
StackDispalyer::StackDispalyer(QWidget* parent)
	:QWidget(parent)
{
	StackList=new QTableWidget(this);
	StackList->setShowGrid(true);
	StackList->setObjectName("StackList");
	StackList->setColumnCount(1);
	StackList->setRowCount(1);
	QHeaderView* VerHea=StackList->verticalHeader();
	VerHea->setResizeMode(QHeaderView::ResizeToContents);
	VerHea->setResizeMode(0,QHeaderView::Stretch);
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
	EffStack.push(a);
	StackList->insertRow(1);
	StackList->setItem(1,0,
		new QTableWidgetItem(a.GetEffectText())
		);

}
void StackDispalyer::Resolve(){
	EffStack.pop();
	StackList->removeRow(1);
}
void StackDispalyer::EmptyStack(){
	EffStack.clear();
	StackList->setRowCount(1);
}