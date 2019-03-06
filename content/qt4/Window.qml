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

import QtQuick 1.0
import gui     1.0

Window
{
    //---------------------------------------------------------------------------------------------
    // Settings
    //---------------------------------------------------------------------------------------------

    width : 640
    height: 480

    minimumWidth : 320
    minimumHeight: 200

    //---------------------------------------------------------------------------------------------
    // Events
    //---------------------------------------------------------------------------------------------

    Component.onCompleted: forceActiveFocus()

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
    }

    //---------------------------------------------------------------------------------------------
    // Childs
    //---------------------------------------------------------------------------------------------

    Rectangle
    {
        anchors.fill: parent

        color: "#323232"
    }

    Text
    {
        id: itemText

        anchors.fill: parent

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment  : Text.AlignVCenter

        text: "Hello World"

        color: "#dcdcdc"

        font.family   : "consolas"
        font.pixelSize: 96
    }

    Text
    {
        anchors.right : parent.right
        anchors.bottom: parent.bottom

        anchors.margins: 16

        text: "Qt " + core.version

        color: "#dcdcdc"

        font.family   : "consolas"
        font.pixelSize: 24
    }

    MouseArea
    {
        anchors.fill: parent

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
        anchors.fill: parent

        color: "transparent"

        border.width: 16
        border.color: "#161616"
    }

    ItemDrag
    {
        anchors.left : parent.left
        anchors.right: parent.right

        height: 64
    }

    ItemResizer
    {
        anchors.fill: parent

        visible: (maximized == false && fullScreen == false)
    }
}
