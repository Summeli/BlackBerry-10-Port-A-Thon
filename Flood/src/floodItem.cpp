/****************************************************************************
**
** Copyright (C) 2011 Kirill (spirit) Klochkov.
** Contact: klochkov.kirill@gmail.com
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/

#include <QTime>
#include <QDebug>

#include "flooditem.h"

FloodItem::FloodItem(int row, int column, FloodModel *board)
    : m_row(row),
      m_column(column),
      m_floodBoard(board)
{

}

QModelIndexList FloodItem::boundaryIndexes() const
{
    QModelIndexList indexes;
    indexes << m_floodBoard->index(row(), column() - 1);        //left
    indexes << m_floodBoard->index(row() - 1, column());        //top
    indexes << m_floodBoard->index(row(), column() + 1);        //right
    indexes << m_floodBoard->index(row() + 1, column());        //bottom
    return indexes;
}

void FloodItem::setColor(const QColor &color)
{
    if (m_color == color)
        return;
    const QColor oldColor = m_color;
    m_color = color;

    const QModelIndexList indexes = boundaryIndexes();

    m_floodBoard->addFloodedItem(this);

    foreach (const QModelIndex &index, indexes) {
        FloodItem *item = m_floodBoard->itemByIndex(index);
        m_floodBoard->addFloodedItem(item);
        if (!item || item->color() != oldColor)
            continue;
        m_floodBoard->setData(index, color, FloodModel::Color);
    }
}
//---------------------------------------------------------------------------------------
FloodModel::FloodModel(QObject *parent)
    : QAbstractTableModel(parent),
      m_rows(10),
      m_columns(10)
{
    QHash<int, QByteArray> roles;
    roles.insert(Color, "color");
    setRoleNames(roles);
    init();
}

void FloodModel::init()
{
    qDeleteAll(m_items);
    m_items.clear();
    m_floodedItems.clear();
    if (!m_colors.size())
        return;

    qsrand(QTime::currentTime().msec());
    for (int row = 0; row < rowCount(); ++row) {
        for (int column = 0; column < columnCount(); ++column) {
            FloodItem *item = itemByIndex(index(row, column));
            if (!item)
                continue;
            const int index = qrand() % m_colors.size();
            const QColor color = m_colors.at(index).value<QColor>();
            item->m_color = color;
        }
    }
    addFloodedItem(itemByIndex(index(0, 0)));
    reset();
    emit newArrangement();
}

void FloodModel::setRowCount(int rows)
{
    m_rows = rows;
    init();
    emit rowCountChnaged();
}

int FloodModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_rows;
}

void FloodModel::setColumnCount(int columns)
{
    m_columns = columns;
    init();
    emit columnCountChnaged();
}

int FloodModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_columns;
}

void FloodModel::setColors(const QVariantList &colors)
{
    m_colors = colors;
    init();
    emit colorsChanged();
}

FloodItem *FloodModel::itemByIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return m_items.value(Cell(index.row(), index.column()));
}

QModelIndex FloodModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (row < 0 || column < 0 || row >= rowCount() || column >= columnCount())
        return QModelIndex();

    FloodItem *item = 0;
    Cell cell(row, column);
    if (m_items.contains(Cell(row, column))) {
        item = m_items.value(cell);
    } else {
        item = new FloodItem(row, column, const_cast<FloodModel *>(this));
        m_items.insert(cell, item);
    }
    return createIndex(row, column);
}

QVariant FloodModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    if (!index.isValid() || role != Color)
        return QVariant();

    FloodItem *item = itemByIndex(index);
    if (!item)
        return QVariant();

    return item->color();
}

bool FloodModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);
    if (!index.isValid() || role != Color)
        return false;

    FloodItem *item = itemByIndex(index);
    if (!item)
        return false;

    item->setColor(value.value<QColor>());

    emit colorChanged(index.row(), index.column());
    emit dataChanged(index, index);
    return true;
}

QColor FloodModel::color(int row, int column) const
{
    return data(index(row, column), Color).value<QColor>();
}

void FloodModel::setColor(int row, int column, const QColor &color)
{
    setData(index(row, column), color, Color);
}

void FloodModel::addFloodedItem(FloodItem *item)
{
    if (!item)
        return;

    FloodItem *topRightItem = itemByIndex(index(0, 0));
    if (topRightItem && topRightItem->color() == item->color() && !m_floodedItems.contains(item))
        m_floodedItems << item;
}
