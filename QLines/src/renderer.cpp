/*******************************************************************
 *
 * Copyright 2006-2007 Dmitry Suzdalev <dimsuz@gmail.com>
 *
 * This file is part of the KDE project "KLines"
 *
 * KLines is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * KLines is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KLines; see the file COPYING.  If not, write to
 * the Free Software Foundation, 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 ********************************************************************/
#include "renderer.h"
// TODO
//#include "prefs.h"

#include <QSvgRenderer>
#include <QPixmapCache>
#include <QDebug>
#include <QMessageBox>
/* TODO
#include <KGameTheme>
#include <kpixmapcache.h>
*/

#include <QPainter>

// note: this should be in sync with svg
static inline char color2char( BallColor col )
{
    switch( col )
    {
    case Blue:
        return 'b';
    case Brown:
        return 'e';
    case Cyan:
        return 'c';
    case Green:
        return 'g';
    case Red:
        return 'r';
    case Violet:
        return 'p';
    case Yellow:
        return 'y';
    default:
        return 'x'; // error
    }
}

KLinesRenderer* KLinesRenderer::self()
{
    static KLinesRenderer instance;
    return &instance;
}

KLinesRenderer::KLinesRenderer()
    : m_cellSize(0),
      m_numBornFrames(0), m_numSelFrames(0), m_numDieFrames(0),
      m_bornDuration(0), m_selDuration(0), m_dieDuration(0),
      m_moveDuration(0)
{
    m_renderer = new QSvgRenderer();
    QPixmapCache::setCacheLimit(3*1024);

    const char defaultTheme[] = ":/klines.svg";
    if ( !loadTheme( defaultTheme ) )
        qDebug()<< "Failed to load theme" << defaultTheme;
}

KLinesRenderer::~KLinesRenderer()
{
    delete m_renderer;
}

QPixmap KLinesRenderer::ballPixmap(BallColor color) const
{
	/*
    QPixmap pix;
    QSize sz = QSize(m_cellSize,m_cellSize);
    pix = QPixmap(sz);
    switch(color)
    {
	    case Blue:
		    pix.fill( Qt::blue );
		    break;
	    case Brown:
		    pix.fill( Qt::darkRed );
		    break;
	    case Cyan:
		    pix.fill( Qt::cyan );
		    break;
	    case Green:
		    pix.fill( Qt::green );
		    break;
	    case Red:
		    pix.fill( Qt::red );
		    break;
	    case Yellow:
		    pix.fill( Qt::yellow );
		    break;
	    default:
		    pix.fill(Qt::gray);
		    break;
    }
    return pix;
    */

    QString id = color2char( color )+QString( "_rest" );
    return pixmapFromCache(id);
}

QPixmap KLinesRenderer::animationFrame( AnimationType type, BallColor color, int frame ) const
{
    QString id;
    switch( type )
    {
    case BornAnim:
        id = color2char( color )+QString( "_born_" ) + QString::number( frame+1 );
        return pixmapFromCache(id);
    case SelectedAnim:
        id = color2char( color )+QString( "_select_" ) + QString::number( frame+1 );
        return pixmapFromCache(id);
    case DieAnim:
        id = color2char( color )+QString( "_die_" ) + QString::number( frame+1 );
        return pixmapFromCache(id);
    case MoveAnim:
        qDebug() << "Move animation type isn't supposed to be handled by KLinesRenderer!";
        return QPixmap();
    default:
        qDebug() << "Warning! Animation type not handled in switch!";
        return QPixmap();
    }
}

QPixmap KLinesRenderer::backgroundTilePixmap() const
{
    return pixmapFromCache( "field_cell" );
}

QPixmap KLinesRenderer::backgroundPixmap( const QSize& size ) const
{
    return pixmapFromCache( "background", size );
}

QPixmap KLinesRenderer::previewPixmap() const
{
    return pixmapFromCache( "preview", QSize(m_cellSize, m_cellSize*3) );
}

bool KLinesRenderer::loadTheme( const QString& themeName )
{
    // variable saying whether to discard old cache upon successful new theme loading
    // we won't discard it if m_currentTheme is empty meaning that
    // this is the first time loadTheme() is called
    // (i.e. during startup) as we want to pick the cache from disc
    bool discardCache = !m_currentTheme.isEmpty();

    if( !m_currentTheme.isEmpty() && m_currentTheme == themeName )
    {
        qDebug() << "Notice: not loading the same theme";
        return true; // this is not an error
    }

    qDebug() << "loading theme" << themeName;

    bool res = m_renderer->load( themeName );
    if ( !res )
    {
	//qDebug() << "Faled to load theme" << themeName;
	MSG("Faled to load theme");
        return false;
    }

    m_currentTheme = themeName;

    /* TODO
    m_numBornFrames = theme.property( "NumBornFrames" ).toInt();
    m_numSelFrames = theme.property( "NumSelectedFrames" ).toInt();
    m_numDieFrames = theme.property( "NumDieFrames" ).toInt();

    m_bornDuration = theme.property( "BornAnimDuration" ).toInt();
    m_selDuration = theme.property( "SelectedAnimDuration" ).toInt();
    m_dieDuration = theme.property( "DieAnimDuration" ).toInt();
    m_moveDuration = theme.property( "MoveAnimDuration" ).toInt();
    */
    m_numBornFrames = 8;
    m_numSelFrames = 8;
    m_numDieFrames = 8;

    m_bornDuration = 400;
    m_selDuration = 500;
    m_dieDuration = 300;
    m_moveDuration = 100;

    if(discardCache)
    {
        qDebug() << "discarding cache";
	QPixmapCache::clear();
    }

    return true;
}

void KLinesRenderer::setCellSize(int cellSize)
{
    if ( m_cellSize == cellSize )
        return;

    m_cellSize = cellSize;
}

QPixmap KLinesRenderer::pixmapFromCache(const QString& svgName, const QSize& customSize) const
{
    if(m_cellSize == 0)
        return QPixmap();

    QPixmap pix;
    QSize sz = customSize.isValid() ? customSize : QSize(m_cellSize,m_cellSize);

    QString cacheName = svgName+QString("_%1x%2").arg(sz.width()).arg(sz.height());
    if(!QPixmapCache::find(cacheName, pix))
    {
//        qDebug() << "putting" << cacheName << "to cache";
        pix = QPixmap( sz );
        pix.fill( Qt::transparent );
        QPainter p( &pix );
        m_renderer->render( &p, svgName );
        p.end();
	QPixmapCache::insert(cacheName, pix);
    }
    return pix;
}

QPixmap KLinesRenderer::backgroundBorderPixmap( const QSize& size ) const
{
    if( !hasBorderElement() )
        return QPixmap();

    return pixmapFromCache( "border", size );
}

bool KLinesRenderer::hasBorderElement() const
{
    return m_renderer->elementExists( "border" );

}
