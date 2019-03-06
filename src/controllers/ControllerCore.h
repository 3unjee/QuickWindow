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

#ifndef CONTROLLERCORE_H
#define CONTROLLERCORE_H

// Gui includes
#include <Controller.h>

// Defines
#define core ControllerCore::instance()

class ControllerCore : public Controller
{
    Q_OBJECT

    Q_PROPERTY(QString version READ version CONSTANT)

private:
    ControllerCore();

public: // Properties
    QString version() const;

private:
    Q_DISABLE_COPY    (ControllerCore)
    DECLARE_CONTROLLER(ControllerCore)
};

#endif // CONTROLLERCORE_H
