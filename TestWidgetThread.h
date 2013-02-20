#ifndef TESTWIDGETTHREAD_H
#define TESTWIDGETTHREAD_H
#include <QThread>
#include "LanServer.h"
class TestWidgetThread : public QThread{
	Q_OBJECT
public:
	TestWidgetThread(QObject* parent=0);
public slots:
	void run();
	void stop(){exit(0);}
private:
	LanServer* Server;
};
#endif