#include "CardBuilder.h"
#include "Card.h"
#include <QLineEdit>
#include <QCheckBox>
#include "ManaCostLabel.h"
#include "CostantsDefinition.h"
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QSpacerItem>
#include <QFileDialog>
CardBuilder::CardBuilder(QWidget* parent)
	:QWidget(parent)
{
	Background=new QFrame(this);
	Background->setObjectName("Background");

	QVBoxLayout* CardLayout=new QVBoxLayout;
	CardPreview=new Card(this);
	CardPreview->setObjectName("CardPreview");
	CardPreview->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
	QSpacerItem* PreviewSpacer=new QSpacerItem(20,20,QSizePolicy::Minimum,QSizePolicy::Expanding);
	CardLayout->addWidget(CardPreview);
	CardLayout->addItem(PreviewSpacer);

	CertifiedCheck=new QCheckBox(this);
	CertifiedCheck->setChecked(false);
	CertifiedCheck->setObjectName("CertifiedCheck");
	CertifiedCheck->setText(tr("Certified Card"));
	connect(CertifiedCheck,SIGNAL(stateChanged(int)),this,SLOT(SetCertified()));
	CertifiedCheck->hide();

	QLabel* NameLabel=new QLabel(this);
	NameLabel->setObjectName("NameLabel");
	NameLabel->setText(tr("Card Name"));
	NameEditor=new QLineEdit(this);
	NameEditor->setObjectName("NameEditor");
	connect(NameEditor,SIGNAL(textEdited(QString)),this,SLOT(SetCardName()));
	QVBoxLayout* NameLayout=new QVBoxLayout;
	NameLayout->addWidget(NameLabel);
	NameLayout->addWidget(NameEditor);

	CardColorGroup=new QGroupBox(this);
	CardColorGroup->setTitle(tr("Card Color"));
	CardColorGroup->setObjectName("CardColorGroup");
	QGridLayout* CardColorLayout=new QGridLayout(CardColorGroup);
	ColorlessCheck=new QCheckBox(this);
	ColorlessCheck->setChecked(true);
	ColorlessCheck->setObjectName("ColorlessCheck");
	ColorlessCheck->setText(tr("Colorless"));
	connect(ColorlessCheck,SIGNAL(stateChanged(int)),this,SLOT(SetCardColor()));
	CardColorLayout->addWidget(ColorlessCheck,0,0);
	WhiteCheck=new QCheckBox(this);
	WhiteCheck->setChecked(false);
	WhiteCheck->setObjectName("WhiteCheck");
	WhiteCheck->setText(tr("White"));
	connect(WhiteCheck,SIGNAL(stateChanged(int)),this,SLOT(SetCardColor()));
	CardColorLayout->addWidget(WhiteCheck,1,0);
	BlueCheck=new QCheckBox(this);
	BlueCheck->setChecked(false);
	BlueCheck->setObjectName("BlueCheck");
	BlueCheck->setText(tr("Blue"));
	connect(BlueCheck,SIGNAL(stateChanged(int)),this,SLOT(SetCardColor()));
	CardColorLayout->addWidget(BlueCheck,2,0);
	BlackCheck=new QCheckBox(this);
	BlackCheck->setChecked(false);
	BlackCheck->setObjectName("BlackCheck");
	BlackCheck->setText(tr("Black"));
	connect(BlackCheck,SIGNAL(stateChanged(int)),this,SLOT(SetCardColor()));
	CardColorLayout->addWidget(BlackCheck,0,1);
	RedCheck=new QCheckBox(this);
	RedCheck->setChecked(false);
	RedCheck->setObjectName("RedCheck");
	RedCheck->setText(tr("Red"));
	connect(RedCheck,SIGNAL(stateChanged(int)),this,SLOT(SetCardColor()));
	CardColorLayout->addWidget(RedCheck,1,1);
	GreenCheck=new QCheckBox(this);
	GreenCheck->setChecked(false);
	GreenCheck->setObjectName("GreenCheck");
	GreenCheck->setText(tr("Green"));
	connect(GreenCheck,SIGNAL(stateChanged(int)),this,SLOT(SetCardColor()));
	CardColorLayout->addWidget(GreenCheck,2,1);
	SetCardColor();

	QLabel* CardCostLabel=new QLabel(this);
	CardCostLabel->setObjectName("CardCostLabel");
	CardCostLabel->setText(tr("Add Mana Cost"));
	ManaCostSelector=new QComboBox(this);
	ManaCostSelector->setObjectName("ManaCostSelector");
	ManaCostSelector->addItem("",-1);
	{
		QPixmap Temp(QPixmap(":/ManaSymbols/CMana.png"));
		QPainter TextPainter(&Temp);
		TextPainter.setFont(QFont ("Arial",60,QFont::Bold));
		TextPainter.drawText(Temp.rect(),Qt::AlignCenter,"1");
		ManaCostSelector->addItem(QIcon(Temp),tr("Colorless"),Constants::ManaCosts::Colorless);
	}	
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/WMana.png")),tr("Plain"),Constants::ManaCosts::W);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/UMana.png")),tr("Island"),Constants::ManaCosts::U);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/BMana.png")),tr("Swamp"),Constants::ManaCosts::B);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/RMana.png")),tr("Mountain"),Constants::ManaCosts::R);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/GMana.png")),tr("Forest"),Constants::ManaCosts::G);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/WUMana.png")),tr("Azorius"),Constants::ManaCosts::WU);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/WBMana.png")),tr("Orzhov"),Constants::ManaCosts::WB);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/WRMana.png")),tr("Boros"),Constants::ManaCosts::WR);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/WGMana.png")),tr("Selesnya"),Constants::ManaCosts::WG);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/UBMana.png")),tr("Dimir"),Constants::ManaCosts::UB);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/URMana.png")),tr("Izzet"),Constants::ManaCosts::UR);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/UGMana.png")),tr("Simic"),Constants::ManaCosts::UG);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/BRMana.png")),tr("Rakdos"),Constants::ManaCosts::BR);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/BGMana.png")),tr("Golgari"),Constants::ManaCosts::BG);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/RGMana.png")),tr("Gruul"),Constants::ManaCosts::RG);
	ManaCostSelector->addItem(QIcon(QPixmap(":/ManaSymbols/SnowMana.png")),tr("Snow"),Constants::ManaCosts::S);
	{
		QPixmap Temp(QPixmap(":/ManaSymbols/CMana.png"));
		QPainter TextPainter(&Temp);
		TextPainter.setFont(QFont ("Arial",60,QFont::Bold));
		TextPainter.drawText(Temp.rect(),Qt::AlignCenter,"X");
		ManaCostSelector->addItem(QIcon(Temp),"X",Constants::ManaCosts::X);
	}
	{
		QPixmap Temp(QPixmap(":/ManaSymbols/CMana.png"));
		QPainter TextPainter(&Temp);
		TextPainter.setFont(QFont ("Arial",60,QFont::Bold));
		TextPainter.drawText(Temp.rect(),Qt::AlignCenter,"Y");
		ManaCostSelector->addItem(QIcon(Temp),"Y",Constants::ManaCosts::Y);
	}
	{
		QPixmap Temp(QPixmap(":/ManaSymbols/CMana.png"));
		QPainter TextPainter(&Temp);
		TextPainter.setFont(QFont ("Arial",60,QFont::Bold));
		TextPainter.drawText(Temp.rect(),Qt::AlignCenter,"Z");
		ManaCostSelector->addItem(QIcon(Temp),"Z",Constants::ManaCosts::Z);
	}	
	//TODO Aggiungi Le Altre
	ManaCostSelector->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	connect(ManaCostSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(AddCardCost(int)));
	ManaCostResetButton=new QPushButton(this);
	ManaCostResetButton->setObjectName("ManaCostResetButton");
	ManaCostResetButton->setText(tr("Reset Mana Cost"));
	connect(ManaCostResetButton,SIGNAL(clicked()),this,SLOT(ResetCardCost()));
	QGridLayout* ManaCostLayout=new QGridLayout;
	ManaCostLayout->addWidget(CardCostLabel,0,0);
	ManaCostLayout->addWidget(ManaCostSelector,1,0);
	ManaCostLayout->addWidget(ManaCostResetButton,1,1);

	QLabel* CardTypeLabel=new QLabel(this);
	CardTypeLabel->setObjectName("CardTypeLabel");
	CardTypeLabel->setText(tr("Add Type"));
	CardTypeSelector=new QComboBox(this);
	CardTypeSelector->addItem("",-1);
	CardTypeSelector->setObjectName("CardTypeSelector");
	for (int i=Constants::CardTypes::Artifact;i<Constants::CardTypes::END;i++){
		CardTypeSelector->addItem(Constants::CardTypeNames[i],i);
	}
	CardTypeSelector->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	connect(CardTypeSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(AddCardType(int)));
	CardTypeResetButton=new QPushButton(this);
	CardTypeResetButton->setObjectName("CardTypeResetButton");
	CardTypeResetButton->setText(tr("Reset Card Type"));
	connect(CardTypeResetButton,SIGNAL(clicked()),this,SLOT(ResetCardType()));
	QGridLayout* CardTypeLayout=new QGridLayout;
	CardTypeLayout->addWidget(CardTypeLabel,0,0);
	CardTypeLayout->addWidget(CardTypeSelector,1,0);
	CardTypeLayout->addWidget(CardTypeResetButton,1,1);


	QLabel* CardSubTypeLabel=new QLabel(this);
	CardSubTypeLabel->setObjectName("CardSubTypeLabel");
	CardSubTypeLabel->setText(tr("Add Sub-Type"));
	CardSubTypeSelector=new QComboBox(this);
	CardSubTypeSelector->addItem("",-1);
	CardSubTypeSelector->setObjectName("CardSubTypeSelector");
	for (int i=0;i<Constants::CardSubTypes::END;i++){
		CardSubTypeSelector->addItem(Constants::CardSubTypesNames[i],i);
	}
	CardSubTypeSelector->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	connect(CardSubTypeSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(AddCardSubType(int)));
	CardSubTypeResetButton=new QPushButton(this);
	CardSubTypeResetButton->setObjectName("CardSubTypeResetButton");
	CardSubTypeResetButton->setText(tr("Reset Card Sub-Type"));
	connect(CardSubTypeResetButton,SIGNAL(clicked()),this,SLOT(ResetCardSubType()));
	QGridLayout* CardSubTypeLayout=new QGridLayout;
	CardSubTypeLayout->addWidget(CardSubTypeLabel,0,0);
	CardSubTypeLayout->addWidget(CardSubTypeSelector,1,0);
	CardSubTypeLayout->addWidget(CardSubTypeResetButton,1,1);

	QLabel* CardAvailableEditionsLabel=new QLabel(this);
	CardAvailableEditionsLabel->setObjectName("CardAvailableEditionsLabel");
	CardAvailableEditionsLabel->setText(tr("Add Edition"));
	CardAvailableEditionsSelector=new QComboBox(this);
	CardAvailableEditionsSelector->addItem("",-1);
	CardAvailableEditionsSelector->setObjectName("CardAvailableEditionsSelector");
	for (int i=0;i<Constants::Editions::END;i++){
		CardAvailableEditionsSelector->addItem(Constants::EditionNames[i],i);
	}
	CardAvailableEditionsSelector->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	connect(CardAvailableEditionsSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(AddCardEdition(int)));
	CardAvailableEditionsResetButton=new QPushButton(this);
	CardAvailableEditionsResetButton->setObjectName("CardAvailableEditionsResetButton");
	CardAvailableEditionsResetButton->setText(tr("Reset Editions"));
	connect(CardAvailableEditionsResetButton,SIGNAL(clicked()),this,SLOT(ResetCardEdition()));
	QGridLayout* CardEditionLayout=new QGridLayout;
	CardEditionLayout->addWidget(CardAvailableEditionsLabel,0,0);
	CardEditionLayout->addWidget(CardAvailableEditionsSelector,1,0);
	CardEditionLayout->addWidget(CardAvailableEditionsResetButton,1,1);

	QLabel* CardEditionLabel=new QLabel(this);
	CardEditionLabel->setObjectName("CardEditionLabel");
	CardEditionLabel->setText(tr("Select Preferred Edition"));
	CardEditionSelector=new QComboBox(this);
	CardEditionSelector->setObjectName("CardEditionSelector");
	CardEditionSelector->setEnabled(false);
	connect(CardEditionSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(SelectEdition(int)));
	QVBoxLayout* EditionSelectorLayout=new QVBoxLayout;
	EditionSelectorLayout->addWidget(CardEditionLabel);
	EditionSelectorLayout->addWidget(CardEditionSelector);

	QLabel* CardRarityLabel=new QLabel(this);
	CardRarityLabel->setObjectName("CardRarityLabel");
	CardRarityLabel->setText(tr("Select Card Rarity"));
	CardRaritySelector=new QComboBox(this);
	CardRaritySelector->setObjectName("CardRaritySelector");
	CardRaritySelector->setEnabled(false);
	CardRaritySelector->addItem(
		QIcon(QPixmap(":/Editions/CommonBackground.png")),
		tr("Common"),
		Constants::CardRarities::Common
		);
	CardRaritySelector->addItem(
		QIcon(QPixmap(":/Editions/UncommonBackground.png")),
		tr("Uncommon"),
		Constants::CardRarities::Uncommon
		);
	CardRaritySelector->addItem(
		QIcon(QPixmap(":/Editions/RareBackground.png")),
		tr("Rare"),
		Constants::CardRarities::Rare
		);
	CardRaritySelector->addItem(
		//QIcon(QPixmap(":/Editions/TimeshiftedBackground.png")),
		tr("Timeshifted"),
		Constants::CardRarities::Timeshifted
		);
	CardRaritySelector->addItem(
		//QIcon(QPixmap(":/Editions/MythicRareBackground.png")),
		tr("Mythic Rare"),
		Constants::CardRarities::MythicRare
		);
	connect(CardRaritySelector,SIGNAL(currentIndexChanged (int)),this,SLOT(SelectRarity(int)));
	QVBoxLayout* RaritySelectorLayout=new QVBoxLayout;
	RaritySelectorLayout->addWidget(CardRarityLabel);
	RaritySelectorLayout->addWidget(CardRaritySelector);

	QLabel* CardImageLabel=new QLabel(this);
	CardImageLabel->setObjectName("CardImageLabel");
	CardImageLabel->setText(tr("Add Image"));
	BrowseForImageButton=new QPushButton(this);
	BrowseForImageButton->setObjectName("BrowseForImageButton");
	BrowseForImageButton->setText(tr("Browse for Image..."));
	BrowseForImageButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	connect(BrowseForImageButton,SIGNAL(clicked()),this,SLOT(AddCardImage()));
	ResetImagesButton=new QPushButton(this);
	ResetImagesButton->setObjectName("ResetImagesButton");
	ResetImagesButton->setText(tr("Reset Images"));
	connect(ResetImagesButton,SIGNAL(clicked()),this,SLOT(ResetCardImages()));
	QGridLayout* ImageLayout=new QGridLayout;
	ImageLayout->addWidget(CardImageLabel,0,0);
	ImageLayout->addWidget(BrowseForImageButton,1,0);
	ImageLayout->addWidget(ResetImagesButton,1,1);

	QLabel* CardImgLabel=new QLabel(this);
	CardImgLabel->setObjectName("CardImgLabel");
	CardImgLabel->setText(tr("Select Preferred Image"));
	CardImageSelector=new QComboBox(this);
	CardImageSelector->setObjectName("CardImageSelector");
	CardImageSelector->setEnabled(false);
	connect(CardImageSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(SelectImage(int)));
	QVBoxLayout* ImageSelectorLayout=new QVBoxLayout;
	ImageSelectorLayout->addWidget(CardImgLabel);
	ImageSelectorLayout->addWidget(CardImageSelector);

	QLabel* CardAvailableBackgroundLabel=new QLabel(this);
	CardAvailableBackgroundLabel->setObjectName("CardAvailableBackgroundLabel");
	CardAvailableBackgroundLabel->setText(tr("Add Background"));
	CardAvailableBackgroundsSelector=new QComboBox(this);
	CardAvailableBackgroundsSelector->addItem("",-1);
	CardAvailableBackgroundsSelector->setObjectName("CardAvailableBackgroundsSelector");
	for (int i=0;i<Constants::CardBacksrounds::END;i++){
		CardAvailableBackgroundsSelector->addItem(
			//QIcon(QPixmap(Constants::BackgroundImages[i])),
			Constants::BackgroundNames[i],
			i);
	}
	CardAvailableBackgroundsSelector->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	connect(CardAvailableBackgroundsSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(AddCardBackground(int)));
	CardAvailableBackgroundsResetButton=new QPushButton(this);
	CardAvailableBackgroundsResetButton->setObjectName("CardAvailableBackgroundsResetButton");
	CardAvailableBackgroundsResetButton->setText(tr("Reset Backgrounds"));
	connect(CardAvailableBackgroundsResetButton,SIGNAL(clicked()),this,SLOT(ResetCardBackground()));
	QGridLayout* CardBackgroundLayout=new QGridLayout;
	CardBackgroundLayout->addWidget(CardAvailableBackgroundLabel,0,0);
	CardBackgroundLayout->addWidget(CardAvailableBackgroundsSelector,1,0);
	CardBackgroundLayout->addWidget(CardAvailableBackgroundsResetButton,1,1);

	QLabel* CardBackgroundLabel=new QLabel(this);
	CardBackgroundLabel->setObjectName("CardEditionLabel");
	CardBackgroundLabel->setText(tr("Select Preferred Edition"));
	CardBackgroundSelector=new QComboBox(this);
	CardBackgroundSelector->setObjectName("CardBackgroundSelector");
	CardBackgroundSelector->setEnabled(false);
	connect(CardBackgroundSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(SelectBackground(int)));
	QVBoxLayout* BackgroundSelectorLayout=new QVBoxLayout;
	BackgroundSelectorLayout->addWidget(CardBackgroundLabel);
	BackgroundSelectorLayout->addWidget(CardBackgroundSelector);

	QHBoxLayout* MainLayout=new QHBoxLayout(this);
	MainLayout->addLayout(CardLayout);
	QVBoxLayout* RightLayout=new QVBoxLayout;
	RightLayout->addWidget(CertifiedCheck);
	RightLayout->addLayout(NameLayout);
	RightLayout->addWidget(CardColorGroup);
	RightLayout->addLayout(ManaCostLayout);
	RightLayout->addLayout(CardTypeLayout);
	RightLayout->addLayout(CardSubTypeLayout);
	RightLayout->addLayout(CardEditionLayout);
	RightLayout->addLayout(EditionSelectorLayout);
	RightLayout->addLayout(RaritySelectorLayout);
	RightLayout->addLayout(ImageLayout);
	RightLayout->addLayout(ImageSelectorLayout);
	RightLayout->addLayout(CardBackgroundLayout);
	RightLayout->addLayout(BackgroundSelectorLayout);
	QSpacerItem* RightSpacer=new QSpacerItem(20,20,QSizePolicy::Minimum,QSizePolicy::Expanding);
	RightLayout->addItem(RightSpacer);
	MainLayout->addLayout(RightLayout);
}
void CardBuilder::resizeEvent(QResizeEvent* event){
	Background->setGeometry(0,0,width(),height());
}
void CardBuilder::SetCertified(){
	CardPreview->SetCertified(CertifiedCheck->isChecked());
}
void CardBuilder::SetCardColor(){
	CardPreview->SetCardColor(-1);
	if (ColorlessCheck->isChecked()){
		WhiteCheck->setChecked(false);
		BlueCheck->setChecked(false);
		BlackCheck->setChecked(false);
		RedCheck->setChecked(false);
		GreenCheck->setChecked(false);
		WhiteCheck->setEnabled(false);
		BlueCheck->setEnabled(false);
		BlackCheck->setEnabled(false);
		RedCheck->setEnabled(false);
		GreenCheck->setEnabled(false);
		return CardPreview->AddCardColor(Constants::CardColor::Colorless);
	}
	else {
		WhiteCheck->setEnabled(true);
		BlueCheck->setEnabled(true);
		BlackCheck->setEnabled(true);
		RedCheck->setEnabled(true);
		GreenCheck->setEnabled(true);
	}
	if (WhiteCheck->isChecked()){
		CardPreview->AddCardColor(Constants::CardColor::White);
		ColorlessCheck->setEnabled(false);
	}
	if (BlueCheck->isChecked()){
		CardPreview->AddCardColor(Constants::CardColor::Blue);
		ColorlessCheck->setEnabled(false);
	}
	if (BlackCheck->isChecked()){
		CardPreview->AddCardColor(Constants::CardColor::Black);
		ColorlessCheck->setEnabled(false);
	}
	if (RedCheck->isChecked()){
		CardPreview->AddCardColor(Constants::CardColor::Red);
		ColorlessCheck->setEnabled(false);
	}
	if (GreenCheck->isChecked()){
		CardPreview->AddCardColor(Constants::CardColor::Green);
		ColorlessCheck->setEnabled(false);
	}
	if (!(WhiteCheck->isChecked() || BlueCheck->isChecked() || BlackCheck->isChecked() || RedCheck->isChecked() || GreenCheck->isChecked()))
		ColorlessCheck->setEnabled(true);
}
void CardBuilder::SetCardName(){
	CardPreview->SetCardName(NameEditor->text().trimmed());
	CardPreview->UpdateAspect();
}
void CardBuilder::ResetCardCost(){
	CardPreview->SetCardCost();
	ManaCostSelector->setCurrentIndex(0);
	CardPreview->UpdateAspect();
}
void CardBuilder::AddCardCost(int index){
	if (index==0) return;
	CardPreview->AddCardCost(ManaCostSelector->itemData(index).toInt(),1);
	ManaCostSelector->setCurrentIndex(0);
	CardPreview->UpdateAspect();
}
void CardBuilder::ResetCardType(){
	CardPreview->SetCardType();
	CardTypeSelector->setCurrentIndex(0);
	CardPreview->UpdateAspect();
}
void CardBuilder::AddCardType(int index){
	if (index==0) return;
	if (CardPreview->GetCardType().contains(CardTypeSelector->itemData(index).toInt())) return;
	CardPreview->AddCardType(CardTypeSelector->itemData(index).toInt());
	CardTypeSelector->setCurrentIndex(0);
	CardPreview->UpdateAspect();
}
void CardBuilder::ResetCardSubType(){
	CardPreview->SetCardSubType();
	CardSubTypeSelector->setCurrentIndex(0);
	CardPreview->UpdateAspect();
}
void CardBuilder::AddCardSubType(int index){
	if (index==0) return;
	if (CardPreview->GetCardSubType().contains(CardSubTypeSelector->itemData(index).toInt())) return;
	CardPreview->AddCardSubType(CardSubTypeSelector->itemData(index).toInt());
	CardSubTypeSelector->setCurrentIndex(0);
	CardPreview->UpdateAspect();
}
void CardBuilder::AddCardEdition(int index){
	if (index==0) return;
	if (CardPreview->GetAvailableEditions().contains(CardAvailableEditionsSelector->itemData(index).toInt())) return;
	CardPreview->AddAvailableEditions(CardAvailableEditionsSelector->itemData(index).toInt());
	CardAvailableEditionsSelector->setCurrentIndex(0);
	CardEditionSelector->addItem(
		QIcon(QPixmap(Constants::EditionSymbolsIcons[CardAvailableEditionsSelector->itemData(index).toInt()])),
		Constants::EditionNames[CardAvailableEditionsSelector->itemData(index).toInt()],
		CardAvailableEditionsSelector->itemData(index).toInt());
	CardEditionSelector->setEnabled(true);
	CardRaritySelector->setEnabled(true);
	CardPreview->UpdateAspect();
}
void CardBuilder::ResetCardEdition(){
	CardPreview->SetAvailableEditions();
	CardAvailableEditionsSelector->setCurrentIndex(0);
	CardEditionSelector->clear();
	CardEditionSelector->setEnabled(false);
	CardRaritySelector->setEnabled(false);
	CardPreview->UpdateAspect();
}
void CardBuilder::SelectEdition(int index){
	CardPreview->SetEdition(CardEditionSelector->itemData(index).toInt());
	CardPreview->UpdateAspect();
}
void CardBuilder::AddCardImage(){
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Image"), "/Pics", tr("Image Files (*.png *.jpg *.bmp)"));
	if(fileName.isNull()) return;
	QPixmap TempPix(fileName);
	CardPreview->AddAvailableImages(TempPix);
	int TempNum = CardPreview->GetAvailableImages().size();
	CardImageSelector->addItem(
		QIcon(TempPix),
		tr("Image %1").arg(TempNum)
		,TempNum-1
		);
	CardImageSelector->setEnabled(true);
	if (TempNum==1){
		CardPreview->SetCardImage(0);
	}
	CardPreview->UpdateAspect();
}
void CardBuilder::ResetCardImages(){
	CardPreview->SetAvailableImages();
	CardImageSelector->clear();
	CardImageSelector->setEnabled(true);
	CardPreview->SetCardImage(-1);
	CardPreview->UpdateAspect();
}
void CardBuilder::SelectImage(int index){
	CardPreview->SetCardImage(CardImageSelector->itemData(index).toInt());
	CardPreview->UpdateAspect();
}
void CardBuilder::AddCardBackground(int index){
	if (index==0) return;
	if (CardPreview->GetAvailableBackgrounds().contains(CardAvailableBackgroundsSelector->itemData(index).toInt())) return;
	CardPreview->AddAvailableBackground(CardAvailableBackgroundsSelector->itemData(index).toInt());
	CardAvailableBackgroundsSelector->setCurrentIndex(0);
	CardBackgroundSelector->addItem(
		//QIcon(QPixmap(Constants::BackgroundImages[CardAvailableEditionsSelector->itemData(index).toInt()])),
		Constants::BackgroundNames[CardAvailableBackgroundsSelector->itemData(index).toInt()],
		CardAvailableBackgroundsSelector->itemData(index).toInt());
	CardBackgroundSelector->setEnabled(true);
	CardPreview->UpdateAspect();
}
void CardBuilder::ResetCardBackground(){
	CardPreview->SetAvailableBackgrounds();
	CardAvailableBackgroundsSelector->setCurrentIndex(0);
	CardBackgroundSelector->clear();
	CardBackgroundSelector->setEnabled(false);
	CardPreview->UpdateAspect();
}
void CardBuilder::SelectBackground(int index){
	CardPreview->SetCardBackground(CardBackgroundSelector->itemData(index).toInt());
	CardPreview->UpdateAspect();
}
void CardBuilder::SelectRarity(int index){
	CardPreview->SetCardrarity(CardRaritySelector->itemData(index).toInt());
	CardPreview->UpdateAspect();
}