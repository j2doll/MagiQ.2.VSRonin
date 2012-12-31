#ifndef CARDBUILDER_H
#define CARDBUILDER_H
#include <QWidget>
class Card;
class QLineEdit;
class QTextEdit;
class QCheckBox;
class ManaCostLabel;
class QComboBox;
class QPushButton;
class QGroupBox;
class QFrame;
class QLabel;
class SelettoreImmagini;
class CardBuilder : public QWidget{
	Q_OBJECT
public:
	CardBuilder(QWidget* parent=0);
private:
	Card* GetCard() const{return CardPreview;}
	void EditingFlipCard(Card* ParentCard);
	bool EditingFlip;
	QFrame* Background;
	Card* CardPreview;
	Card* FlippedCard;
	QCheckBox* CertifiedCheck;
	QLineEdit* NameEditor;
	QGroupBox* CardColorGroup;
	QCheckBox* ColorlessCheck;
	QCheckBox* WhiteCheck;
	QCheckBox* BlueCheck;
	QCheckBox* BlackCheck;
	QCheckBox* RedCheck;
	QCheckBox* GreenCheck;
	QComboBox* ManaCostSelector;
	QPushButton* ManaCostResetButton;
	QComboBox* CardTypeSelector;
	QPushButton* CardTypeResetButton;
	QComboBox* CardSubTypeSelector;
	QPushButton* CardSubTypeResetButton;
	QComboBox* CardAvailableEditionsSelector;
	QPushButton* CardAvailableEditionsResetButton;
	QComboBox* CardEditionSelector;
	QComboBox* CardRaritySelector;
	QPushButton* BrowseForImageButton;
	QPushButton* ResetImagesButton;
	QComboBox* CardImageSelector;
	QComboBox* CardAvailableBackgroundsSelector;
	QPushButton* CardAvailableBackgroundsResetButton;
	QComboBox* CardBackgroundSelector;
	QTextEdit* CardFlavorTextEdit;
	QLabel* PowerLabel;
	QGroupBox* HasPTGroup;
	QLineEdit* PowerEdit;
	QLineEdit* ToughnesEdit;
	QCheckBox* ManaSourceCheck;
	QCheckBox* HasManaCostCheck;
	QPushButton* AddFlippedButton;
	QPushButton* RemoveFlippedButton;
	QPushButton* SaveButton;
	QPushButton* OpenButton;
	QPushButton* ResetButton;
	QPushButton* ExitButton;
signals:
	void FlippedAccepted(Card* FlipCard);
	void FlippedRejected();
protected:
	void resizeEvent(QResizeEvent* event);
	void closeEvent(QCloseEvent *event);
private slots:
	void SetCertified();
	void SetCardColor();
	void SetCardName();
	void AddCardCost(int index);
	void ResetCardCost();
	void AddCardType(int index);
	void ResetCardType();
	void AddCardSubType(int index);
	void ResetCardSubType();
	void AddCardEdition(int index);
	void ResetCardEdition();
	void SelectEdition(int index);
	void AddCardImage();
	void ResetCardImages();
	void SelectImage(int index);
	void AddCardBackground(int index);
	void ResetCardBackground();
	void SelectBackground(int index);
	void SelectRarity(int index);
	void SetFlavorText();
	void SetPower();
	void SetToughnes();
	void SetHasPT(bool a);
	void SetManaSource();
	void SetNoManaCost();
	void AddFlippedCard();
	void RemoveFlippedCard();
	void SaveCard();
	void ResetAll();
	void SetCard(Card* a);
	void SetFlippedCard(Card* a);
	void OpenCard();
};
#endif