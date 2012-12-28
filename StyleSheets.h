#ifndef STYLESHEETS_H
#define STYLESHEETS_H
#include "CostantsDefinition.h"
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
			"Card #Rear{"
				"border-image: url(:/CardImage/Rear.png);"
			"}"
		//PT Box
			+QString("Card[CardBackground=\"%1\"] #PTLabel{").arg(Constants::CardBacksrounds::Colorless)+
				"background-color: rgb(222, 222, 222);"
			"}"
			+QString("Card[CardBackground=\"%1\"] #PTLabel{").arg(Constants::CardBacksrounds::Green)+
				"background-color: rgb(155, 211, 174);"
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
		);
}
#endif