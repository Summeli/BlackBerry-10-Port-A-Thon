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

#ifndef FLOOD_H
#define FLOOD_H

#include <QAbstractTableModel>
#include <QColor>
#include <QPoint>
#include <QDeclarativeListProperty>
#include <QDeclarativeItem>

class FloodModel;

class FloodItem
{
    friend class FloodModel;

public:
    FloodItem(int row, int column, FloodModel *board);

    void setColor(const QColor &color);
    QColor color() const { return m_color; }

    int row() const { return m_row; }
    int column() const { return m_column; }

private:
    QModelIndexList boundaryIndexes() const;

private:
    int m_row;
    int m_column;
    QColor m_color;
    FloodModel *m_floodBoard;
};

class FloodModel : public QAbstractTableModel
{
    Q_OBJECT

    friend class FloodItem;

    Q_PROPERTY(int rows READ rowCount WRITE setRowCount NOTIFY rowCountChnaged)
    Q_PROPERTY(int columns READ columnCount WRITE setColumnCount NOTIFY columnCountChnaged)
    Q_PROPERTY(QVariantList colors READ colors WRITE setColors NOTIFY colorsChanged)
    Q_PROPERTY(bool flooded READ flooded NOTIFY colorChanged)
public:
    enum Roles { Color = Qt::UserRole + 1 };

    explicit FloodModel(QObject *parent = 0);

    void setRowCount(int rows);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    void setColumnCount(int columns);
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void setColors(const QVariantList &colors);
    QVariantList colors() const { return m_colors; }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    bool flooded() const { return m_floodedItems.count() == m_items.count(); }

public slots:
    QColor color(int row, int column) const;
    void setColor(int row, int column, const QColor &color);
    void init();

signals:
    void rowCountChnaged();
    void columnCountChnaged();
    void colorsChanged();
    void colorChanged(int row, int column);
    void newArrangement();

private:
    FloodItem *itemByIndex(const QModelIndex &index) const;
    void emitColorChanged(int row, int column) { emit colorChanged(row, column); }
    void addFloodedItem(FloodItem *item);

private:
    class Cell : public QPoint
    {
    public:
        Cell(int row, int column) : QPoint(row, column) {}

        int row() const { return x(); }
        int column() const { return y(); }

        bool operator < (const Cell &other) const
        {
            if (row() < other.row())
                return true;
            if (row() == other.row() && column() < other.column())
                return true;
            return false;
        }
    };

    int m_rows;
    int m_columns;
    QVariantList m_colors;
    mutable QMap<Cell, FloodItem *> m_items;
    QList<FloodItem *> m_floodedItems;
};

#endif // FLOOD_H

