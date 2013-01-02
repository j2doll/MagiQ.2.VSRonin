#ifndef COSTANTSDEFINITION_H
#define COSTANTSDEFINITION_H
#include <QSize>
#include <QString>
#include <QObject>
namespace Constants{
	namespace CardRarities{
		enum {Common,Uncommon,Rare,Timeshifted,MythicRare};
	}
	namespace CardColor {enum {
		Colorless
		,White
		,Blue
		,Black
		,Red
		,Green
	};}
	namespace ManaCosts{
		enum{
			Colorless
			,W
			,U
			,B
			,R
			,G
			,WU
			,WB
			,WR
			,WG
			,UB
			,UR
			,UG
			,BR
			,BG
			,RG
			,S
			,X
			,Y
			,Z
			,LW
			,LU
			,LB
			,LR
			,LG
			,C2W
			,C2U
			,C2B
			,C2R
			,C2G
			,END
		};
	}
	const QString ManaCostsNames[]={
		"W"
		,"U"
		,"B"
		,"R"
		,"G"
		,"WU"
		,"WB"
		,"WR"
		,"WG"
		,"UB"
		,"UR"
		,"UG"
		,"BR"
		,"BG"
		,"RG"
		,"S"
		,"X"
		,"Y"
		,"Z"
		,"LW"
		,"LU"
		,"LB"
		,"LR"
		,"LG"
	};
	namespace PTBoxTypes{
		enum {CBox, WBox, UBox, BBox, RBox, GBox, GoldBox};
	}
	namespace CardTypes{enum {
		NONE=-1
		,Basic
		,Land
		,Creature
		,Planeswalker
		,Sorcery
		,Instant
		,Artifact
		,Enchantment
		,Legendary
		,Snow
		,Tribal
		,World
		,END
	};}
	namespace CardSubTypes{enum {
		NONE=-1
		,Ally
		,Angel
		,Artificier
		,AssemblyWorker
		,Beast
		,Berserker
		,Bird
		,Boar
		,Cat
		,Chicken
		,Chimera
		,Cleric
		,Construct
		,Dragon
		,Drake
		,Dreadnought
		,Drone
		,Elemental
		,Elf
		,Faerie
		,Fish
		,Frog
		,Gargoyle
		,Gnome
		,Goblin
		,Golem
		,Homunculus
		,Horror
		,Horse
		,Hound
		,Human
		,Hydra
		,Illusion
		,Imp
		,Insect
		,Jellyfish
		,Juggernaut
		,Knight
		,Leviathan
		,Licid
		,Masticore
		,Minotaur
		,Myr
		,Pest
		,Rhino
		,Rougue
		,Scarecrow
		,Scorpion
		,Scout
		,Serpent
		,Shaman
		,Shapeshifter
		,Sheep
		,Slith
		,Sliver
		,Snake
		,Soldier
		,Spellshaper
		,Sphinx
		,Spider
		,Spirit
		,Thoper
		,Vedalken
		,Viashino
		,Wall
		,Warrior
		,Wizard
		,Wurm
		,Zombie
		,END
	};}
	namespace CardBacksrounds{enum {
		LandColorless,
		LandWhite,
		LandBlue,
		LandBlack,
		LandRed,
		LandGreen,
		LandGold,
		LandWhiteBlue,
		LandWhiteBlack,
		LandWhiteRed,
		LandWhiteGreen,
		LandBlueBlack,
		LandBlueRed,
		LandBlueGreen,
		LandBlackRed,
		LandBlackGreen,
		LandRedGreen,
		Colorless,
		White,
		Blue,
		Black,
		Red,
		Green,
		Gold,
		WhiteBlue,
		WhiteBlack,
		WhiteRed,
		WhiteGreen,
		BlueBlack,
		BlueRed,
		BlueGreen,
		BlackRed,
		BlackGreen,
		RedGreen,
		END
	};}
	const QString BackgroundNames[]={
		QObject::tr("Land Colorless")
		,QObject::tr("Land White")
		,QObject::tr("Land Blue")
		,QObject::tr("Land Black")
		,QObject::tr("Land Red")
		,QObject::tr("Land Green")
		,QObject::tr("Land Gold")
		,QObject::tr("Land White Blue")
		,QObject::tr("Land White Black")
		,QObject::tr("Land White Red")
		,QObject::tr("Land White Green")
		,QObject::tr("Land Blue Black")
		,QObject::tr("Land Blue Red")
		,QObject::tr("Land Blue Green")
		,QObject::tr("Land Black Red")
		,QObject::tr("Land Black Green")
		,QObject::tr("Land Red Green")
		,QObject::tr("Colorless")
		,QObject::tr("White")
		,QObject::tr("Blue")
		,QObject::tr("Black")
		,QObject::tr("Red")
		,QObject::tr("Green")
		,QObject::tr("Gold")
		,QObject::tr("White Blue")
		,QObject::tr("White Black")
		,QObject::tr("White Red")
		,QObject::tr("White Green")
		,QObject::tr("Blue Black")
		,QObject::tr("Blue Red")
		,QObject::tr("Blue Green")
		,QObject::tr("Black Red")
		,QObject::tr("Black Green")
		,QObject::tr("Red Green")
	};
	const QString BackgroundImages[]={
		":/CardImage/LandCBase.png"
		,":/CardImage/LandWBase.png"
		,":/CardImage/LandUBase.png"
		,":/CardImage/LandBBase.png"
		,":/CardImage/LandRBase.png"
		,":/CardImage/LandGBase.png"
		,":/CardImage/LandGoldBase.png"
		,":/CardImage/LandWUBase.png"
		,":/CardImage/LandWBBase.png"
		,":/CardImage/LandWRBase.png"
		,":/CardImage/LandWGBase.png"
		,":/CardImage/LandUBBase.png"
		,":/CardImage/LandURBase.png"
		,":/CardImage/LandUGBase.png"
		,":/CardImage/LandBRBase.png"
		,":/CardImage/LandBGBase.png"
		,":/CardImage/LandRGBase.png"
		,":/CardImage/CBase.png"
		,":/CardImage/WBase.png"
		,":/CardImage/UBase.png"
		,":/CardImage/BBase.png"
		,":/CardImage/RBase.png"
		,":/CardImage/GBase.png"
		,":/CardImage/GoldBase.png"
		,":/CardImage/WUBase.png"
		,":/CardImage/WBBase.png"
		,":/CardImage/WRBase.png"
		,":/CardImage/WGBase.png"
		,":/CardImage/UBBase.png"
		,":/CardImage/URBase.png"
		,":/CardImage/UGBase.png"
		,":/CardImage/BRBase.png"
		,":/CardImage/BGBase.png"
		,":/CardImage/RGBase.png"
	};
	namespace Editions{
		enum{
			NONE=-1
			,TSP
			,AVR
			,END
		};
	}
	const QString EditionNames[]={
		QObject::tr("Time Spiral")
		,QObject::tr("Avacyn Restored")
	};
	const QString EditionSymbolsIcons[]={
		":/Editions/TSP.png"
		,":/Editions/AVR.png"
	};
	const QSize EditionSymbolSyze[]={
		QSize(89,101) //TSP
		,QSize(92,93) //AVR
	};
	const QString CardTypeNames[]={
		QObject::tr("Artifact")
		,"Basic"
		,"Creature"
		,"Enchantment"
		,"Instant"
		,"Land"
		,"Legendary"
		,"Planeswalker"
		,"Snow"
		,"Sorcery"
		,"Tribal"
		,"World"
	};
	const QString CardSubTypesNames[]={
		"Ally"
		,"Angel"
		,"Artificier"
		,"AssemblyWorker"
		,"Beast"
		,"Berserker"
		,"Bird"
		,"Boar"
		,"Cat"
		,"Chicken"
		,"Chimera"
		,"Cleric"
		,"Construct"
		,"Dragon"
		,"Drake"
		,"Dreadnought"
		,"Drone"
		,"Elemental"
		,"Elf"
		,"Faerie"
		,"Fish"
		,"Frog"
		,"Gargoyle"
		,"Gnome"
		,"Goblin"
		,"Golem"
		,"Homunculus"
		,"Horror"
		,"Horse"
		,"Hound"
		,"Human"
		,"Hydra"
		,"Illusion"
		,"Imp"
		,"Insect"
		,"Jellyfish"
		,"Juggernaut"
		,"Knight"
		,"Leviathan"
		,"Licid"
		,"Masticore"
		,"Minotaur"
		,"Myr"
		,"Pest"
		,"Rhino"
		,"Rougue"
		,"Scarecrow"
		,"Scorpion"
		,"Scout"
		,"Serpent"
		,"Shaman"
		,"Shapeshifter"
		,"Sheep"
		,"Slith"
		,"Sliver"
		,"Snake"
		,"Soldier"
		,"Spellshaper"
		,"Sphinx"
		,"Spider"
		,"Spirit"
		,"Thoper"
		,"Vedalken"
		,"Viashino"
		,"Wall"
		,"Warrior"
		,"Wizard"
		,"Wurm"
		,"Zombie"
	};
	namespace CounterTypes{
		enum{
			Plus1Plus1
			,Minus1Minus1
			,Age
		};
	}
}
#endif