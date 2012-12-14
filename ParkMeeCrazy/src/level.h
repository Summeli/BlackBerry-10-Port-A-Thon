#ifndef LEVEL_H
#define LEVEL_H

#include <QString>

class Level
{
public:
    Level();

    Level(int levelId, int minMoves, QString levelData, int personalbest = 0, int currentMoves = 0, QString currentState="");

    //again, I'm too lazy for setters and getters
    int m_levelId; //from database
    int m_minMoves;
    QString m_levelData;
    int m_personalBest; //from QSettings
    int m_currentMoves;
    QString m_currentState;
};

#endif // LEVEL_H
