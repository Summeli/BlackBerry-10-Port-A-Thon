#ifndef BOARDUTILS_H
#define BOARDUTILS_H

#include <QtCore>
#include <QtSql>

#include "car.h"
#include "level.h"


#define BOARD_SIZE 6

class BoardUtils : public QObject
{
    Q_OBJECT
public:
    explicit BoardUtils(QObject *parent = 0);

    Q_PROPERTY(CarModel* carModel READ getCarModel WRITE setCarModel NOTIFY carModelChanged)


signals:
    void carModelChanged();

public slots:
    void readBoardFromString(const QString& str);
    void debugPrintBoard();
    CarModel* getCarModel() { return m_carmodel; }
    void setCarModel(CarModel* otherCarModel) { if(m_carmodel != otherCarModel) { m_carmodel = otherCarModel; emit carModelChanged();} }
    void computeDragMinMaxXY(int carIdx);
    int getDragMinX(int carIdx);
    int getDragMinY(int carIdx);
    int getDragMaxX(int carIdx);
    int getDragMaxY(int carIdx);
    void updateBoardMatrix();
    void updateCarPosition(int carIdx, int x, int y);
    QString colorTransform(const QString& color);

    void startGame();
    void restartLevel();
    bool loadLevel(int levelId);
    void saveState();
    void levelCleared(int levelId, int moves);
    QString getCurrentState();
    int getCurrentLevel();
    int getMaxLevels() { return m_numLevels; }
    int getCurrentMoves() { return m_level.m_currentMoves; }
    void setCurrentMoves(int currMoves) { m_level.m_currentMoves = currMoves; }

    QString adjustPath(const QString &path);

private:
    int m_board[BOARD_SIZE][BOARD_SIZE]; //0 or 1
    CarModel* m_carmodel;
    int m_carCount;
    QHash<QString,QString> m_colors;
    QSettings m_sett;
    Level m_level;
    int m_numLevels;
    QSqlQuery m_query;
};

#endif // BOARDUTILS_H
