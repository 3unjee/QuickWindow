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

#include "QuickResizer.h"

// Gui includes
#include <QuickView.h>
#include <QuickWindow.h>

//-------------------------------------------------------------------------------------------------
// Defines

#ifdef Q_OS_WIN
#define SZ_SIZETOPLEFT      0xf004
#define SZ_SIZETOPRIGHT     0xf005
#define SZ_SIZEBOTTOMLEFT   0xf007
#define SZ_SIZEBOTTOMRIGHT  0xf008
#define SZ_SIZELEFT         0xf001
#define SZ_SIZERIGHT        0xf002
#define SZ_SIZETOP          0xf003
#define SZ_SIZEBOTTOM       0xf006
#endif

/*!
    \class QuickResizer

    \brief QML resizer item.
*/

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

QuickResizer::QuickResizer(ResizeType type, QuickItem * parent) : QuickMouseArea(parent)
{
    _type = type;

    setAcceptedMouseButtons(Qt::LeftButton);

    Qt::CursorShape shape;

    if      (type == TopLeft)      shape = Qt::SizeFDiagCursor;
    else if (type == TopRight)     shape = Qt::SizeBDiagCursor;
    else if (type == BottomLeft)   shape = Qt::SizeBDiagCursor;
    else if (type == BottomRight)  shape = Qt::SizeFDiagCursor;
    else if (type == Left)         shape = Qt::SizeHorCursor;
    else if (type == Right)        shape = Qt::SizeHorCursor;
    else if (type == Top)          shape = Qt::SizeVerCursor;
    else /* if (type == Bottom) */ shape = Qt::SizeVerCursor;

    setCursor(shape);
}

//-------------------------------------------------------------------------------------------------
// Events
//-------------------------------------------------------------------------------------------------

#ifdef QT_4
/* virtual */ void QuickResizer::mousePressEvent(QGraphicsSceneMouseEvent *)
#else
/* virtual */ void QuickResizer::mousePressEvent(QMouseEvent *)
#endif
{
    _window->setResizing(true);

#ifdef Q_OS_WIN
    uint orientation;

    if      (_type == TopLeft)     orientation = SZ_SIZETOPLEFT;
    else if (_type == TopRight)    orientation = SZ_SIZETOPRIGHT;
    else if (_type == BottomLeft)  orientation = SZ_SIZEBOTTOMLEFT;
    else if (_type == BottomRight) orientation = SZ_SIZEBOTTOMRIGHT;
    else if (_type == Left)        orientation = SZ_SIZELEFT;
    else if (_type == Right)       orientation = SZ_SIZERIGHT;
    else if (_type == Top)         orientation = SZ_SIZETOP;
    else if (_type == Bottom)      orientation = SZ_SIZEBOTTOM;
    else                           orientation = 0;

    ReleaseCapture();

    PostMessage((HWND) _view->winId(), WM_SYSCOMMAND, orientation, 0);
#endif // Q_OS_WIN
}
