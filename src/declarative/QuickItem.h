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

#ifndef QUICKITEM_H
#define QUICKITEM_H

// Qt includes
#ifdef QT_4
#include <QDeclarativeItem>
#else
#include <QQuickItem>
#endif

// Forward declarations
class QuickView;
class QuickWindow;

#ifdef QT_4
class QuickItem : public QDeclarativeItem
#else
class QuickItem : public QQuickItem
#endif
{
    Q_OBJECT

#ifdef QT_4
public:
    explicit QuickItem(QDeclarativeItem * parent = NULL);
#else
public:
    explicit QuickItem(QQuickItem * parent = NULL);
#endif

#ifdef QT_4
protected: // QGraphicsItem reimplementation
    /* virtual */ QVariant itemChange(GraphicsItemChange change, const QVariant & value);
#else
protected: // QQuickItem reimplementation
    /* virtual */ void itemChange(ItemChange change, const ItemChangeData & value);
#endif

protected: // Variables
    QuickView   * _view;
    QuickWindow * _window;
};

#endif // QUICKITEM_H
