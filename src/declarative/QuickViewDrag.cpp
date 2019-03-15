//=================================================================================================
/*
    Copyright (C) 2019 Benjamin Arnaud. <http://bunjee.me> <bunjee@omega.gg>

    - GNU General Public License Usage:
    This file may be used under the terms of the GNU General Public License version 3 as published
    by the Free Software Foundation and appearing in the LICENSE.md file included in the packaging
    of this file. Please review the following information to ensure the GNU General Public License
    requirements will be met: https://www.gnu.org/licenses/gpl.html.
*/
//=================================================================================================

#include "QuickViewDrag.h"

// Gui includes
#include <QuickView.h>
#include <QuickWindow.h>

/*!
    \class QuickViewDrag

    \brief QML drag item.
*/

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

QuickViewDrag::QuickViewDrag(QuickItem * parent) : QuickItem(parent)
{
    _pressed = false;

    setAcceptHoverEvents(true);

    setAcceptedMouseButtons(Qt::LeftButton);
}

//-------------------------------------------------------------------------------------------------
// Events
//-------------------------------------------------------------------------------------------------

#ifdef QT_4
/* virtual */ void QuickViewDrag::mousePressEvent(QGraphicsSceneMouseEvent *)
#else
/* virtual */ void QuickViewDrag::mousePressEvent(QMouseEvent *)
#endif
{
    _pressed = true;
}

#ifdef QT_4
/* virtual */ void QuickViewDrag::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
#else
/* virtual */ void QuickViewDrag::mouseReleaseEvent(QMouseEvent *)
#endif
{
    _pressed = false;
}

//-------------------------------------------------------------------------------------------------

#ifdef QT_4
/* virtual */ void QuickViewDrag::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
#else
/* virtual */ void QuickViewDrag::mouseDoubleClickEvent(QMouseEvent *)
#endif
{
    if (_window->isMaximized())
    {
         _window->setMaximized(false);
    }
    else _window->setMaximized(true);
}

//-------------------------------------------------------------------------------------------------

#ifdef QT_4
/* virtual */ void QuickViewDrag::mouseMoveEvent(QGraphicsSceneMouseEvent *)
#else
/* virtual */ void QuickViewDrag::mouseMoveEvent(QMouseEvent *)
#endif
{
    if (_pressed == false) return;

#ifdef Q_OS_WIN
    if (_view->isFullScreen()) return;

    _pressed = false;

    ReleaseCapture();

    PostMessage((HWND) _view->winId(), WM_SYSCOMMAND, 0xf012, 0);
#endif
}
