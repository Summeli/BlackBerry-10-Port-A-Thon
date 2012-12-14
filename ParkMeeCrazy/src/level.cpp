#include "level.h"

Level::Level() :
    m_levelId(-1),
    m_minMoves(0),
    m_levelData(""),
    m_personalBest(0),
    m_currentMoves(0),
    m_currentState("")
{
}

Level::Level(int levelId, int minMoves, QString levelData, int personalbest, int currentMoves, QString currentState)
{
    m_levelId = levelId;
    m_minMoves = minMoves;
    m_levelData = levelData;
    m_personalBest = personalbest;
    m_currentMoves = currentMoves;
    m_currentState = currentState;
}
