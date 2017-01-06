import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Object Classification")

    FileDialog {
        id: fileDialog
        title: "Please choose a picture"
        folder: shortcuts.pictures
        onAccepted: {
            var url=fileDialog.fileUrl
            console.log("You chose: " + url)
            
            var result = image_loader.loadImage(url)
            console.log(result)
            classification.text="classification: "+result
            var str =url.toString()
            var array=str.split('/')
            var name = array[array.length-1]

            var source = "image://ip/"+name
            console.log(source)
            form.imageToShow.source=source
        }
        onRejected: {
            console.log("Canceled")
           
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem { 
                text: "Open..." 
                onTriggered: fileDialog.open()
            }
            MenuItem { text: "Close" }
        }
    }

    statusBar: StatusBar {
        RowLayout {
            anchors.fill: parent
            Label {
                id: classification
                text: "Ready" 
            }
        }
    }
    MainForm {
        id:form
        anchors.fill: parent
    }
}
