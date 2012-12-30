#ifndef POWERTOUGHNESLABEL_H
#define POWERTOUGHNESLABEL_H
#include <QWidget>
#include <QBitmap>
class QLabel;
class PowerToughnesLabel : public QWidget{
	Q_OBJECT
	Q_PROPERTY(int SelectedBase READ GetSelectedBase WRITE SetSelectedBase)
private:
	enum {PTFontSize=12};
	QLabel* MainLabel;
	int SelectedBase;
	QString PTString;
	QBitmap LabelBase;
	int Loyalty;
	void UpadateAspect();
	bool IsPT;
public:
	PowerToughnesLabel(QWidget* parent=0);
	void SetCountPT() {IsPT=true;}
	void SetCountLoyalty() {IsPT=false;}
	void SetSelectedBase(int a);
	int GetSelectedBase() const {return SelectedBase;}
	void SetPTString(const QString& a){PTString=a; UpadateAspect();}
	void SetLoyalty(int a){if (a<0) a=0; Loyalty=a; UpadateAspect();}
	const QString& GetPTString() const {return PTString;}
protected:
	void resizeEvent(QResizeEvent* event);
};
#endif