#include "car.h"

Car::Car()
{
}

Car::Car(const Car &other)
{
    this->x = other.x;
    this->y = other.y;
    this->orientation = other.orientation;
    this->isRed = other.isRed;
    this->length = other.length;
    this->color.clear();
    this->color.append(other.color);
    this->name.clear();
    this->name.append(other.name);
    this->carIdx = other.carIdx;
    this->dragMaxX = other.dragMaxX;
    this->dragMaxY = other.dragMaxY;
    this->dragMinX = other.dragMinX;
    this->dragMinY = other.dragMinY;
}

Car::Car(int x, int y, int orientation, int isRed, int length, QString color) :
    x(x), y(y),
    orientation(orientation),
    isRed(isRed),
    length(length),
    color(color),
    name("NN"),
    carIdx(0),
    dragMinX(0),
    dragMaxX(0),
    dragMinY(0),
    dragMaxY(0)
{
}

QString Car::toString()
{
    QString ret("");
    ret.append("%1,").arg(x);
    ret.append("%2,").arg(y);
    ret.append("%3,").arg(orientation);
    ret.append("%4,").arg(isRed);
    ret.append("%5,").arg(length);
    ret.append(color);
    return ret;
}

CarModel::CarModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;

    roles[XRole] = "X";
    roles[YRole] = "Y";
    roles[OrientationRole] = "Orientation";
    roles[IsRedRole] = "IsRed";
    roles[LengthRole] = "Length";
    roles[ColorRole] = "Color";
    roles[NameRole] = "Name";
    roles[CarIdxRole] = "CarIdx";
    roles[DragMinX] = "DragMinX";
    roles[DragMaxX] = "DragMaxX";
    roles[DragMinY] = "DragMinY";
    roles[DragMaxY] = "DragMaxY";

    setRoleNames(roles);
}

void CarModel::addCar(Car *car)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_cars << car;
    endInsertRows();
}

void CarModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount()-1);
    m_cars.clear();
    endRemoveRows();
}

int CarModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_cars.count();
}

QVariant CarModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_cars.count())
        return QVariant();

    Car *c = m_cars[index.row()];
    const Car &car = *c;
    switch(role) {
        case XRole: return car.x;
        case YRole: return car.y;
        case OrientationRole: return car.orientation;
        case IsRedRole: return car.isRed;
        case LengthRole: return car.length;
        case ColorRole: return car.color;
        case NameRole: return car.name;
        case CarIdxRole: return car.carIdx;
        case DragMinX : return car.dragMinX;
        case DragMaxX : return car.dragMaxX;
        case DragMinY : return car.dragMinY;
        case DragMaxY : return car.dragMaxY;
        default: return QVariant();
    }
}

Car* CarModel::getCarAt(int index)
{
    return m_cars.at(index);
}
