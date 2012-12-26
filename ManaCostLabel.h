#ifndef MANACOSTLABEL_H
#define MANACOSTLABEL_H
#include <QWidget>
#include <QList>
#include <QPixmap>
#include <QStringList>
class QLabel;
class QHBoxLayout;
class ManaCostLabel : public QWidget{
	Q_OBJECT
private:
	enum {ManaFontSize=60};
	enum {TranslateX=-20};
	enum {TranslateY=-20};
	QPixmap WSymbol;
	QPixmap USymbol;
	QPixmap BSymbol;
	QPixmap RSymbol;
	QPixmap GSymbol;
	QPixmap CSymbol;
	QPixmap CWSymbol;
	QPixmap CUSymbol;
	QPixmap CBSymbol;
	QPixmap CRSymbol;
	QPixmap CGSymbol;
	QPixmap WUSymbol;
	QPixmap WBSymbol;
	QPixmap WRSymbol;
	QPixmap WGSymbol;
	QPixmap UBSymbol;
	QPixmap URSymbol;
	QPixmap UGSymbol;
	QPixmap BRSymbol;
	QPixmap BGSymbol;
	QPixmap RGSymbol;
	QHBoxLayout* MainLayout;
	QList<QLabel*> SymbolsLabels;
	QString CostString;
	int NumberOfSymbols;
	int ConvertedManaCost;
	QStringList SortedManaList(const QStringList& Base);
public:
	ManaCostLabel(QWidget* parent=0);
	ManaCostLabel(QWidget* parent,const QString& Cost);
	const QString& GetCostString() const {return CostString;}
	int GetNumberOfSymbols() const {return NumberOfSymbols;}
public slots:
	void SetCostString(const QString& Cost);
	void ClearCost();
};
#endif