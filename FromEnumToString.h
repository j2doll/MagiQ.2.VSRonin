#ifndef FROMENUMTOSTRING_H
#define FROMENUMTOSTRING_H
#include <QWidget>
class QPlainTextEdit;
class QPushButton;
class FromEnumToString : public QWidget{
	Q_OBJECT
public:
	FromEnumToString(QWidget* parent=0);
private:
	QPlainTextEdit* EnumFormat;
	QPlainTextEdit* StringFormat;
	QPushButton* Conversion;
private slots:
	void EnumToString();
};
#endif