/***************************************************************************
    begin                : Fri May 19 2000
    copyright            : (C) 2000 by Roman Razilov
    email                : Roman.Razilov@gmx.de
    copyright            : (C) 2006 by Dmitry Suzdalev
    email                : dimsuz@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KLINES_H
#define KLINES_H

#include <QMainWindow>

class MainWidget;

class KLinesMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    KLinesMainWindow();
    ~KLinesMainWindow();

protected:
    void setupActions();

public Q_SLOTS:
    void startGame();

private Q_SLOTS:
    void viewHighScore();
    void gameOver(int score);
    void showNextToggled(bool show);
    void configureSettings();
    void loadSettings();
    void showAboutDialog();

private:
    MainWidget *mwidget;
    QAction *m_actShowNext;
};

#endif
