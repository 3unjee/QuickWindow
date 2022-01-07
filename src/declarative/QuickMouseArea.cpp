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

#include "QuickMouseArea.h"

#ifdef QT_NEW
// Gui includes
#include <QuickView.h>
#include <QuickWindow.h>
#endif

/*!
    \class QuickMouseArea

    \brief QML mouse area.
*/

#ifdef QT_NEW

//-------------------------------------------------------------------------------------------------
// Static variables

static const int MOUSEAREA_DELAY_TOUCH = 200;

#endif

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

/* explicit */ QuickMouseArea::QuickMouseArea(QuickItem * parent) : QuickItem(parent)
{
#if defined(QT_NEW) && QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    setAcceptTouchEvents(true);
#endif
}

#ifdef QT_NEW

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

#ifdef QT_5
        if (point.state() == Qt::TouchPointPressed)
#else
        if (point.state() == QEventPoint::Pressed)
#endif
        {
            _view->_touchId = point.id();

            _window->setTouching(true);

            QPoint screenPos = point.screenPos().toPoint();

            QPoint localPos = _view->mapFromGlobal(screenPos);

            QCursor::setPos(screenPos);

            QMouseEvent eventMove(QEvent::MouseMove, localPos, screenPos,
                                  Qt::LeftButton, Qt::NoButton, Qt::NoModifier);

            QMouseEvent eventPress(QEvent::MouseButtonPress, localPos, screenPos,
                                   Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

            QCoreApplication::sendEvent(_view, &eventMove);
            QCoreApplication::sendEvent(_view, &eventPress);
        }
    }
    else
    {
        foreach (const QTouchEvent::TouchPoint & point, points)
        {
            if (point.id() != id) continue;

#ifdef QT_5
            if (point.state() == Qt::TouchPointMoved)
#else
            if (point.state() == QEventPoint::Updated)
#endif
            {
                QPoint screenPos = point.screenPos().toPoint();

                QPoint localPos = _view->mapFromGlobal(screenPos);

                QCursor::setPos(screenPos);

                QMouseEvent eventMove(QEvent::MouseMove, localPos, screenPos,
                                      Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

                QCoreApplication::sendEvent(_view, &eventMove);
            }
#ifdef QT_5
            else if (point.state() == Qt::TouchPointReleased)
#else
            else if (point.state() == QEventPoint::Released)
#endif
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

                _window->setTouching(false);
            }

            return;
        }
    }
}

/* virtual */ void QuickMouseArea::touchUngrabEvent()
{
    if (_view == NULL) return;

    _view->_touchId = -1;

    _window->setTouching(false);

    QuickItem::touchUngrabEvent();
}

#endif
