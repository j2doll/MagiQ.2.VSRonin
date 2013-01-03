#include "EffectsBuilder.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	EffectsBuilder w;
	w.show();
	return a.exec();
}
