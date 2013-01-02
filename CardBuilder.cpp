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
#include <QTextEdit>
#include <QMessageBox>
#include <QCloseEvent>
#include "ConstantProportionLayout.h"
#include "SelettoreImmagini.h"
CardBuilder::CardBuilder(QWidget* parent)
	:QWidget(parent)
	,EditingFlip(false)
{
	setMinimumHeight(720);
	setWindowIcon(QIcon(QPixmap(":/CardImage/Rear.png")));
	setWindowTitle(tr("Card Editor"));
	Background=new QFrame(this);
	Background->setObjectName("Background");

	ConstantProportionLayout* CardLayout=new ConstantProportionLayout;
	CardPreview=new Card(this);
	CardPreview->setObjectName("CardPreview");
	CardPreview->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	AddFlippedButton=new QPushButton(this);
	AddFlippedButton->setObjectName("AddFlippedButton");
	AddFlippedButton->setText("Add Split or Flip Card");
	connect(AddFlippedButton,SIGNAL(clicked()),this,SLOT(AddFlippedCard()));
	FlippedCard=new Card(this);
	FlippedCard->setObjectName("FlippedCard");
	FlippedCard->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	FlippedCard->SetCovered(true);
	FlippedCard->UpdateAspect();
	RemoveFlippedButton=new QPushButton(this);
	RemoveFlippedButton->setObjectName("RemoveFlippedButton");
	RemoveFlippedButton->setText("Remove Split or Flip Card");
	RemoveFlippedButton->hide();
	connect(RemoveFlippedButton,SIGNAL(clicked()),this,SLOT(RemoveFlippedCard()));
	connect(RemoveFlippedButton,SIGNAL(clicked()),RemoveFlippedButton,SLOT(hide()));
	CardLayout->addWidget(CardPreview);
	CardLayout->addWidget(AddFlippedButton);
	CardLayout->addWidget(FlippedCard);
	CardLayout->addWidget(RemoveFlippedButton);

	CertifiedCheck=new QCheckBox(this);
	CertifiedCheck->setChecked(false);
	CertifiedCheck->setObjectName("CertifiedCheck");
	CertifiedCheck->setText(tr("Certified Card"));
	connect(CertifiedCheck,SIGNAL(stateChanged(int)),this,SLOT(SetCertified()));
	QHBoxLayout* BasicChecksLayout=new QHBoxLayout;
	BasicChecksLayout->addWidget(CertifiedCheck);
	CertifiedCheck->hide();

	QGroupBox* NameGroup=new QGroupBox(this);
	NameGroup->setObjectName("NameGroup");
	NameGroup->setTitle(tr("Card Name"));
	NameEditor=new QLineEdit(this);
	NameEditor->setObjectName("NameEditor");
	connect(NameEditor,SIGNAL(textEdited(QString)),this,SLOT(SetCardName()));
	QVBoxLayout* NameLayout=new QVBoxLayout(NameGroup);
	NameLayout->addWidget(NameEditor);

	QGroupBox* ColorCostGroup=new QGroupBox(this);
	ColorCostGroup->setObjectName("ColorCostGroup");
	ColorCostGroup->setTitle(tr("Mana Cost & Card Color"));
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
	HasManaCostCheck=new QCheckBox(this);
	HasManaCostCheck->setObjectName("HasManaCostCheck");
	HasManaCostCheck->setChecked(false);
	HasManaCostCheck->setText("No Mana Cost");
	connect(HasManaCostCheck,SIGNAL(stateChanged(int)),this,SLOT(SetNoManaCost()));
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
	QGridLayout* ManaCostLayout=new QGridLayout(ColorCostGroup);
	ManaCostLayout->addWidget(CardCostLabel,0,0);
	ManaCostLayout->addWidget(ManaCostSelector,1,0);
	ManaCostLayout->addWidget(HasManaCostCheck,0,1);
	ManaCostLayout->addWidget(ManaCostResetButton,1,1);
	ManaCostLayout->addWidget(CardColorGroup,2,0,1,2);

	QGroupBox* CardTypeGroup=new QGroupBox(this);
	CardTypeGroup->setObjectName("CardTypeGroup");
	CardTypeGroup->setTitle(tr("Card Type"));
	QLabel* CardTypeLabel=new QLabel(this);
	CardTypeLabel->setObjectName("CardTypeLabel");
	CardTypeLabel->setText(tr("Add Type"));
	CardTypeSelector=new QComboBox(this);
	CardTypeSelector->addItem("",-1);
	CardTypeSelector->setObjectName("CardTypeSelector");
	for (int i=0;i<Constants::CardTypes::END;i++){
		CardTypeSelector->addItem(Constants::CardTypeNames[i],i);
	}
	CardTypeSelector->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	connect(CardTypeSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(AddCardType(int)));
	CardTypeResetButton=new QPushButton(this);
	CardTypeResetButton->setObjectName("CardTypeResetButton");
	CardTypeResetButton->setText(tr("Reset Card Type"));
	connect(CardTypeResetButton,SIGNAL(clicked()),this,SLOT(ResetCardType()));
	QGridLayout* CardTypeLayout=new QGridLayout(CardTypeGroup);
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
	CardTypeLayout->addWidget(CardSubTypeLabel,2,0);
	CardTypeLayout->addWidget(CardSubTypeSelector,3,0);
	CardTypeLayout->addWidget(CardSubTypeResetButton,3,1);

	QLabel* CardAvailableEditionsLabel=new QLabel(this);
	CardAvailableEditionsLabel->setObjectName("CardAvailableEditionsLabel");
	CardAvailableEditionsLabel->setText(tr("Add Edition"));
	CardAvailableEditionsSelector=new QComboBox(this);
	CardAvailableEditionsSelector->addItem("",-1);
	CardAvailableEditionsSelector->setObjectName("CardAvailableEditionsSelector");
	for (int i=0;i<Constants::Editions::END;i++){
		CardAvailableEditionsSelector->addItem(
			QIcon(QPixmap(Constants::EditionSymbolsIcons[i])),
			Constants::EditionNames[i],
			i);
	}
	CardAvailableEditionsSelector->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	connect(CardAvailableEditionsSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(AddCardEdition(int)));
	CardAvailableEditionsResetButton=new QPushButton(this);
	CardAvailableEditionsResetButton->setObjectName("CardAvailableEditionsResetButton");
	CardAvailableEditionsResetButton->setText(tr("Reset Editions"));
	connect(CardAvailableEditionsResetButton,SIGNAL(clicked()),this,SLOT(ResetCardEdition()));
	

	QGroupBox* EditionRarityGroup=new QGroupBox(this);
	EditionRarityGroup->setObjectName("EditionRarityGroup");
	EditionRarityGroup->setTitle(tr("Edition and Rarity"));
	QLabel* CardEditionLabel=new QLabel(this);
	CardEditionLabel->setObjectName("CardEditionLabel");
	CardEditionLabel->setText(tr("Select Preferred Edition"));
	CardEditionSelector=new QComboBox(this);
	CardEditionSelector->setObjectName("CardEditionSelector");
	CardEditionSelector->setEnabled(false);
	connect(CardEditionSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(SelectEdition(int)));
	

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
	QGridLayout* CardEditionLayout=new QGridLayout(EditionRarityGroup);
	CardEditionLayout->addWidget(CardAvailableEditionsLabel,0,0);
	CardEditionLayout->addWidget(CardAvailableEditionsSelector,1,0);
	CardEditionLayout->addWidget(CardAvailableEditionsResetButton,1,1);
	CardEditionLayout->addWidget(CardEditionLabel,2,0,1,2);
	CardEditionLayout->addWidget(CardEditionSelector,3,0,1,2);
	CardEditionLayout->addWidget(CardRarityLabel,4,0,1,2);
	CardEditionLayout->addWidget(CardRaritySelector,5,0,1,2);

	QGroupBox* ImageGroup=new QGroupBox(this);
	ImageGroup->setObjectName("ImageGroup");
	ImageGroup->setTitle(tr("Card Image"));
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

	QLabel* CardImgLabel=new QLabel(this);
	CardImgLabel->setObjectName("CardImgLabel");
	CardImgLabel->setText(tr("Select Preferred Image"));
	CardImageSelector=new SelettoreImmagini(this);
	CardImageSelector->setObjectName("CardImageSelector");
	CardImageSelector->ImpostaPosDidascalia(SelettoreImmagini::Sopra);
	CardImageSelector->setEnabled(false);
	connect(CardImageSelector,SIGNAL(IndexChanged(int)),this,SLOT(SelectImage(int)));
	CardImageJump=new QComboBox(this);
	CardImageJump->setObjectName("CardImageJump");
	CardImageJump->hide();
	connect(CardImageJump,SIGNAL(currentIndexChanged(int)),this,SLOT(ImageJump(int)));
	QGridLayout* ImageLayout=new QGridLayout(ImageGroup);
	ImageLayout->addWidget(CardImageLabel,0,0);
	ImageLayout->addWidget(BrowseForImageButton,1,0);
	ImageLayout->addWidget(ResetImagesButton,1,1);
	ImageLayout->addWidget(CardImgLabel,2,0,1,2);
	ImageLayout->addWidget(CardImageJump,3,0,1,2);
	ImageLayout->addWidget(CardImageSelector,4,0,1,2);
	
	QGroupBox* CardBackgroundGroup=new QGroupBox(this);
	CardBackgroundGroup->setObjectName("CardBackgroundGroup");
	CardBackgroundGroup->setTitle(tr("Background"));
	QLabel* CardAvailableBackgroundLabel=new QLabel(this);
	CardAvailableBackgroundLabel->setObjectName("CardAvailableBackgroundLabel");
	CardAvailableBackgroundLabel->setText(tr("Add Background"));
	CardAvailableBackgroundsSelector=new QComboBox(this);
	CardAvailableBackgroundsSelector->addItem("",-1);
	CardAvailableBackgroundsSelector->setObjectName("CardAvailableBackgroundsSelector");
	for (int i=0;i<Constants::CardBacksrounds::END;i++){
		CardAvailableBackgroundsSelector->addItem(
			QIcon(QPixmap(Constants::BackgroundImages[i])),
			Constants::BackgroundNames[i],
			i);
	}
	CardAvailableBackgroundsSelector->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	connect(CardAvailableBackgroundsSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(AddCardBackground(int)));
	CardAvailableBackgroundsResetButton=new QPushButton(this);
	CardAvailableBackgroundsResetButton->setObjectName("CardAvailableBackgroundsResetButton");
	CardAvailableBackgroundsResetButton->setText(tr("Reset Backgrounds"));
	connect(CardAvailableBackgroundsResetButton,SIGNAL(clicked()),this,SLOT(ResetCardBackground()));
	QGridLayout* CardBackgroundLayout=new QGridLayout(CardBackgroundGroup);
	CardBackgroundLayout->addWidget(CardAvailableBackgroundLabel,0,0);
	CardBackgroundLayout->addWidget(CardAvailableBackgroundsSelector,1,0);
	CardBackgroundLayout->addWidget(CardAvailableBackgroundsResetButton,1,1);

	QLabel* CardBackgroundLabel=new QLabel(this);
	CardBackgroundLabel->setObjectName("CardEditionLabel");
	CardBackgroundLabel->setText(tr("Select Preferred Background"));
	CardBackgroundSelector=new QComboBox(this);
	CardBackgroundSelector->setObjectName("CardBackgroundSelector");
	CardBackgroundSelector->setEnabled(false);
	connect(CardBackgroundSelector,SIGNAL(currentIndexChanged (int)),this,SLOT(SelectBackground(int)));
	CardBackgroundLayout->addWidget(CardBackgroundLabel,2,0,1,2);
	CardBackgroundLayout->addWidget(CardBackgroundSelector,3,0,1,2);

	QLabel* FlavorTextLabel=new QLabel(this);
	FlavorTextLabel->setObjectName("FlavorTextLabel");
	FlavorTextLabel->setText(tr("Flavor Text"));
	CardFlavorTextEdit=new QTextEdit(this);
	CardFlavorTextEdit->setObjectName("CardFlavorTextEdit");
	CardFlavorTextEdit->setHtml("");
	CardFlavorTextEdit->setToolTip(tr("You can use HTML tags to customize the text"));
	connect(CardFlavorTextEdit,SIGNAL(textChanged()),this,SLOT(SetFlavorText()));
	QVBoxLayout* FlavorTextLayout=new QVBoxLayout;
	FlavorTextLayout->addWidget(FlavorTextLabel);
	FlavorTextLayout->addWidget(CardFlavorTextEdit);

	HasPTGroup=new QGroupBox(this);
	HasPTGroup->setObjectName("HasPTGroup");
	HasPTGroup->setCheckable(true);
	HasPTGroup->setChecked(false);
	HasPTGroup->setTitle(tr("Has Power & Toughness"));
	connect(HasPTGroup,SIGNAL(toggled(bool)),this,SLOT(SetHasPT(bool)));
	QGridLayout* PTGroupLayout=new QGridLayout(HasPTGroup);
	QRegExpValidator PTValidator(QRegExp("[*\\d+]"));
	PowerLabel=new QLabel(this);
	PowerLabel->setObjectName("PowerLabel");
	PowerLabel->setText(tr("Power"));
	PowerEdit=new QLineEdit(this);
	PowerEdit->setEnabled(false);
	PowerEdit->setObjectName("PowerEdit");
	PowerEdit->setValidator(&PTValidator);
	connect(PowerEdit,SIGNAL(textEdited(QString)),this,SLOT(SetPower()));
	QLabel* ToughnessLabel=new QLabel(this);
	ToughnessLabel->setObjectName("ToughnessLabel");
	ToughnessLabel->setText(tr("Toughness"));
	ToughnesEdit=new QLineEdit(this);
	ToughnesEdit->setObjectName("ToughnesEdit");
	ToughnesEdit->setValidator(&PTValidator);
	ToughnesEdit->setEnabled(false);
	connect(ToughnesEdit,SIGNAL(textEdited(QString)),this,SLOT(SetToughnes()));
	PTGroupLayout->addWidget(PowerLabel,0,0);
	PTGroupLayout->addWidget(PowerEdit,1,0);
	PTGroupLayout->addWidget(ToughnessLabel,0,1);
	PTGroupLayout->addWidget(ToughnesEdit,1,1);

	ResetButton=new QPushButton(this);
	ResetButton->setObjectName("ResetButton");
	ResetButton->setText(tr("Reset"));
	connect(ResetButton,SIGNAL(clicked()),this,SLOT(ResetAll()));
	OpenButton=new QPushButton(this);
	OpenButton->setObjectName("OpenButton");
	OpenButton->setText(tr("Open Card File"));
	connect(OpenButton,SIGNAL(clicked()),this,SLOT(OpenCard()));
	SaveButton=new QPushButton(this);
	SaveButton->setObjectName("SaveButton");
	SaveButton->setText(tr("Save"));
	connect(SaveButton,SIGNAL(clicked()),this,SLOT(SaveCard()));
	ExitButton=new QPushButton(this);
	ExitButton->setObjectName("ExitButton");
	ExitButton->setText(tr("Exit"));
	connect(ExitButton,SIGNAL(clicked()),this,SLOT(close()));
	QHBoxLayout* WidgetButtonsLayout=new QHBoxLayout;
	QSpacerItem* RightButtonSpacer=new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
	QSpacerItem* LeftButtonSpacer=new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
	WidgetButtonsLayout->addItem(LeftButtonSpacer);
	WidgetButtonsLayout->addWidget(OpenButton);
	WidgetButtonsLayout->addWidget(SaveButton);
	WidgetButtonsLayout->addWidget(ResetButton);
	WidgetButtonsLayout->addWidget(ExitButton);
	WidgetButtonsLayout->addItem(RightButtonSpacer);

	QHBoxLayout* MainLayout=new QHBoxLayout;
	MainLayout->addLayout(CardLayout);
	QVBoxLayout* RightLayout=new QVBoxLayout;
	RightLayout->addLayout(BasicChecksLayout);
	RightLayout->addWidget(NameGroup);
	RightLayout->addWidget(ColorCostGroup);
	RightLayout->addWidget(CardTypeGroup);
	RightLayout->addWidget(EditionRarityGroup);
	QVBoxLayout* RightLayout2=new QVBoxLayout;
	RightLayout2->addWidget(ImageGroup);
	RightLayout2->addWidget(CardBackgroundGroup);
	RightLayout2->addLayout(FlavorTextLayout);
	RightLayout2->addWidget(HasPTGroup);
	MainLayout->addLayout(RightLayout);
	MainLayout->addLayout(RightLayout2);
	QVBoxLayout* WidgetLayout=new QVBoxLayout(this);
	WidgetLayout->addLayout(MainLayout);
	WidgetLayout->addLayout(WidgetButtonsLayout);
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
	ColorlessCheck->setEnabled(true);
	ColorlessCheck->setChecked(true);
	CardPreview->UpdateAspect();
}
void CardBuilder::AddCardCost(int index){
	if (index==0) return;
	CardPreview->AddCardCost(ManaCostSelector->itemData(index).toInt(),1);
	QMap<int,int> TempMap(CardPreview->GetCardCost());
	if (TempMap[Constants::ManaCosts::W]>0 || TempMap[Constants::ManaCosts::LW]>0 || TempMap[Constants::ManaCosts::C2W]>0){
		ColorlessCheck->setChecked(false);
		WhiteCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::U]>0 || TempMap[Constants::ManaCosts::LU]>0 || TempMap[Constants::ManaCosts::C2U]>0){
		ColorlessCheck->setChecked(false);
		BlueCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::B]>0 || TempMap[Constants::ManaCosts::LB]>0 || TempMap[Constants::ManaCosts::C2B]>0){
		ColorlessCheck->setChecked(false);
		BlackCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::R]>0 || TempMap[Constants::ManaCosts::LR]>0 || TempMap[Constants::ManaCosts::C2R]>0){
		ColorlessCheck->setChecked(false);;
		RedCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::G]>0 || TempMap[Constants::ManaCosts::LG]>0 || TempMap[Constants::ManaCosts::C2G]>0){
		ColorlessCheck->setChecked(false);
		GreenCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::WU]>0){
		ColorlessCheck->setChecked(false);
		WhiteCheck->setChecked(true);
		BlueCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::WB]>0){
		ColorlessCheck->setChecked(false);
		WhiteCheck->setChecked(true);
		BlackCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::WR]>0){
		ColorlessCheck->setChecked(false);
		WhiteCheck->setChecked(true);
		RedCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::WG]>0){
		ColorlessCheck->setChecked(false);
		WhiteCheck->setChecked(true);
		GreenCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::UB]>0){
		ColorlessCheck->setChecked(false);
		BlackCheck->setChecked(true);
		BlueCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::UR]>0){
		ColorlessCheck->setChecked(false);
		RedCheck->setChecked(true);
		BlueCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::UG]>0){
		ColorlessCheck->setChecked(false);
		GreenCheck->setChecked(true);
		BlueCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::BR]>0){
		ColorlessCheck->setChecked(false);
		RedCheck->setChecked(true);
		BlackCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::BG]>0){
		ColorlessCheck->setChecked(false);
		GreenCheck->setChecked(true);
		BlackCheck->setChecked(true);
	}
	if (TempMap[Constants::ManaCosts::RG]>0){
		ColorlessCheck->setChecked(false);
		GreenCheck->setChecked(true);
		RedCheck->setChecked(true);
	}
	ManaCostSelector->setCurrentIndex(0);
	CardPreview->UpdateAspect();
}
void CardBuilder::ResetCardType(){
	CardPreview->SetCardType();
	CardTypeSelector->setCurrentIndex(0);
	CardPreview->UpdateAspect();
	HasPTGroup->setChecked(false);
	HasManaCostCheck->setChecked(false);
}
void CardBuilder::AddCardType(int index){
	if (index==0) return;
	if (CardPreview->GetCardType().contains(CardTypeSelector->itemData(index).toInt())) return;
	CardPreview->AddCardType(CardTypeSelector->itemData(index).toInt());
	QList<int> Temp(CardPreview->GetCardType());
	if (Temp.contains(Constants::CardTypes::Creature))
		HasPTGroup->setChecked(true);
	if (Temp.contains(Constants::CardTypes::Planeswalker)) HasPTGroup->setChecked(true);
	if (Temp.contains(Constants::CardTypes::Land)) HasManaCostCheck->setChecked(true);
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
	QStringList fileNames = QFileDialog::getOpenFileNames(this,
		tr("Open Image"), "/Pics/", tr("Image Files (*.png *.jpg *.bmp)"));
	if (fileNames.isEmpty()) return;
	foreach(QString fileName,fileNames){
		if(fileName.isNull()) continue;
		QPixmap TempPix(fileName);
		CardPreview->AddAvailableImages(TempPix);
		int TempNum = CardPreview->GetAvailableImages().size();
		QString TempString(fileName.right(fileName.size()-fileName.lastIndexOf(QRegExp("[\\\\/]"))-1));
		TempString=TempString.left(TempString.lastIndexOf('.'));
		CardImageSelector->AggiungiImmagine(
			TempPix,
			TempString
			);
		CardImageSelector->setEnabled(true);
		CardImageJump->addItem(TempPix,TempString,TempNum-1);
		if(TempNum>5) CardImageJump->show();
		if (TempNum==1){
			CardPreview->SetCardImage(0);
		}
	}
	CardPreview->UpdateAspect();
}
void CardBuilder::ResetCardImages(){
	CardPreview->SetAvailableImages();
	CardImageSelector->CancellaTutte();
	CardImageSelector->setEnabled(false);
	CardImageJump->clear();
	CardImageJump->hide();
	CardPreview->SetCardImage(-1);
	CardPreview->UpdateAspect();
}
void CardBuilder::SelectImage(int index){
	CardImageJump->setCurrentIndex(index);
	//CardImageJump->setCurrentIndex(CardImageJump->findData(index)); //Safer but Slower
	CardPreview->SetCardImage(index);
	CardPreview->UpdateAspect();
}
void CardBuilder::ImageJump(int index){
	CardImageSelector->SetIndex(CardImageJump->itemData(index).toInt());
}
void CardBuilder::AddCardBackground(int index){
	if (index==0) return;
	if (CardPreview->GetAvailableBackgrounds().contains(CardAvailableBackgroundsSelector->itemData(index).toInt())) return;
	CardPreview->AddAvailableBackground(CardAvailableBackgroundsSelector->itemData(index).toInt());
	CardAvailableBackgroundsSelector->setCurrentIndex(0);
	CardBackgroundSelector->addItem(
		QIcon(QPixmap(Constants::BackgroundImages[CardAvailableBackgroundsSelector->itemData(index).toInt()])),
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
void CardBuilder::SetFlavorText(){
	CardPreview->SetCardFlavorText(CardFlavorTextEdit->toHtml());
	CardPreview->UpdateAspect();
}
void CardBuilder::SetHasPT(bool a){
	if (!a){
		PowerEdit->setText("");
		ToughnesEdit->setText("");
	}
	bool Temp=CardPreview->GetCardType().contains(Constants::CardTypes::Planeswalker);
	PowerEdit->setEnabled(a);
	ToughnesEdit->setEnabled(a && !Temp);
	if (Temp){PowerLabel->setText(tr("Loyalty"));}
	else {PowerLabel->setText(tr("Power"));}
	CardPreview->SetHasPT(a);
	CardPreview->UpdateAspect();
}
void CardBuilder::SetPower(){
	if (PowerEdit->text()=="*") CardPreview->SetCardPower(Card::StarPowerToughness);
	else CardPreview->SetCardPower(PowerEdit->text().toInt());
	CardPreview->UpdateAspect();
}
void CardBuilder::SetToughnes(){
	if (ToughnesEdit->text()=="*") CardPreview->SetCardToughness(Card::StarPowerToughness);
	else CardPreview->SetCardToughness(ToughnesEdit->text().toInt());
	CardPreview->UpdateAspect();
}
void CardBuilder::SetNoManaCost(){
	CardPreview->SetHasManaCost(!HasManaCostCheck->isChecked());
	ManaCostSelector->setEnabled(!HasManaCostCheck->isChecked());
	ManaCostResetButton->setEnabled(!HasManaCostCheck->isChecked());
	CardPreview->UpdateAspect();
}
void CardBuilder::RemoveFlippedCard(){
	CardPreview->SetFlippedCard(NULL);
	CardPreview->SetHasFlipped(Card::NoFlip);
	FlippedCard->operator=(Card());
	FlippedCard->SetCovered(true);
	FlippedCard->UpdateAspect();
	AddFlippedButton->setText(tr("Add Split or Flip Card"));
}
void CardBuilder::AddFlippedCard(){
	FlippedCard->SetCovered(false);
	CardBuilder* FlippedBuilder=new CardBuilder;
	FlippedBuilder->setWindowModality(Qt::ApplicationModal);
	FlippedBuilder->SetCard(FlippedCard);
	FlippedBuilder->EditingFlipCard(CardPreview);
	connect(FlippedBuilder,SIGNAL(FlippedAccepted(Card*)),this,SLOT(SetFlippedCard(Card*)));
	connect(FlippedBuilder,SIGNAL(FlippedAccepted(Card*)),FlippedBuilder,SLOT(deleteLater()));
	connect(FlippedBuilder,SIGNAL(FlippedRejected()),FlippedBuilder,SLOT(deleteLater()));
	FlippedBuilder->show();
}
void CardBuilder::ResetAll(){
	QMessageBox SureToReset(QMessageBox::Question,tr("Are you Sure?"),tr("Are you sure you want to reset the card?\nAny unsaved change will be lost"),QMessageBox::Yes | QMessageBox::No,this);
	SureToReset.setDefaultButton(QMessageBox::Yes);
	if (SureToReset.exec()==QMessageBox::No) return;
	CertifiedCheck->setChecked(false);
	NameEditor->setText("");
	CardPreview->SetCardName("");
	CardFlavorTextEdit->setHtml("");
	CardPreview->SetCardFlavorText("");
	ColorlessCheck->setEnabled(true);
	ColorlessCheck->setChecked(true);
	ResetCardCost();
	ResetCardType();
	ResetCardSubType();
	ResetCardEdition();
	ResetCardImages();
	ResetCardBackground();
	HasManaCostCheck->setChecked(false);
	RemoveFlippedCard();
	CardPreview->UpdateAspect();
}
void CardBuilder::closeEvent(QCloseEvent *event){
	QMessageBox SureToExit(QMessageBox::Question,tr("Are you Sure?"),tr("Are you sure you want to Exit?\nAny unsaved change will be lost"),QMessageBox::Yes | QMessageBox::No,this);
	SureToExit.setDefaultButton(QMessageBox::Yes);
	if (SureToExit.exec()==QMessageBox::No) return event->ignore();
	if (EditingFlip) emit FlippedRejected();
	event->accept();
}
void CardBuilder::OpenCard(){
	QString fileName = QFileDialog::getOpenFileName(0, tr("Open File"),
		"/Cards/",
		tr("MagiQ Card (*.mqc)"));
	if (fileName.isEmpty()) return;
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this,tr("Error"),tr("Cannot open file for reading: ") + file.errorString());
		return;
	}
	QDataStream input(&file);
	input.setVersion(QDataStream::Qt_4_7);
	input >> *CardPreview;
	file.close();
	if (CardPreview->GetHasFlipped()==Card::HasFlip){
		SetFlippedCard(CardPreview->GetFlippedCard());
	}
	SetCard(CardPreview);
}
void CardBuilder::SaveCard(){
	if (EditingFlip){
		emit FlippedAccepted(CardPreview);
		hide();
		return;
	}
	QString fileName = QFileDialog::getSaveFileName(0, tr("Save File"),
		"/Cards/"+CardPreview->GetCardName()+".mqc",
		tr("MagiQ Card (*.mqc)"));
	if (fileName.isEmpty()) return;
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::critical(this,tr("Error"),tr("Cannot open file for writing: ") + file.errorString());
		return;
	}
	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_4_7);
	out << *CardPreview;
	file.close();
}
void CardBuilder::EditingFlipCard(Card* ParentCard){
	EditingFlip=true;
	FlippedCard->operator=(*ParentCard);
	FlippedCard->SetCovered(false);
	AddFlippedButton->hide();
	CardPreview->SetHasFlipped(Card::AllreadyFlipped);
	CardPreview->SetFlippedCard(ParentCard);
	FlippedCard->UpdateAspect();
}
void CardBuilder::SetCard(Card* a){
	CardPreview->operator=(*a);
	CardPreview->UpdateAspect();
	NameEditor->setText(CardPreview->GetCardName());
	QList<int> TempList=CardPreview->GetCardColor();
	if (TempList.contains(Constants::CardColor::White)){
		ColorlessCheck->setChecked(false);
		WhiteCheck->setChecked(true);
	}
	if (TempList.contains(Constants::CardColor::Blue)){
		ColorlessCheck->setChecked(false);
		BlueCheck->setChecked(true);
	}
	if (TempList.contains(Constants::CardColor::Black)){
		ColorlessCheck->setChecked(false);
		BlackCheck->setChecked(true);
	}
	if (TempList.contains(Constants::CardColor::Red)){
		ColorlessCheck->setChecked(false);
		RedCheck->setChecked(true);
	}
	if (TempList.contains(Constants::CardColor::Black)){
		ColorlessCheck->setChecked(false);
		BlackCheck->setChecked(true);
	}
	HasManaCostCheck->setChecked(!CardPreview->GetHasManaCost());
	TempList=CardPreview->GetAvailableEditions();
	if (!TempList.contains(Constants::Editions::NONE)){
		CardEditionSelector->setEnabled(true);
		CardRaritySelector->setEnabled(true);
		for (QList<int>::const_iterator i=TempList.begin();i!=TempList.end();i++){
			CardEditionSelector->addItem(
				QIcon(QPixmap(Constants::EditionSymbolsIcons[*i])),
				Constants::EditionNames[*i],
				*i);
		}
	}
	QList<QPixmap> ImageList=CardPreview->GetAvailableImages();
	CardImageSelector->setEnabled(!ImageList.isEmpty());
	for (int i=0;i<ImageList.size();i++){
		CardImageSelector->AggiungiImmagine(
			ImageList.at(i),
			tr("Image %1").arg(i+1)
			);
	}
	TempList=CardPreview->GetAvailableBackgrounds();
	CardBackgroundSelector->setEnabled(!TempList.isEmpty());
	for (QList<int>::const_iterator i=TempList.begin();i!=TempList.end();i++){
		CardBackgroundSelector->addItem(
			QIcon(QPixmap(Constants::BackgroundImages[*i])),
			Constants::BackgroundNames[*i],
			*i);
	}
	CardFlavorTextEdit->setHtml(CardPreview->GetCardFlavorText());
	if (CardPreview->GetHasPT()){
		HasPTGroup->setChecked(true);
		if (CardPreview->GetCardPower()==Card::StarPowerToughness)
			PowerEdit->setText(QString("*"));
		else
			PowerEdit->setText(QString("%1").arg(CardPreview->GetCardPower()));
		if (CardPreview->GetCardToughness()==Card::StarPowerToughness)
			PowerEdit->setText(QString("*"));
		else
			PowerEdit->setText(QString("%1").arg(CardPreview->GetCardToughness()));
	}
}
void CardBuilder::SetFlippedCard(Card* a){
	if (!a) return RemoveFlippedCard();
	FlippedCard->operator=(*a);
	FlippedCard->UpdateAspect();
	CardPreview->SetHasFlipped(Card::HasFlip);
	CardPreview->SetFlippedCard(FlippedCard);
	RemoveFlippedButton->show();
	AddFlippedButton->setText(tr("Edit Split or Flip Card"));
	FlippedCard->SetCovered(false);
	FlippedCard->UpdateAspect();
}