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

#ifndef CONTROLLER_H
#define CONTROLLER_H

// Qt includes
#include <QObject>

//-------------------------------------------------------------------------------------------------
// Defines

#define DECLARE_CONTROLLER(Class)     \
                                      \
public:                               \
static Class * instance()             \
{                                     \
    if (Class::_instance == NULL)     \
    {                                 \
        Class::_instance = new Class; \
    }                                 \
                                      \
    return Class::_instance;          \
}                                     \
                                      \
static Class * instancePointer()      \
{                                     \
    return Class::_instance;          \
}                                     \
                                      \
static void clearInstance()           \
{                                     \
    Class::_instance = NULL;          \
}                                     \
                                      \
private:                              \
static Class * _instance;             \

#define INIT_CONTROLLER(Class)       \
                                     \
    Class * Class::_instance = NULL; \

//-------------------------------------------------------------------------------------------------

#define CREATE_CONTROLLER(Class, parent)       \
                                               \
    Class::instance()->initController(parent); \

#define CLEAR_CONTROLLER(Class) \
                                \
    Class::clearInstance();     \

//-------------------------------------------------------------------------------------------------

#define GET_CONTROLLER(Class, Name)          \
                                             \
    Class * Name = Class::instancePointer(); \

#define CONTROLLER(Class, Name)       \
                                      \
    Class * Name = Class::instance(); \
                                      \
    Q_ASSERT(Name);                   \

//-------------------------------------------------------------------------------------------------

class Controller : public QObject
{
    Q_OBJECT

protected:
    Controller();

public: // Interface
    void initController(QObject * parent);

private: // Variables
    bool _created;
};

#endif // CONTROLLER_H
