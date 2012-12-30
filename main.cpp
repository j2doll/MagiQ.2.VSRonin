#include "CardBuilder.h"
#include <QtGui>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CardBuilder w;
	w.show();
	return a.exec();
}
