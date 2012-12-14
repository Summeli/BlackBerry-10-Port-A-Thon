/***********************************************************************
 *
 * Copyright (C) 2009 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

/***********************************************************************
  * The symbian port of this application with very minor changes has been
  * done by kamalakshan <kamalakshantv@gmail.com> on December 29, 2010
 ***********************************************************************/

#include "window.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QPixmap>
#include <QSplashScreen>
#include <QtGui>
#include <QtCore>

// Needed Symbian specific headers
#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#endif

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    app.setApplicationName("Peg-E");
    app.setApplicationVersion("1.1.0");
    app.setOrganizationDomain("summeli.fi");
    app.setOrganizationName("summeli");

#ifdef Q_OS_SYMBIAN
    QPixmap pixmap(":/splash.png");
    QSplashScreen *splash =  new QSplashScreen(pixmap);
    splash->showFullScreen();
    splash->raise();
    QMutex *mutex = new QMutex();
    mutex->lock();
    QWaitCondition sleep;
    sleep.wait(mutex,2000);
    mutex->unlock();
    delete splash;
#endif
    QTranslator qt_translator;
    qt_translator.load("qt_" + QLocale::system().name());
    app.installTranslator(&qt_translator);

    QTranslator translator;
    translator.load("peg-e_" + QLocale::system().name());
    app.installTranslator(&translator);

    Window window;

    // Symbian specific code
#ifdef Q_OS_SYMBIAN
    CAknAppUi* appUi = dynamic_cast<CAknAppUi*> (CEikonEnv::Static()->AppUi());
    TRAPD(error,
          if (appUi) {
        // Lock application orientation into landscape
        appUi->SetOrientationL(CAknAppUi::EAppUiOrientationPortrait);
    }
    );
#endif
#ifdef Q_OS_SYMBIAN
    window.showMaximized();
#else
    window.showFullScreen();
#endif
    return app.exec();
}
