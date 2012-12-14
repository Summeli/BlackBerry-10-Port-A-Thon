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
#include "saboutdialog.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>

class SAboutDialogPrivate
{
public:
    SAboutDialogPrivate() :
        m_wgtScrollArea(0), m_wgtAppInfo(0),
        m_lblAppNameVer(0), m_lblLicenseNote(0),
        m_lblAuthors(0), m_lblCredits(0)
    {}

    QScrollArea* m_wgtScrollArea;
    // this widget gets inserted into the scroll area
    QWidget* m_wgtAppInfo;

    QLabel* m_lblAppNameVer;
    QLabel* m_lblLicenseNote;
    QLabel* m_lblAuthors;
    QLabel* m_lblCredits;
};

SAboutDialog::SAboutDialog(QWidget* parent)
    : QDialog(parent), d(new SAboutDialogPrivate)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel* aboutLabel = new QLabel("<h3>About</h3>", this);
    QLabel* authorsLabel = new QLabel("<b>Authors:</b>", this);
    QLabel* creditsLabel = new QLabel("<b>Credits:</b>", this);

    d->m_wgtScrollArea = new QScrollArea(this);
    mainLayout->addWidget(aboutLabel);
    mainLayout->addWidget(d->m_wgtScrollArea);

    d->m_wgtAppInfo = new QWidget(this);

    d->m_lblAppNameVer = new QLabel(this);

    d->m_lblLicenseNote = new QLabel(this);
    d->m_lblLicenseNote->hide(); // if no text

    d->m_lblAuthors = new QLabel(this);
    d->m_lblAuthors->show();
    d->m_lblAuthors->setWordWrap(true);
    d->m_lblCredits = new QLabel(this);
    d->m_lblCredits->setWordWrap(true);

    QVBoxLayout *lay = new QVBoxLayout(d->m_wgtAppInfo);
    lay->setSpacing( 8 );
    lay->addWidget(d->m_lblAppNameVer);
    lay->addWidget(d->m_lblLicenseNote);
    lay->addWidget(authorsLabel);
    lay->addWidget(d->m_lblAuthors);
    lay->addWidget(creditsLabel);
    lay->addWidget(d->m_lblCredits);
    lay->addStretch(1);

    d->m_wgtScrollArea->setWidget(d->m_wgtAppInfo);
    d->m_wgtScrollArea->setWidgetResizable(true);

}

SAboutDialog::~SAboutDialog()
{
    delete d;
}

void SAboutDialog::setAppNameAndVersion(const QString& appName, const QString& appVer)
{
    d->m_lblAppNameVer->setText("<b>" + appName + " Version " + appVer + "</b>");
}

void SAboutDialog::setLicenseNote(const QString& licenseNote)
{
    d->m_lblLicenseNote->show();
    d->m_lblLicenseNote->setText(licenseNote);
}

void SAboutDialog::addAuthor( const QString& name, const QString& description, const QString& email )
{
    QString curText = d->m_lblAuthors->text();

    if (!curText.isEmpty()) {
        curText.append("<br><br>");
    }

    QString newText = name +
        "<br>&nbsp;&nbsp;" + description +
        "<br>&nbsp;&nbsp;" + email;

    curText.append(newText);
    d->m_lblAuthors->setText(curText);
}

void SAboutDialog::addCredit( const QString& name, const QString& description, const QString& email )
{
    QString curText = d->m_lblCredits->text();

    if (!curText.isEmpty()) {
        curText.append("<br><br>");
    }

    QString newText = name +
        "<br>&nbsp;&nbsp;" + description +
        "<br>&nbsp;&nbsp;" + email;

    curText.append(newText);
    d->m_lblCredits->setText(curText);
}

void SAboutDialog::mousePressEvent(QMouseEvent *){
	this->close();
}
