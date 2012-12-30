#ifndef COSTANTPROPORTIONLAYOUT_H
#define COSTANTPROPORTIONLAYOUT_H
#include <QLayout>
#include <QList>
#include <QStyle>
class ConstantProportionLayout : public QLayout{
public:
	ConstantProportionLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
	ConstantProportionLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
	~ConstantProportionLayout();
	void addItem(QLayoutItem *item);
	int horizontalSpacing() const;
	int verticalSpacing() const;
	int count() const;
	QLayoutItem *itemAt(int index) const;
	QLayoutItem *takeAt(int index);
	Qt::Orientations expandingDirections() const;
	bool hasHeightForWidth() const{return true;}
	int heightForWidth(int width) const;
	QSize minimumSize() const;
	void setGeometry(const QRect &rect);
	QSize sizeHint() const;
	void SetHorizontal(){Vertical=false;}
	void SetVertical(){Vertical=true;}
private:
	int doLayout(const QRect &rect, bool testOnly) const;
	int smartSpacing(QStyle::PixelMetric pm) const;
	int CalculateHeight(const QRect &rect) const;
	QList<QLayoutItem *> itemList;
	int m_hSpace;
	int m_vSpace;
	bool Vertical;
};
#endif