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

#ifndef QUICKRESIZER_H
#define QUICKRESIZER_H

// Gui includes
#include <QuickItem.h>

class QuickResizer : public QuickItem
{
    Q_OBJECT

public: // Enums
    enum ResizeType
    {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Left,
        Right,
        Top,
        Bottom
    };

public:
    explicit QuickResizer(ResizeType type, QuickItem * parent = NULL);

protected: // Events
#ifdef QT_4
    /* virtual */ void mousePressEvent(QGraphicsSceneMouseEvent * event);
#else
    /* virtual */ void mousePressEvent(QMouseEvent * event);
#endif

private: // Variables
    ResizeType _type;
};

#endif // QUICKRESIZER_H
