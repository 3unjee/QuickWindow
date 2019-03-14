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

#ifndef QUICKWINDOW_H
#define QUICKWINDOW_H

// Qt includes
#ifdef QT_4
#include <QEventLoop>
#endif

// Gui includes
#include <QuickItem.h>

// Forward declarations
class QuickView;

class QuickWindow : public QuickItem
{
    Q_OBJECT

    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)

    Q_PROPERTY(int width  READ width  WRITE setWidth  NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)

    Q_PROPERTY(qreal ratio READ ratio NOTIFY ratioChanged)

    Q_PROPERTY(int minimumWidth READ minimumWidth WRITE setMinimumWidth
               NOTIFY minimumWidthChanged)

    Q_PROPERTY(int minimumHeight READ minimumHeight WRITE setMinimumHeight
               NOTIFY minimumHeightChanged)

    Q_PROPERTY(int maximumWidth READ maximumWidth WRITE setMaximumWidth
               NOTIFY maximumWidthChanged)

    Q_PROPERTY(int maximumHeight READ maximumHeight WRITE setMaximumHeight
               NOTIFY maximumHeightChanged)

    Q_PROPERTY(int mouseX READ mouseX WRITE setMouseX NOTIFY mouseXChanged)
    Q_PROPERTY(int mouseY READ mouseY WRITE setMouseY NOTIFY mouseYChanged)

    Q_PROPERTY(bool maximized  READ isMaximized  WRITE setMaximized  NOTIFY maximizedChanged)
    Q_PROPERTY(bool fullScreen READ isFullScreen WRITE setFullScreen NOTIFY fullScreenChanged)
    Q_PROPERTY(bool locked     READ isLocked     WRITE setLocked     NOTIFY lockedChanged)

public:
    explicit QuickWindow(QuickItem * parent = NULL);

    /* virtual */ ~QuickWindow();

public: // Interface
    Q_INVOKABLE void close();

    Q_INVOKABLE void centerWindow();

    Q_INVOKABLE bool saveShot(const QString & fileName,
                              int x = 0, int y = 0, int width = -1, int height = -1) const;

    Q_INVOKABLE bool compressShots(const QString & path, int quality = 0) const;

protected: // QuickItem reimplementation
    /* virtual */ void componentComplete();

private: // Functions
    void setRatio(qreal ratio);

    void setMouseX(int x);
    void setMouseY(int y);

private slots:
    void onGeometryChanged();

signals:
    void xChanged();
    void yChanged();

    void widthChanged ();
    void heightChanged();

    void ratioChanged();

    void minimumWidthChanged ();
    void minimumHeightChanged();

    void maximumWidthChanged ();
    void maximumHeightChanged();

    void mouseXChanged();
    void mouseYChanged();

    void maximizedChanged ();
    void fullScreenChanged();
    void lockedChanged    ();

public: // Properties
    QuickView * view() const;

    void setX(int x);
    void setY(int y);

    void setWidth (int width);
    void setHeight(int height);

    qreal ratio() const;

    int  minimumWidth   () const;
    void setMinimumWidth(int width);

    int  minimumHeight   () const;
    void setMinimumHeight(int height);

    int  maximumWidth   () const;
    void setMaximumWidth(int width);

    int  maximumHeight   () const;
    void setMaximumHeight(int height);

    int mouseX() const;
    int mouseY() const;

    bool isMaximized() const;
    void setMaximized(bool maximized);

    bool isFullScreen() const;
    void setFullScreen(bool fullScreen);

    bool isLocked() const;
    void setLocked(bool locked);

private: // Variables
    QuickView * _view;

    qreal _ratio;

    int _mouseX;
    int _mouseY;

private:
    friend class QuickView;
};

#endif // QUICKWINDOW_H
