#include "SelettoreImmagini.h"
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QPainter>
#include <QGridLayout>
#include <QTransform>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include <QTimer>
SelettoreImmagini::SelettoreImmagini(QWidget* parent, int Orient)
:QWidget(parent)
,Freccia(256,256)
,IndiceCorrente(-1)
,DurataAnimazione(500)
,VoidImage(256,256)
,Continua(false)
,VisDidascalia(Nessuna)
,SettingIndex(-1)
{
	if (Orient==Verticale || Orient==Orizzontale) Orientamento=Orient;
	else Orientamento=Orizzontale;
	Freccia.fill(QColor(0,0,0,0));
	QPainter Disegnatore(&Freccia);
	Disegnatore.setBrush(QBrush(QColor(0,0,0,255)));
	Disegnatore.setPen(QColor(0,0,0,255));
	QVector<QPoint> Punti;
	Punti.append(QPoint(0,256));
	Punti.append(QPoint(128,0));
	Punti.append(QPoint(256,256));
	Disegnatore.drawPolygon(QPolygon(Punti));

	PulsanteBasso=new QPushButton(this);
	PulsanteBasso->setObjectName("PulsanteBasso");
	PulsanteBasso->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	PulsanteBasso->setMinimumHeight(5);
	PulsanteBasso->setEnabled(false);
	connect(PulsanteBasso,SIGNAL(clicked()),this,SLOT(NextImage()));

	PulsanteDestra=new QPushButton(this);
	PulsanteDestra->setObjectName("PulsanteDestra");
	PulsanteDestra->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
	PulsanteDestra->setMinimumWidth(5);
	PulsanteDestra->setEnabled(false);
	connect(PulsanteDestra,SIGNAL(clicked()),this,SLOT(NextImage()));

	PulsanteAlto=new QPushButton(this);
	PulsanteAlto->setObjectName("PulsanteAlto");
	PulsanteAlto->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	PulsanteAlto->setMinimumHeight(5);
	PulsanteAlto->setEnabled(false);
	connect(PulsanteAlto,SIGNAL(clicked()),this,SLOT(PrevImage()));

	PulsanteSinistra=new QPushButton(this);
	PulsanteSinistra->setObjectName("PulsanteSinistra");
	PulsanteSinistra->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
	PulsanteSinistra->setMinimumWidth(5);
	PulsanteSinistra->setEnabled(false);
	connect(PulsanteSinistra,SIGNAL(clicked()),this,SLOT(PrevImage()));

	Displayer=new QFrame(this);
	Displayer->setObjectName("Displayer");
	Displayer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	Displayer->setMinimumSize(50,50);

	PulsanteAlto->setMaximumHeight(Displayer->height()/5);
	PulsanteBasso->setMaximumHeight(Displayer->height()/5);
	PulsanteDestra->setMaximumWidth(Displayer->width()/5);
	PulsanteSinistra->setMaximumWidth(Displayer->width()/5);

	VoidImage.fill(QColor(0,0,0,255));

	IconaCorrente=new QLabel(Displayer);
	IconaCorrente->setObjectName("IconaCorrente");
	IconaCorrente->setScaledContents(true);
	IconaCorrente->setPixmap(VoidImage);
	IconaSuccessiva=new QLabel(Displayer);
	IconaSuccessiva->setObjectName("IconaSuccessiva");
	IconaSuccessiva->setScaledContents(true);
	IconaSuccessiva->setPixmap(VoidImage);
	IconaSuccessiva->hide();

	DidascaliaSopra=new QLabel(this);
	DidascaliaSopra->setObjectName("DidascaliaSopra");
	DidascaliaSopra->setText("");
	DidascaliaSopra->hide();
	DidascaliaSotto=new QLabel(this);
	DidascaliaSotto->setObjectName("DidascaliaSotto");
	DidascaliaSotto->setText("");
	DidascaliaSotto->hide();

	QVBoxLayout* LayoutDisplayer=new QVBoxLayout;
	LayoutDisplayer->setMargin(0);
	LayoutDisplayer->setSpacing(0);
	LayoutDisplayer->addWidget(DidascaliaSopra);
	LayoutDisplayer->addWidget(Displayer);
	LayoutDisplayer->addWidget(DidascaliaSotto);
	QGridLayout* LayoutVisulizzatore=new QGridLayout(this);
	LayoutVisulizzatore->addWidget(PulsanteAlto,0,1);
	LayoutVisulizzatore->addWidget(PulsanteSinistra,1,0);
	LayoutVisulizzatore->addLayout(LayoutDisplayer,1,1);
	LayoutVisulizzatore->addWidget(PulsanteDestra,1,2);
	LayoutVisulizzatore->addWidget(PulsanteBasso,2,1);
	if (Orientamento==Orizzontale){PulsanteAlto->hide(); PulsanteBasso->hide();}
	else {PulsanteDestra->hide(); PulsanteSinistra->hide();}

	setStyleSheet(
		"#DidascaliaSopra {"
		"font: 12px bold;"
		"}"
		"#DidascaliaSotto {"
		"font: 12px bold;"
		"}"
		"#PulsanteDestra {"
		"border: none;"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0 stop:0 rgba(30, 40, 203, 255), stop:0.4 rgba(74,88,213,255), stop:1 rgba(101, 113, 218, 255));"
		"}"
		"#PulsanteDestra:disabled {"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0 stop:0 #333, stop:0.4 #555, stop:1 #888);"
		"}"
		"#PulsanteDestra:hover {"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0 stop:0 rgba(30, 40, 203, 255), stop:0.4 rgba(46,54,224,255), stop:1 rgba(82, 95, 214, 255));"
		"}"
		"#PulsanteDestra:pressed {"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0 stop:0 rgba(101, 113, 218, 255), stop:0.6 rgba(74,88,213,255), stop:1 rgba(30, 40, 203, 255) );"
		"}"

		"#PulsanteSinistra {"
		"border: none;"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0 stop:0 rgba(101, 113, 218, 255), stop:0.6 rgba(74,88,213,255), stop:1 rgba(30, 40, 203, 255) );"
		"}"
		"#PulsanteSinistra:disabled {"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0 stop:0 #888, stop:0.6 #555, stop:1 #333 );"
		"}"
		"#PulsanteSinistra:hover {"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0 stop:1 rgba(30, 40, 203, 255), stop:0.6 rgba(46,54,224,255), stop:0 rgba(82, 95, 214, 255));"
		"}"
		"#PulsanteSinistra:pressed {"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0 stop:0 rgba(30, 40, 203, 255), stop:0.4 rgba(74,88,213,255), stop:1 rgba(101, 113, 218, 255));"
		"}"

		"#PulsanteBasso {"
		"border: none;"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1 stop:0 rgba(30, 40, 203, 255), stop:0.4 rgba(74,88,213,255), stop:1 rgba(101, 113, 218, 255));"
		"}"
		"#PulsanteBasso:disabled {"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1 stop:0 #333, stop:0.4 #555, stop:1 #888);"
		"}"
		"#PulsanteBasso:hover {"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1 stop:0 rgba(30, 40, 203, 255), stop:0.4 rgba(46,54,224,255), stop:1 rgba(82, 95, 214, 255));"
		"}"
		"#PulsanteBasso:pressed {"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1 stop:0 rgba(101, 113, 218, 255), stop:0.6 rgba(74,88,213,255), stop:1 rgba(30, 40, 203, 255) );"
		"}"

		"#PulsanteAlto {"
		"border: none;"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1 stop:0 rgba(101, 113, 218, 255), stop:0.6 rgba(74,88,213,255), stop:1 rgba(30, 40, 203, 255) );"
		"}"
		"#PulsanteAlto:disabled {"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1 stop:0 #888, stop:0.6 #555, stop:1 #333 );"
		"}"
		"#PulsanteAlto:hover {"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1 stop:1 rgba(30, 40, 203, 255), stop:0.6 rgba(46,54,224,255), stop:0 rgba(82, 95, 214, 255));"
		"}"
		"#PulsanteAlto:pressed {"
		"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1 stop:0 rgba(30, 40, 203, 255), stop:0.4 rgba(74,88,213,255), stop:1 rgba(101, 113, 218, 255));"
		"}"
		);

	//Test
	/*ImpostaPosDidascalia(Sopra);
	QPixmap Prova(256,256);
	Prova.fill(QColor(255,0,0,255));
	AggiungiImmagine(Prova,"Rosso");
	Prova.fill(QColor(0,255,0,255));
	AggiungiImmagine(Prova,"Verde");
	Prova.fill(QColor(0,0,255,255));
	AggiungiImmagine(Prova,"Blu");
	Prova.fill(QColor(255,255,255,255));
	AggiungiImmagine(Prova,"Bianco");*/
}
void SelettoreImmagini::ImpostaOrientamento(int a){
	if (a==Orizzontale && a!=Orientamento){
		Orientamento=Orizzontale;
		PulsanteDestra->show();
		PulsanteSinistra->show();
		PulsanteAlto->hide();
		PulsanteBasso->hide();
		AggiornaAttivazionePulsanti();
		update();
	}
	else if (a==Verticale && a!=Orientamento){
		Orientamento=Verticale;
		PulsanteDestra->hide();
		PulsanteSinistra->hide();
		PulsanteAlto->show();
		PulsanteBasso->show();
		AggiornaAttivazionePulsanti();
		update();
	}
}
void SelettoreImmagini::ImpostaPosDidascalia(int a){
	if (a==Nessuna){
		DidascaliaSotto->hide();
		DidascaliaSopra->hide();
		VisDidascalia=Nessuna;
	}
	else if (a==Sopra){
		DidascaliaSotto->hide();
		DidascaliaSopra->show();
		VisDidascalia=Sopra;
	}
	else if (a==Sotto){
		DidascaliaSopra->hide();
		DidascaliaSotto->show();
		VisDidascalia=Sotto;
	}
	update();
}
void SelettoreImmagini::resizeEvent(QResizeEvent *event){
	PulsanteAlto->setMaximumHeight(Displayer->height()/5);
	PulsanteBasso->setMaximumHeight(Displayer->height()/5);
	PulsanteDestra->setMaximumWidth(Displayer->width()/5);
	PulsanteSinistra->setMaximumWidth(Displayer->width()/5);
	QTransform Rotazione;
	Rotazione.reset();
	if (Orientamento==Verticale) PulsanteAlto->setMask(Freccia.scaled(
			Displayer->width()
			,PulsanteAlto->height()
		).mask());
	Rotazione.rotate(90);
	if (Orientamento==Orizzontale) PulsanteDestra->setMask(Freccia.scaled(
			Displayer->height()
			+(DidascaliaSopra->height()*(VisDidascalia==Sopra))
			+(DidascaliaSotto->height()*(VisDidascalia==Sotto))
			,PulsanteDestra->width()
		).transformed(Rotazione).mask());
	Rotazione.rotate(90);
	if (Orientamento==Verticale) PulsanteBasso->setMask(Freccia.scaled(
			Displayer->width()
			,PulsanteBasso->height()
		).transformed(Rotazione).mask());
	Rotazione.rotate(90);
	if (Orientamento==Orizzontale) PulsanteSinistra->setMask(Freccia.scaled(
			Displayer->height()
			+(DidascaliaSopra->height()*(VisDidascalia==Sopra))
			+(DidascaliaSotto->height()*(VisDidascalia==Sotto))
			,PulsanteSinistra->width()
		).transformed(Rotazione).mask());
	IconaCorrente->resize(Displayer->width(),Displayer->height());
	IconaCorrente->move(0,0);
	IconaSuccessiva->resize(Displayer->width(),Displayer->height());
	QWidget::resizeEvent(event);
}
void SelettoreImmagini::AggiungiImmagine(const QPixmap& fonte, const QString& Didas){
	Immagini.append(fonte);
	Didascalie.append(Didas);
	if (IndiceCorrente<0){
		IconaCorrente->setPixmap(Immagini.first());
		IndiceCorrente=0;
		DidascaliaSopra->setText(Didascalie.first());
		DidascaliaSotto->setText(Didascalie.first());
		emit IndexChanged(0);
	}
	AggiornaAttivazionePulsanti();
}
void SelettoreImmagini::SetIndex(int a){
	if (a==IndiceCorrente){
		emit IndexChanged(IndiceCorrente);
		SettingIndex=-1;
		AggiornaAttivazionePulsanti();
		return;
	}
	if (a<0 || a>Immagini.size()-1) return;
	if (!isVisible()){
		IconaCorrente->setPixmap(Immagini.at(a));
		DidascaliaSotto->setText(Didascalie.at(a));
		DidascaliaSopra->setText(Didascalie.at(a));
		IndiceCorrente=a;
		emit IndexChanged(IndiceCorrente);
		AggiornaAttivazionePulsanti();
		return;
	}
	SettingIndex=a;
	int DurataAttuale=DurataAnimazione;
	DurataAnimazione/=qAbs(a-IndiceCorrente);
	if(IndiceCorrente<a) NextImage();
	else if(IndiceCorrente>a) PrevImage();
	DurataAnimazione=DurataAttuale;
}
void SelettoreImmagini::NextImage(){
	Continua=false;
	if (IndiceCorrente==Immagini.size()-1 || IndiceCorrente<0) return;
	disconnect(PulsanteBasso,SIGNAL(clicked()),this,SLOT(NextImage()));
	disconnect(PulsanteDestra,SIGNAL(clicked()),this,SLOT(NextImage()));
	disconnect(PulsanteAlto,SIGNAL(clicked()),this,SLOT(PrevImage()));
	disconnect(PulsanteSinistra,SIGNAL(clicked()),this,SLOT(PrevImage()));
	connect(PulsanteBasso,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	connect(PulsanteDestra,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	connect(PulsanteAlto,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	connect(PulsanteSinistra,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	DidascaliaSopra->setText("");
	DidascaliaSotto->setText("");
	IconaSuccessiva->setPixmap(Immagini.at(IndiceCorrente+1));
	IconaSuccessiva->show();
	if (IndiceCorrente+1==Immagini.size()-1)  {PulsanteBasso->setEnabled(false); PulsanteDestra->setEnabled(false);}
	QParallelAnimationGroup* Animazioni=new QParallelAnimationGroup;
	QPropertyAnimation* AnimCurrent=new QPropertyAnimation(IconaCorrente,"pos",Displayer);
	AnimCurrent->setDuration(DurataAnimazione);
	AnimCurrent->setEasingCurve(QEasingCurve::Linear);
	AnimCurrent->setKeyValueAt(0.0,QPoint(0,0));
	if (Orientamento==Orizzontale) AnimCurrent->setKeyValueAt(1.0,QPoint(-Displayer->width()-10,0));
	else AnimCurrent->setKeyValueAt(1.0,QPoint(0,-Displayer->height()-10));
	Animazioni->addAnimation(AnimCurrent);
	QPropertyAnimation* AnimNext=new QPropertyAnimation(IconaSuccessiva,"pos",Displayer);
	AnimNext->setDuration(DurataAnimazione);
	AnimNext->setEasingCurve(QEasingCurve::Linear);
	AnimNext->setKeyValueAt(1.0,QPoint(0,0));
	if (Orientamento==Orizzontale) AnimNext->setKeyValueAt(0.0,QPoint(Displayer->width()+10,0));
	else AnimNext->setKeyValueAt(0.0,QPoint(0,Displayer->height()+10));
	Animazioni->addAnimation(AnimNext);
	connect(Animazioni,SIGNAL(finished()),this,SLOT(Successiva()));
	Animazioni->start(QAbstractAnimation::DeleteWhenStopped);
}
void SelettoreImmagini::PrevImage(){
	Continua=false;
	if (IndiceCorrente<=0) return;
	disconnect(PulsanteBasso,SIGNAL(clicked()),this,SLOT(NextImage()));
	disconnect(PulsanteDestra,SIGNAL(clicked()),this,SLOT(NextImage()));
	disconnect(PulsanteAlto,SIGNAL(clicked()),this,SLOT(PrevImage()));
	disconnect(PulsanteSinistra,SIGNAL(clicked()),this,SLOT(PrevImage()));
	connect(PulsanteBasso,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	connect(PulsanteDestra,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	connect(PulsanteAlto,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	connect(PulsanteSinistra,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	DidascaliaSopra->setText("");
	DidascaliaSotto->setText("");
	IconaSuccessiva->setPixmap(Immagini.at(IndiceCorrente-1));
	IconaSuccessiva->show();
	if (IndiceCorrente-1==0)  {PulsanteAlto->setEnabled(false); PulsanteSinistra->setEnabled(false);}
	QParallelAnimationGroup* Animazioni=new QParallelAnimationGroup;
	QPropertyAnimation* AnimCurrent=new QPropertyAnimation(IconaCorrente,"pos",Displayer);
	AnimCurrent->setDuration(DurataAnimazione);
	AnimCurrent->setEasingCurve(QEasingCurve::Linear);
	AnimCurrent->setKeyValueAt(0.0,QPoint(0,0));
	if (Orientamento==Orizzontale) AnimCurrent->setKeyValueAt(1.0,QPoint(Displayer->width()+10,0));
	else AnimCurrent->setKeyValueAt(1.0,QPoint(0,Displayer->height()+10));
	Animazioni->addAnimation(AnimCurrent);
	QPropertyAnimation* AnimNext=new QPropertyAnimation(IconaSuccessiva,"pos",Displayer);
	AnimNext->setDuration(DurataAnimazione);
	AnimNext->setEasingCurve(QEasingCurve::Linear);
	AnimNext->setKeyValueAt(1.0,QPoint(0,0));
	if (Orientamento==Orizzontale) AnimNext->setKeyValueAt(0.0,QPoint(-Displayer->width()-10,0));
	else AnimNext->setKeyValueAt(0.0,QPoint(0,-Displayer->height()-10));
	Animazioni->addAnimation(AnimNext);
	connect(Animazioni,SIGNAL(finished()),this,SLOT(Precedente()));
	Animazioni->start(QAbstractAnimation::DeleteWhenStopped);
}
void SelettoreImmagini::Successiva(){
	IconaCorrente->raise();
	IconaCorrente->move(0,0);
	IconaCorrente->setPixmap(Immagini.at(++IndiceCorrente));
	IconaSuccessiva->hide();
	DidascaliaSopra->setText(Didascalie.at(IndiceCorrente));
	DidascaliaSotto->setText(Didascalie.at(IndiceCorrente));
	AggiornaAttivazionePulsanti();
	connect(PulsanteBasso,SIGNAL(clicked()),this,SLOT(NextImage()));
	connect(PulsanteDestra,SIGNAL(clicked()),this,SLOT(NextImage()));
	connect(PulsanteAlto,SIGNAL(clicked()),this,SLOT(PrevImage()));
	connect(PulsanteSinistra,SIGNAL(clicked()),this,SLOT(PrevImage()));
	disconnect(PulsanteBasso,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	disconnect(PulsanteDestra,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	disconnect(PulsanteAlto,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	disconnect(PulsanteSinistra,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	if (Continua) NextImage();
	if (SettingIndex>0) SetIndex(SettingIndex);
	else {emit IndexChanged(IndiceCorrente);}
}
void SelettoreImmagini::Precedente(){
	IconaCorrente->raise();
	IconaCorrente->move(0,0);
	IconaCorrente->setPixmap(Immagini.at(--IndiceCorrente));
	IconaSuccessiva->hide();
	DidascaliaSopra->setText(Didascalie.at(IndiceCorrente));
	DidascaliaSotto->setText(Didascalie.at(IndiceCorrente));
	AggiornaAttivazionePulsanti();
	connect(PulsanteBasso,SIGNAL(clicked()),this,SLOT(NextImage()));
	connect(PulsanteDestra,SIGNAL(clicked()),this,SLOT(NextImage()));
	connect(PulsanteAlto,SIGNAL(clicked()),this,SLOT(PrevImage()));
	connect(PulsanteSinistra,SIGNAL(clicked()),this,SLOT(PrevImage()));
	disconnect(PulsanteBasso,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	disconnect(PulsanteDestra,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	disconnect(PulsanteAlto,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	disconnect(PulsanteSinistra,SIGNAL(clicked()),this,SLOT(ImpostaContinua()));
	if (Continua) PrevImage();
	if (SettingIndex>0) SetIndex(SettingIndex);
	else {emit IndexChanged(IndiceCorrente);}
}
void SelettoreImmagini::AggiornaAttivazionePulsanti(){
	PulsanteBasso->setEnabled(IndiceCorrente>=0 && IndiceCorrente<Immagini.size()-1);
	PulsanteDestra->setEnabled(IndiceCorrente>=0 && IndiceCorrente<Immagini.size()-1);
	PulsanteAlto->setEnabled(IndiceCorrente>0);
	PulsanteSinistra->setEnabled(IndiceCorrente>0);
}