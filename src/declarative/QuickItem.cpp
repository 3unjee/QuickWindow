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

#include "QuickItem.h"

// Gui includes
#include <QuickView.h>
#ifdef QT_4
#include <GraphicsScene.h>
#endif

/*!
    \class QuickItem

    \brief QML item.
*/

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

#ifdef QT_4
QuickItem::QuickItem(QDeclarativeItem * parent) : QDeclarativeItem(parent)
#else
QuickItem::QuickItem(QQuickItem * parent) : QQuickItem(parent)
#endif
{
    _view   = NULL;
    _window = NULL;
}

#ifdef QT_4
//-------------------------------------------------------------------------------------------------
// QGraphicsItem protected reimplementation
//-------------------------------------------------------------------------------------------------

/* virtual */ QVariant QuickItem::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if (change == ItemSceneHasChanged)
    {
        QGraphicsScene * scene = qvariant_cast<QGraphicsScene *> (value);

        GraphicsScene * mainScene = static_cast<GraphicsScene *> (scene);

        if (mainScene)
        {
            _view = mainScene->_view;

            _window = _view->_window;
        }
        else
        {
            _view   = NULL;
            _window = NULL;
        }
    }

    return QDeclarativeItem::itemChange(change, value);
}
#else
//-------------------------------------------------------------------------------------------------
// QuickItem protected reimplementation
//-------------------------------------------------------------------------------------------------

/* virtual */ void QuickItem::itemChange(ItemChange change, const ItemChangeData & value)
{
    if (change == ItemSceneChange)
    {
        _view = static_cast<QuickView *> (value.window);

        if (_view)
        {
             _window = _view->_window;
        }
        else _window = NULL;
    }

    QQuickItem::itemChange(change, value);
}
#endif
