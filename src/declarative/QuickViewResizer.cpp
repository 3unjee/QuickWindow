//=================================================================================================
/*
    Copyright (C) 2019-2021 Benjamin Arnaud. <http://bunjee.me> <bunjee@omega.gg>

    - GNU Lesser General Public License Usage:
    This file may be used under the terms of the GNU Lesser General Public License version 3 as
    published by the Free Software Foundation and appearing in the LICENSE.md file included in the
    packaging of this file. Please review the following information to ensure the GNU Lesser
    General Public License requirements will be met: https://www.gnu.org/licenses/lgpl.html.
*/
//=================================================================================================

#include "QuickViewResizer.h"

// Gui includes
#include <QuickResizer.h>

/*!
    \class QuickViewResizer

    \brief QML view resizer item.
*/

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

QuickViewResizer::QuickViewResizer(QuickItem * parent) : QuickItem(parent)
{
    _topLeft     = new QuickResizer(QuickResizer::TopLeft,     this);
    _topRight    = new QuickResizer(QuickResizer::TopRight,    this);
    _bottomLeft  = new QuickResizer(QuickResizer::BottomLeft,  this);
    _bottomRight = new QuickResizer(QuickResizer::BottomRight, this);
    _left        = new QuickResizer(QuickResizer::Left,        this);
    _right       = new QuickResizer(QuickResizer::Right,       this);
    _top         = new QuickResizer(QuickResizer::Top,         this);
    _bottom      = new QuickResizer(QuickResizer::Bottom,      this);

    _size = 4;

    refreshCoordinates();
}

//-------------------------------------------------------------------------------------------------
// Protected QuickItem reimplementation
//-------------------------------------------------------------------------------------------------

/* virtual */ void QuickViewResizer::geometryChanged(const QRectF & newGeometry,
                                                     const QRectF & oldGeometry)
{
#ifdef QT_OLD
    QuickItem::geometryChanged(newGeometry, oldGeometry);
#else
    QuickItem::geometryChange(newGeometry, oldGeometry);
#endif


    if (oldGeometry.size() == newGeometry.size()) return;

    refreshCoordinates();
}

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------

void QuickViewResizer::refreshCoordinates()
{
    int width  = this->width ();
    int height = this->height();

    //---------------------------------------------------------------------------------------------
    // Position

    int size2x = _size * 2;

    int sizeX = width  - size2x;
    int sizeY = height - size2x;

#ifdef QT_4
    _topLeft    ->setPos(QPoint(0,     0));
    _topRight   ->setPos(QPoint(sizeX, 0));
    _bottomLeft ->setPos(QPoint(0,     sizeY));
    _bottomRight->setPos(QPoint(sizeX, sizeY));
#else
    _topLeft    ->setPosition(QPoint(0,     0));
    _topRight   ->setPosition(QPoint(sizeX, 0));
    _bottomLeft ->setPosition(QPoint(0,     sizeY));
    _bottomRight->setPosition(QPoint(sizeX, sizeY));
#endif

    sizeX = width  - _size;
    sizeY = height - _size;

#ifdef QT_4
    _left  ->setPos(QPoint(0,      size2x));
    _right ->setPos(QPoint(sizeX,  size2x));
    _top   ->setPos(QPoint(size2x, 0));
    _bottom->setPos(QPoint(size2x, sizeY));
#else
    _left  ->setPosition(QPoint(0,      size2x));
    _right ->setPosition(QPoint(sizeX,  size2x));
    _top   ->setPosition(QPoint(size2x, 0));
    _bottom->setPosition(QPoint(size2x, sizeY));
#endif

    //---------------------------------------------------------------------------------------------
    // Size

    _topLeft    ->setSize(QSize(size2x, size2x));
    _topRight   ->setSize(QSize(size2x, size2x));
    _bottomLeft ->setSize(QSize(size2x, size2x));
    _bottomRight->setSize(QSize(size2x, size2x));

    int size4x = _size * 4;

    sizeX = width  - size4x;
    sizeY = height - size4x;

    _left  ->setSize(QSize(_size, sizeY));
    _right ->setSize(QSize(_size, sizeY));
    _top   ->setSize(QSize(sizeX, _size));
    _bottom->setSize(QSize(sizeX, _size));
}

//-------------------------------------------------------------------------------------------------
// Properties
//-------------------------------------------------------------------------------------------------

int QuickViewResizer::size() const
{
    return _size;
}

void QuickViewResizer::setSize(int size)
{
    if (_size == size) return;

    _size = size;

    refreshCoordinates();

    emit sizeChanged();
}
