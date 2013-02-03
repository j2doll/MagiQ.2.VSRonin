#include "EffectData.h"
#include <QMessageBox>
#include "CardData.h"
EffectData::EffectData()
	:EffectType(EffectsConstants::EffectTypes::PassiveEffect)
	,ManaEffect(false)
	,HiddenEffect(false)
	,DoesntStack(false)
	,EffectBody(-1)
	,EffectID(0)
	,CardAttached(NULL)
{}
EffectData::EffectData(const EffectData& a)
	:EffectType(a.EffectType)
	,ManaEffect(a.ManaEffect)
	,DoesntStack(a.DoesntStack)
	,HiddenEffect(a.HiddenEffect)
	,EffectCost(a.EffectCost)
	,Targets(a.Targets)
	,Triggers(a.Triggers)
	,EffectBody(a.EffectBody)
	,EffectText(a.EffectText)
	,EffectTooltip(a.EffectTooltip)
	,EffectID(a.EffectID)
	,CardAttached(a.CardAttached)
{}
const EffectData& EffectData::operator=(const EffectData& a){
	EffectType=a.EffectType;
	ManaEffect=a.ManaEffect;
	DoesntStack=a.DoesntStack;
	HiddenEffect=a.HiddenEffect;
	EffectCost=a.EffectCost;
	Targets=a.Targets;
	Triggers=a.Triggers;
	EffectBody=a.EffectBody;
	EffectText=a.EffectText;
	EffectTooltip=a.EffectTooltip;
	EffectID=a.EffectID;
	CardAttached=a.CardAttached;
	return *this;
}
QDataStream &operator<<(QDataStream &out, const EffectData &effect)
{
	out << qint32(Constants::CardVersion)
		<< quint32(effect.GetEffectID())
		<< qint32(effect.GetEffectType())
		<< effect.GetManaEffect()
		<< effect.GetDoesntStack()
		<< effect.GetHiddenEffect()
		<< effect.GetEffectCost()
		<< effect.GetTargets()
		<< effect.GetTriggers()
		<< qint32(effect.GetEffectBody())
		<< effect.GetEffectText()
		<< effect.GetVariableValues()
		;
	return out;
}
QDataStream &operator>>(QDataStream &input, EffectData &effect){
	qint32 Numbers;
	quint32 UnsNumbers;
	QString Strings;
	QList<int> IntLists;
	QMap<int,int> IntMap;
	bool Booleans;
	input >> Numbers;
	if (Numbers!=Constants::CardVersion){
		QMessageBox::critical(0,QObject::tr("Wrong File Type"),QObject::tr("Error: Unable to Read the File\nMake sure it's a valid MagiQ Effect File"));
		return input;
	}
	input >> UnsNumbers; effect.SetEffectID(UnsNumbers);
	input >> Numbers; effect.SetEffectType(Numbers);
	input >> Booleans; effect.SetManaEffect(Booleans);
	input >> Booleans; effect.SetDoesntStack(Booleans);
	input >> Booleans; effect.SetHiddenEffect(Booleans);
	input >> IntMap; effect.SetEffectCost(IntMap); IntMap.clear();
	input >> IntMap; effect.SetTargets(IntMap); IntMap.clear();
	input >> IntLists; effect.SetTriggers(IntLists); IntLists.clear();
	input >> Numbers; effect.SetEffectBody(Numbers);
	input >> Strings; effect.SetEffectText(Strings);
	input >> IntLists; effect.SetVariableValues(IntLists);
	return input;
}
void EffectData::ReplaceSpecificSelectedTarget(int key, int oldval, int newval){
	for(QMultiMap<int,int>::iterator i=SelectedTargets.begin();i!=SelectedTargets.end();i++){
		if(i.key()==key && i.value()==oldval) i.value()=newval;
	}
}