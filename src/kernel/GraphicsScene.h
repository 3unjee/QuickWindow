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

#ifndef GRAPHICSDRAG_H
#define GRAPHICSDRAG_H

#ifdef QT_4

#include <QGraphicsScene>

// Forward declarations
class QuickView;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicsScene(QuickView * view);

private:
    QuickView * _view;

private:
    friend class QuickItem;
};

#endif // QT_4
#endif // GRAPHICSDRAG_H
