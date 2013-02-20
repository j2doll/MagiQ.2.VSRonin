#include "TestWidgetThread.h"
TestWidgetThread::TestWidgetThread(QObject* parent)
	:QThread(parent)
{

}
void TestWidgetThread::run(){
	Server=new LanServer(this);
	Server->StartListening();
	exec();
}