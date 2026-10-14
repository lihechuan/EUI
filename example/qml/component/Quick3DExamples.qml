import EUI 1.0
import QtQuick 2.15
import QtQuick.Window 2.15

ExamplePage {
    id: page

    readonly property string usageCode: 'import "../../quick3d/littlest-tokyo" as Tokyo\nimport QtQuick\n\n'
        + 'Tokyo.TokyoView {\n    width: 960\n    height: 640\n    language: "'
        + ELocale.language + '"\n}\n'

    title: ELocale.texts.Quick3DTitle

    ExampleSection {
        componentName: "TokyoScene"

        ExampleCase {
            code: page.usageCode
            title: ELocale.texts.Quick3DTitle
            description: ELocale.texts.Quick3DDescription
            sourceFiles: [
                {path: "TokyoScene.qml", code: exampleCatalog.quick3DSource("TokyoScene.qml")},
                {path: "TokyoView.qml", code: exampleCatalog.quick3DSource("TokyoView.qml")},
                {path: "Usage.qml", code: page.usageCode},
                {path: "assets/LittlestTokyo.qml", code: exampleCatalog.quick3DSource("assets/LittlestTokyo.qml")}
            ]

            Rectangle {
                id: preview

                readonly property bool available: exampleCatalog.quick3DAvailable
                readonly property bool software: GraphicsInfo.api === GraphicsInfo.Software

                radius: 8
                height: 600
                width: parent.width
                objectName: "quick3dPreview"
                color: ETheme.dark ? "#1e2026" : "#e7e5e2"

                Loader {
                    id: sceneLoader

                    asynchronous: true
                    anchors.fill: parent
                    objectName: "quick3dSceneLoader"
                    source: "../../quick3d/littlest-tokyo/TokyoView.qml"
                    active: preview.available && !preview.software
                        && preview.GraphicsInfo.api !== GraphicsInfo.Unknown

                    onLoaded: {
                        item.language = Qt.binding(function() { return ELocale.language })
                        item.active = Qt.binding(function() { return page.visible })
                    }
                }
                Column {
                    id: installation

                    spacing: 16
                    anchors.centerIn: parent
                    width: Math.min(520, parent.width - 48)
                    visible: sceneLoader.status !== Loader.Ready

                    EText {
                        color: "#55514d"
                        width: parent.width
                        wrapMode: Text.WordWrap
                        objectName: "quick3dStatus"
                        horizontalAlignment: Text.AlignHCenter
                        text: !preview.available ? exampleRuntime.installed ? ELocale.texts.Quick3DInstalled
                            : ELocale.texts.Quick3DRequirement
                            : preview.software ? ELocale.texts.Quick3DSoftware
                            : sceneLoader.status === Loader.Error ? ELocale.texts.Quick3DError
                            : ELocale.texts.Quick3DLoading
                    }
                    EButton {
                        objectName: "quick3dInstallButton"
                        visible: !preview.available || preview.software
                        anchors.horizontalCenter: parent.horizontalCenter
                        enabled: exampleRuntime.supported && !exampleRuntime.busy
                        text: exampleRuntime.busy ? exampleRuntime.stage === "preparing" ? ELocale.texts.Quick3DPreparing
                            : exampleRuntime.stage === "installing" ? ELocale.texts.Quick3DInstalling
                            : exampleRuntime.stage === "verifying" ? ELocale.texts.Quick3DVerifying : ELocale.texts.Quick3DOpening
                            : exampleRuntime.installed ? ELocale.texts.Quick3DOpen : ELocale.texts.Quick3DInstall

                        onClicked: exampleRuntime.install(ELocale.language, ETheme.dark)
                    }
                    EProgress {
                        showText: true
                        width: parent.width
                        visible: exampleRuntime.busy
                        objectName: "quick3dInstallProgress"
                        percentage: Math.round(exampleRuntime.progress * 100)
                    }
                    EText {
                        color: "#55514d"
                        width: parent.width
                        wrapMode: Text.WordWrap
                        objectName: "quick3dPlatformStatus"
                        horizontalAlignment: Text.AlignHCenter
                        text: !exampleRuntime.supported ? ELocale.texts.Quick3DUnsupported.arg(exampleRuntime.platform)
                            : exampleRuntime.error.length > 0 ? ELocale.texts["Quick3DError_" + exampleRuntime.error]
                            : exampleRuntime.stage === "idle" ? ELocale.texts.Quick3DPlatform.arg(exampleRuntime.platform)
                            : exampleRuntime.stage === "preparing" ? ELocale.texts.Quick3DStage_preparing.arg(exampleRuntime.platform)
                            : ELocale.texts["Quick3DStage_" + exampleRuntime.stage] || ""
                    }
                }
                Timer {
                    repeat: true
                    interval: 100
                    running: sceneLoader.status === Loader.Ready

                    onTriggered: {
                        if (sceneLoader.item && sceneLoader.item.fps > 0)
                            exampleRuntime.confirmReady()
                    }
                }
            }
        }
    }
}
