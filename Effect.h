#ifndef EFFECT_H
#define EFFECT_H
#include <QWidget>
#include <QMap>
#include "EffectsConstants.h"
class QPushButton;
class Card;
class QLabel;
class Effect: public QWidget{
	Q_OBJECT
	Q_PROPERTY(int EffectType READ GetEffectType)
private:
// Visual Elements
	QPushButton* EffectButton;
	QLabel* EffectLabel;
// Effect Properties (this section is serialized)
	int EffectType;
	bool ManaEffect;
	bool DoesntStack;
	bool HiddenEffect;
	QMap<int,int> EffectCost;
	QMap<int,int> Targets;
	QList<int> Triggers;
	int EffectBody;
	QString EffectText;
// Game Properties
	Card* AttachedCard;
// Functions
	QString FromCostToString();
public:
	void SetEffectBody(int a){if (a>=-1 && a<EffectsConstants::Effects::END) EffectBody=a;}
	void SetEffectBody() {EffectBody=-1;}
	int GetEffectBody() const {return EffectBody;}
	void SetEffectText(const QString& a);
	const QString& GetEffectText() const {return EffectText;}
	const QList<int>& GetTriggers() const {return Triggers;}
	void SetTargets();
	void SetTargets(const QMap<int,int>& a){Targets.clear(); Targets=a;}
	void SetTargets(int key, int val){SetTargets(); AddTarget(key,val);}
	const QMap<int,int>& GetTargets() const {return Targets;}
	void AddTarget(int key,int val){if (key>=0 && key<=EffectsConstants::Targets::Planeswalker) Targets[key]+=val;}
	void SetTriggers(const QList<int>& a){Triggers.clear(); Triggers=a;}
	void SetTriggers(){Triggers.clear();}
	void SetTriggers(int a){Triggers.clear(); if (
		a>=EffectsConstants::Triggers::EntersTheBattlefield &&
		a<=EffectsConstants::Triggers::Dies
		) Triggers.append(a);}
	void AddTrigger(int a){if (
		a>=EffectsConstants::Triggers::EntersTheBattlefield &&
		a<=EffectsConstants::Triggers::Dies
		) Triggers.append(a);}
	void SetManaEffect(bool a){ManaEffect=a;}
	bool GetManaEffect() const {return ManaEffect;}
	void SetDoesntStack(bool a){DoesntStack=a;}
	bool GetDoesntStack() const {return DoesntStack;}
	void SetHiddenEffect(bool a){HiddenEffect=a;}
	bool GetHiddenEffect() const {return HiddenEffect;}
	Effect(QWidget* parent=0);
	Effect(const Effect& a,QWidget* parent=0);
	Effect& operator=(const Effect& a);
	void SetEffectType(int a){if (a>=EffectsConstants::EffectTypes::PassiveEffect && a<=EffectsConstants::EffectTypes::OnResolutionEffect) EffectType=a;}
	const QMap<int,int>& GetEffectCost() const {return EffectCost;}
	void SetEffectCost(const QMap<int,int>& a){EffectCost.clear(); EffectCost=a;}
	void SetEffectCost();
	void SetEffectCost(int key, int cost){SetEffectCost(); EffectCost.insert(key,cost);}
	void AddEffectCost(int key, int cost){if(key>=0 && key<EffectsConstants::EffectCosts::END) EffectCost[key]+=cost;}
	int GetEffectType() const {return EffectType;}
	void SetAttachedCard(Card* a){AttachedCard=a;}
	const Card* GetAttachedCard() const {return AttachedCard;}
public slots:
	void UpdateAspect();
	void Activate();
	void Resolve();
	void ManaPayed();
	void SacrificePayed();
	void TapsPayed();
	void TargetsSelected();
signals:
	void AskForMana(Effect*,QMap<int,int>);
	void AskForSacrifices(Effect*,QMap<int,int>);
	void AskForTaps(Effect*,QMap<int,int>);
	void AskForTargets(Effect*,QMap<int,int>);
	void SendToStack(Effect*);
};
QDataStream &operator<<(QDataStream &out, const Effect &effect);
QDataStream &operator>>(QDataStream &in, Effect &effect);
#endif