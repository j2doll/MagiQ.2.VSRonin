#ifndef EFFECTDATA_H
#define EFFECTDATA_H
#include <QString>
#include <QMap>
#include <QMultiMap>
#include <QList>
#include <QDataStream>
#include "EffectsConstants.h"
class CardData;
class EffectData{
public:
	EffectData();
	EffectData(const EffectData& a);
	const EffectData& operator=(const EffectData& a);

	unsigned int GetEffectID() const {return EffectID;}
	int GetEffectType() const {return EffectType;}
	bool GetManaEffect() const {return ManaEffect;}
	bool GetDoesntStack() const {return DoesntStack;}
	bool GetHiddenEffect() const {return HiddenEffect;}
	const QMap<int,int>& GetEffectCost() const {return EffectCost;}
	const QMap<int,int>& GetTargets() const {return Targets;}
	const QMultiMap<int,int>& GetSelectedTargets() const {return SelectedTargets;}
	const QList<int>& GetTriggers() const {return Triggers;}
	int GetEffectBody() const {return EffectBody;}
	const QString& GetEffectText() const {return EffectText;}
	const QString& GetEffectTooltip() const {return EffectTooltip;}
	CardData*const& GetCardAttached() const {return CardAttached;}
	const QList<int>& GetVariableValues() const {return VariableValues;}

	void SetEffectID(unsigned int a){EffectID=a;}
	void SetEffectType(int a){if (a>=EffectsConstants::EffectTypes::PassiveEffect && a<=EffectsConstants::EffectTypes::OnResolutionEffect) EffectType=a;}
	void SetManaEffect(bool a){ManaEffect=a;}
	void SetDoesntStack(bool a){DoesntStack=a;}
	void SetHiddenEffect(bool a){HiddenEffect=a;}
	void SetEffectCost(const QMap<int,int>& a){EffectCost.clear(); EffectCost=a;}
	void SetEffectCost();
	void SetEffectCost(int key, int cost){SetEffectCost(); EffectCost.insert(key,cost);}
	void AddEffectCost(int key, int cost){if(key>=0 && key<EffectsConstants::EffectCosts::END) EffectCost[key]+=cost;}
	void SetTargets(){Targets.clear();}
	void SetTargets(const QMap<int,int>& a){Targets.clear(); Targets=a;}
	void SetTargets(int key, int val){SetTargets(); AddTarget(key,val);}
	void AddTarget(int key,int val){if (key>=0 && key<=EffectsConstants::Targets::Planeswalker) Targets[key]+=val;}
	void SetSelectedTargets(){SelectedTargets.clear();}
	void SetSelectedTargets(const QMultiMap<int,int>& a){SelectedTargets.clear(); SelectedTargets=a;}
	void AddSelectedTarget(int key, int val){SelectedTargets.insert(key,val);}
	void ReplaceSelectedTarget(int key, int val){SelectedTargets.replace(key,val);}
	void ReplaceSpecificSelectedTarget(int key, int oldval, int newval);
	void SetSelectedTargets(int key, int val){SetSelectedTargets(); AddSelectedTarget(key,val);}
	void SetTriggers(const QList<int>& a){Triggers.clear(); Triggers=a;}
	void SetTriggers(){Triggers.clear();}
	void SetTriggers(int a){Triggers.clear(); AddTrigger(a);}
	void AddTrigger(int a){if (
		a>=EffectsConstants::Triggers::EntersTheBattlefield &&
		a<=EffectsConstants::Triggers::Dies
		) Triggers.append(a);}
	void SetEffectBody(int a){if (a>=-1 && a<EffectsConstants::Effects::END) EffectBody=a;}
	void SetEffectBody() {EffectBody=-1;}
	void SetEffectText(const QString& a){EffectText=a;}
	void SetEffectTooltip(const QString& a){EffectTooltip=a;}
	void SetCardAttached(CardData* a){CardAttached=a;}
	void SetVariableValues(const QList<int>& a){VariableValues.clear(); VariableValues=a;}
	void SetVariableValues(){VariableValues.clear();}
	void AddVariableValue(int a){VariableValues.append(a);}
	void SetVariableValues(int a){SetVariableValues(); AddVariableValue(a);}
private:
	unsigned int EffectID;
	int EffectType;
	bool ManaEffect;
	bool DoesntStack;
	bool HiddenEffect;
	QMap<int,int> EffectCost;
	QMap<int,int> Targets;
	QMultiMap<int,int> SelectedTargets;
	QList<int> Triggers;
	int EffectBody;
	QString EffectText;
	QString EffectTooltip;
	CardData* CardAttached;
	QList<int> VariableValues;
};
QDataStream &operator<<(QDataStream &out, const EffectData &effect);
QDataStream &operator>>(QDataStream &in, EffectData &effect);
#endif