#include "ChatWidget.h"
#include <QFrame>
#include <QPushButton>
#include <QTextEdit>
#include "SmilesSelector.h"
#include <QGridLayout>
#include <QKeyEvent>
#include <QDateTime>
#include "StyleSheets.h"
ChatWidget::ChatWidget(QWidget* parent)
	:QWidget(parent)
	,UserColor(0,0,0)
	,UserName(tr("User"))
	,ShowTimeStamp(true)
{
	setMinimumSize(341,210);
	Background=new QFrame(this);
	Background->setObjectName("Background");
	QGridLayout* MainLayout=new QGridLayout(this);
	ChatText=new QTextEdit(this);
	ChatText->setObjectName("ChatText");
	ChatText->setReadOnly(true);
	ChatText->setText("");
	ChatText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ChatText->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	ChatText->setContextMenuPolicy(Qt::NoContextMenu);
	MainLayout->addWidget(ChatText,0,0,1,3);
	MessageText=new QTextEdit(this);
	MessageText->setObjectName("MessageText");
	MessageText->installEventFilter(this);
	MessageText->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	MessageText->setMaximumHeight(MessageTextMaxHei);
	MessageText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	MessageText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	MessageText->setContextMenuPolicy(Qt::NoContextMenu);
	MainLayout->addWidget(MessageText,1,0);
	SmilesButton=new QPushButton(this);
	SmilesButton->setObjectName("SmilesButton");
	SmilesButton->setIcon(QIcon(QPixmap(":/Smiles/happy.png")));
	MainLayout->addWidget(SmilesButton,1,1);
	SendButton=new QPushButton(this);
	SendButton->setObjectName("SendButton");
	SendButton->setText(tr("Send"));
	MainLayout->addWidget(SendButton,1,2);
	SmiSelector=new SmileSelector(this);
	SmiSelector->setObjectName("SmiSelector");
	SmiSelector->resize(SmiSelector->sizeHint());
	SmiSelector->hide();

	connect(SendButton,SIGNAL(clicked()),this,SLOT(SendMessage()));
	connect(SmilesButton,SIGNAL(clicked()),SmiSelector,SLOT(show_toggle()));
	connect(SmiSelector,SIGNAL(selected(int)),this,SLOT(addSmile(int)));
	setStyleSheet(StyleSheets::ChatCSS);
}
void ChatWidget::resizeEvent(QResizeEvent* event){
	Background->setGeometry(0,0,width(),height());
	SmiSelector->move(width()-333,height()-204);
}
QString ChatWidget::ProcessaSmiles(const QString& str) const{
	QString result(str);
	result.replace("\n","<br/>");
	for (int i=0;i<Constants::Num_Smiles;i++){
		result.replace(Constants::Emotes[i][0],Constants::Emotes[i][1]);
	}
	return result;
}
bool ChatWidget::eventFilter(QObject *target, QEvent *event){
	if(target==MessageText){
		if (event->type()==QEvent::KeyPress )
		{
			QKeyEvent* keyEvent=static_cast<QKeyEvent *>(event);
			if (keyEvent->key()==Qt::Key_Enter || keyEvent->key()==Qt::Key_Return){
				if(keyEvent->modifiers() & Qt::AltModifier)
					MessageText->textCursor().insertText("\n");
				else{
					SendMessage();
					return true;
				}
			}
			if (keyEvent->key()==Qt::Key_Tab){
				focusNextChild();
				return true;
			}
		}
	}
	return QWidget::eventFilter(target,event);
}
void ChatWidget::addSmile(int id){
	if (id<0 || id>Constants::Num_Smiles) return;
	MessageText->textCursor().insertText(
		Constants::Emotes[id][0]
	);
	MessageText->setFocus();
}
void ChatWidget::SetUserColor(const QString& a){
	UserColor.setNamedColor(a);
	if(!UserColor.isValid()) UserColor.setRgb(0,0,0);
}
void ChatWidget::SendMessage(){
	if (MessageText->toPlainText().isEmpty()) return;
	QDateTime timestamp(QDateTime::currentDateTime());
	QString result(
		QString("<font color=\"")+
		UserColor.name()+
		QString("\"><b>")+
		(ShowTimeStamp ? timestamp.time().toString(tr("hh:mm:ss","Format to show time"))+" - " : "")+
		UserName+
		QString("</b></font>: ")+
		ProcessaSmiles(MessageText->toPlainText())
		);
	MessageText->clear();
	emit OutgoingMessage(result);
}
void ChatWidget::IncomingMesage(QString Msg){
	ChatText->append(Msg);
	ChatText->ensureCursorVisible();
}
void ChatWidget::Disconnected(){
	ChatText->append("<hr/><font color=\"red\"><b>"+tr("Disconnected")+"</b></font>");
	ChatText->ensureCursorVisible();
}
void ChatWidget::Connected(){
	ChatText->append("<font color=\"green\"><b>"+tr("Connected")+"</b></font>");
	ChatText->ensureCursorVisible();
}