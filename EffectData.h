#ifndef EFFECTDATA_H
#define EFFECTDATA_H
#include <QString>
#include <QMap>
#include <QList>
#include <QDataStream>
#include "EffectsConstants.h"
class EffectData{
public:
	EffectData();
	EffectData(const EffectData& a);
	int GetEffectType() const {return EffectType;}
	bool GetManaEffect() const {return ManaEffect;}
	bool GetDoesntStack() const {return DoesntStack;}
	bool GetHiddenEffect() const {return HiddenEffect;}
	const QMap<int,int>& GetEffectCost() const {return EffectCost;}
	const QMap<int,int>& GetTargets() const {return Targets;}
	const QList<int>& GetTriggers() const {return Triggers;}
	int GetEffectBody() const {return EffectBody;}
	const QString& GetEffectText() const {return EffectText;}
	const QString& GetEffectTooltip() const {return EffectTooltip;}

	void SetEffectType(int a){if (a>=EffectsConstants::EffectTypes::PassiveEffect && a<=EffectsConstants::EffectTypes::OnResolutionEffect) EffectType=a;}
	void SetManaEffect(bool a){ManaEffect=a;}
	void SetDoesntStack(bool a){DoesntStack=a;}
	void SetHiddenEffect(bool a){HiddenEffect=a;}
	void SetEffectCost(const QMap<int,int>& a){EffectCost.clear(); EffectCost=a;}
	void SetEffectCost();
	void SetEffectCost(int key, int cost){SetEffectCost(); EffectCost.insert(key,cost);}
	void AddEffectCost(int key, int cost){if(key>=0 && key<EffectsConstants::EffectCosts::END) EffectCost[key]+=cost;}
	void SetTargets();
	void SetTargets(const QMap<int,int>& a){Targets.clear(); Targets=a;}
	void SetTargets(int key, int val){SetTargets(); AddTarget(key,val);}
	void AddTarget(int key,int val){if (key>=0 && key<=EffectsConstants::Targets::Planeswalker) Targets[key]+=val;}
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
private:
	int EffectType;
	bool ManaEffect;
	bool DoesntStack;
	bool HiddenEffect;
	QMap<int,int> EffectCost;
	QMap<int,int> Targets;
	QList<int> Triggers;
	int EffectBody;
	QString EffectText;
	QString EffectTooltip;
};
QDataStream &operator<<(QDataStream &out, const EffectData &effect);
QDataStream &operator>>(QDataStream &in, EffectData &effect);
#endif