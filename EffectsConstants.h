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
			Creature
			,Player
			,CreatureOrPlayer
			//Other Targets Here
			,Planeswalker
		};
	}
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
		"<img src=\":/Effects/CMana.png\" >" //Temporaneo
		,"<img src=\":/Effects/WMana.png\" >"
		,"<img src=\":/Effects/UMana.png\" >"
		,"<img src=\":/Effects/BMana.png\" >"
		,"<img src=\":/Effects/RMana.png\" >"
		,"<img src=\":/Effects/GMana.png\" >"
		,"<img src=\":/Effects/WUMana.png\">"
		,"<img src=\":/Effects/WBMana.png\" >"
		,"<img src=\":/Effects/WRMana.png\" >"
		,"<img src=\":/Effects/WGMana.png\" >"
		,"<img src=\":/Effects/UBMana.png\" >"
		,"<img src=\":/Effects/URMana.png\" >"
		,"<img src=\":/Effects/UGMana.png\" >"
		,"<img src=\":/Effects/BRMana.png\" >"
		,"<img src=\":/Effects/BGMana.png\" >"
		,"<img src=\":/Effects/RGMana.png\" >"
		,"<img src=\":/Effects/TapSymbol.png\">"
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
			,END
		};
	}
	const QString EffectStrings[]={
		QObject::tr("Add %1 to your mana pool").arg("<img src=\":/Effects/WMana.png\" >")
		,QObject::tr("Add %1 to your mana pool").arg("<img src=\":/Effects/UMana.png\" >")
		,QObject::tr("Add %1 to your mana pool").arg("<img src=\":/Effects/BMana.png\" >")
		,QObject::tr("Add %1 to your mana pool").arg("<img src=\":/Effects/RMana.png\" >")
		,QObject::tr("Add %1 to your mana pool").arg("<img src=\":/Effects/GMana.png\" >")
	};
}
#endif