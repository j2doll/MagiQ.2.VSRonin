#include "CardBuilder.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	CardBuilder w;
	w.show();
	return a.exec();
}
