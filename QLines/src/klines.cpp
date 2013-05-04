/*
    Copyright 2000 Roman Merzlyakov <roman@sbrf.barrt.ru>
    Copyright 2000 Roman Razilov <roman@sbrf.barrt.ru>
    Copyright 2007 Dimitry Suzdalev <dimsuz@gmail.com>
    Copyright 2007 Simon Hürlimann <simon.huerlimann@huerlisi.ch>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "klines.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QCoreApplication>
#include <QDesktopWidget>

#include "saboutdialog.h"

#include "mwidget.h"

/* TODO
#include "renderer.h"
#include "prefs.h"
*/
#include "scene.h"

KLinesMainWindow::KLinesMainWindow()
{
    setWindowTitle( "KLines" );

    mwidget = new MainWidget(this);
    setCentralWidget( mwidget );

    connect(mwidget->scene(), SIGNAL(gameOver(int)), SLOT(gameOver(int)));
    setupActions();

/* TODO
    stateChanged("init");
*/
}

KLinesMainWindow::~KLinesMainWindow()
{
}

void KLinesMainWindow::setupActions()
{
    QAction *actNewGame = new QAction("New Game", this);
    connect( actNewGame, SIGNAL(triggered(bool)), SLOT(startGame()) );

    QAction *actUndo = new QAction("Undo Move", this);
    connect( actUndo, SIGNAL(triggered(bool)), mwidget->scene(), SLOT(undo()) );

    QAction *actEndTurn = new QAction("End Turn", this);
    connect( actEndTurn, SIGNAL(triggered(bool)), mwidget->scene(), SLOT(endTurn()) );

    m_actShowNext = new QAction("Show Next Balls", this);
    m_actShowNext->setCheckable(true);
    m_actShowNext->setChecked(true); // TODO read from app settings
    mwidget->setShowNextColors(true); // TODO read from app settings
    connect(m_actShowNext, SIGNAL(triggered(bool)), SLOT(showNextToggled(bool)));

    QAction *actAbout = new QAction("About", this);
    connect( actAbout, SIGNAL(triggered(bool)), SLOT(showAboutDialog()) );

    QAction *actQuit = new QAction("Quit", this);
    connect( actQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()) );

    menuBar()->addAction(actNewGame);
    menuBar()->addAction(actUndo);
    menuBar()->addAction(actEndTurn);
    //menuBar()->addAction(m_actShowNext);
    menuBar()->addAction(actAbout);
    //menuBar()->addAction(actQuit);

  /* TODO
  // Game
  KStandardGameAction::highscores(this, SLOT(viewHighScore()), actionCollection());

  KStandardAction::preferences( this, SLOT( configureSettings() ), actionCollection() );
  */
}

void KLinesMainWindow::gameOver(int score)
{
    /* TODO
    KScoreDialog d(KScoreDialog::Name | KScoreDialog::Score, this);
    d.setConfigGroup( "Highscore" );
    d.addScore(score, KScoreDialog::AskName);
    d.exec();
    */
}

void KLinesMainWindow::viewHighScore()
{
    /* TODO
   KScoreDialog d(KScoreDialog::Name | KScoreDialog::Score, this);
   d.setConfigGroup( "Highscore" );
   d.exec();
   */
}

void KLinesMainWindow::showAboutDialog()
{
    SAboutDialog dlg(this);
    dlg.setAppNameAndVersion("QLine", "1.5-bb10" );
    dlg.setLicenseNote("Distributed under the terms of GPL v2 or any later version \n"
    		"You can download the source code for this app at \n"
    		"https://github.com/Summeli/BlackBerry-10-Port-A-Thon");
    dlg.addAuthor("Antti Pohjola", "BlackBerry 10 porting", "antti.pohjola@gmail.com");
    dlg.addCredit("Dmitry Suzdalev1", "Port to graphics view and symbian OS. Current maintainer", "dimsuz@gmail.com");
    dlg.addCredit("Roman Merzlyakov", "Original Authors of KLines", "roman@sbrf.barrt.ru");
    dlg.addCredit("Roman Razilov", "Original Authors of KLines", "Roman.Razilov@gmx.de");
    dlg.addCredit("Dmitry Suzdalev", "Original Authors of KLines", "dimsuz@gmail.com>");
    dlg.addCredit("Eugene Trounev", "SVG artwork", "it-s@gmail.com");

    QDesktopWidget desktop;
    dlg.resize( desktop.availableGeometry().width(), desktop.availableGeometry().height()/2 );

    dlg.exec();
}

void KLinesMainWindow::startGame()
{
    mwidget->scene()->startNewGame();
}

void KLinesMainWindow::showNextToggled(bool show)
{
    mwidget->setShowNextColors(show);

    QString showNextTxt = m_actShowNext->text();
    if (show)
        showNextTxt.replace("Show", "Hide" );
    else
        showNextTxt.replace("Hide", "Show" );

    m_actShowNext->setText( showNextTxt );

    /* TODO
    Prefs::setShowNext(show);
    Prefs::self()->writeConfig();
    */
}


// FIXME these are strings from old tutorial
// leave them if I ever want to use them when I'll impelement tutorial mode
/**
       msg = i18n("The goal of the game is to put\n"
       msg = i18n("You can make horizontal, vertical\n"
                  "and diagonal lines.");
       msg = i18n("Each turn, three new balls are placed on the board.");
       msg = i18n("Every turn, you can move one ball.");
       msg = i18n("To move a ball, click on it with the mouse,\n"
                  "then click where you want the ball to go.");
       msg = i18n("You just moved the blue ball!");
       msg = i18n("Balls can be moved to every position on the board,\n"
                  "as long as there are no other balls in their way.");
       msg = i18n("Now we only need one more blue ball.");
       msg = i18n("It seems to be our lucky day!");
       msg = i18n("Hurray! And away they go!\n"
                  "Now lets try the green balls.");
       msg = i18n("Now you try!\n"
                  "Click on the green ball and move it to the others!");
       msg = i18n("Almost, try again!");
       msg = i18n("Very good!");
       msg = i18n("Whenever you complete a line you get an extra turn.");
       msg = i18n("This is the end of this tutorial.\n"
                  "Feel free to finish the game!");
                  */

void KLinesMainWindow::configureSettings()
{
	/* TODO
    if ( KConfigDialog::showDialog( "settings" ) )
        return;
    KConfigDialog *dialog = new KConfigDialog( this, "settings", Prefs::self() );
    dialog->addPage( new KGameThemeSelector( dialog, Prefs::self(), KGameThemeSelector::NewStuffDisableDownload  ), i18n( "Theme" ), "games-config-theme" );
    connect( dialog, SIGNAL( settingsChanged(const QString&) ), this, SLOT( loadSettings() ) );
    dialog->setHelp(QString(),"klines");
    dialog->show();
    */
}

void KLinesMainWindow::loadSettings()
{
	/* TODO
    if ( !KLinesRenderer::self()->loadTheme(Prefs::theme()) )
    {
        KMessageBox::error( this,  i18n( "Failed to load \"%1\" theme. Please check your installation.", Prefs::theme() ) );
        return;
    }
    */
    QRectF r = mwidget->scene()->sceneRect();
    mwidget->scene()->invalidate( r, QGraphicsScene::BackgroundLayer ); // redraw background
    mwidget->scene()->resizeScene( (int)r.width(), (int)r.height() ); // redraw scene
}
