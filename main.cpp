#include "BattleGround.h"
#include <QApplication>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BattleGround w;
	w.show();
	return a.exec();
}

//TEST main
/*
#include <QApplication>
#include "TestWidget.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TestWidget Prova;
	Prova.show();
	return a.exec();
}*/