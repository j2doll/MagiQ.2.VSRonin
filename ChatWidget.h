#ifndef CHATWIDGET_H
#define CHATWIDGET_H
#include <QWidget>
class QFrame;
class QPushButton;
class QTextEdit;
class SmileSelector;
class ChatWidget :public QWidget{
	Q_OBJECT
public:
	ChatWidget(QWidget* parent=0);
	const QString& GetUserName() const {return UserName;}
	const QColor& GetUserColor() const {return UserColor;}
	bool GetShowTimeStamp() const {return ShowTimeStamp;}
	void SetUserName(const QString& a){UserName=a;}
	void SetUserColor(const QColor& a){UserColor=a;}
	void SetUserColor(const QString& a);
	void SetShowTimeStamp(bool a){ShowTimeStamp=a;}
private:
	QString UserName;
	QColor UserColor;
	bool ShowTimeStamp;
	enum {MessageTextMaxHei=35};
	QFrame* Background;
	QTextEdit* ChatText;
	QTextEdit* MessageText;
	QPushButton* SmilesButton;
	QPushButton* SendButton;
	SmileSelector* SmiSelector;
	QString ProcessaSmiles(const QString& str) const;
private slots:
	void SendMessage();
	void addSmile(int id);
public slots:
	void IncomingMesage(QString Msg);
	void Disconnected();
	void Connected();
protected:
	void resizeEvent(QResizeEvent* event);
	bool eventFilter(QObject *target, QEvent *event);
signals:
	void OutgoingMessage(QString);
};
#endif