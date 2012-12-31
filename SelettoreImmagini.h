#ifndef SELETTOREIMMAGINI_H
#define SELETTOREIMMAGINI_H
#include <QWidget>
#include <QList>
#include <QBitmap>
class QPushButton;
class QFrame;
class QLabel;

class SelettoreImmagini: public QWidget{
	Q_OBJECT
private:
	int Orientamento;
	QPushButton* PulsanteAlto;
	QPushButton* PulsanteBasso;
	QPushButton* PulsanteDestra;
	QPushButton* PulsanteSinistra;
	QFrame* Displayer;
	QLabel* IconaCorrente;
	QLabel* IconaSuccessiva;
	QLabel* DidascaliaSopra;
	QLabel* DidascaliaSotto;
	QPixmap Freccia;
	QPixmap VoidImage;
	QList<QPixmap> Immagini;
	QList<QString> Didascalie;
	int IndiceCorrente;
	unsigned int DurataAnimazione;
	int VisDidascalia;
	bool Continua;
	int SettingIndex;
	void AggiornaAttivazionePulsanti();
public:
	enum{Verticale,Orizzontale};
	enum{Nessuna,Sopra,Sotto};
	SelettoreImmagini(QWidget* parent=0, int Orient=Orizzontale);
	int GetOrientamento() const {return Orientamento;}
	void AggiungiImmagine(const QPixmap& fonte, const QString& Didas="");
	int GetIndex() const {return IndiceCorrente;}
	int GetNumeroImmagini() const {return Immagini.size();}
protected:
	void resizeEvent(QResizeEvent *event);
public slots:
	void SetIndex(int a);
	void ImpostaPosDidascalia(int a);
	void ImpostaOrientamento(int a);
private slots:
	void ImpostaContinua(){Continua=true;}
	void NextImage();
	void PrevImage();
	void Successiva();
	void Precedente();
signals:
	void IndexChanged(int);
};
#endif