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

#ifndef QUICKVIEWDRAG_H
#define QUICKVIEWDRAG_H

// Gui includes
#include <QuickMouseArea.h>

class QuickViewDrag : public QuickMouseArea
{
    Q_OBJECT

public:
    explicit QuickViewDrag(QuickItem * parent = NULL);

protected: // Events
#ifdef QT_4
    /* virtual */ void mousePressEvent  (QGraphicsSceneMouseEvent * event);
    /* virtual */ void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    /* virtual */ void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
    /* virtual */ void mouseMoveEvent       (QGraphicsSceneMouseEvent * event);
#else
    /* virtual */ void mousePressEvent  (QMouseEvent * event);
    /* virtual */ void mouseReleaseEvent(QMouseEvent * event);

    /* virtual */ void mouseDoubleClickEvent(QMouseEvent * event);
    /* virtual */ void mouseMoveEvent       (QMouseEvent * event);
#endif

signals:
    void pressed();

private: // Variables
    bool _pressed;
};

#endif // QUICKVIEWDRAG_H
