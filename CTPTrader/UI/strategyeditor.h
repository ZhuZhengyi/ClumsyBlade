#ifndef STRATEGYEDITOR_H
#define STRATEGYEDITOR_H

#include <QtGui>

namespace Ui {
class StrategyEditor;
}

enum LAN_TYPE
{
    LAN_LUA,
    LAN_C,
    LAN_CPLUS,
    LAN_CSHARP
};

class StrategyEditor : public QDialog
{
    Q_OBJECT
    
public:
    explicit StrategyEditor(LAN_TYPE type, QWidget *parent = 0);
    ~StrategyEditor();

private slots:
    void run();
    
private:
    Ui::StrategyEditor *ui;
};

#endif // STRATEGYEDITOR_H
