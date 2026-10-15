# 新增组件与功能扩展

这批组件与功能在 EUI `1.3.0` 中引入，使用 EUI 的 Qt/QML 接口。全部从 `import EUI 1.0` 导入；Gallery 中按组件名称搜索即可进入交互示例，展示代码可以展开、复制。新增 26 个公开类型，每个至少提供 5 个演示；既有组件增加 15 个针对功能扩展的演示。属性类型、默认值与绑定表达式同时纳入 Gallery 的中英文 API 表。

## 组件入口

| 类别 | 公开类型 | 主要能力 |
| --- | --- | --- |
| 字段与输入 | `ELabel`、`EFloatLabel`、`EIftaLabel` | 标签关联、点击聚焦、浮动标签、输入框内固定标签 |
| 输入与选择 | `EInputGroup`、`EKnob`、`EListbox` | 混合输入组合、旋钮、单选/多选列表及过滤 |
| 数据操作 | `EOrderList`、`EOrgChart`、`EMeterGroup` | 拖动和按钮排序、组织图、分段计量与图例 |
| 容器与编辑 | `EFieldset`、`EDeferred`、`EInplace` | 标题边框及折叠、视口延迟加载、查看/编辑切换 |
| 导航 | `EDock`、`EMegaMenu`、`ESidebar` | 悬停放大停靠栏、多列菜单、可折叠/响应式侧边导航 |
| 图片与标签 | `ECompare`、`EChip` | 图片/任意内容叠层对比、图标/头像胶囊标签 |
| 效果与焦点 | `EAnimateOnScroll`、`ERipple`、`EFocusTrap` | 视口进出动画、点击涟漪、Tab/Shift+Tab 区域内循环 |
| 弹窗与命令 | `EDynamicDialog`、`EDialogService`、`ETerminal` | 动态创建/传参/返回结果、弹窗管理、命令历史和异步响应 |
| 工作组件 | `EScheduler`、`ETaskBoard`、`ETextEditor` | 事件排程、任务列流转、富文本格式与撤销/重做 |

## 对比与数值

`ECompare.value` 为前层显示百分比（0–100），`vertical` 切换上下分隔，`hover` 开启悬停跟随。`beforeSource`、`afterSource` 接受图片 URL；`beforeComponent`、`afterComponent` 可替换为任意 QML 内容，`handleComponent` 自定义滑块。`setValue(value)` 处理范围限制，`moved(value)` 通知用户操作。方向键微调，Home/End 到达边界。

```qml
ECompare {
    width: 600
    height: 320
    beforeSource: "before.png"
    afterSource: "after.png"
}
```

`EKnob` 使用 `from`、`to`、`stepSize`、`value`；支持拖动、滚轮、方向键及 Home/End。`readOnly` 保持显示并禁止操作。`EMeterGroup.values` 接受 `{label, value, color}` 数组；`maximum` 决定总量，超出总量的部分不会溢出计量条，负数或非有限数不参与显示。

`EMeterGroup.circular: true` 显示圆环分段计量，优先于 `vertical`。`diameter` 设置圆环直径，`barSize` 设置环的粗细；宽度不足时圆环自动缩小。`labelsOnTop` 同样适用于圆环，中央显示有效数值之和及总量。Gallery 提供圆环及自定义总量、置顶标签两个示例。

横向和纵向分段条均按外轮廓裁剪圆角，内部各段保持连续。Gallery 的“圆角分段条”示例使用细条和五段不同比例，现有两个圆环示例保留，分段计量共 8 个示例。

```qml
EMeterGroup {
    width: 360
    diameter: 180
    barSize: 16
    circular: true
    values: [{label: "Used", value: 40, color: "#409eff"},
        {label: "Cache", value: 25, color: "#67c23a"},
        {label: "Free", value: 35, color: "#909399"}]
}
```

示例中的自定义强调色统一使用 Primary `#409eff`、Success `#67c23a`、Warning `#e6a23c`、Danger `#f56c6c` 和 Info `#909399`。这些颜色用于示例数据、布局色块、对比内容、旋钮、动画、排程等展示，不修改组件默认主题或 Gallery 外壳配色。

## 表单与排序

`ELabel.target` 指向字段，点击标签后聚焦；可勾选的目标还会切换勾选状态。`EFormItem` 的默认标签同样关联其字段。`EFloatLabel` 与 `EIftaLabel` 将默认子控件作为字段，也可显式指定 `target`。在包装器内给字段设置 `width: parent.width`。

`EInputGroup` 的默认内容使用 `RowLayout` 排列，可组合 `EInput`、`ESelect`、按钮等，默认 `spacing: 8`，避免边框及焦点状态重叠。需要伸展的字段设置 `Layout.fillWidth: true`；固定宽度的控件使用 `Layout.preferredWidth`，不要依赖由布局接管的 `width`。可以设置 `spacing: 16` 自定义字段之间的像素距离，也可以将 `spacing` 绑定到可变化的值。Gallery 的“自定义间距”示例通过数字输入框实时调节 0～48px，展示两个输入框随间距变化重新分配宽度。

`EInput.showPasswordStrength: true` 与 `showPassword: true` 配合，在输入聚焦时显示强度和规则。`passwordRules` 为 `{label, minLength}` 或 `{label, pattern}` 数组；`passwordRequirements` 返回 `{label, passed}`，`passwordStrength` 返回通过规则的比例。这是界面规则反馈，业务密码校验仍使用业务自己的规则。

`EListbox.model` 接受基本值数组或对象数组；对象默认读取 `label`、`value`、`disabled`，可用 `labelKey`、`valueKey` 修改字段名。对象没有值字段时以对象本身作为值。单选使用 `value`；多选设置 `multiple: true` 并使用 `values`。开启 `filterable` 后显示搜索框，`filterText` 也可由外部读写。方向键导航，空格或 Enter 选择。自定义 `itemDelegate` 从 `parent.optionData` 与 `parent.selected` 读取状态。

`EOrderList` 复用列表选择，提供拖动、上下和置顶/置底；`moveItem(from, to)` 移动一项，`moveSelected("up" | "down" | "top" | "bottom")` 移动所选项。模型更新通过新数组赋值，`changed(model)` 通知结果。禁用项禁止直接移动；置顶/置底在含禁用项的列表中返回 `false`，避免越过固定项。

`ETransfer.reorderable` 开启两栏各自的上下排序，`draggable` 开启跨栏拖动；两项默认关闭，保留既有交互。`reorder(right, from, to)` 中 `right` 指定目标栏；目标栏排序同步更新 `value`，源栏排序更新 `data`。`reordered(right)` 通知内部排序。

## 组织图与内容生命周期

`EOrgChart.model` 接受根节点或根节点数组，节点使用 `{key, label, children, disabled}`。`key` 省略时使用层级路径；显式 key 必须唯一。`collapsedKeys` 管理折叠节点，`selectedKeys` 管理选择。`selectionMode` 支持 `none`、`single`、`multiple`、`checkbox`；checkbox 模式级联处理后代。`toggle(key)`、`select(key)` 更新对应状态；`errorString` 报告重复 key、循环或无效模型。

`EDeferred` 的默认内容为一个 Component；进入可见视口后才创建。`once: false` 在离开时卸载，默认 `once: true` 保留；`margin` 提前加载，`viewport` 可指定视口，同时遵守裁剪祖先的边界。通过 `item`、`status`、`loaded()` 获取加载状态，`reset()` 重置激活。

`EAnimateOnScroll` 使用相同视口判断，并调用已有 `EAnimation`。配置 `enterEffect`、`leaveEffect`、`duration`、`distance`，`once` 控制是否只进入播放一次。`ERipple` 可作为内容容器，或通过 `target` 附加到现有控件，`pulse(x, y)` 支持程序触发。`EFocusTrap.active` 开关焦点约束；控件必须可见、启用且可通过 Tab 聚焦，才进入循环。

`EFieldset` 的 `title` 嵌入边框，`collapsible` 启用折叠，`collapsed` 为状态，`toggle()` 切换。`EInplace` 默认内容是编辑器 Component，`displayComponent` 定义展示状态；`open()`、`close()` 切换，`item` 访问当前编辑器。

## 弹窗与右键菜单

```qml
EApp {
    id: app

    EButton {
        text: "Edit"
        onClicked: app.dialog.open(editorComponent, {name: "EUI"}, {
            title: "Edit account",
            onClose: function(result, accepted) {
                if (accepted)
                    console.log(result.name)
            }
        })
    }
    Component {
        id: editorComponent

        Column {
            property var dialogRef
            property string name

            EInput { id: field; text: parent.name }
            EButton {
                text: "Save"
                onClicked: parent.dialogRef.resolve({name: field.text})
            }
        }
    }
}
```

也可独立创建 `EDialogService`，其 `open(component, properties, options)` 返回弹窗实例，`dialogs` 列出当前弹窗，`closeAll()` 取消全部弹窗。`options` 支持 `title`、`width`、`onClose`。`EDynamicDialog.openComponent(component, properties)` 用于复用实例；`instance` 访问内容，`resolve(value)` 返回结果，`finished(result, accepted)` 通知完成。服务在弹窗退出动画结束后销毁实例。

`EDropdown.openAt(x, y, reference)` 在 reference 的局部坐标打开菜单，未指定时使用现有 reference 或组件本身。`trigger: "contextmenu"` 自动使用鼠标位置。Qt Popup 会处理窗口边界的避让。

## 导航、图片与命令

`EDock.model` 使用 `{label, iconSource, command, disabled}`；`placement` 支持 top、bottom、left、right，`magnification` 配置悬停倍数。`EMegaMenu.model` 使用顶层菜单的 `children` 作为多列分组，每个分组的 `children` 为操作条目；`open(index)`、`close()` 控制显示，`command(command, item)` 返回操作。

停靠栏的隐式尺寸会为悬停放大预留边距，包含首尾按钮及纵向布局；由外部布局设置尺寸时，应保留组件的隐式尺寸，避免强制缩小后裁切放大内容。

`ESidebar` 复用 `EMenu` 模型和 `EDrawer`。`collapsed` 控制图标折叠，`mode: "offcanvas"` 使用抽屉，`overlay: true` 强制覆盖模式；`responsive` 配合 `breakpoint` 在窄窗口切换抽屉。`variant` 支持 sidebar、floating、inset。`headerComponent`、`footerComponent` 及默认内容扩展导航区域，`selected(index, item)` 通知选择。覆盖模式下请将打开按钮放在 Sidebar 外部。

`ETag` 和 `EChip` 支持 `iconSource`、`avatarSource` 及原有 `closable`。`EImage` 和 `EImageViewer` 使用 `showThumbnails` 显示缩略图，工具栏增加水平/垂直翻转和下载。`flipPreview(vertical)` 切换翻转，`downloadPreview(destination)` 将当前原图保存到本地文件 URL，省略目标时打开保存选择器；`allowDownload` 控制入口，`downloadBusy`、`downloaded(destination)`、`downloadFailed(message)` 通知保存状态。支持本地、qrc、data、HTTP(S) 来源；保存使用原始文件数据，不包含预览缩放、旋转或翻转变换。

`ETerminal` 默认提供 help、echo、clear，保留命令历史并用上下键查找。`commandHandler(command)` 可返回值或 Promise，错误显示为错误输出；`execute()`、`write()`、`clear()` 供外部调用。此控件通过应用提供的处理函数运行命令，未接入操作系统 Shell。

## 工作组件

`EScheduler.events` 使用 `{key, title, start, end, allDay, color, disabled}` 数组。`date` 为当前日期，`view` 支持 day、week、month，`firstDayOfWeek` 支持 0–6。点击事件产生 `eventClicked(event)`，点击空白日期产生 `slotClicked(date)`，拖动到另一天保留开始时间与持续时长。`moveEvent(key, start)` 和 `resizeEvent(key, end)` 修改事件，`changed(events)` 通知新数组；`eventDelegate` 从 `parent.eventData` 读取数据。`readOnly` 禁止修改。日/周视图按日期组织事件列表，未实现按小时刻度的时间网格、循环事件或资源调度引擎。

`ETaskBoard.columns` 使用 `{key, title, limit, disabled}`，`tasks` 使用 `{key, title, status, disabled}`。卡片可以拖动到另一列或在列内排序。`moveTask(key, status, position)` 提供相同操作；`beforeMove(task, status, position)` 返回 false 可取消，列的 `limit` 限制流入数量。`filterable` 开启标题筛选，`cardDelegate` 从 `parent.taskData` 读取数据。更新通过 `changed(tasks)` 与 `taskMoved(task, from, to, index)` 通知；业务持久化由调用方处理。

拖动时显示跟随指针的卡片预览，原位置淡化并保留空间，可接收任务的目标列高亮；移出看板释放或被业务规则拒绝时恢复原状。预览复用实际卡片图像，支持自定义 `cardDelegate`。

`ETextEditor.text` 为 Qt RichText HTML，`plainText` 获取纯文本，`editor` 访问底层 TextEdit。工具栏支持粗体、斜体、下划线、删除线、段落对齐、列表、清除格式及撤销/重做。`applyFormat(operation, value)` 还支持 color、size、family、link；链接只接受 HTTP(S) 或 mailto。光标无选择时字符格式作用于当前单词，选择文本时作用于所选文本。`undo()`、`redo()` 与 Qt 文档编辑历史一致。HTML 格式范围以 Qt 富文本支持为准。

这些类型补齐项目已确认的组件与主要功能缺口，使用 EUI 自身接口；不声称与 PrimeVue 或其 PRO Suite 的全部 API 和产品功能逐项兼容。

## 本机验证环境限制

Qt 6.8.3 使用默认 JIT 执行完整组件回归与渲染检查。Qt 5.15.2 Win64 的完整连续创建回归触发上游 [QTBUG-89513](https://github.com/qt/qtdeclarative/commit/35614462443c100b6753b335b58a134fed4b5c35)：JIT 异常处理记录跨页时，记录所在页仍为只读可执行状态。Qt 5 的功能回归显式使用解释器；默认 JIT 的全套回归未通过。此测试设置沿用既有 Gallery 对 Qt 5.15.2 的兼容措施，本轮未修改 EUI 插件、Gallery 启动设置或本机 Qt SDK。Qt 5 offscreen 无法抓取帧缓冲，组件套件的 6 项渲染检查及交互套件的 5 项分段计量像素检查明确跳过；Qt 6 执行这些检查，Qt 5 的圆环、圆角条像素与交互在隔离 Windows 桌面补跑通过。复现命令与测试配置见 [validation/primevue](../validation/primevue/README.md)。

## 本轮验证结果

| 检查 | Qt 5.15.2 Win64 | Qt 6.8.3 Win64 |
| --- | --- | --- |
| EUI 插件与 Gallery 构建 | 通过 | 通过 |
| 公开组件状态回归 | 756 通过 | 756 通过 |
| 自定义插槽回归 | 57 通过 | 57 通过 |
| 新组件与功能回归（含 149 个展示代码） | 193 通过，6 项渲染跳过 | 199 通过，无跳过 |
| Gallery 交互（隔离 Windows 桌面） | 58 通过，无跳过 | 58 通过，无跳过 |
| 本次配色涉及的 39 个组件页面 | 156 个状态通过 | 156 个状态通过 |
| 全部 152 个组件页面 | 608 个状态通过（复跑） | 608 个状态通过 |
| Gallery 双语 API 展示 | 353 通过 | 353 通过 |
| Gallery 滚动条回归 | 6 通过 | 6 通过 |

界面反馈回归覆盖输入组合间距与窄宽度、默认及自定义卡片拖动、拒绝移动与取消、圆环数值和像素、圆角条横向/纵向的两端裁剪与窄色块，以及停靠栏四个方向、三种放大倍数的首尾边界。此次配色页面检查按 39 个组件 × 2 个主题 × 2 个宽度统计；QtTest 的初始化与清理另计，不计入这 156 个状态。

扩大页面检查时，Qt 5 首次连续运行在 `Qt5Qml.dll` 中发生一次访问异常；随后全部页面连续复跑通过，本次受影响的 39 个页面分别执行也全部通过。首次日志保存在 `build/example-palette/pages-qt5.txt`，复跑日志为 `pages-final-qt5.txt`。此偶发异常的根因尚未确认，不能将一次复跑通过解释为已验证旧 SDK 的长期稳定性。

静态文档校验覆盖 152 个 Gallery 条目和 3468 行属性说明。本次修改的 QML 文件及新增展示代码通过本仓库排版规则检查，`git diff --check` 通过。配色与圆角条的验证记录写入本机 `build/example-palette/`，之前四项界面修复的记录位于 `build/component-feedback/`；更早的完整组件库测试见 [全库测试报告](library-validation-1.3.0.md)。

## 输入组合间距与侧栏显示检查

输入组合增加第 6 个示例，通过数字输入框实时绑定 `spacing`。回归检查 0、4、16、32、48px 在 320/480 两种宽度下的实际间距和字段边界。

侧栏滚动区域启用独立图层，保留原来的 Qt 字体渲染、字体和字号，减少与右侧动态示例之间的绘制影响。新增 8 组往返悬停检查，覆盖徽章、输入组合页面的亮暗主题和中英文；每组实际悬停右侧控件与左侧菜单 24 次，并检查文字像素。与 4 组字体检查、滚轮检查、4 组输入组合页面检查一同运行，Qt 5 与 Qt 6 均为 19 项通过（含初始化与清理）。Qt 5 使用 125% 缩放，Qt 6 使用 100% 缩放；测试在隔离 Windows 桌面执行。

用户报告 Qt 5 Release 程序中的文字消失后不会在再次悬停时恢复。本机在调整前未稳定重现这一持续异常，因此此次侧栏改动属于针对性的绘制兼容处理，不能据此认定原故障根因已经确认或已穷举所有驱动条件。日志与实际渲染截图保存在 `build/nav-hover/`。
