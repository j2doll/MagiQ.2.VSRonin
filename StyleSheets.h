#ifndef STYLESHEETS_H
#define STYLESHEETS_H
#include "CostantsDefinition.h"
#include "EffectsConstants.h"
namespace StyleSheets{
	const QString CardCSS(
	//PTLabel
			"PowerToughnesLabel #MainLabel{"
				"border-image: url(:/PTLabelBases/CBox.png);"
			"}"
			+QString("PowerToughnesLabel[SelectedBase=\"%1\"] #MainLabel{").arg(Constants::PTBoxTypes::WBox)+
				"border-image: url(:/PTLabelBases/WBox.png);"
			"}"
			+QString("PowerToughnesLabel[SelectedBase=\"%1\"] #MainLabel{").arg(Constants::PTBoxTypes::UBox)+
				"border-image: url(:/PTLabelBases/UBox.png);"
			"}"
			+QString("PowerToughnesLabel[SelectedBase=\"%1\"] #MainLabel{").arg(Constants::PTBoxTypes::BBox)+
				"border-image: url(:/PTLabelBases/BBox.png);"
			"}"
			+QString("PowerToughnesLabel[SelectedBase=\"%1\"] #MainLabel{").arg(Constants::PTBoxTypes::RBox)+
				"border-image: url(:/PTLabelBases/RBox.png);"
			"}"
			+QString("PowerToughnesLabel[SelectedBase=\"%1\"] #MainLabel{").arg(Constants::PTBoxTypes::GBox)+
				"border-image: url(:/PTLabelBases/GBox.png);"
			"}"
			+QString("PowerToughnesLabel[SelectedBase=\"%1\"] #MainLabel{").arg(Constants::PTBoxTypes::GoldBox)+
				"border-image: url(:/PTLabelBases/GoldBox.png);"
			"}"
	//Card
		//Background
			"Card #Background{"
				+QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::Colorless])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandColorless)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandColorless])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandWhite)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandWhite])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandBlue)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandBlue])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandBlack)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandBlack])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandRed)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandRed])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandGreen)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandGreen])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandGold)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandGold])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandWhiteBlue)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandWhiteBlue])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandWhiteBlack)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandWhiteBlack])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandWhiteRed)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandWhiteRed])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandWhiteGreen)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandWhiteGreen])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandBlueBlack)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandBlueBlack])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandBlueRed)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandBlueRed])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandBlueGreen)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandBlueGreen])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandBlackRed)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandBlackRed])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandBlackGreen)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandBlackGreen])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::LandRedGreen)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::LandRedGreen])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::White)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::White])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::Blue)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::Blue])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::Black)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::Black])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::Red)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::Red])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::Green)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::Green])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::Gold)+
			QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::Gold])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::WhiteBlue)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::WhiteBlue])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::WhiteBlack)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::WhiteBlack])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::WhiteRed)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::WhiteRed])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::WhiteGreen)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::WhiteGreen])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::BlueBlack)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::BlueBlack])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::BlueRed)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::BlueRed])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::BlueGreen)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::BlueGreen])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::BlackRed)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::BlackRed])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::BlackGreen)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::BlackGreen])+
			"}"
			+QString("Card[CardBackground=\"%1\"] #Background{").arg(Constants::CardBacksrounds::RedGreen)+
				QString("border-image: url(%1);").arg(Constants::BackgroundImages[Constants::CardBacksrounds::RedGreen])+
			"}"
			"Card #Rear{"
				"border-image: url(:/CardImage/Rear.png);"
			"}"
		//Editions
			+QString("Card[CardRarity=\"%1\"] #EditionLabel{").arg(Constants::CardRarities::Common)+ 
				"border-image: url(:/Editions/CommonBackground.png);"
			"}"
			+QString("Card[CardRarity=\"%1\"] #EditionLabel{").arg(Constants::CardRarities::Uncommon)+ 
				"border-image: url(:/Editions/UncommonBackground.png);"
			"}"
			+QString("Card[CardRarity=\"%1\"] #EditionLabel{").arg(Constants::CardRarities::Rare)+
				"border-image: url(:/Editions/RareBackground.png);"
			"}"
			+QString("Card[CardRarity=\"%1\"] #EditionLabel{").arg(Constants::CardRarities::MythicRare)+
				"border-image: url(:/Editions/MythicRareBackground.png);"
			"}"
			+QString("Card[CardRarity=\"%1\"] #EditionLabel{").arg(Constants::CardRarities::Timeshifted)+
				"border-image: url(:/Editions/TimeshiftedBackground.png);"
			"}"
		//Effect Table
			"Card #EffectsTable{"
				"background-color: rgba(0,0,0,0);"
				"border: none;"
			"}"
			/*"#EffectsTable QScrollBar:vertical{"
				"border-radius: 15px;"
				"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
					"stop: 0 rgb(76,76,76), stop: 1 rgb(230,230,230,));"
			"}"
			"#EffectsTable QScrollBar::handle:vertical {"
				"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
					"stop: 1 rgb(76,76,76), stop: 0 rgb(230,230,230,));"
				"border: none;"
			"}"
			"#EffectsTable QScrollBar::up-arrow:vertical{"
				"background-color: rgba(0,0,0,0);"
				"image: url(:/Various/UpArrow.png);"
			"}"
			"#EffectsTable QScrollBar::down-arrow:vertical {"
				"background-color: rgba(0,0,0,0);"
				"image: url(:/Various/DownArrow.png);"
			"}"*/

		);
		const QString EffectCSS(
			"Effect #EffectButton{"
				"background-color: rgba(0,0,0,0);"
				"border: none;"
			"}"
			"Effect #EffectButton:pressed{"
				"background-color: rgba(0,0,0,0);"
				"border: none;"
			"}"
			+QString("Effect[EffectType=\"%1\"] #EffectButton{").arg(EffectsConstants::EffectTypes::ActivatedEffect)+
				"border: 1px solid #444;"
				"border-radius: 6px;"
				"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
					"stop: 0 rgba(180,180,180,170), stop: 1 rgba(230,230,230,170));"
			"}"
			+QString("Effect[EffectType=\"%1\"] #EffectButton:pressed{").arg(EffectsConstants::EffectTypes::ActivatedEffect)+
				"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
					"stop: 0 rgba(61,115,136,170), stop: 1 rgba(109,207,246,170));"	
				"background-color: rgba(109,207,246,170);"
			"}"
			);
}
#endif