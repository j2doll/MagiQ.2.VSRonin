#include "FromEnumToString.h"
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStringList>
FromEnumToString::FromEnumToString(QWidget* parent/* =0 */)
	:QWidget(parent)
{
	EnumFormat=new QPlainTextEdit(this);
	EnumFormat->setObjectName("EnumFormat");
	StringFormat=new QPlainTextEdit(this);
	StringFormat->setObjectName("StringFormat");
	StringFormat->setReadOnly(true);
	connect(EnumFormat,SIGNAL(textChanged()),StringFormat,SLOT(clear()));
	Conversion=new QPushButton(this);
	Conversion->setObjectName("Conversion");
	Conversion->setText("Convert");
	connect(Conversion,SIGNAL(clicked()),this,SLOT(EnumToString()));
	QVBoxLayout* MainLayout=new QVBoxLayout(this);
	MainLayout->addWidget(EnumFormat);
	MainLayout->addWidget(Conversion);
	MainLayout->addWidget(StringFormat);
}
void FromEnumToString::EnumToString(){
	QString Sorgente(EnumFormat->toPlainText());
	Sorgente.replace(',',"");
	QStringList Righe=Sorgente.split('\n');
	QString Risultato("");
	for (QStringList::const_iterator i=Righe.begin();i!=Righe.end();i++){
		if(i!=Righe.begin()) Risultato.append("\n,");
		//Risultato.append('\"'+i->trimmed()+'\"');
		Risultato.append("QObject::tr(\""+i->trimmed()+"\")");
	}
	StringFormat->setPlainText(Risultato.trimmed());
}
