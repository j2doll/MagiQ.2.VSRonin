#include "CardBuilder.h"
#include <QApplication>

#include "Effect.h"
#include <QtGui>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/*QFrame *TestB=new QFrame;
	QHBoxLayout* MainLay=new QHBoxLayout(TestB);
	QTableWidget* Prova=new QTableWidget(TestB);
	Prova->setObjectName("EffectsTable");
	Prova->verticalHeader()->setVisible(false);
	Prova->horizontalHeader()->setVisible(false);
	Prova->horizontalHeader()->setStretchLastSection(true);
	Prova->verticalHeader()->setStretchLastSection(true);
	Prova->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	Prova->setSortingEnabled(false);
	//Prova->setShowGrid(false);
	Prova->setSelectionMode(QAbstractItemView::NoSelection);
	Prova->setFocusPolicy(Qt::NoFocus);
	Prova->setColumnCount(1);
	Prova->setRowCount(0);
	int RequiredHeight=0;
	for (int i=0;i<5;i++){
		Effect* provaEff=new Effect(TestB);
		provaEff->SetEffectText(QString("Prova %1").arg(i+1));
		provaEff->SetEffectType(1);
		provaEff->UpdateAspect();
		Prova->insertRow(0);
		Prova->setCellWidget(0,0,provaEff);
		Prova->setRowHeight(0,provaEff->GetMinimumHeight());
		RequiredHeight+=provaEff->GetMinimumHeight();
	}
	QLabel* FlavorTextLabel=new QLabel(TestB);
	FlavorTextLabel->setObjectName("FlavorTextLabel");
	FlavorTextLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
	FlavorTextLabel->setWordWrap(true);
	FlavorTextLabel->setScaledContents(true);
	FlavorTextLabel->setText("<i>FlavorAdj</i>");
	Prova->insertRow(Prova->rowCount());
	Prova->setCellWidget(Prova->rowCount()-1,0,FlavorTextLabel);
	Prova->setRowHeight(Prova->rowCount()-1,qMax(FlavorTextLabel->sizeHint().height(),Prova->height()-RequiredHeight));
	MainLay->addWidget(Prova);
	TestB->setStyleSheet(
	"QFrame #EffectsTable{"
		"background-color: rgba(0,0,0,0);"
		//"border: none;"
		"}"
		"QFrame #EffectsTable::item{"
		"border: none;"
		"padding: 0px;"
		"}"
		);
	TestB->show();*/
	CardBuilder w;
	w.show();
	return a.exec();
}
