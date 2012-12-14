/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "mainwindow.h"

#include <QLayout>
#include <QLabel>
#include <QGraphicsView>

#ifndef Q_OS_QNX
#include <KAction>
#include <KActionCollection>
#include <KDebug>
#include <KScoreDialog>
#include <KStandardGameAction>
#include <KStatusBar>
#include <KToggleAction>
#endif

#include "mainarea.h"

#ifndef Q_OS_QNX
#include "kollisionconfig.h"
#endif

#ifdef Q_OS_QNX
#define i18n // dummy
// quick'n'dirty
#define i18np(arg1,arg2,arg3) QString(arg2).arg(arg3)
#endif

MainWindow::MainWindow()
{
    m_main = new MainArea();
    QGraphicsView* view = new QGraphicsView(m_main, this);
    view->setOptimizationFlags( QGraphicsView::DontClipPainter |
                                QGraphicsView::DontSavePainterState |
                                QGraphicsView::DontAdjustForAntialiasing );
//    view->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    view->setCacheMode( QGraphicsView::CacheBackground );
    view->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    view->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    setCentralWidget(view);

    setupActions();

#ifndef Q_OS_QNX
    QLayout* l = layout();
    Q_ASSERT(l);
    l->setSizeConstraint(QLayout::SetFixedSize);

    // setup status bar
    KStatusBar* bar = statusBar();
    Q_ASSERT(bar);
    m_time_label = new QLabel("");
    bar->addPermanentWidget(m_time_label);

    m_balls_label = new QLabel("");
    bar->addWidget(m_balls_label);
//     bar->setItemAlignment(STATUSBAR_BALLS, Qt::AlignLeft);
#endif

    connect(m_main, SIGNAL(changeGameTime(int)), this, SLOT(setGameTime(int)));
    connect(m_main, SIGNAL(changeBallNumber(int)), this, SLOT(setBallNumber(int)));
    connect(m_main, SIGNAL(changeState(bool)), this, SLOT(changeState(bool)));
    connect(m_main, SIGNAL(pause(bool)), this, SLOT(pause(bool)));

#ifndef Q_OS_QNX
    stateChanged("playing", KXMLGUIClient::StateReverse);
#endif
    connect(m_main, SIGNAL(starting()), this, SLOT(newGame()));
    connect(m_main, SIGNAL(gameOver(int)), this, SLOT(gameOver(int)));

#ifndef Q_OS_QNX
    KGameDifficulty::init(this, this, SLOT(difficultyChanged(KGameDifficulty::standardLevel)));
    KGameDifficulty::setRestartOnChange(KGameDifficulty::RestartOnChange);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Easy);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Medium);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Hard);
    KGameDifficulty::setLevel(KGameDifficulty::standardLevel(KollisionConfig::gameDifficulty()));
#endif
    
#ifdef Q_OS_QNX
    //setMinimumSize(m_main->sceneRect().size().toSize());
#endif
}

MainWindow::~MainWindow()
{
    delete m_main;
}

void MainWindow::setupActions()
{
    // TODO
#ifndef Q_OS_QNX
    // Game
    KAction* abort = actionCollection()->addAction("game_abort");
    abort->setText(i18n("Abort game"));
    connect(abort, SIGNAL(triggered()), m_main, SLOT(abort()));

    KStandardGameAction::pause(m_main, SLOT(togglePause()), actionCollection());
    KStandardGameAction::highscores(this, SLOT(highscores()), actionCollection());
    KStandardGameAction::quit(this, SLOT(close()), actionCollection());

    KAction* action;
    action = new KToggleAction(i18n("&Play Sounds"), this);
    action->setChecked(KollisionConfig::enableSounds());
    actionCollection()->addAction("options_sounds", action);
    connect(action, SIGNAL(triggered(bool)), m_main, SLOT(enableSounds(bool)));

    setupGUI(Create | Save | Keys | StatusBar);
#endif
}

void MainWindow::newGame()
{
#ifndef Q_OS_QNX
    stateChanged("playing");
    m_lastUsedDifficulty = KGameDifficulty::localizedLevelString();
#endif
}

void MainWindow::gameOver(int time)
{
#ifndef Q_OS_QNX
    stateChanged("playing", KXMLGUIClient::StateReverse);

    KScoreDialog ksdialog(KScoreDialog::Name, this);
    ksdialog.addLocalizedConfigGroupNames(KGameDifficulty::localizedLevelStrings()); //Add all the translations of the group names
    ksdialog.setConfigGroupWeights(KGameDifficulty::levelWeights());
    ksdialog.setConfigGroup(m_lastUsedDifficulty);
    KScoreDialog::FieldInfo scoreInfo;
    scoreInfo[KScoreDialog::Score].setNum(time);
    if (ksdialog.addScore(scoreInfo, KScoreDialog::AskName)) {
        ksdialog.exec();
    }
#else
    Q_UNUSED(time)
#endif
}

void MainWindow::highscores()
{
#ifndef Q_OS_QNX
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Time, this);
    ksdialog.addLocalizedConfigGroupNames(KGameDifficulty::localizedLevelStrings()); //Add all the translations of the group names
    ksdialog.setConfigGroupWeights(KGameDifficulty::levelWeights());
    ksdialog.setConfigGroup(KGameDifficulty::localizedLevelString());
    ksdialog.exec();
#endif
}

void MainWindow::setBallNumber(int number)
{
#ifndef Q_OS_QNX
    m_balls_label->setText(i18n("Balls: %1", number));
#else
    Q_UNUSED(number);
#endif
}

void MainWindow::setGameTime(int time)
{
#ifndef Q_OS_QNX
    m_time_label->setText(i18np("Time: %1 second", "Time: %1 seconds", time));
#else
    Q_UNUSED(time);
#endif
}

void MainWindow::changeState(bool running) {
    showCursor(!running);
#ifndef Q_OS_QNX
    KGameDifficulty::setRunning(running);
#endif
}

#ifndef Q_OS_QNX
void MainWindow::difficultyChanged(KGameDifficulty::standardLevel level)
{
    m_main->abort();
    KollisionConfig::setGameDifficulty((int) level);
    KollisionConfig::self()->writeConfig();
}
#endif

void MainWindow::pause(bool p)
{
    showCursor(p);
}

void MainWindow::showCursor(bool show)
{
#ifndef Q_OS_QNX
    if (show) {
        centralWidget()->setCursor(QCursor());
    }
    else {
        centralWidget()->setCursor(Qt::BlankCursor);
    }
#else
    Q_UNUSED(show)
#endif
}
