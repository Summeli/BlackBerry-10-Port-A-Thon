/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// for Q_OS_SYMBIAN
#include <QtGlobal>

#ifndef Q_OS_QNX
#include <KGameDifficulty>
#include <KXmlGuiWindow>
#include "ui_preferences.h"

#else

#include <QMainWindow>

#endif

class QLabel;
class MainArea;

#ifndef Q_OS_QNX
class MainWindow : public KXmlGuiWindow
#else
class MainWindow : public QMainWindow
#endif
{
Q_OBJECT
    QLabel* m_balls_label;
    QLabel* m_time_label;
    // TODO disabled for now
#ifndef Q_OS_QNX
    Ui_Preferences m_pref_ui;
#endif
    MainArea* m_main;
    QPair<QByteArray, QString> m_lastUsedDifficulty;

    void showCursor(bool show);
public:
    MainWindow();
    ~MainWindow();
protected:
    void setupActions();
protected slots:
    void setGameTime(int time);
    void setBallNumber(int number);

    void newGame();
    void gameOver(int);
    void highscores();
    void changeState(bool);
#ifndef Q_OS_QNX
    void difficultyChanged(KGameDifficulty::standardLevel level);
#endif
    void pause(bool);
};


#endif // MAINWINDOW_H

