/*
    Copyright 2009 Dimitry Suzdalev <dimsuz@gmail.com>

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

#ifndef S_ABOUT_DIALOG_H
#define S_ABOUT_DIALOG_H

#include <QDialog>

class SAboutDialogPrivate;

/**
 * Implements a simple about dialog which shows author & credit info
 * in a nicely formatted manner.
 *
 * Usage:
 * SAboutDialog *dialog = new SAboutDialog(parent);
 * dialog->addAuthor( "My Name", "Main developer", "I@cute.org" );
 * dialog->exec();
 */
class SAboutDialog  : public QDialog
{
    Q_OBJECT

public:
    explicit SAboutDialog(QWidget* parent = 0);
    virtual ~SAboutDialog();

    void setAppNameAndVersion( const QString& appName, const QString& appVer );
    // aka "Distributed under the terms of GPL v2 or any later version"
    void setLicenseNote( const QString& licenseNote );

    void addAuthor( const QString& name, const QString& description, const QString& email );
    void addCredit( const QString& name, const QString& description, const QString& email );

private:
    void mousePressEvent(QMouseEvent *);

private:
    SAboutDialogPrivate * const d;
};

#endif
