#ifndef EFFECTSBUILDER_H
#define EFFECTSBUILDER_H
#include <QWidget>
class QComboBox;
class QCheckBox;
class QPushButton;
class QFrame;
class QLabel;
class Effect;
class SmilesSelector;
class QLineEdit;
class QTextEdit;
class QSpinBox;
class QTableWidget;
class EffectsBuilder: public QWidget{
	Q_OBJECT
private:
	enum {TableRowHeight=14};
	Effect* EffectPreview;
	QFrame* Background;
	QComboBox* EffectTypeCombo;
	QCheckBox* ManaEffectCheck;
	QCheckBox* DoesntStackCheck;
	QCheckBox* HiddenEffectCheck;
	QLabel* TriggersLabel;
	QComboBox* AddTriggerCombo;
	QTableWidget* TriggersTable;
	QPushButton* ResetTriggersButton;
	QComboBox* AddCostCombo;
	QPushButton* ResetCostButton;
	QTextEdit* EffectTextEditor;
	QPushButton* SymbolsMenuButton;
	SmilesSelector* SymbolsSelector;
	QComboBox* TargetTypeSelector;
	QSpinBox* TargetNumberSpin;
	QPushButton* AddTargetButton;
	QPushButton* ResetTargetButton;
	QTableWidget* TargetsTable;
	QLabel* CostViewer;
	QComboBox* EffectBodySelector;
	QPushButton* ResetAllButton;
	QPushButton* SaveButton;
	QPushButton* OpenButton;
	QPushButton* AbortButton;
public:
	EffectsBuilder(QWidget* parent=0);
	Effect* GetEffect() const {return EffectPreview;}
protected:
	void resizeEvent(QResizeEvent* event);
private slots:
	void SetEffectText();
	void SetEffectType(int index);
	void ResetTriggers();
	void ResetCost();
	void AddTrigger(int index);
	void AddCost(int index);
	void AddSymbol(int index);
	void SetManaEffect();
	void SetDoesntStack();
	void SetHiddenEffect();
	void TargetTypeSelected(int index);
	void AddTarget();
	void ResetTarget();
	void SetEffectBody(int index);
	void OpenFromFile();
public slots:
	void ResetAll();
	void Open(const Effect &a);
	void ShowOpenButton(bool a);
	void SaveToFile();
signals:
	void Saved();
};
#endif