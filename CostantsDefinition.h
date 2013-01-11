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
		,Token
		,Legendary
		,Snow
		,Tribal
		,World
		,END
	};}
	const QString CardTypeNames[]={
		QObject::tr("Basic")
		,QObject::tr("Land")
		,QObject::tr("Creature")
		,QObject::tr("Planeswalker")
		,QObject::tr("Sorcery")
		,QObject::tr("Instant")
		,QObject::tr("Artifact")
		,QObject::tr("Enchantment")
		,QObject::tr("Token")
		,QObject::tr("Legendary")
		,QObject::tr("Snow")
		,QObject::tr("Tribal")
		,QObject::tr("World")
	};
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
		,Forest
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
		,Island
		,Jellyfish
		,Juggernaut
		,Knight
		,Leviathan
		,Licid
		,Masticore
		,Minotaur
		,Mountain
		,Myr
		,Pest
		,Plain
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
		,Swamp
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
	const QString CardSubTypesNames[]={
		QObject::tr("Ally")
		,QObject::tr("Angel")
		,QObject::tr("Artificier")
		,QObject::tr("AssemblyWorker")
		,QObject::tr("Beast")
		,QObject::tr("Berserker")
		,QObject::tr("Bird")
		,QObject::tr("Boar")
		,QObject::tr("Cat")
		,QObject::tr("Chicken")
		,QObject::tr("Chimera")
		,QObject::tr("Cleric")
		,QObject::tr("Construct")
		,QObject::tr("Dragon")
		,QObject::tr("Drake")
		,QObject::tr("Dreadnought")
		,QObject::tr("Drone")
		,QObject::tr("Elemental")
		,QObject::tr("Elf")
		,QObject::tr("Faerie")
		,QObject::tr("Fish")
		,QObject::tr("Forest")
		,QObject::tr("Frog")
		,QObject::tr("Gargoyle")
		,QObject::tr("Gnome")
		,QObject::tr("Goblin")
		,QObject::tr("Golem")
		,QObject::tr("Homunculus")
		,QObject::tr("Horror")
		,QObject::tr("Horse")
		,QObject::tr("Hound")
		,QObject::tr("Human")
		,QObject::tr("Hydra")
		,QObject::tr("Illusion")
		,QObject::tr("Imp")
		,QObject::tr("Insect")
		,QObject::tr("Island")
		,QObject::tr("Jellyfish")
		,QObject::tr("Juggernaut")
		,QObject::tr("Knight")
		,QObject::tr("Leviathan")
		,QObject::tr("Licid")
		,QObject::tr("Masticore")
		,QObject::tr("Minotaur")
		,QObject::tr("Mountain")
		,QObject::tr("Myr")
		,QObject::tr("Pest")
		,QObject::tr("Plain")
		,QObject::tr("Rhino")
		,QObject::tr("Rougue")
		,QObject::tr("Scarecrow")
		,QObject::tr("Scorpion")
		,QObject::tr("Scout")
		,QObject::tr("Serpent")
		,QObject::tr("Shaman")
		,QObject::tr("Shapeshifter")
		,QObject::tr("Sheep")
		,QObject::tr("Slith")
		,QObject::tr("Sliver")
		,QObject::tr("Snake")
		,QObject::tr("Soldier")
		,QObject::tr("Spellshaper")
		,QObject::tr("Sphinx")
		,QObject::tr("Spider")
		,QObject::tr("Spirit")
		,QObject::tr("Swamp")
		,QObject::tr("Thoper")
		,QObject::tr("Vedalken")
		,QObject::tr("Viashino")
		,QObject::tr("Wall")
		,QObject::tr("Warrior")
		,QObject::tr("Wizard")
		,QObject::tr("Wurm")
		,QObject::tr("Zombie")
	};
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
			,ISD
			,AVR
			,END
		};
	}
	const QString EditionNames[]={
		QObject::tr("Time Spiral")
		,QObject::tr("Innistrad")
		,QObject::tr("Avacyn Restored")
	};
	const QString EditionSymbolsIcons[]={
		":/Editions/TSP.png"
		,":/Editions/ISD.png"
		,":/Editions/AVR.png"
	};
	const QSize EditionSymbolSyze[]={
		QSize(89,101) //TSP
		,QSize(272,246) //ISD
		,QSize(92,93) //AVR
	};
	namespace CounterTypes{
		enum{
			Plus1Plus1
			,Age
		};
	}
	namespace Legality{enum{Legacy,Modern,Standard,Extended,Commander,END_Legal};}
	namespace CardFlipMode{enum{AllreadyFlipped=-1,NoFlip=0,HasFlip=1};};
	enum {CardVersion=0};
}
#endif