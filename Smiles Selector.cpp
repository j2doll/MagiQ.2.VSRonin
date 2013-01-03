#include "Smiles Selector.h"
#include "SmileButton.h"
#include <QFrame>
#include <QGridLayout>
SmilesSelector::SmilesSelector(QWidget* parent,int SmilePR)
	:QWidget(parent)
	,SmilesPerRow(SmilePR)
{
	Sfondo=new QFrame(this);
	Sfondo->setStyleSheet("background-color: rgb(229, 229, 229);"
		"border-width: 1px;"
		"border-color: rgb(0, 0, 0);"
		"border-style: inset;"
		"border-radius: 5px;"
		);
	QGridLayout *Lay= new QGridLayout;
	QSizePolicy politica(QSizePolicy::Preferred, QSizePolicy::Preferred);
	politica.setHorizontalStretch(0);
	politica.setVerticalStretch(0);
	for (int i=0;i<Symbols::Num_Symbols;i++){
		emotes[i]=new SmileButton(this,i);
		emotes[i]->setObjectName("emoticonButton"+i);
		emotes[i]->setContextMenuPolicy(Qt::NoContextMenu);
		politica.setHeightForWidth(emotes[i]->sizePolicy().hasHeightForWidth());
		emotes[i]->setSizePolicy(politica);
		emotes[i]->setMaximumSize(24,24);
		emotes[i]->setMinimumSize(24,24);
		QIcon icona;
		icona.addFile(Symbols::symbols_paths[i], QSize(), QIcon::Normal, QIcon::Off);
		emotes[i]->setIcon(icona);
		connect(emotes[i],SIGNAL(clicked(int)),this,SIGNAL(selected(int)));
		connect(emotes[i],SIGNAL(clicked(int)),this,SLOT(hide())) ;
		Lay->addWidget(emotes[i],i/SmilesPerRow,i%SmilesPerRow);
	}
	setLayout(Lay);
	hide();
}
void SmilesSelector::resizeEvent(QResizeEvent* event){
	Sfondo->setGeometry(2,2,sizeHint().width()-4,sizeHint().height()-4);
}
void SmilesSelector::show_toggle(){
	if (isVisible())
		hide();
	else
		show();
}

