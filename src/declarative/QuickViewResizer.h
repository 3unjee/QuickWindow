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

#ifndef QUICKVIEWRESIZER_H
#define QUICKVIEWRESIZER_H

// Gui includes
#include <QuickItem.h>

// Forward declarations
class QuickResizer;

class QuickViewResizer : public QuickItem
{
    Q_OBJECT

    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)

public:
    explicit QuickViewResizer(QuickItem * parent = NULL);

protected: // QuickItem reimplementation
#ifdef QT_OLD
    /* virtual */ void geometryChanged(const QRectF &newGeometry,
                                       const QRectF &oldGeometry);
#else
    /* virtual */ void geometryChange(const QRectF &newGeometry,
                                      const QRectF &oldGeometry);
#endif

private: // Functions
    void refreshCoordinates();

signals:
    void sizeChanged();

public: // Properties
    int  size() const;
    void setSize(int size);

private: // Variables
    QuickResizer * _topLeft;
    QuickResizer * _topRight;
    QuickResizer * _bottomLeft;
    QuickResizer * _bottomRight;
    QuickResizer * _left;
    QuickResizer * _right;
    QuickResizer * _top;
    QuickResizer * _bottom;

    int _size;
};

#endif // QUICKVIEWRESIZER_H
