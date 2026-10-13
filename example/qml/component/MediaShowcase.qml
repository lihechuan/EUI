import EUI 1.0
import QtQuick 2.15

Column {
    spacing: 24
    width: parent.width

    ExampleSection {
        componentName: "EMediaPlayer"

        ExampleCase {
            objectName: "showcaseEMediaPlayer0"
            title: ELocale.texts.PlaybackControls
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EInput {
                        id: source

                        placeholderText: "输入本地文件地址或媒体 URL"
                        width: Math.min(480, parent.width)
                    }
                    EMediaPlayer {
                        id: player

                        height: 220
                        autoPlay: false
                        source: source.text
                        width: Math.min(640, parent.width)
                    }
                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "播放"
                            enabled: source.text.length > 0

                            onClicked: player.play()
                        }
                        EButton {
                            text: "暂停"

                            onClicked: player.pause()
                        }
                        EButton {
                            text: "停止"

                            onClicked: player.stop()
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EInput {
                    id: source

                    placeholderText: "输入本地文件地址或媒体 URL"
                    width: Math.min(480, parent.width)
                }
                EMediaPlayer {
                    id: player

                    height: 220
                    autoPlay: false
                    source: source.text
                    width: Math.min(640, parent.width)
                }
                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "播放"
                        enabled: source.text.length > 0

                        onClicked: player.play()
                    }
                    EButton {
                        text: "暂停"

                        onClicked: player.pause()
                    }
                    EButton {
                        text: "停止"

                        onClicked: player.stop()
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMediaPlayer"

        ExampleCase {
            title: ELocale.texts.VolumeAndMute
            objectName: "showcaseEMediaPlayer1"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EInput {
                        id: source

                        placeholderText: "输入本地文件地址或媒体 URL"
                        width: Math.min(480, parent.width)
                    }
                    EMediaPlayer {
                        id: player

                        height: 220
                        autoPlay: false
                        source: source.text
                        width: Math.min(640, parent.width)
                    }
                    Column {
                        spacing: 16
                        width: parent.width

                        ESlider {
                            to: 1
                            from: 0
                            value: 0.5
                            stepSize: 0.05
                            width: Math.min(480, parent.width)

                            onMoved: player.volume = value
                        }
                        ESwitch {
                            text: "静音"

                            onCheckedChanged: player.muted = checked
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EInput {
                    id: source

                    placeholderText: "输入本地文件地址或媒体 URL"
                    width: Math.min(480, parent.width)
                }
                EMediaPlayer {
                    id: player

                    height: 220
                    autoPlay: false
                    source: source.text
                    width: Math.min(640, parent.width)
                }
                Column {
                    spacing: 16
                    width: parent.width

                    ESlider {
                        to: 1
                        from: 0
                        value: 0.5
                        stepSize: 0.05
                        width: Math.min(480, parent.width)

                        onMoved: player.volume = value
                    }
                    ESwitch {
                        text: "静音"

                        onCheckedChanged: player.muted = checked
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMediaPlayer"

        ExampleCase {
            title: ELocale.texts.CustomSeeking
            objectName: "showcaseEMediaPlayer2"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EInput {
                        id: source

                        placeholderText: "输入本地文件地址或媒体 URL"
                        width: Math.min(480, parent.width)
                    }
                    EMediaPlayer {
                        id: player

                        height: 220
                        autoPlay: false
                        source: source.text
                        controlsVisible: false
                        width: Math.min(640, parent.width)
                    }
                    ESlider {
                        from: 0
                        to: player.duration
                        value: player.position
                        enabled: player.duration > 0
                        width: Math.min(480, parent.width)

                        onMoved: player.seek(value)
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EInput {
                    id: source

                    placeholderText: "输入本地文件地址或媒体 URL"
                    width: Math.min(480, parent.width)
                }
                EMediaPlayer {
                    id: player

                    height: 220
                    autoPlay: false
                    source: source.text
                    controlsVisible: false
                    width: Math.min(640, parent.width)
                }
                ESlider {
                    from: 0
                    to: player.duration
                    value: player.position
                    enabled: player.duration > 0
                    width: Math.min(480, parent.width)

                    onMoved: player.seek(value)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMediaPlayer"

        ExampleCase {
            title: ELocale.texts.MediaErrors
            objectName: "showcaseEMediaPlayer3"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EInput {
                        id: source

                        placeholderText: "输入本地文件地址或媒体 URL"
                        width: Math.min(480, parent.width)
                    }
                    EMediaPlayer {
                        id: player

                        height: 220
                        autoPlay: false
                        source: source.text
                        width: Math.min(640, parent.width)
                    }
                    EText {
                        width: parent.width
                        wrapMode: Text.WordWrap
                        text: player.errorString.length > 0 ? player.errorString : "等待媒体地址"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EInput {
                    id: source

                    placeholderText: "输入本地文件地址或媒体 URL"
                    width: Math.min(480, parent.width)
                }
                EMediaPlayer {
                    id: player

                    height: 220
                    autoPlay: false
                    source: source.text
                    width: Math.min(640, parent.width)
                }
                EText {
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: player.errorString.length > 0 ? player.errorString : "等待媒体地址"
                }
            }
        }
    }
}
