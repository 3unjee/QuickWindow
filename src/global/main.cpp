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

// Qt includes
#ifdef QT_4
#include <QApplication>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeComponent>
#else
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#ifndef Q_OPENGL
#include <QQuickWindow>
#endif
#endif

// Application includes
#include <ControllerCore.h>
#include <QuickWindow.h>
#include <QuickViewDrag.h>
#include <QuickViewResizer.h>

//-------------------------------------------------------------------------------------------------
// Global variables

#ifdef QT_4
QDeclarativeEngine * engine;
#else
QQmlApplicationEngine * engine;
#endif

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------

#ifdef QT_4
QApplication * create(int & argc, char ** argv)
#else
QGuiApplication * create(int & argc, char ** argv)
#endif
{
#ifdef QT_4
    QCoreApplication::setAttribute(Qt::AA_ImmediateWidgetCreation);
#elif defined(QT_5)
    // NOTE: We want to handle dpi scaling ourselves.
    qputenv("QT_SCREEN_SCALE_FACTORS", "1");

#ifndef Q_OPENGL
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);
#endif
#else // QT_6
    // NOTE: We want to handle dpi scaling ourselves.
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    qputenv("QT_SCREEN_SCALE_FACTORS",   "1");

#ifndef Q_OPENGL
    QQuickWindow::setGraphicsApi(QSGRendererInterface::Software);
#endif

    // NOTE: We want the view geometry to be integer based at all time. We don't need this when
    //       setting QT_ENABLE_HIGHDPI_SCALING to 0.
    //QGuiApplication
    //    ::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Round);
#endif

#ifdef QT_4
    QApplication * application = new QApplication(argc, argv);
#else
    QGuiApplication * application = new QGuiApplication(argc, argv);
#endif

    qmlRegisterType<QuickWindow>     ("gui", 1,0, "Window");
    qmlRegisterType<QuickViewDrag>   ("gui", 1,0, "ItemDrag");
    qmlRegisterType<QuickViewResizer>("gui", 1,0, "ItemResizer");

    return application;
}

//-------------------------------------------------------------------------------------------------

void load(const QUrl & url)
{
#ifdef QT_4
    QDeclarativeComponent component(engine);

    component.loadUrl(url);

    if (component.create() == NULL)
    {
        qFatal("load: Cannot create Main QML object: %s.", qPrintable(component.errorString()));
    }
#else
    engine->load(url);
#endif
}

//-------------------------------------------------------------------------------------------------

int main(int argc, char * argv[])
{
#ifdef QT_4
    QApplication * application = create(argc, argv);

    engine = new QDeclarativeEngine(application);

    QDeclarativeContext * context = engine->rootContext();
#else
    QGuiApplication * application = create(argc, argv);

    engine = new QQmlApplicationEngine(application);

    QQmlContext * context = engine->rootContext();
#endif

    qmlRegisterUncreatableType<ControllerCore>("gui", 1,0, "Core", "Core is not creatable");

    CREATE_CONTROLLER(ControllerCore, application);

    context->setContextProperty("core", core);

#ifdef QT_4
    load(QUrl("../content/qt4/Window.qml"));
#elif defined(QT_5)
    load(QUrl("../content/qt5/Window.qml"));
#else
    load(QUrl("../content/qt6/Window.qml"));
#endif

    return application->exec();
}
