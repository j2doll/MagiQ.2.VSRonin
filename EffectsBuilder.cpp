#include "EffectsBuilder.h"
#include "EffectsConstants.h"
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QFrame>
#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include "Effect.h"
#include "Smiles Selector.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QHeaderView>
EffectsBuilder::EffectsBuilder(QWidget* parent)
	:QWidget(parent)
{
	Background=new QFrame(this);
	Background->setObjectName("Background");

	QGroupBox* EffectPreviewGroup=new QGroupBox(this);
	QVBoxLayout* EffectPreviewlayout=new QVBoxLayout(EffectPreviewGroup);
	EffectPreviewGroup->setObjectName("EffectPreviewGroup");
	EffectPreviewGroup->setTitle(tr("Preview"));
	EffectPreview=new Effect(this);
	EffectPreview->setObjectName("EffectPreview");
	EffectPreviewlayout->addWidget(EffectPreview);

	QGroupBox* NameTextGroup=new QGroupBox(this);
	NameTextGroup->setObjectName("NameTextGroup");
	NameTextGroup->setTitle(tr("Name and Text"));
	QGridLayout* NameTextLayout=new QGridLayout(NameTextGroup);
	QLabel* EffectTextLabel=new QLabel(this);
	EffectTextLabel->setObjectName("EffectTextLabel");
	EffectTextLabel->setText(tr("Effect Text"));
	NameTextLayout->addWidget(EffectTextLabel,0,0);
	SymbolsMenuButton=new QPushButton(this);
	SymbolsMenuButton->setObjectName("SymbolsMenuButton");
	SymbolsMenuButton->setIcon(QIcon(QPixmap(":/Effects/TapBig.png")));
	SymbolsMenuButton->setToolTip(tr("Insert Symbol"));
	NameTextLayout->addWidget(SymbolsMenuButton,0,1);
	SymbolsSelector=new SmilesSelector(this);
	SymbolsSelector->setObjectName("SymbolsSelector");
	connect(SymbolsMenuButton,SIGNAL(clicked()),SymbolsSelector,SLOT(show_toggle()));
	connect(SymbolsSelector,SIGNAL(selected(int)),this,SLOT(AddSymbol(int)));
	EffectTextEditor=new QTextEdit(this);
	EffectTextEditor->setObjectName("EffectTextEditor");
	connect(EffectTextEditor,SIGNAL(textChanged()),this,SLOT(SetEffectText()));
	NameTextLayout->addWidget(EffectTextEditor,1,0,1,2);

	QGroupBox* EffectTypeGroup=new QGroupBox(this);
	QGridLayout* EffectTypelayout=new QGridLayout(EffectTypeGroup);
	EffectTypeGroup->setObjectName("EffectTypeGroup");
	EffectTypeGroup->setTitle(tr("Effect Type"));
	EffectTypeCombo=new QComboBox(this);
	EffectTypeCombo->setObjectName("EffectTypeCombo");
	for (int i=0;i<=EffectsConstants::EffectTypes::OnResolutionEffect;i++){
		EffectTypeCombo->addItem(
			EffectsConstants::EffectTypesNames[i]
			,i);
	}
	connect(EffectTypeCombo,SIGNAL(currentIndexChanged (int)),this,SLOT(SetEffectType(int)));
	EffectTypelayout->addWidget(EffectTypeCombo,0,0,1,2);
	TriggersLabel=new QLabel(this);
	TriggersLabel->setObjectName("TriggersLabel");
	TriggersLabel->setText(tr("Add Trigger"));
	EffectTypelayout->addWidget(TriggersLabel,1,0);
	ResetTriggersButton=new QPushButton(this);
	ResetTriggersButton->setObjectName("ResetTriggersButton");
	ResetTriggersButton->setText("Reset Triggers");
	connect(ResetTriggersButton,SIGNAL(clicked()),this,SLOT(ResetTriggers()));
	EffectTypelayout->addWidget(ResetTriggersButton,2,1);
	AddTriggerCombo=new QComboBox(this);
	AddTriggerCombo->setObjectName("AddTriggerCombo");
	AddTriggerCombo->addItem("",-1);
	for (int i=EffectsConstants::Triggers::EntersTheBattlefield;i<=EffectsConstants::Triggers::Dies;i++){
		QString TextToPrint(EffectsConstants::TriggersNames[i]);
		if (TextToPrint.contains("%1")){
			if (TextToPrint.contains("%2"))
				TextToPrint=TextToPrint.arg(tr("When")).arg(tr("Card"));
		}
		AddTriggerCombo->addItem(
			TextToPrint
			,i);
	}
	connect(AddTriggerCombo,SIGNAL(currentIndexChanged (int)),this,SLOT(AddTrigger(int)));
	EffectTypelayout->addWidget(AddTriggerCombo,2,0);
	TriggersLabel->hide();
	AddTriggerCombo->hide();
	ResetTriggersButton->hide();

	QGroupBox* CostTragetBox=new QGroupBox(this);
	QGridLayout* CostTragetlayout=new QGridLayout(CostTragetBox);
	CostTragetBox->setObjectName("CostTragetBox");
	CostTragetBox->setTitle(tr("Cost and Targets"));
	QLabel* CostLabel=new QLabel(this);
	CostLabel->setObjectName("CostLabel");
	CostLabel->setText(tr("Add Cost"));
	CostTragetlayout->addWidget(CostLabel,0,0);
	ResetCostButton=new QPushButton(this);
	ResetCostButton->setObjectName("ResetCostButton");
	ResetCostButton->setEnabled(false);
	ResetCostButton->setText("Reset Cost");
	connect(ResetCostButton,SIGNAL(clicked()),this,SLOT(ResetCost()));
	CostTragetlayout->addWidget(ResetCostButton,1,1);
	AddCostCombo=new QComboBox(this);
	AddCostCombo->setObjectName("AddCostCombo");
	AddCostCombo->addItem("",-1);
	for (int i=EffectsConstants::EffectCosts::ColorlessMana;i<=EffectsConstants::EffectCosts::Tap;i++){
		QString IconPath(EffectsConstants::EffectCostStrings[i]);
		AddCostCombo->addItem(
			QIcon(QPixmap(IconPath.replace(QRegExp("<img src=\"(.+)\" >"),"\\1")))
			,""
			,i);
	}
	for (int i=EffectsConstants::EffectCosts::Tap+1;i<EffectsConstants::EffectCosts::END;i++){
		QString TextToPrint(EffectsConstants::EffectCostStrings[i]);
		if (TextToPrint.contains("%1")){
			TextToPrint=TextToPrint.arg(1);
		}
		AddCostCombo->addItem(
			TextToPrint
			,i);
	}
	connect(AddCostCombo,SIGNAL(currentIndexChanged (int)),this,SLOT(AddCost(int)));
	CostTragetlayout->addWidget(AddCostCombo,1,0);
	CostViewer=new QLabel(this);
	CostViewer->setObjectName("CostViewer");
	CostViewer->setText(tr("Cost: "));
	CostViewer->setAlignment(Qt::AlignCenter);
	CostTragetlayout->addWidget(CostViewer,2,0,1,2);
	QLabel* TargetTypeLabel=new QLabel(this);
	TargetTypeLabel->setObjectName("TargetTypeLabel");
	TargetTypeLabel->setText(tr("Select Target Type"));
	CostTragetlayout->addWidget(TargetTypeLabel,3,0);
	QLabel* TargetNumberLabel=new QLabel(this);
	TargetNumberLabel->setObjectName("TargetNumberLabel");
	TargetNumberLabel->setText(tr("Select Target Type"));
	CostTragetlayout->addWidget(TargetNumberLabel,3,1);
	TargetTypeSelector=new QComboBox(this);
	TargetTypeSelector->setObjectName("TargetTypeSelector");
	TargetTypeSelector->addItem("No Targets",EffectsConstants::Targets::NoTargets);
	for (int i=0;i<=EffectsConstants::Targets::Planeswalker;i++){
		TargetTypeSelector->addItem(
			EffectsConstants::TargetNames[i]
			,i);
	}
	connect(TargetTypeSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(TargetTypeSelected(int)));
	CostTragetlayout->addWidget(TargetTypeSelector,4,0);
	TargetNumberSpin=new QSpinBox(this);
	TargetNumberSpin->setObjectName("TargetNumberSpin");
	TargetNumberSpin->setRange(1,100);
	TargetNumberSpin->setEnabled(false);
	CostTragetlayout->addWidget(TargetNumberSpin,4,1);
	TargetsTable=new QTableWidget(this);
	TargetsTable->setObjectName("TargetsTable");
	TargetsTable->setColumnCount(2);
	QStringList TempHeaders;
	TempHeaders.append(tr("Target Type"));
	TempHeaders.append(tr("Number of Targets"));
	TargetsTable->setHorizontalHeaderLabels(TempHeaders);
	TargetsTable->verticalHeader()->setVisible(false);
	TargetsTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	CostTragetlayout->addWidget(TargetsTable,6,0,1,2);
	AddTargetButton=new QPushButton(this);
	AddTargetButton->setObjectName("AddTargetButton");
	AddTargetButton->setText(tr("Add Target"));
	AddTargetButton->setEnabled(false);
	connect(AddTargetButton,SIGNAL(clicked()),this,SLOT(AddTarget()));
	CostTragetlayout->addWidget(AddTargetButton,5,0);
	ResetTargetButton=new QPushButton(this);
	ResetTargetButton->setObjectName("ResetTargetButton");
	ResetTargetButton->setText(tr("Reset Targets"));
	ResetTargetButton->setEnabled(false);
	connect(ResetTargetButton,SIGNAL(clicked()),this,SLOT(ResetTarget()));
	CostTragetlayout->addWidget(ResetTargetButton,5,1);
	QGroupBox* CheckBoxesGroup=new QGroupBox(this);
	CheckBoxesGroup->setObjectName("CheckBoxesGroup");
	CheckBoxesGroup->setTitle(tr("Effect Properties"));
	ManaEffectCheck=new QCheckBox(this);
	ManaEffectCheck->setObjectName("ManaEffectCheck");
	ManaEffectCheck->setText(tr("Is a Mana Effect"));
	connect(ManaEffectCheck,SIGNAL(stateChanged(int)),this,SLOT(SetManaEffect()));
	DoesntStackCheck=new QCheckBox(this);
	DoesntStackCheck->setObjectName("DoesntStackCheck");
	DoesntStackCheck->setText(tr("Effect resolves without going into the stack"));
	connect(DoesntStackCheck,SIGNAL(stateChanged(int)),this,SLOT(SetDoesntStack()));
	HiddenEffectCheck=new QCheckBox(this);
	HiddenEffectCheck->setObjectName("HiddenEffectCheck");
	HiddenEffectCheck->setText(tr("Effect is not printed on the card"));
	connect(HiddenEffectCheck,SIGNAL(stateChanged(int)),this,SLOT(SetHiddenEffect()));
	QVBoxLayout* ChecksLayout=new QVBoxLayout(CheckBoxesGroup);
	ChecksLayout->addWidget(ManaEffectCheck);
	ChecksLayout->addWidget(DoesntStackCheck);
	ChecksLayout->addWidget(HiddenEffectCheck);

	QVBoxLayout* MainLayout=new QVBoxLayout(this);
	MainLayout->addWidget(EffectPreviewGroup);
	MainLayout->addWidget(NameTextGroup);
	MainLayout->addWidget(EffectTypeGroup);
	MainLayout->addWidget(CostTragetBox);
	MainLayout->addWidget(CheckBoxesGroup);
}
void EffectsBuilder::resizeEvent(QResizeEvent* event){
	Background->setGeometry(0,0,width(),height());
	SymbolsSelector->setGeometry(51*width()/333,18*height()/513,250,130); //FIXME
	TargetsTable->setColumnWidth(0,TargetsTable->width()*3/5);
	TargetsTable->setColumnWidth(1,TargetsTable->width()*2/5);
}
void EffectsBuilder::SetEffectType(int index){
	if (EffectTypeCombo->itemData(index).toInt()==EffectsConstants::EffectTypes::TriggeredEffect){
		TriggersLabel->show();
		AddTriggerCombo->show();
		ResetTriggersButton->show();
	}
	else{
		ResetTriggers();
		TriggersLabel->hide();
		AddTriggerCombo->hide();
		ResetTriggersButton->hide();
	}
	EffectPreview->SetEffectType(EffectTypeCombo->itemData(index).toInt());
	EffectPreview->UpdateAspect();
}
void EffectsBuilder::ResetTriggers(){
	EffectPreview->SetTriggers();
	EffectPreview->UpdateAspect();
}
void EffectsBuilder::AddTrigger(int index){
	if (index==0) return;
	EffectPreview->AddTrigger(AddTriggerCombo->itemData(index).toInt());
	EffectPreview->UpdateAspect();
	AddTriggerCombo->setCurrentIndex(0);
}
void EffectsBuilder::SetManaEffect(){
	EffectPreview->SetManaEffect(ManaEffectCheck->isChecked());
}
void EffectsBuilder::SetHiddenEffect(){
	EffectPreview->SetHiddenEffect(HiddenEffectCheck->isChecked());
	EffectPreview->UpdateAspect();
}
void EffectsBuilder::SetDoesntStack(){
	EffectPreview->SetDoesntStack(DoesntStackCheck->isChecked());
}
void EffectsBuilder::ResetCost(){
	CostViewer->setText(tr("Cost: "));
	ResetCostButton->setEnabled(false);
	EffectPreview->SetEffectCost();
	EffectPreview->UpdateAspect();
}
void EffectsBuilder::AddCost(int index){
	if (index==0) return;
	ResetCostButton->setEnabled(true);
	EffectPreview->AddEffectCost(AddCostCombo->itemData(index).toInt(),1);
	EffectPreview->UpdateAspect();
	QString TextToPrint=CostViewer->text();
	TextToPrint.append(EffectsConstants::EffectCostStrings[AddCostCombo->itemData(index).toInt()]);
	CostViewer->setText(TextToPrint);
	AddCostCombo->setCurrentIndex(0);
}
void EffectsBuilder::SetEffectText(){
	EffectPreview->SetEffectText(EffectTextEditor->toHtml());
	EffectPreview->UpdateAspect();
}
void EffectsBuilder::AddSymbol(int index){
	if (index<0 || index>=Symbols::Num_Symbols) return;
	EffectTextEditor->insertHtml(QString("<img src=\"%1\" height=\"13\" />").arg(Symbols::symbols_paths[index]));
}
void EffectsBuilder::TargetTypeSelected(int index){
	if (index==0){
		TargetNumberSpin->setEnabled(false);
		AddTargetButton->setEnabled(false);
	}
	else {
		TargetNumberSpin->setEnabled(true);
		AddTargetButton->setEnabled(true);
	}
	TargetNumberSpin->setValue(1);
}
void  EffectsBuilder::AddTarget(){
	int TempType=TargetTypeSelector->itemData(TargetTypeSelector->currentIndex()).toInt();
	EffectPreview->AddTarget(TempType,TargetNumberSpin->value());
	TargetsTable->setSortingEnabled(false);
	int temp=TargetsTable->rowCount();
	TargetsTable->setRowCount(temp+1);
	TargetsTable->setItem(temp,0,
		new QTableWidgetItem(EffectsConstants::TargetNames[TempType])
	);
	TargetsTable->setItem(temp,1,
		new QTableWidgetItem(QString("%1").arg(TargetNumberSpin->value()))
	);
	TargetsTable->setRowHeight(temp,TableRowHeight);
	TargetsTable->setSortingEnabled(true);
	ResetTargetButton->setEnabled(true);
	TargetTypeSelector->setCurrentIndex(0);
}
void  EffectsBuilder::ResetTarget(){
	TargetTypeSelector->setCurrentIndex(0);
	EffectPreview->SetTargets();
	TargetsTable->setRowCount(0);
	TargetNumberSpin->setEnabled(false);
	AddTargetButton->setEnabled(false);
	ResetTargetButton->setEnabled(false);
}