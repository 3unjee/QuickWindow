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

#ifdef QT_4
#include "GraphicsScene.h"

// Gui includes
#include <QuickView.h>

/*!
    \class GraphicsScene

    \brief Graphics scene.
*/

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

/* explicit */ GraphicsScene::GraphicsScene(QuickView * view) : QGraphicsScene(view)
{
    _view = view;
}

#endif // QT_4
