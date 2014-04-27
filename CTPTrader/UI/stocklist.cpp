#include "stocklist.h"
#include <QPainter>
#include <QMouseEvent>
#include <QHeaderView>
#include <QTextItem>
#include "tickchart.h"

#include "../ShareLib/share_headers/utils/StringUtil.h"

//int indexA = 0;

StockList::StockList(QWidget *parent) :
    QTableView(parent)
{
    delegate=new MyItemDelegate(this);

    model=new MyStandardItemModel;
    model->setRowCount(0);
    model->setColumnCount(14);

    this->setModel(model);
    this->setItemDelegate(delegate);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setMouseTracking(true);//important
    this->setSelectionMode(SingleSelection);
    int index = 0;
    model->setHeaderData(index++, Qt::Horizontal, tr("InstrumentID"));
    model->setHeaderData(index++, Qt::Horizontal, tr("LastPrice"));
    model->setHeaderData(index++, Qt::Horizontal, tr("Volume"));
    model->setHeaderData(index++, Qt::Horizontal, tr("Turnover"));
    model->setHeaderData(index++, Qt::Horizontal, tr("OpenInterest"));
    model->setHeaderData(index++, Qt::Horizontal, tr("PreClosePrice"));
    model->setHeaderData(index++, Qt::Horizontal, tr("PreOpenInterest"));
    model->setHeaderData(index++, Qt::Horizontal, tr("OpenPrice"));
    model->setHeaderData(index++, Qt::Horizontal, tr("HighestPrice"));
    model->setHeaderData(index++, Qt::Horizontal, tr("LowestPrice"));
    model->setHeaderData(index++, Qt::Horizontal, tr("ClosePrice"));
    model->setHeaderData(index++, Qt::Horizontal, tr("UpperLimitPrice"));
    model->setHeaderData(index++, Qt::Horizontal, tr("LowerLimitPrice"));
    model->setHeaderData(index++, Qt::Horizontal, tr("AveragePrice"));

    QHeaderView *header;

    header = this->verticalHeader();
    header->hide();

    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    this->setColumnWidth(0, 100);
    this->setColumnWidth(1, 60);
    this->setColumnWidth(2, 70);
    this->setColumnWidth(8, 60);
    this->setColumnWidth(9, 60);
    this->setColumnWidth(3, 100);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    cmenu = new QMenu(this);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(show_contextmenu(const QPoint&)));

    connect(delegate, SIGNAL(updatePanelSignal(int)),
            this, SLOT(updatePanelSlot(int)));

}


void StockList::addSlot()
{
    int rowId = rowAt(mapFromGlobal(QCursor::pos()).y());

    QVariant var = model->data(model->index(rowId - 1, 0), Qt::TextAlignmentRole);

    qDebug() << "addSlot itemname=" << var.toString();

    emit addModelSignal(var.toString());

    //emit addItemSignal(item);
}

void StockList::addItemSlot(QStandardItem * item)
{
    model->appendRow(item);
}

void StockList::deleteSlot()
{
    model->removeRow(currentRow);

}

void StockList::show_contextmenu(const QPoint & point)
{
    if (cmenu)
    {
        //cmenu->actions().at(0)->setEnabled(false);
        currentRow = this->rowAt(point.y());
        int maxRow = model->rowCount();
        //qDebug() << "currentRow=" << currentRow << "maxRow=" << maxRow;
        if (currentRow < 0 || currentRow >= maxRow)
        {
            return;
        }
        cmenu->exec(QCursor::pos());
    }
}

void StockList::updateData(CThostFtdcDepthMarketDataField *data)
{
    QList<QStandardItem *> list = model->findItems(data->InstrumentID, Qt::MatchExactly, 0);
    if(list.isEmpty())
    {
        return;
    }

    QModelIndex qIndex = model->indexFromItem(list[0]);

    if (qIndex.isValid())
    {
        int index = 1;
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->LastPrice, Qt::TextAlignmentRole);
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->Volume, Qt::TextAlignmentRole);
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->Turnover, Qt::TextAlignmentRole);
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->OpenInterest, Qt::TextAlignmentRole);
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->PreClosePrice, Qt::TextAlignmentRole);
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->PreOpenInterest, Qt::TextAlignmentRole);
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->OpenPrice, Qt::TextAlignmentRole);
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->HighestPrice, Qt::TextAlignmentRole);
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->LowestPrice, Qt::TextAlignmentRole);
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->ClosePrice, Qt::TextAlignmentRole);
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->UpperLimitPrice, Qt::TextAlignmentRole);
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->LowerLimitPrice, Qt::TextAlignmentRole);
        list[0]->model()->setData(list[0]->model()->index(qIndex.row(), index++), data->AveragePrice, Qt::TextAlignmentRole);
        model->add_md(qIndex.row(), data);

        if(delegate && delegate->getIndex() != -1 && delegate->getIndex() == qIndex.row())
        {
            emit updatePanelSignal(data);
        }
        /*
        else{
        emit updatePanelSignal(data);
        }*/
    }

}

void StockList::updatePanelSlot(int row)
{
    CThostFtdcDepthMarketDataField * data = 0;
    data = model->getMd(row);
    if(data){
        emit updatePanelSignal(data);
    }
}

void StockList::addModel(QString item)
{
    // cout << " StockList::addModel" << endl;
    QList<QStandardItem *> list = model->findItems(item, Qt::MatchExactly, 0);
    if(list.size() == 0)
    {
        QStandardItem *item2 = new QStandardItem(item);
        item2->setData(item, Qt::TextAlignmentRole);
        model->appendRow(item2);
    }
}

void StockList::mouseMoveEvent(QMouseEvent * event)
{
    int column=this->columnAt(event->x());
    int row=this->rowAt(event->y());
    if(column == 0 && row != -1){
        this->setCursor(Qt::PointingHandCursor);
    }
    else{
        this->setCursor(Qt::ArrowCursor);
    }
}

MyItemDelegate::MyItemDelegate(QObject * parent)
    :QItemDelegate(parent)
{
    index = -1;
}

void MyItemDelegate::paint(QPainter * painter,
                           const QStyleOptionViewItem & option,
                           const QModelIndex & index) const
{

    if(index.column() != 0)
        QItemDelegate::paint(painter, option, index);

    switch(index.column())
    {
    case 0:
    {
        painter->setPen(QColor(255, 255, 0));
        QVariant var = index.data(Qt::TextAlignmentRole);
        QRect rect=option.rect;
        painter->drawText(rect, Qt::AlignCenter, var.toString());
        break;
    }
    case 1:
    {
        QModelIndex i2 = index.sibling(index.row(), 5);
        QVariant var = index.data(Qt::TextAlignmentRole);
        QVariant lastClose = i2.data(Qt::TextAlignmentRole);
        QColor color = getQColorByVal(var.toDouble(), lastClose.toDouble());
        painter->setPen(color);
        QRect rect=option.rect;
        painter->drawText(rect, Qt::AlignCenter, checkValue(var.toDouble()));
        break;
    }
    case 2:
    {
        QVariant var = index.data(Qt::TextAlignmentRole);
        QRect rect=option.rect;
        QModelIndex i1 = index.sibling(index.row(), 1);
        QModelIndex i2 = index.sibling(index.row(), 5);
        QVariant last = i1.data(Qt::TextAlignmentRole);
        QVariant lastClose = i2.data(Qt::TextAlignmentRole);
        QColor color = getQColorByVal(last.toDouble(), lastClose.toDouble());
        painter->setPen(color);
        painter->drawText(rect, Qt::AlignCenter, var.toString());
        break;
    }
    case 3: //Turnover
    {
        QVariant var = index.data(Qt::TextAlignmentRole);
        QRect rect=option.rect;
        QModelIndex i1 = index.sibling(index.row(), 1);
        QModelIndex i2 = index.sibling(index.row(), 5);
        QVariant last = i1.data(Qt::TextAlignmentRole);
        QVariant lastClose = i2.data(Qt::TextAlignmentRole);
        QColor color = getQColorByVal(last.toDouble(), lastClose.toDouble());
        painter->setPen(color);
        painter->drawText(rect, Qt::AlignCenter, checkValue(var.toDouble()));
        break;
    }
    case 4: //OpenInterest
    {
        QVariant var = index.data(Qt::TextAlignmentRole);
        QRect rect=option.rect;
        QModelIndex i1 = index.sibling(index.row(), 1);
        QModelIndex i2 = index.sibling(index.row(), 5);
        QVariant last = i1.data(Qt::TextAlignmentRole);
        QVariant lastClose = i2.data(Qt::TextAlignmentRole);
        QColor color = getQColorByVal(last.toDouble(), lastClose.toDouble());
        painter->setPen(color);
        painter->drawText(rect, Qt::AlignCenter, checkValue(var.toDouble()));
        break;
    }
    case 7: //OpenPrice
    case 8: //HighestPrice
    case 9: //LowestPrice
    case 10: //ClosePrice
    {
        QVariant var = index.data(Qt::TextAlignmentRole);
        QRect rect=option.rect;
        QModelIndex i2 = index.sibling(index.row(), 5);
        QVariant lastClose = i2.data(Qt::TextAlignmentRole);
        QColor color = getQColorByVal(var.toDouble(), lastClose.toDouble());
        painter->setPen(color);
        painter->drawText(rect, Qt::AlignCenter, checkValue(var.toDouble()));
        break;
    }
    case 11:
    {
        painter->setPen(QColor(255, 0, 0));
        QVariant var = index.data(Qt::TextAlignmentRole);
        QRect rect=option.rect;
        painter->drawText(rect, Qt::AlignCenter, var.toString());
        break;
    }
    case 12:
    {
        painter->setPen(QColor(0, 255, 0));
        QVariant var = index.data(Qt::TextAlignmentRole);
        QRect rect=option.rect;
        painter->drawText(rect, Qt::AlignCenter, var.toString());
        break;
    }
    case 13:
    {
        QVariant var = index.data(Qt::TextAlignmentRole);
        double d = var.toDouble();
        QString showRes;
        showRes.sprintf("%0.2f", d);
        QRect rect=option.rect;
        painter->setPen(QColor(255, 255, 255));
        painter->drawText(rect, Qt::AlignCenter, showRes);
        break;
    }
    default:
    {
        painter->setPen(QColor(100, 100, 100));
        QVariant var = index.data(Qt::TextAlignmentRole);
        QRect rect=option.rect;
        painter->drawText(rect, Qt::AlignCenter, var.toString());
    }
    }

}

bool MyItemDelegate::editorEvent(QEvent * event,
                                 QAbstractItemModel * model,
                                 const QStyleOptionViewItem & /*option*/,
                                 const QModelIndex & index2)
{

    // cout << "type=" << event->type() << endl;

    switch(event->type())
    {
    case QEvent::MouseButtonPress:
    {
        index = index2.row();
        emit updatePanelSignal(index);
        break;
    }
    case QEvent::MouseButtonDblClick:
    {
        index = index2.row();
        QVariant var = model->data(index2.sibling(index2.row(), 0), Qt::TextAlignmentRole);
        emit switchToTickChartSignal(var.toString());
        return true;
    }
    }

    return false;
}

QVariant MyStandardItemModel::data(
        const QModelIndex & index,
        int role) const
{
    return QStandardItemModel::data(index, role);
}

QVariant MyStandardItemModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const
{

    return QStandardItemModel::headerData(section,orientation,role);
}

void MyStandardItemModel::add_md(int row,CThostFtdcDepthMarketDataField * md)
{
    if(!md_map.contains(row))
    {
        md_map.insert(row, md);
    }else{
        md_map[row]=md;
    }

}

CThostFtdcDepthMarketDataField *MyStandardItemModel::getMd(int row)
{
    return md_map[row];
}

NormalStockList::NormalStockList(QWidget *parent): StockList(parent)
{
    initCmenu();
}

void NormalStockList::initCmenu()
{
    QAction *addAction = cmenu->addAction(QApplication::translate("NormalStockList", "Add to Customed List", 0, QApplication::UnicodeUTF8));

    connect(addAction, SIGNAL(triggered(bool)), this, SLOT(addSlot()));

}

CustomedStockList::CustomedStockList(QWidget *parent): StockList(parent)
{
    initCmenu();
}

void CustomedStockList::initCmenu()
{
    QAction *deleteAction = cmenu->addAction(QApplication::translate("CustomedStockList", "Delete from Customed List", 0, QApplication::UnicodeUTF8));
    connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteSlot()));

}
