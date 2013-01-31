#ifndef EFFECTCONSTANTS_H
#define EFFECTCONSTANTS_H
#include <QObject>
namespace EffectsConstants{
	namespace EffectTypes{
		enum{
			PassiveEffect,
			ActivatedEffect,
			TriggeredEffect,
			//Other types go here
			OnResolutionEffect
		};
	}
	const QString EffectTypesNames[]={
		QObject::tr("Passive")
		,QObject::tr("Activated")
		,QObject::tr("Triggered")
		,QObject::tr("On Resolution")
	};
	namespace Triggers{
		enum{
			EntersTheBattlefield
			//Other Triggers Go Here
			,Dies
		};
	}
	const QString TriggersNames[]={
		QObject::tr("%1 %2 Enters The Battlefield")
		,QObject::tr("%1 %2 Dies")
	};
	namespace Targets{
		enum{
			NoTargets=-1
			,Creature
			,Player
			,CreatureOrPlayer
			//Other Targets Here
			,Planeswalker
		};
	}
	const QString TargetNames[]={
		QObject::tr("Creature")
		,QObject::tr("Player")
		,QObject::tr("Creature or Player")
		,QObject::tr("Planeswalker")
	};
	namespace EffectCosts{
		enum{
			ColorlessMana
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
			//Other Mana goes here
			,Tap
			,TapOtherCreatures
			//Other Tap Go here
			,TapVampires
			,SacrificeCrature
			//Other Scarifices go here
			,SicrificeItself
			,END
		};
	}
	const QString EffectCostStrings[]={
		"<img src=\":/Effects/CMana1.png\" >" //Temporaneo
		,"<img src=\":/Effects/WMana.png\" >"
		,"<img src=\":/Effects/UMana.png\" >"
		,"<img src=\":/Effects/BMana.png\" >"
		,"<img src=\":/Effects/RMana.png\" >"
		,"<img src=\":/Effects/GMana.png\" >"
		,"<img src=\":/Effects/WUMana.png\" >"
		,"<img src=\":/Effects/WBMana.png\" >"
		,"<img src=\":/Effects/WRMana.png\" >"
		,"<img src=\":/Effects/WGMana.png\" >"
		,"<img src=\":/Effects/UBMana.png\" >"
		,"<img src=\":/Effects/URMana.png\" >"
		,"<img src=\":/Effects/UGMana.png\" >"
		,"<img src=\":/Effects/BRMana.png\" >"
		,"<img src=\":/Effects/BGMana.png\" >"
		,"<img src=\":/Effects/RGMana.png\" >"
		,"<img src=\":/Effects/TapSymbol.png\" >"
		,QObject::tr("Tap %1 other creature(s)")
		,QObject::tr("Tap %1 vampire(s) you control")
		,QObject::tr("Sacrifice %1 other creature(s)")
		,QObject::tr("Sacrifice it")
	};
	namespace Effects{
		enum{
			AddWToManaPool
			,AddUToManaPool
			,AddBToManaPool
			,AddRToManaPool
			,AddGToManaPool
			,DrawCards
			,END
		};
	}
	const QString EffectStrings[]={
		QObject::tr("Add White to your mana pool")
		,QObject::tr("Add Blue to your mana pool")
		,QObject::tr("Add Black to your mana pool")
		,QObject::tr("Add Red to your mana pool")
		,QObject::tr("Add Green to your mana pool")
		,QObject::tr("Draw %1 Card(s)")
	};
}
namespace Symbols{
	const int Num_Symbols= 31; 
	const QString symbols_paths[Num_Symbols]={
		":/Effects/TapSymbol.png",
		":/Effects/CMana0.png",
		":/Effects/CMana1.png",
		":/Effects/CMana2.png",
		":/Effects/CMana3.png",
		":/Effects/CMana4.png",
		":/Effects/CMana5.png",
		":/Effects/CMana6.png",
		":/Effects/CMana7.png",
		":/Effects/CMana8.png",
		":/Effects/WMana.png",
		":/Effects/UMana.png",
		":/Effects/BMana.png",
		":/Effects/RMana.png",
		":/Effects/GMana.png",
		":/Effects/WUMana.png",
		":/Effects/WBMana.png",
		":/Effects/WRMana.png",
		":/Effects/WGMana.png",
		":/Effects/UBMana.png",
		":/Effects/URMana.png",
		":/Effects/UGMana.png",
		":/Effects/BRMana.png",
		":/Effects/BGMana.png",
		":/Effects/RGMana.png",
		":/Effects/SnowMana.png",
		":/Effects/CWMana.png",
		":/Effects/CUMana.png",
		":/Effects/CBMana.png",
		":/Effects/CRMana.png",
		":/Effects/CGMana.png",
	};
}
#endif