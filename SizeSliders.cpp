#include "SizeSliders.h"
#include <QSpinBox>
#include <QLabel>
#include <QGridLayout>
SizeSliders::SizeSliders(QWidget* parent)
	:QWidget(parent)
	,AttachedWidget(NULL)
{
	QGridLayout* MainLayout=new QGridLayout(this);
	WidthLabel=new QLabel(this);
	WidthLabel->setText("Width:");
	WidthLabel->setObjectName("WidthLabel");
	MainLayout->addWidget(WidthLabel,0,0);
	WidthSlider=new QSpinBox(this);
	WidthSlider->setRange(-10000,10000);
	WidthSlider->setObjectName("WidthSlider");
	connect(WidthSlider,SIGNAL(valueChanged(int)),this,SLOT(WidChange(int)));
	MainLayout->addWidget(WidthSlider,0,1);

	HeightLabel=new QLabel(this);
	HeightLabel->setText("Height:");
	HeightLabel->setObjectName("HeightLabel");
	MainLayout->addWidget(HeightLabel,1,0);
	HeightSlider=new QSpinBox(this);
	HeightSlider->setRange(-10000,10000);
	HeightSlider->setObjectName("HeightSlider");
	connect(HeightSlider,SIGNAL(valueChanged(int)),this,SLOT(HeiChange(int)));
	MainLayout->addWidget(HeightSlider,1,1);

	HorPosLabel=new QLabel(this);
	HorPosLabel->setText("X:");
	HorPosLabel->setObjectName("HorPosLabel");
	MainLayout->addWidget(HorPosLabel,2,0);
	HorPosSlider=new QSpinBox(this);
	HorPosSlider->setRange(-10000,10000);
	HorPosSlider->setObjectName("HorPosSlider");
	connect(HorPosSlider,SIGNAL(valueChanged(int)),this,SLOT(PosXChange(int)));
	MainLayout->addWidget(HorPosSlider,2,1);

	VerPosLabel=new QLabel(this);
	VerPosLabel->setText("Y:");
	VerPosLabel->setObjectName("VerPosLabel");
	MainLayout->addWidget(VerPosLabel,3,0);
	VerPosSlider=new QSpinBox(this);
	VerPosSlider->setRange(-10000,10000);
	VerPosSlider->setObjectName("HorPosSlider");
	connect(VerPosSlider,SIGNAL(valueChanged(int)),this,SLOT(PosYChange(int)));
	MainLayout->addWidget(VerPosSlider,3,1);
	connect(this,SIGNAL(GeometryChanged(int,int,int,int)),this,SLOT(EditAttachedWidget()));
}
void SizeSliders::UpdateSliders() const{
	WidthSlider->setValue(wid);
	HeightSlider->setValue(hei);
	HorPosSlider->setValue(posx);
	VerPosSlider->setValue(posy);
}
void SizeSliders::AttachToWidget(QWidget* a){
	AttachedWidget=a;
	HorPosSlider->setValue(a->pos().x());
	VerPosSlider->setValue(a->pos().y());
	WidthSlider->setValue(a->width());
	HeightSlider->setValue(a->height());
}
void SizeSliders::EditAttachedWidget(){
	if (AttachedWidget) AttachedWidget->setGeometry(posx,posy,wid,hei);
}