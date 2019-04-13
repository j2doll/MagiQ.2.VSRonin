#-------------------------------------------------
#
# MagiQ2.pro
#
#-------------------------------------------------

TARGET = MagiQ2
TEMPLATE = app

QT += core 
QT += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG += c++14

RESOURCES += \
MagiQ.qrc

INCLUDEPATH += .

HEADERS += \
BattleGround.h \              
Card.h \
CardBuilder.h \                
CardData.h \
CardViewer.h \                 
ChatWidget.h \
ComunicationConstants.h \
ConstantProportionLayout.h \
ControlledLayout.h \
CostantsDefinition.h \
Deck.h \
Effect.h \
EffectData.h \                 
EffectsBuilder.h \
EffectsConstants.h \           
ErrorCodes.h \
FromEnumToString.h \           
HandLayout.h \
Judge.h \                      
LanClient.h \
LanServer.h \                  
LanServerSocket.h \
LanServerThread.h \            
MagiQPlayer.h \
ManaCostLabel.h \              
PhasesDisplayer.h \
PlayerInfoDisplayer.h \        
PowerToughnesLabel.h \
RoundedCornersLabel.h \        
SelettoreImmagini.h \
SignalerArrow.h \              
SizeSliders.h \
SmileButton.h \                
SmilesSelector.h \
StackDisplayer.h \             
StyleSheets.h \
"Symbols Selector.h" \
TestWidget.h \
TestWidgetThread.h

SOURCES += \
BattleGround.cpp \               
Card.cpp \
CardBuilder.cpp \                
CardData.cpp \
CardViewer.cpp \                 
ChatWidget.cpp \
ConstantProportionLayout.cpp \   
ControlledLayout.cpp \
Deck.cpp \                       
Effect.cpp \
EffectData.cpp \                 
EffectsBuilder.cpp \
FromEnumToString.cpp \           
HandLayout.cpp \
Judge.cpp \                      
LanClient.cpp \
LanServer.cpp \                  
LanServerSocket.cpp \
LanServerThread.cpp \            
MagiQPlayer.cpp \
main.cpp \
ManaCostLabel.cpp \
PhasesDisplayer.cpp \            
PlayerInfoDisplayer.cpp \
PowerToughnesLabel.cpp \         
RoundedCornersLabel.cpp \
SelettoreImmagini.cpp \          
SignalerArrow.cpp \
SizeSliders.cpp \                
SmileButton.cpp \
SmilesSelector.cpp \             
StackDisplayer.cpp \
"Symbols Selector.cpp" \
TestWidget.cpp \
TestWidgetThread.cpp





