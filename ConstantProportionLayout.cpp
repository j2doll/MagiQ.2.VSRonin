#include "ConstantProportionLayout.h"
#include <QLayoutItem>
#include <QWidget>
ConstantProportionLayout::ConstantProportionLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
	: QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing), Vertical(true)
{
	setContentsMargins(margin, margin, margin, margin);
}

ConstantProportionLayout::ConstantProportionLayout(int margin, int hSpacing, int vSpacing)
	: m_hSpace(hSpacing), m_vSpace(vSpacing), Vertical(true)
{
	setContentsMargins(margin, margin, margin, margin);
}
ConstantProportionLayout::~ConstantProportionLayout()
{
	QLayoutItem *item;
	while ((item = takeAt(0)))
		delete item;
}
void ConstantProportionLayout::addItem(QLayoutItem *item)
{
	itemList.append(item);
}
int ConstantProportionLayout::horizontalSpacing() const
{
	if (m_hSpace >= 0) {
		return m_hSpace;
	} else {
		return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
	}
}

int ConstantProportionLayout::verticalSpacing() const
{
	if (m_vSpace >= 0) {
		return m_vSpace;
	} else {
		return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
	}
}
int ConstantProportionLayout::count() const
{
	return itemList.size();
}

QLayoutItem *ConstantProportionLayout::itemAt(int index) const
{
	return itemList.value(index);
}

QLayoutItem *ConstantProportionLayout::takeAt(int index)
{
	if (index >= 0 && index < itemList.size())
		return itemList.takeAt(index);
	else
		return 0;
}


int ConstantProportionLayout::heightForWidth(int width) const
{
	int height = doLayout(QRect(0, 0, width, 0), true);
	return height;
}
void ConstantProportionLayout::setGeometry(const QRect &rect)
{
	QLayout::setGeometry(rect);
	doLayout(rect, false);
}

QSize ConstantProportionLayout::sizeHint() const {return minimumSize();}
QSize ConstantProportionLayout::minimumSize() const
{
	QSize size;
	QLayoutItem *item;
	foreach (item, itemList)
		size = size.expandedTo(item->minimumSize());

	size += QSize(2*margin(), 2*margin());
	return size;
}
int ConstantProportionLayout::doLayout(const QRect &rect, bool testOnly) const
{
	if (CalculateHeight(rect)>rect.height())
		return rect.height();
	int left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
	int x = effectiveRect.x();
	int y = effectiveRect.y();
	int spaceY=0;
	QLayoutItem *item;
	foreach (item, itemList) {
		if (!item->isEmpty()){
			QWidget *wid = item->widget();
			if (Vertical){
				spaceY = verticalSpacing();
				if (spaceY == -1)
					spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
				if (!testOnly)
					item->setGeometry(QRect(x, y, effectiveRect.width(), effectiveRect.width()*item->sizeHint().height()/item->sizeHint().width()));
				y=y+effectiveRect.width()*item->sizeHint().height()/item->sizeHint().width()+spaceY;
			}
			else{
				int spaceX = horizontalSpacing();
				if (spaceX == -1)
					spaceX = wid->style()->layoutSpacing(
					QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
				if (!testOnly)
					item->setGeometry(QRect(x, y, effectiveRect.width(), effectiveRect.width()*item->sizeHint().height()/item->sizeHint().width()));
				x=x+effectiveRect.width()+spaceX;
			}
		}
	}
	return y + -spaceY+ bottom;
}
int ConstantProportionLayout::CalculateHeight(const QRect &rect) const{
	int left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
	int x = effectiveRect.x();
	int y = effectiveRect.y();
	int spaceY=0;
	QLayoutItem *item;
	foreach (item, itemList) {
		if (!item->isEmpty()){
			QWidget *wid = item->widget();
			if (Vertical){
				spaceY = verticalSpacing();
				if (spaceY == -1)
					spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
				y=y+effectiveRect.width()*item->sizeHint().height()/item->sizeHint().width()+spaceY;
			}
			else{
				int spaceX = horizontalSpacing();
				if (spaceX == -1)
					spaceX = wid->style()->layoutSpacing(
					QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
				x=x+effectiveRect.width()+spaceX;
			}
		}
	}
	return y + -spaceY+ bottom;
}
int ConstantProportionLayout::smartSpacing(QStyle::PixelMetric pm) const
{
	QObject *parent = this->parent();
	if (!parent) {
		return -1;
	} else if (parent->isWidgetType()) {
		QWidget *pw = static_cast<QWidget *>(parent);
		return pw->style()->pixelMetric(pm, 0, pw);
	} else {
		return static_cast<QLayout *>(parent)->spacing();
	}
}
Qt::Orientations ConstantProportionLayout::expandingDirections() const {
	if (Vertical)
		return Qt::Horizontal;
	else
		return Qt::Vertical;
}