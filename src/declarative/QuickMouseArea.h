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

#ifndef QUICKMOUSEAREA_H
#define QUICKMOUSEAREA_H

// Gui includes
#include <QuickItem.h>

class QuickMouseArea : public QuickItem
{
    Q_OBJECT

public:
    explicit QuickMouseArea(QuickItem * parent = NULL);

#ifdef QT_5
protected: // Events
    /* virtual */ void touchEvent(QTouchEvent * event);

    /* virtual */ void touchUngrabEvent();
#endif
};

#endif // QUICKMOUSEAREA_H
