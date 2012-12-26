#ifndef STYLESHEETS_H
#define STYLESHEETS_H
namespace StyleSheets{
	const QString GlobalCSS(
			"Card[CardBackground=\"0\"] #Background{"
				"border-image: url(:/CardImage/CBase.png);"
			"}"
			"Card #Rear{"
				"border-image: url(:/CardImage/Rear.png);"
			"}"
			"Card[CardRarity=\"0\"] #EditionLabel{" //Common
				"border-image: url(:/Editions/CommonBackground.png);"
			"}"
			"Card[CardRarity=\"1\"] #EditionLabel{" //Uncommon
				"border-image: url(:/Editions/UncommonBackground.png);"
			"}"
			"Card[CardRarity=\"2\"] #EditionLabel{" //Rare
				"border-image: url(:/Editions/RareBackground.png);"
			"}"
		);
}
#endif