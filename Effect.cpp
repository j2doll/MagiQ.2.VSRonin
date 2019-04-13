#include "Effect.h"
#include "Card.h"
#include "StyleSheets.h"
#include "CostantsDefinition.h"
#include "MagiQPlayer.h"

Effect::Effect(QWidget* parent/* =0 */)
	:QWidget(parent)
	,AttachedCard(NULL)
	,EffectType(EffectsConstants::EffectTypes::PassiveEffect)
	,ManaEffect(false)
	,HiddenEffect(false)
	,EffectBody(-1)
	,DoesntStack(false)
{
	PrepareUi();
	SetEffectCost();
	UpdateAspect();
}
void Effect::PrepareUi(){
    EffectButton = new QPushButton(this);
	EffectButton->setObjectName("EffectButton");
	EffectButton->setCheckable(false);
	EffectButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	EffectLabel=new QLabel(EffectButton);
	EffectLabel->setObjectName("EffectLabel");
	EffectLabel->setScaledContents(true);
	EffectLabel->setWordWrap(true);
	QVBoxLayout* EffectButtonLayout=new QVBoxLayout(EffectButton);
	EffectButtonLayout->addWidget(EffectLabel);
	EffectButtonLayout->setContentsMargins(5,1,5,1);
	EffectButtonLayout->setSpacing(0);
	QVBoxLayout* MainLayout=new QVBoxLayout(this);
	MainLayout->setMargin(0);
	MainLayout->setSpacing(0);
	MainLayout->addWidget(EffectButton);
}

int Effect::GetMinimumHeight() const
{
    return EffectLabel->sizeHint().height()+2;
}

void Effect::UpdateAspect()
{
	if (EffectType!=EffectsConstants::EffectTypes::ActivatedEffect) EffectButton->setFlat(true);
	EffectLabel->setText(EffectText);
	if (EffectLabel->sizeHint().height()>height())
		resize(width(),EffectLabel->sizeHint().height());
	setStyleSheet(StyleSheets::EffectCSS);
	update();
}
void Effect::SetEffectCost(){
	EffectCost.clear();
	for (int i=0;i<EffectsConstants::EffectCosts::END;i++)
		EffectCost.insert(i,0);
}
QString Effect::FromCostToString(){
	QString Result("");
	if (EffectType==EffectsConstants::EffectTypes::TriggeredEffect){
		foreach(int index,Triggers){
			if (Result.isEmpty()){
				QString TextToPrint(EffectsConstants::TriggersNames[index]);
				if (TextToPrint.contains("%1")){
					if (TextToPrint.contains("%2"))
						TextToPrint=TextToPrint.arg(tr("When")).arg(AttachedCard ? AttachedCard->GetCardName() : tr("Card"));
				}
				Result.append(TextToPrint);
			}
			else {
				QString TextToPrint(EffectsConstants::TriggersNames[index]);
				if (TextToPrint.contains("%1")){
					if (TextToPrint.contains("%2"))
						TextToPrint=TextToPrint.arg(tr(" or")).arg(AttachedCard ? AttachedCard->GetCardName() : tr("Card"));
				}
				Result.append(TextToPrint);
			}
		}
	}
	for (int i=EffectsConstants::EffectCosts::W;i<EffectsConstants::EffectCosts::Tap;i++){
		if (EffectCost[i]>0){
			if (!Result.isEmpty()) 
				Result.append(tr(" you may pay"));
			Result.append(EffectsConstants::EffectCostStrings[i]);
		}
	}
	if (EffectCost[EffectsConstants::EffectCosts::Tap]>0){
		if (!Result.isEmpty()) Result.append(", ");
		Result.append(EffectsConstants::EffectCostStrings[EffectsConstants::EffectCosts::Tap]);
	}
	return Result;
}
void Effect::Activate(){
	//On Activation of the effect it checks so that every mana requirement is satisfied
	for (int i=EffectsConstants::EffectCosts::W;i<EffectsConstants::EffectCosts::Tap;i++){
		if (EffectCost[i]>0){
			QMap<int,int> CostReq;
			for (int j=EffectsConstants::EffectCosts::W;j<EffectsConstants::EffectCosts::Tap;j++)
				CostReq[j]=EffectCost.value(j,0);
			emit AskForMana(this,CostReq);
			return;
		}
	}
	ManaPayed();
}
void Effect::ManaPayed(){
	//Now ask for sacrifices
	for (int i=EffectsConstants::EffectCosts::SacrificeCrature;i<EffectsConstants::EffectCosts::SicrificeItself;i++){
		if (EffectCost[i]>0){
			QMap<int,int> CostReq;
			for (int j=EffectsConstants::EffectCosts::SacrificeCrature;j<EffectsConstants::EffectCosts::SicrificeItself;j++)
				CostReq[j]=EffectCost.value(j,0);
			emit AskForSacrifices(this,CostReq);
			return;
		}
	}
	SacrificePayed();
}
void Effect::SacrificePayed(){
	//Now ask for Taps
	for (int i=EffectsConstants::EffectCosts::TapOtherCreatures;i<=EffectsConstants::EffectCosts::TapVampires;i++){
		if (EffectCost[i]>0){
			QMap<int,int> CostReq;
			for (int j=EffectsConstants::EffectCosts::TapOtherCreatures;j<EffectsConstants::EffectCosts::TapVampires;j++)
				CostReq[j]=EffectCost.value(j,0);
			emit AskForTaps(this,CostReq);
			return;
		}
	}
	TapsPayed();
}
void Effect::TapsPayed(){
	//Ask for Targets
	for (int i=EffectsConstants::Targets::Creature;i<=EffectsConstants::Targets::Planeswalker;i++){
		if (EffectCost[i]>0){
			QMap<int,int> TragetReq;
			for (int j=EffectsConstants::Targets::Creature;j<EffectsConstants::Targets::Planeswalker;j++)
				TragetReq[j]=EffectCost.value(j,0);
			emit AskForTargets(this,TragetReq);
			return;
		}
	}
	TargetsSelected();
}
void Effect::TargetsSelected(){
	//After Satisfying the external costs check for other costs 
	if (EffectCost[EffectsConstants::EffectCosts::Tap]>0){
		AttachedCard->SetTapped(true);
		AttachedCard->UpdateAspect();
	}
	//If the ability doesn't stack resolve it
	if (ManaEffect || DoesntStack) return Resolve();
	//else Add the effect to the stack
	emit SendToStack(this);
}
void Effect::Resolve(){
	switch(EffectBody){
	case EffectsConstants::Effects::AddWToManaPool:
		AttachedCard->GetController()->AddManaPool(Constants::ManaCosts::W,1); break;
	case EffectsConstants::Effects::AddUToManaPool:
		AttachedCard->GetController()->AddManaPool(Constants::ManaCosts::U,1); break;
	case EffectsConstants::Effects::AddBToManaPool:
		AttachedCard->GetController()->AddManaPool(Constants::ManaCosts::B,1); break;
	case EffectsConstants::Effects::AddRToManaPool:
		AttachedCard->GetController()->AddManaPool(Constants::ManaCosts::R,1); break;
	case EffectsConstants::Effects::AddGToManaPool:
		AttachedCard->GetController()->AddManaPool(Constants::ManaCosts::G,1); break;
	}
}
void Effect::SetTargets(){
	Targets.clear();
	for (int i=0;i<=EffectsConstants::Targets::Planeswalker;i++){
		Targets.insert(i,0);
	}
}
void Effect::SetEffectText(const QString& a){
	EffectText=a;
}
Effect::Effect(const Effect& a,QWidget* parent)
	:QWidget(parent)
	,AttachedCard(NULL)
	,EffectType(a.EffectType)
	,ManaEffect(a.ManaEffect)
	,DoesntStack(a.DoesntStack)
	,HiddenEffect(a.HiddenEffect)
	,EffectCost(a.EffectCost)
	,Targets(a.Targets)
	,Triggers(a.Triggers)
	,EffectBody(a.EffectBody)
	,EffectText(a.EffectText)
	,EffectTooltip(a.EffectTooltip)
	,VariableValues(a.VariableValues)
{
	PrepareUi();
	UpdateAspect();
}
Effect::Effect(const EffectData& a,QWidget* parent)
	:QWidget(parent)
	,AttachedCard(NULL)
	,EffectType(a.GetEffectType())
	,ManaEffect(a.GetManaEffect())
	,DoesntStack(a.GetDoesntStack())
	,HiddenEffect(a.GetHiddenEffect())
	,EffectCost(a.GetEffectCost())
	,Targets(a.GetTargets())
	,Triggers(a.GetTriggers())
	,EffectBody(a.GetEffectBody())
	,EffectText(a.GetEffectText())
	,EffectTooltip(a.GetEffectTooltip())
	,VariableValues(a.GetVariableValues())
{
	PrepareUi();
	UpdateAspect();
}
Effect& Effect::operator=(const Effect& a){
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
	VariableValues=a.VariableValues;
	return *this;
}
QDataStream &operator<<(QDataStream &out, const Effect &effect)
{
	out << qint32(Constants::CardVersion)
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

QDataStream &operator>>(QDataStream &input, Effect &effect){
	qint32 Numbers;
	QString Strings;
	QList<int> IntLists;
	QMap<int,int> IntMap;
	bool Booleans;
	input >> Numbers;
	if (Numbers!=Constants::CardVersion){
		QMessageBox::critical(0,QObject::tr("Wrong File Type"),QObject::tr("Error: Unable to Read the File\nMake sure it's a valid MagiQ Effect File"));
		return input;
	}
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
	effect.UpdateAspect();
	return input;
}
