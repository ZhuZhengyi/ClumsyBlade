#ifndef STOCKLIST_H
#define STOCKLIST_H

#include <QTableView>
#include <QItemDelegate>
#include <QAbstractTableModel>
#include <QStandardItem>
#include <QPixmap>
#include "../ShareLib/share_headers/ctp_headers/ThostFtdcUserApiStruct.h"


class MyItemDelegate:public QItemDelegate
{
    Q_OBJECT

public:
    MyItemDelegate(QObject * parent=0);
    virtual ~ MyItemDelegate(){}

    void paint(QPainter * painter,
        const QStyleOptionViewItem & option,
        const QModelIndex & index) const;
    bool editorEvent(QEvent * event,
        QAbstractItemModel * model,
        const QStyleOptionViewItem & option,
        const QModelIndex & index);

    int getIndex()
    {
        return index;
    }

signals:
    void switchToTickChartSignal(QString);
    void updatePanelSignal(int);

private:
    // QPixmap favouritePixmap;
    // QPixmap notFavouritePixmap;

    int index;

    QColor getQColorByVal(const double val, const double base) const
    {
        if(val == base)
        {
            return QColor(255, 255, 255);
        }
        else if(val < base)
        {
            return QColor(0, 255, 0);
        }

        return QColor(255, 0, 0);
    }

};

class MyStandardItemModel:public QStandardItemModel
{
    Q_OBJECT
public:
    MyStandardItemModel(QObject * parent=0)
        :QStandardItemModel(parent){}
    virtual ~ MyStandardItemModel(){}

    QMap<int, CThostFtdcDepthMarketDataField *> md_map;

    QVariant data(const QModelIndex & index,
        int role=Qt::DisplayRole) const;
    QVariant headerData(int section,
        Qt::Orientation orientation,
        int role=Qt::DisplayRole) const;

    void add_md(int row,CThostFtdcDepthMarketDataField * md);
    CThostFtdcDepthMarketDataField * getMd(int row);
};

class StockList : public QTableView
{
    Q_OBJECT
public:
    StockList(QWidget *parent = 0);
    virtual ~ StockList()
    {
        if(delegate)
        {
            delete delegate;
        }
        if(model)
        {
            delete model;
        }
    }

    MyItemDelegate* getDelegate()
    {
        return delegate;
    }
    
signals:
    void updatePanelSignal(CThostFtdcDepthMarketDataField*);
    void addItemSignal(QStandardItem *);
    void addModelSignal(QString);
    
public slots:
    void addModel(QString);

protected:

    QMenu * cmenu;
    void mouseMoveEvent(QMouseEvent * event);
    virtual void initCmenu() = 0;

public:
    void updateData(CThostFtdcDepthMarketDataField *data);

private:

    MyItemDelegate *delegate;
    MyStandardItemModel *model;
    int currentRow;

private slots:
    void addSlot();
    void deleteSlot();
    void show_contextmenu(const QPoint&);
    void updatePanelSlot(int);

protected slots:
    void addItemSlot(QStandardItem *);
    
};

class NormalStockList:public StockList
{

public:
    NormalStockList(QWidget *parent = 0);

protected:

    void initCmenu();


};

class CustomedStockList:public StockList
{

public:
    CustomedStockList(QWidget *parent = 0);

protected:

    void initCmenu();


};


#endif // STOCKLIST_H
