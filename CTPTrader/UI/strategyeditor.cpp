#include "strategyeditor.h"
#include "ui_strategyeditor.h"

#include "utils/highlighter.h"

#include "../ShareLib/share_headers/lua_include/lua.hpp"
#include "../ShareLib/share_headers/utils/datamanager.h"

#include "QtXml"

StrategyEditor::StrategyEditor(LAN_TYPE type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StrategyEditor)
{
    ui->setupUi(this);

    connect(ui->runBtn, SIGNAL(clicked()), this, SLOT(run()));

    Highlighter *hl = new Highlighter(ui->textEdit->document());

    hl->setTextQueue(DataManager::GetInstance()->GetKeyword());
}

void StrategyEditor::run()
{

    QString str = ui->textEdit->toPlainText();

    qDebug() << str;

    /*

    lua_State *l;
    int error;
    l = lua_open();
    luaL_openlibs(l);

    error = luaL_loadstring(l, str.toAscii().constData()) || lua_pcall(l, 0, 0, 0);

    if (error)
    {
        qDebug() << "error";
        lua_pop(l, 1);  /* pop error message from the stack
    }

    lua_close(l);
    */

}

StrategyEditor::~StrategyEditor()
{
    delete ui;
}
