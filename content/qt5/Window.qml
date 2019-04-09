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

import QtQuick 2.0
import gui     1.0

Window
{
    //---------------------------------------------------------------------------------------------
    // Properties
    //---------------------------------------------------------------------------------------------

    property int dp8 : dp(8)
    property int dp16: dp(16)
    property int dp24: dp(24)
    property int dp64: dp(64)
    property int dp96: dp(96)

    property int dp320: dp(320)
    property int dp200: dp(200)

    property int dp640: dp(640)
    property int dp480: dp(480)

    property bool touch: false

    //---------------------------------------------------------------------------------------------
    // Settings
    //---------------------------------------------------------------------------------------------

    width : dp640
    height: dp480

    minimumWidth : dp320
    minimumHeight: dp200

    //---------------------------------------------------------------------------------------------
    // Events
    //---------------------------------------------------------------------------------------------

    Component.onCompleted: forceActiveFocus()

    onResizingChanged:
    {
        if (isTouching == false) return;

        if (isResizing)
        {
            touch = true;
        }
        else timer.restart();
    }

    //---------------------------------------------------------------------------------------------
    // Keys
    //---------------------------------------------------------------------------------------------

    Keys.onPressed:
    {
        if (event.key == Qt.Key_Escape)
        {
            event.accepted = true;

            close();
        }
        else if (event.key == Qt.Key_F12)
        {
            event.accepted = true;

            takeShot();
        }
    }

    //---------------------------------------------------------------------------------------------
    // Functions
    //---------------------------------------------------------------------------------------------

    function dp(value)
    {
        return value * ratio;
    }

    function takeShot()
    {
        console.debug("Saving shot: QuickWindow.png");

        saveShot("../QuickWindow.png");

        compressShots(".");
    }

    //---------------------------------------------------------------------------------------------

    function touchStart()
    {
        if (isTouching == false || resizer.visible == false) return;

        touch = true;

        timer.restart();
    }

    function touchClear()
    {
        if (isResizing) return;

        touch = false;
    }

    //---------------------------------------------------------------------------------------------
    // Childs
    //---------------------------------------------------------------------------------------------

    Timer
    {
        id: timer

        interval: 3000

        onTriggered: touchClear()
    }

    Rectangle
    {
        id: background

        anchors.fill: parent

        anchors.margins: dp8

        color: "#323232"
    }

    Text
    {
        id: itemText

        anchors.fill: background

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment  : Text.AlignVCenter

        text: "Hello World"

        color: "#dcdcdc"

        font.family   : "consolas"
        font.pixelSize: dp96
    }

    Text
    {
        anchors.right : background.right
        anchors.bottom: background.bottom

        anchors.margins: dp8

        text: "Qt " + core.version

        color: "#dcdcdc"

        font.family   : "consolas"
        font.pixelSize: dp24
    }

    MouseArea
    {
        anchors.fill: background

        onClicked:
        {
            if (itemText.text == "Hello World")
            {
                 itemText.text = "Click !";
            }
            else itemText.text = "Hello World";
        }
    }

    Rectangle
    {
        id: borders

        anchors.fill: parent

        color: "transparent"

        border.width: resizer.size

        border.color: (touch) ? "#008cdc"
                              : "#161616"

        Behavior on border.width { ColorAnimation { duration: 150 } }
        Behavior on border.color { ColorAnimation { duration: 150 } }
    }

    ItemDrag
    {
        anchors.left : parent.left
        anchors.right: parent.right

        height: dp64

        onPressed: touchStart()
    }

    ItemResizer
    {
        id: resizer

        anchors.fill: parent

        size: (touch) ? dp16 : dp8

        visible: (maximized == false && fullScreen == false)

        onVisibleChanged: touchClear()
    }
}
