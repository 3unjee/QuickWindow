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

#include "QuickMouseArea.h"

#ifdef QT_5
// Gui includes
#include <QuickView.h>
#endif

/*!
    \class QuickMouseArea

    \brief QML mouse area.
*/

#ifdef QT_5

//-------------------------------------------------------------------------------------------------
// Static variables

static const int MOUSEAREA_DELAY_TOUCH = 200;

#endif

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

/* explicit */ QuickMouseArea::QuickMouseArea(QuickItem * parent) : QuickItem(parent)
{
#ifdef QT_5
    setAcceptTouchEvents(false);
#endif
}

#ifdef QT_5

//-------------------------------------------------------------------------------------------------
// Protected events
//-------------------------------------------------------------------------------------------------

/* virtual */ void QuickMouseArea::touchEvent(QTouchEvent * event)
{
    if (_view == NULL) return;

    const QList<QTouchEvent::TouchPoint> & points = event->touchPoints();

    int id = _view->_touchId;

    if (id == -1)
    {
        if (points.isEmpty()) return;

        QTouchEvent::TouchPoint point = points.first();

        if (point.state() == Qt::TouchPointPressed)
        {
            _view->_touchId = point.id();

            QPoint screenPos = point.screenPos().toPoint();

            QPoint localPos = _view->mapFromGlobal(screenPos);

            QCursor::setPos(screenPos);

            QMouseEvent eventMove(QEvent::MouseMove, localPos, screenPos,
                                  Qt::LeftButton, Qt::NoButton, Qt::NoModifier);

            QMouseEvent eventPress(QEvent::MouseButtonPress, localPos, screenPos,
                                   Qt::LeftButton, Qt::NoButton, Qt::NoModifier);

            QCoreApplication::sendEvent(_view, &eventMove);
            QCoreApplication::sendEvent(_view, &eventPress);
        }
    }
    else
    {
        foreach (const QTouchEvent::TouchPoint & point, points)
        {
            if (point.id() != id) continue;

            if (point.state() == Qt::TouchPointMoved)
            {
                QPoint screenPos = point.screenPos().toPoint();

                QPoint localPos = _view->mapFromGlobal(screenPos);

                QCursor::setPos(screenPos);

                QMouseEvent eventMove(QEvent::MouseMove, localPos, screenPos,
                                      Qt::LeftButton, Qt::NoButton, Qt::NoModifier);

                QCoreApplication::sendEvent(_view, &eventMove);
            }
            else if (point.state() == Qt::TouchPointReleased)
            {
                QPoint screenPos = point.screenPos().toPoint();

                QPoint localPos = _view->mapFromGlobal(screenPos);

                QMouseEvent eventRelease(QEvent::MouseButtonRelease, localPos, screenPos,
                                         Qt::LeftButton, Qt::NoButton, Qt::NoModifier);

                if (_view->_touchItem == this && _view->_touchTimer.isActive())
                {
                    _view->_touchTimer.stop();

                    _view->_touchItem = NULL;

                    QMouseEvent eventClick(QEvent::MouseButtonDblClick, localPos, screenPos,
                                           Qt::LeftButton, Qt::NoButton, Qt::NoModifier);

                    QCoreApplication::sendEvent(_view, &eventClick);
                    QCoreApplication::sendEvent(_view, &eventRelease);
                }
                else
                {
                    QCoreApplication::sendEvent(_view, &eventRelease);

                    _view->_touchItem = this;

                    _view->_touchTimer.start(MOUSEAREA_DELAY_TOUCH);
                }

                _view->_touchId = -1;
            }

            return;
        }
    }
}

/* virtual */ void QuickMouseArea::touchUngrabEvent()
{
    if (_view == NULL) return;

    _view->_touchId = -1;

    QuickItem::touchUngrabEvent();
}

#endif
