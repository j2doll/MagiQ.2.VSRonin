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
	namespace Phases{enum{
		Untap
		,Upkeep
		,Draw
		,PreCombatMain
		,BeginOfCombat
		,DeclareAttackers
		,DeclareBlockers
		,CombatDamage
		,EndOfCombat
		,PostCombatMain
		,EndOfTurn
		,Cleanup
		,TurnEnd
	};}
	const QString PhasesNames[]={
		QObject::tr("Untap")
		,QObject::tr("Upkeep")
		,QObject::tr("Draw")
		,QObject::tr("Pre-Combat Main")
		,QObject::tr("Begin of Combat")
		,QObject::tr("Declare Attackers")
		,QObject::tr("Declare Blockers")
		,QObject::tr("Combat Damage")
		,QObject::tr("End of Combat")
		,QObject::tr("Post-Combat Main")
		,QObject::tr("End of Turn")
		,QObject::tr("Cleanup")
		,QObject::tr("Turn End")
	};
	const QString PhaseImages[]={
		":/Phases/Untap.png"
		,":/Phases/Upkeep.png"
		,":/Phases/Draw.png"
		,":/Phases/Main.png"
		,":/Phases/BeginCombat.png"
		,":/Phases/Attack.png"
		,":/Phases/Block.png"
		,":/Phases/Damage.png"
		,":/Phases/EndOfCombat.png"
		,":/Phases/Main.png"
		,":/Phases/Untap.png"
		,":/Phases/Untap.png"
		,":/Phases/Untap.png"
	};
	namespace GameMode{enum{FreeForAll,TwoHeadedGiant,Commander,Archenemy,Planechase,END};}
	namespace Legality{enum{Any,Legacy,Modern,Standard,Extended,Commander,END};}
	namespace CardFlipMode{enum{AllreadyFlipped=-1,NoFlip=0,HasFlip=1};}
	enum {CardVersion=0};
	enum {Num_Smiles= 40}; 
		const QString Emotes[Num_Smiles][2]={
			{"L:)","<img src=\":/Smiles/loser.png\" width=\"24\" height=\"24\" />"},
			{"Z:)","<img src=\":/Smiles/sleeping.png\" width=\"24\" height=\"24\" />"},
			{"(S)","<img src=\":/Smiles/afraid.png\" width=\"24\" height=\"24\" />"},
			{"(L)","<img src=\":/Smiles/amorous.png\" width=\"24\" height=\"24\" />"},
			{"(A)","<img src=\":/Smiles/angel.png\" width=\"24\" height=\"24\" />"},
			{"8o|","<img src=\":/Smiles/angry.png\" width=\"24\" height=\"24\" />"},
			{"|(","<img src=\":/Smiles/arrogant.png\" width=\"24\" height=\"24\" />"},
			{"(U)","<img src=\":/Smiles/beat-up.png\" width=\"24\" height=\"24\" />"},
			{"(B)","<img src=\":/Smiles/bomb.png\" width=\"24\" height=\"24\" />"},
			{"o_0","<img src=\":/Smiles/bulgy-eyes.png\" width=\"24\" height=\"24\" />"},
			{"(O)","<img src=\":/Smiles/clock.png\" width=\"24\" height=\"24\" />"},
			{":S","<img src=\":/Smiles/confused.png\" width=\"24\" height=\"24\" />"},
			{":'(","<img src=\":/Smiles/crying.png\" width=\"24\" height=\"24\" />"},
			{"(N)","<img src=\":/Smiles/curse.png\" width=\"24\" height=\"24\" />"},
			{"(6)","<img src=\":/Smiles/devil.png\" width=\"24\" height=\"24\" />"},
			{":#","<img src=\":/Smiles/disappointed.png\" width=\"24\" height=\"24\" />"},
			{":^)","<img src=\":/Smiles/dont-know.png\" width=\"24\" height=\"24\" />"},
			{":$","<img src=\":/Smiles/embarrassed.png\" width=\"24\" height=\"24\" />"},
			{":D","<img src=\":/Smiles/excited.png\" width=\"24\" height=\"24\" />"},
			{"*)","<img src=\":/Smiles/eyeroll.png\" width=\"24\" height=\"24\" />"},
			{"(V)","<img src=\":/Smiles/fingers-crossed.png\" width=\"24\" height=\"24\" />"},
			{":!","<img src=\":/Smiles/foot-in-mouth.png\" width=\"24\" height=\"24\" />"},
			{"(G)","<img src=\":/Smiles/freaked-out.png\" width=\"24\" height=\"24\" />"},
			{"8)","<img src=\":/Smiles/glasses-cool.png\" width=\"24\" height=\"24\" />"},
			{":)","<img src=\":/Smiles/happy.png\" width=\"24\" height=\"24\" />"},
			{"-.-\"","<img src=\":/Smiles/hot.png\" width=\"24\" height=\"24\" />"},
			{"<3","<img src=\":/Smiles/in_love.png\" width=\"24\" height=\"24\" />"},
			{":*","<img src=\":/Smiles/kiss.png\" width=\"24\" height=\"24\" />"},
			{":|","<img src=\":/Smiles/neutral.png\" width=\"24\" height=\"24\" />"},
			{":@","<img src=\":/Smiles/pissed-off.png\" width=\"24\" height=\"24\" />"},
			{"?:(","<img src=\":/Smiles/question.png\" width=\"24\" height=\"24\" />"},
			{":(","<img src=\":/Smiles/sad.png\" width=\"24\" height=\"24\" />"},
			{"^o)","<img src=\":/Smiles/sarcastic.png\" width=\"24\" height=\"24\" />"},
			{":O","<img src=\":/Smiles/shocked.png\" width=\"24\" height=\"24\" />"},
			{":X","<img src=\":/Smiles/shut-mouth.png\" width=\"24\" height=\"24\" />"},
			{"+o(","<img src=\":/Smiles/sick.png\" width=\"24\" height=\"24\" />"},
			{":P","<img src=\":/Smiles/tongue.png\" width=\"24\" height=\"24\" />"},
			{":[","<img src=\":/Smiles/vampire.png\" width=\"24\" height=\"24\" />"},
			{"XD","<img src=\":/Smiles/XD.png\" width=\"24\" height=\"24\" />"},
			{";)","<img src=\":/Smiles/wink.png\" width=\"24\" height=\"24\" />"}
		};
		const QString smiles_paths[Num_Smiles]={
			":/Smiles/loser.png",
			":/Smiles/sleeping.png",
			":/Smiles/afraid.png",
			":/Smiles/amorous.png",
			":/Smiles/angel.png" ,
			":/Smiles/angry.png",
			":/Smiles/arrogant.png",
			":/Smiles/beat-up.png",
			":/Smiles/bomb.png",
			":/Smiles/bulgy-eyes.png",
			":/Smiles/clock.png",
			":/Smiles/confused.png",
			":/Smiles/crying.png",
			":/Smiles/curse.png",
			":/Smiles/devil.png",
			":/Smiles/disappointed.png",
			":/Smiles/dont-know.png",
			":/Smiles/embarrassed.png",
			":/Smiles/excited.png",
			":/Smiles/eyeroll.png",
			":/Smiles/fingers-crossed.png",
			":/Smiles/foot-in-mouth.png",
			":/Smiles/freaked-out.png",
			":/Smiles/glasses-cool.png",
			":/Smiles/happy.png",
			":/Smiles/hot.png",
			":/Smiles/in_love.png",
			":/Smiles/kiss.png",
			":/Smiles/neutral.png",
			":/Smiles/pissed-off.png",
			":/Smiles/question.png",
			":/Smiles/sad.png",
			":/Smiles/sarcastic.png",
			":/Smiles/shocked.png",
			":/Smiles/shut-mouth.png",
			":/Smiles/sick.png",
			":/Smiles/tongue.png",
			":/Smiles/vampire.png",
			":/Smiles/XD.png",
			":/Smiles/wink.png"
		};
		const QString PlayerColors[]={
			"#336699" //Blue
			,"#CC0000" //Red
			,"#005A04" //Green
			,"#FF9900" //Orange
			//Extra Colors
			,"#993366" //Fuchsia
			,"#663300" //Brown
			,"#E47297" //Pink
			,"#FFD900" //Yellow
		};
}
#endif