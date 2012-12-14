#ifndef CAR_H
#define CAR_H

#include <QtCore>

class Car
{
public:
    explicit Car();
    Car(int x, int y, int orientation, int isRed, int length, QString color);
    Car(const Car &other);

    QString toString();
    //public all the way, because
    //I'm beeing lazy with setters and getters :D
    int x,y;
    int orientation;
    int isRed;
    int length;
    QString color;
    QString name;
    int carIdx;
    int dragMinX, dragMaxX;
    int dragMinY, dragMaxY;

};

class CarModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CarRoles {
        XRole = Qt::UserRole + 1,
        YRole,
        OrientationRole,
        IsRedRole,
        LengthRole,
        ColorRole,
        NameRole,
        CarIdxRole,
        DragMinX,
        DragMaxX,
        DragMinY,
        DragMaxY
    };
    CarModel(QObject *parent = 0);
    void addCar(Car* car);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void clear();
    Car* getCarAt(int index);

private:
    QList<Car* > m_cars;
};

#endif // CAR_H
