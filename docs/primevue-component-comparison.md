# EUI 与 PrimeVue 组件对照

核查日期：2026-10-15。依据用户提供的 [ConfirmDialog 页面](https://primevue.dev/confirmdialog/)及其所属的 [PrimeVue 官方组件总目录](https://primevue.dev/components/)；入口页面页脚显示 PrimeVue 5.0.2，部分子页面缓存仍显示 5.0.1。

## 补齐后的当前状态

`1.3.0` 新增 26 个公开类型，当前共 156 个公开 QML 类型，Gallery 有 152 个组件条目。原审计的 17 项公共能力缺口和 9 项主要功能缺口均已有实现入口；另增加 Scheduler、Task Board、Text Editor 三类原生工作组件。接口、模型、操作方法及具体范围见 [新增组件与功能扩展](primevue-completion.md)。

| 原缺口 | 当前入口 |
| --- | --- |
| FloatLabel / IftaLabel / Knob / Listbox | EFloatLabel / EIftaLabel / EKnob / EListbox |
| OrderList / OrgChart / Deferred / Fieldset | EOrderList / EOrgChart / EDeferred / EFieldset |
| Dock / MegaMenu / Compare | EDock / EMegaMenu / ECompare |
| AnimateOnScroll / FocusTrap / Inplace / MeterGroup / Ripple / Terminal | EAnimateOnScroll / EFocusTrap / EInplace / EMeterGroup / ERipple / ETerminal |
| InputGroup / Label / DynamicDialog / Sidebar / Chip | EInputGroup / ELabel / EDynamicDialog + EDialogService / ESidebar / EChip |
| 密码规则与强度 | EInput.showPasswordStrength / passwordRules |
| 穿梭框排序与拖动 | ETransfer.reorderable / draggable |
| 右键坐标定位 | EDropdown.openAt(x, y, reference) |
| 图片缩略图、翻转、保存 | EImage / EImageViewer |
| Scheduler / Task Board / Text Editor | EScheduler / ETaskBoard / ETextEditor |

Gallery 为每个新类型提供至少 5 个展示，并在既有组件页面添加扩展展示，共新增 149 个场景。Qt 5.15.2（Win64 功能回归使用解释器，运行时限制见功能扩展说明）与 Qt 6.8.3 的本地构建和功能检查覆盖公共类型、示例源码、选择与排序、拖拽、焦点、视口加载、弹窗生命周期、富文本格式和图片保存；Qt 6 软件后端额外验证浅色/深色渲染与 Compare 像素裁剪。

以下保留**实施前的审计快照**，其中“缺少”“部分覆盖”和历史数量用于说明此次补齐依据，不再表示当前实现状态。用途补齐不表示 EUI 与 PrimeVue 或 PRO Suite 的全部 API、布局细节和产品功能逐项等价。

## 实施前的范围与判定

主表覆盖官网当前基础目录的 **91 项、10 个分类**。PRO Suite 单独列出，标注 Roadmap 的计划项不计为已提供组件。目录侧栏中 Deferred 链接到 DeferredContent，OrgChart 链接到 OrganizationChart，主表保留侧栏名称。

本地核查覆盖 `source/qt5/imports/eui/qmldir`、`source/EUI.cpp` 的公开类型及 singleton 注册、`example/src/ExampleCatalog.cpp`、相关 QML 实现及其私有调用链。当前 qmldir 有 130 个公开 QML 类型，Gallery 有 126 条目录记录；这些数量与 PrimeVue 的分类粒度不同，不能直接换算覆盖率。没有仅根据名称判断缺失，也没有将 Qt 原生继承能力视为不存在。

| 判定 | 数量 | 含义 |
| --- | ---: | --- |
| 核心对应 | 63 | 已有直接组件、既有组件组合或 Qt 原生能力承接主要用途；不代表全部 API、变体、可访问性和交互行为等价。 |
| 部分覆盖 | 9 | 已有相关实现，但仍有明确的主要封装或增强能力缺口。 |
| 缺少公共能力 | 17 | 没有可直接使用的对应公共组件或通用封装；示例内部代码不算公共导出。 |
| 平台差异 | 2 | Web 布局传播或 DOM/CSS 机制与 Qt/QML 不同，单列说明，不直接作为应补组件。 |

本次为目录与源码静态核查，没有运行逐组件功能、渲染、键盘或可访问性等价测试。“核心对应”是用途对照，不是完整功能兼容性认证。

## 缺少公共组件或通用能力：17 项

| 分类 | PrimeVue 官方资料 | 当前缺口 |
| --- | --- | --- |
| 表单 | [FloatLabel](https://primevue.dev/floatlabel/) | 缺随焦点或输入内容浮动的字段标签。 |
| 表单 | [IftaLabel](https://primevue.dev/iftalabel/) | 缺输入框内部顶端固定标签的通用容器。 |
| 表单 | [Knob](https://primevue.dev/knob/) | 缺可拖动或用键盘调整数值的旋钮；ECircleProgress 是进度显示。 |
| 表单 | [Listbox](https://primevue.dev/listbox/) | 缺常驻展示、支持单选或多选及筛选的独立选择列表；ESelect 是弹出选择，EList 是列表展示。 |
| 数据 | [OrderList](https://primevue.dev/orderlist/) | 缺带列表项选择和上下重排交互的通用排序列表。 |
| 数据 | [OrgChart](https://primevue.dev/organizationchart/) | 缺节点与连接线构成的组织结构图；ETree 不等于图形组织布局。 |
| 面板 | [Deferred](https://primevue.dev/deferredcontent/) | 没有导出的通用视口延迟加载组件；Gallery 内部 ExampleSection 已有相关实现。 |
| 面板 | [Fieldset](https://primevue.dev/fieldset/) | 缺标题嵌入边框、可折叠的字段分组容器。 |
| 菜单 | [Dock](https://primevue.dev/dock/) | 缺 Dock 形态的图标导航组件。 |
| 菜单 | [MegaMenu](https://primevue.dev/megamenu/) | 缺将多列、分组子菜单同时展开的大型导航菜单。 |
| 媒体 | [Compare](https://primevue.dev/compare/) | 缺拖动分隔线比较两份图片或内容的组件；ESplitter 调整布局尺寸，不提供叠层揭示对比。 |
| 其他 | [AnimateOnScroll](https://primevue.dev/animateonscroll/) | 已有 EAnimation 动画执行器，缺视口进入/离开时自动触发动画的公共封装。 |
| 其他 | [FocusTrap](https://primevue.dev/focustrap/) | 缺可用于任意容器的 Tab/Shift+Tab 焦点圈定公共组件；不据此否定 Qt 弹窗自身的焦点行为。 |
| 其他 | [Inplace](https://primevue.dev/inplace/) | 缺通用查看/编辑双态内容容器；表格内编辑不能替代任意内容的公共封装。 |
| 其他 | [MeterGroup](https://primevue.dev/metergroup/) | 缺多段计量条及对应图例；单值 EProgress 不是多段分布显示。 |
| 其他 | [Ripple](https://primevue.dev/ripple/) | 缺可附加到任意目标的点击水波纹效果。 |
| 其他 | [Terminal](https://primevue.dev/terminal/) | 缺命令输入、历史与响应输出的终端交互组件；ECodeView 是代码展示。 |

## 部分覆盖：9 项

| PrimeVue 官方资料 | EUI 现有入口 | 尚缺能力 |
| --- | --- | --- |
| [InputGroup](https://primevue.dev/inputgroup/) | `EInput` | prependComponent/appendComponent 可组合输入前后内容；没有面向多种表单控件的统一输入组容器。 |
| [InputPassword](https://primevue.dev/inputpassword/) | `EInput` | showPassword/passwordVisible 提供密码与显隐切换；缺内置强度计及密码规则提示。 |
| [Label](https://primevue.dev/label/) | `EFormItem / EText` | 已有标签文字、必填标记及布局；缺独立的标签与目标控件关联、点击聚焦能力。 |
| [PickList](https://primevue.dev/picklist/) | `ETransfer` | 已有双栏筛选和左右搬移；缺列表内上下重排及拖拽搬移交互。 |
| [DynamicDialog](https://primevue.dev/dynamicdialog/) | `EDialog` | contentComponent、Loader、loadedContent 及 destroyOnClose 支持动态内容；缺统一打开任意组件、传参及返回结果的对话框服务。 |
| [ContextMenu](https://primevue.dev/contextmenu/) | `EDropdown / ETable` | EDropdown 有 contextmenu 触发，ETable 有内部右键菜单；通用下拉按 reference 边缘定位，缺鼠标坐标定位的统一接口。 |
| [Sidebar](https://primevue.dev/sidebar/) | `EAside / EMenu / EDrawer` | 新版 Sidebar 是导航外壳；已有容器、菜单与抽屉，缺统一的 icon/offcanvas 模式、布局变体及响应式状态。 |
| [Gallery](https://primevue.dev/gallery/) | `EImage / EImageViewer` | 已有多图切换、缩放、旋转；缺成套缩略图导航、翻转和下载等能力。 |
| [Chip](https://primevue.dev/chip/) | `ETag` | 已有文本、圆角与关闭交互；缺 Chip 内建图标/头像及相应组合布局。 |

## 全量逐项对照：91 项

### Form / 表单（28 项）

| PrimeVue 官方资料 | EUI 现有入口 | 判定 | 说明 |
| --- | --- | --- | --- |
| [AutoComplete](https://primevue.dev/autocomplete/) | `EAutocomplete` | 核心对应 | 输入建议。 |
| [CascadeSelect](https://primevue.dev/cascadeselect/) | `ECascader` | 核心对应 | 级联选择。 |
| [Checkbox](https://primevue.dev/checkbox/) | `ECheckBox` | 核心对应 | 复选框。 |
| [DatePicker](https://primevue.dev/datepicker/) | `EDatePicker / EDateTimePicker` | 核心对应 | 日期、日期时间选择。 |
| [FloatLabel](https://primevue.dev/floatlabel/) | — | 缺少公共能力 | 缺随焦点或输入内容浮动的字段标签。 |
| [IconField](https://primevue.dev/iconfield/) | `EInput` | 核心对应 | prefixIcon/suffixIcon 及 prefixComponent/suffixComponent 提供输入内图标与自定义内容。 |
| [IftaLabel](https://primevue.dev/iftalabel/) | — | 缺少公共能力 | 缺输入框内部顶端固定标签的通用容器。 |
| [InputColor](https://primevue.dev/inputcolor/) | `EColorPicker` | 核心对应 | 颜色选择。 |
| [InputGroup](https://primevue.dev/inputgroup/) | `EInput` | 部分覆盖 | prependComponent/appendComponent 可组合输入前后内容；没有面向多种表单控件的统一输入组容器。 |
| [InputNumber](https://primevue.dev/inputnumber/) | `ESpinBox` | 核心对应 | 数值录入与步进。 |
| [InputOtp](https://primevue.dev/inputotp/) | `EInputOtp` | 核心对应 | 验证码分格录入。 |
| [InputPassword](https://primevue.dev/inputpassword/) | `EInput` | 部分覆盖 | showPassword/passwordVisible 提供密码与显隐切换；缺内置强度计及密码规则提示。 |
| [InputTags](https://primevue.dev/inputtags/) | `EInputTag` | 核心对应 | 标签输入。 |
| [InputText](https://primevue.dev/inputtext/) | `EInput` | 核心对应 | 单行输入。 |
| [KeyFilter](https://primevue.dev/keyfilter/) | `EInput / Qt TextField.validator` | 核心对应 | Qt 原生校验器可约束输入；不是 PrimeVue 预设模式或键盘事件策略的逐项等价实现。 |
| [Knob](https://primevue.dev/knob/) | — | 缺少公共能力 | 缺可拖动或用键盘调整数值的旋钮；ECircleProgress 是进度显示。 |
| [Label](https://primevue.dev/label/) | `EFormItem / EText` | 部分覆盖 | 已有标签文字、必填标记及布局；缺独立的标签与目标控件关联、点击聚焦能力。 |
| [Listbox](https://primevue.dev/listbox/) | — | 缺少公共能力 | 缺常驻展示、支持单选或多选及筛选的独立选择列表；ESelect 是弹出选择，EList 是列表展示。 |
| [Mask](https://primevue.dev/mask/) | `EInput / Qt TextField.inputMask` | 核心对应 | Qt 原生 inputMask 承接固定格式掩码；PrimeVue 动态掩码、预设及格式语法未作等价核验。 |
| [RadioButton](https://primevue.dev/radiobutton/) | `ERadio` | 核心对应 | 单选框。 |
| [Rating](https://primevue.dev/rating/) | `ERate` | 核心对应 | 评分。 |
| [Select](https://primevue.dev/select/) | `ESelect` | 核心对应 | 下拉选择。 |
| [SelectButton](https://primevue.dev/selectbutton/) | `ESegmented / ERadioButton / ECheckBoxButton` | 核心对应 | 分段选择及单选、多选按钮形态。 |
| [Slider](https://primevue.dev/slider/) | `ESlider / ERangeSlider` | 核心对应 | 单值及范围滑块。 |
| [Textarea](https://primevue.dev/textarea/) | `ETextArea` | 核心对应 | 多行输入。 |
| [ToggleButton](https://primevue.dev/togglebutton/) | `ECheckBoxButton` | 核心对应 | checked 具持久选中背景与文字样式。 |
| [ToggleSwitch](https://primevue.dev/toggleswitch/) | `ESwitch` | 核心对应 | 开关。 |
| [TreeSelect](https://primevue.dev/treeselect/) | `ETreeSelect` | 核心对应 | 树选择。 |

### Button / 按钮（3 项）

| PrimeVue 官方资料 | EUI 现有入口 | 判定 | 说明 |
| --- | --- | --- | --- |
| [Button](https://primevue.dev/button/) | `EButton` | 核心对应 | 按钮。 |
| [SpeedDial](https://primevue.dev/speeddial/) | `EFloatButtonGroup` | 核心对应 | click/hover 展开浮动操作组，支持上下左右；未认定覆盖圆形等全部布局变体。 |
| [SplitButton](https://primevue.dev/splitbutton/) | `EDropdown` | 核心对应 | splitButton 将主操作与右侧下拉按钮分开。 |

### Data / 数据（10 项）

| PrimeVue 官方资料 | EUI 现有入口 | 判定 | 说明 |
| --- | --- | --- | --- |
| [DataTable](https://primevue.dev/datatable/) | `ETable` | 核心对应 | 表格、排序、筛选、分页及选择；本次不作全部高级功能对齐。 |
| [DataView](https://primevue.dev/dataview/) | `EList` | 核心对应 | items、grid、pagination、itemDelegate 提供列表/网格和分页，排序可由输入数据提供。 |
| [OrderList](https://primevue.dev/orderlist/) | — | 缺少公共能力 | 缺带列表项选择和上下重排交互的通用排序列表。 |
| [OrgChart](https://primevue.dev/organizationchart/) | — | 缺少公共能力 | 缺节点与连接线构成的组织结构图；ETree 不等于图形组织布局。 |
| [Paginator](https://primevue.dev/paginator/) | `EPagination` | 核心对应 | 分页。 |
| [PickList](https://primevue.dev/picklist/) | `ETransfer` | 部分覆盖 | 已有双栏筛选和左右搬移；缺列表内上下重排及拖拽搬移交互。 |
| [Timeline](https://primevue.dev/timeline/) | `ETimeline` | 核心对应 | 时间线。 |
| [Tree](https://primevue.dev/tree/) | `ETree` | 核心对应 | 树形展示。 |
| [TreeTable](https://primevue.dev/treetable/) | `ETable` | 核心对应 | treeProps、树节点展开、懒加载与选择联动提供树形表格。 |
| [VirtualScroller](https://primevue.dev/virtualscroller/) | `EListy` | 核心对应 | 虚拟列表；未认定与 PrimeVue 各方向滚动变体逐项等价。 |

### Panel / 面板（11 项）

| PrimeVue 官方资料 | EUI 现有入口 | 判定 | 说明 |
| --- | --- | --- | --- |
| [Accordion](https://primevue.dev/accordion/) | `ECollapse` | 核心对应 | 可折叠内容组。 |
| [Card](https://primevue.dev/card/) | `ECard` | 核心对应 | 卡片容器及头尾自定义内容。 |
| [Deferred](https://primevue.dev/deferredcontent/) | — | 缺少公共能力 | 没有导出的通用视口延迟加载组件；Gallery 内部 ExampleSection 已有相关实现。 |
| [Divider](https://primevue.dev/divider/) | `EDivider` | 核心对应 | 分割线。 |
| [Fieldset](https://primevue.dev/fieldset/) | — | 缺少公共能力 | 缺标题嵌入边框、可折叠的字段分组容器。 |
| [Panel](https://primevue.dev/panel/) | `ECard / ECollapse` | 核心对应 | 静态面板由 ECard 承接，折叠面板由 ECollapse 承接；没有独立 EPanel。 |
| [ScrollArea](https://primevue.dev/scrollarea/) | `EScrollbar` | 核心对应 | EScrollbar 根类型为 T.ScrollView，包含滚动容器和主题滚动条。 |
| [Splitter](https://primevue.dev/splitter/) | `ESplitter / ESplitterPanel` | 核心对应 | 分隔与调整面板尺寸。 |
| [Stepper](https://primevue.dev/stepper/) | `EWizard / ESteps` | 核心对应 | EWizard 管理步骤内容、导航、lazy 与 beforeLeave；ESteps 提供步骤展示。 |
| [Tabs](https://primevue.dev/tabs/) | `ETabs` | 核心对应 | 标签页。 |
| [Toolbar](https://primevue.dev/toolbar/) | `EToolBar` | 核心对应 | 工具栏。 |

### Overlay / 浮层（7 项）

| PrimeVue 官方资料 | EUI 现有入口 | 判定 | 说明 |
| --- | --- | --- | --- |
| [ConfirmDialog](https://primevue.dev/confirmdialog/) | `EMessageBox` | 核心对应 | confirm(message, heading)、confirmed/canceled/completed 及关闭控制提供确认对话框；不是相同的 Vue 服务 API。 |
| [ConfirmPopup](https://primevue.dev/confirmpopup/) | `EPopconfirm` | 核心对应 | 锚定确认弹层。 |
| [Dialog](https://primevue.dev/dialog/) | `EDialog` | 核心对应 | 对话框。 |
| [Drawer](https://primevue.dev/drawer/) | `EDrawer` | 核心对应 | 抽屉。 |
| [DynamicDialog](https://primevue.dev/dynamicdialog/) | `EDialog` | 部分覆盖 | contentComponent、Loader、loadedContent 及 destroyOnClose 支持动态内容；缺统一打开任意组件、传参及返回结果的对话框服务。 |
| [Popover](https://primevue.dev/popover/) | `EPopover` | 核心对应 | 锚定浮层。 |
| [Tooltip](https://primevue.dev/tooltip/) | `EToolTip` | 核心对应 | 提示。 |

### File / 文件（1 项）

| PrimeVue 官方资料 | EUI 现有入口 | 判定 | 说明 |
| --- | --- | --- | --- |
| [FileUpload](https://primevue.dev/fileupload/) | `EUpload` | 核心对应 | 文件上传；本次不作全部协议和服务端行为核验。 |

### Menu / 菜单（9 项）

| PrimeVue 官方资料 | EUI 现有入口 | 判定 | 说明 |
| --- | --- | --- | --- |
| [Breadcrumb](https://primevue.dev/breadcrumb/) | `EBreadcrumb` | 核心对应 | 面包屑。 |
| [CommandMenu](https://primevue.dev/commandmenu/) | `ECommandPalette` | 核心对应 | 可搜索命令面板。 |
| [ContextMenu](https://primevue.dev/contextmenu/) | `EDropdown / ETable` | 部分覆盖 | EDropdown 有 contextmenu 触发，ETable 有内部右键菜单；通用下拉按 reference 边缘定位，缺鼠标坐标定位的统一接口。 |
| [Dock](https://primevue.dev/dock/) | — | 缺少公共能力 | 缺 Dock 形态的图标导航组件。 |
| [MegaMenu](https://primevue.dev/megamenu/) | — | 缺少公共能力 | 缺将多列、分组子菜单同时展开的大型导航菜单。 |
| [Menu](https://primevue.dev/menu/) | `EMenu / EDropdown` | 核心对应 | 已有菜单、弹出菜单与分组/子菜单；复选和单选菜单项等细节未认定完全覆盖。 |
| [Menubar](https://primevue.dev/menubar/) | `EMenuBar` | 核心对应 | 水平菜单栏。 |
| [Sidebar](https://primevue.dev/sidebar/) | `EAside / EMenu / EDrawer` | 部分覆盖 | 新版 Sidebar 是导航外壳；已有容器、菜单与抽屉，缺统一的 icon/offcanvas 模式、布局变体及响应式状态。 |
| [TieredMenu](https://primevue.dev/tieredmenu/) | `EDropdown / EMenuBar` | 核心对应 | 通过私有 MenuBranch 递归 children，提供多级弹出子菜单。 |

### Messages / 消息（2 项）

| PrimeVue 官方资料 | EUI 现有入口 | 判定 | 说明 |
| --- | --- | --- | --- |
| [Message](https://primevue.dev/message/) | `EAlert` | 核心对应 | 页面内消息；EMessage 更接近临时提示，不能仅按名字映射。 |
| [Toast](https://primevue.dev/toast/) | `EMessage / ENotification` | 核心对应 | 临时消息及通知。 |

### Media / 媒体（3 项）

| PrimeVue 官方资料 | EUI 现有入口 | 判定 | 说明 |
| --- | --- | --- | --- |
| [Carousel](https://primevue.dev/carousel/) | `ECarousel` | 核心对应 | 轮播。 |
| [Compare](https://primevue.dev/compare/) | — | 缺少公共能力 | 缺拖动分隔线比较两份图片或内容的组件；ESplitter 调整布局尺寸，不提供叠层揭示对比。 |
| [Gallery](https://primevue.dev/gallery/) | `EImage / EImageViewer` | 部分覆盖 | 已有多图切换、缩放、旋转；缺成套缩略图导航、翻转和下载等能力。 |

### Misc / 其他（17 项）

| PrimeVue 官方资料 | EUI 现有入口 | 判定 | 说明 |
| --- | --- | --- | --- |
| [AnimateOnScroll](https://primevue.dev/animateonscroll/) | — | 缺少公共能力 | 已有 EAnimation 动画执行器，缺视口进入/离开时自动触发动画的公共封装。 |
| [Avatar](https://primevue.dev/avatar/) | `EAvatar / EAvatarGroup` | 核心对应 | 头像及头像组。 |
| [Badge](https://primevue.dev/badge/) | `EBadge` | 核心对应 | 徽标。 |
| [BlockUI](https://primevue.dev/blockui/) | `ELoading` | 核心对应 | target/fullscreen/lock 提供局部/全局遮罩及指针拦截；本次未运行键盘焦点阻断验证。 |
| [Chip](https://primevue.dev/chip/) | `ETag` | 部分覆盖 | 已有文本、圆角与关闭交互；缺 Chip 内建图标/头像及相应组合布局。 |
| [Fluid](https://primevue.dev/fluid/) | `QML anchors / width / Layouts` | 平台差异 | 无同名公共容器；Qt 使用 anchors、宽度绑定和 Layout.fillWidth，不能直接等同于自动向所有后代表单控件传播 fluid 的机制。 |
| [FocusTrap](https://primevue.dev/focustrap/) | — | 缺少公共能力 | 缺可用于任意容器的 Tab/Shift+Tab 焦点圈定公共组件；不据此否定 Qt 弹窗自身的焦点行为。 |
| [Inplace](https://primevue.dev/inplace/) | — | 缺少公共能力 | 缺通用查看/编辑双态内容容器；表格内编辑不能替代任意内容的公共封装。 |
| [MeterGroup](https://primevue.dev/metergroup/) | — | 缺少公共能力 | 缺多段计量条及对应图例；单值 EProgress 不是多段分布显示。 |
| [ProgressBar](https://primevue.dev/progressbar/) | `EProgress` | 核心对应 | 线形进度条。 |
| [ProgressSpinner](https://primevue.dev/progressspinner/) | `ELoading / ECircleProgress` | 核心对应 | 加载动画及确定数值的圆形进度由不同控件承接。 |
| [Ripple](https://primevue.dev/ripple/) | — | 缺少公共能力 | 缺可附加到任意目标的点击水波纹效果。 |
| [ScrollTop](https://primevue.dev/scrolltop/) | `EBacktop` | 核心对应 | 返回顶部。 |
| [Skeleton](https://primevue.dev/skeleton/) | `ESkeleton / ESkeletonItem` | 核心对应 | 骨架屏。 |
| [StyleClass](https://primevue.dev/styleclass/) | `QML states / transitions / EAnimation` | 平台差异 | 操作 DOM/CSS class 是 Web 机制；Qt 由属性绑定、状态、过渡及动画实现相近效果，不建议直接复制 class 操作接口。 |
| [Tag](https://primevue.dev/tag/) | `ETag` | 核心对应 | 标签。 |
| [Terminal](https://primevue.dev/terminal/) | — | 缺少公共能力 | 缺命令输入、历史与响应输出的终端交互组件；ECodeView 是代码展示。 |

## 容易误判的本地证据

- `source/qt5/imports/eui/controls/indicator/EMessageBox.qml` 有 `alert()`、`confirm()`、`prompt()` 以及 `confirmed`、`canceled`、`completed` 信号。因此 [ConfirmDialog](https://primevue.dev/confirmdialog/) 的主要用途已存在，无需因缺同名 EConfirmDialog 而判缺。
- `source/qt5/imports/eui/controls/navigation/EDropdown.qml` 有 `splitButton` 和 `trigger: "contextmenu"`。主操作/下拉操作分开；右键触发后当前仍按 reference 边缘定位。其 `MenuBranch.qml` 调用链递归处理 children，可以承接多级弹出菜单。
- `source/qt5/imports/eui/controls/data/ETable.qml` 有 `treeProps`、`hasTree`、`toggleRowExpansion()`、`updateKeyChildren()` 及懒加载/复选联动。TreeTable 不应按名称计为缺失。
- `source/qt5/imports/eui/controls/navigation/EWizard.qml` 有步骤内容、`next()`、`previous()`、`beforeLeave` 和 lazy Loader，可承接 Stepper 的主要工作流。
- `source/qt5/imports/eui/controls/form/EInput.qml` 根类型为 Qt TextField，显式提供 prefix/suffix 图标、前后附加 Component、密码显隐，并继承 validator/inputMask。本次没有将继承来的输入能力误判为不存在；也没有认定 Qt 原生语法与 PrimeVue 指令一致。
- `source/qt5/imports/eui/controls/form/ECheckBoxButton.qml` 继承 ECheckBox，checked 会改变持久背景和文字样式，可对应 ToggleButton。普通 EButton 的 pressed 反馈不应单独作为持久切换按钮的证据。
- `source/qt5/imports/eui/controls/data/EList.qml` 有 items、grid、分页和响应式列数，足以承接 DataView 的主要用途。它没有通用 Listbox 选择或 OrderList 重排交互，不能因此把这两项也算已具备。
- `source/qt5/imports/eui/controls/data/EImageViewer.qml` 继承 EImage；`EImage.qml` 提供多图列表、切换、缩放和旋转。因此 [Gallery](https://primevue.dev/gallery/) 是部分覆盖；[Compare](https://primevue.dev/compare/) 所需的叠层揭示对比则未找到。
- `source/qt5/imports/eui/controls/indicator/ELoading.qml` 有 target、fullscreen、lock 和遮罩 MouseArea；可承接 BlockUI 的主要遮罩用途，不能因没有 EBlockUI 就算缺失。
- `example/qml/component/ExampleSection.qml` 的 inViewport、activated 与 Loader 实现了示例延迟创建。它属于 Gallery 内部，未导出为通用 DeferredContent。
- `source/utils/EAnimation.h` 及 `source/EUI.cpp` 确认 EAnimation 是公共 singleton，提供 play/stop；它没有提供通用视口滚动触发封装。因此已有动画库与缺 AnimateOnScroll 并不矛盾。
- 新版 [Sidebar](https://primevue.dev/sidebar/) 是带 icon/offcanvas 模式、布局变体及可选遮罩的导航外壳，与 Drawer 独立。单页曾在网页提取工具超时，已通过直接请求官方 HTML 核实；官方 [组件总目录](https://primevue.dev/components/) 也描述了其导航用途。EAside + EMenu + EDrawer 是相关基础能力，未视为整套 Sidebar 已完成。

## Web 平台差异

- [Fluid](https://primevue.dev/fluid/) 向后代表单控件传递填满容器的布局语义。QML 可使用 anchors、width 绑定与 QtQuick.Layouts；当前没有同名传播容器，也不将这视为必须照搬的组件设计。
- [StyleClass](https://primevue.dev/styleclass/) 操作 DOM 元素的 CSS class 及进出场 class。QML 使用属性、states、transitions 和 EAnimation；应对照最终交互需求，不直接移植 CSS class 接口。

## PRO Suite 与 Roadmap

官网将 PRO Suite 与基础目录分开。下列用途另作范围说明，不进入上面的 91 项统计：

| 官网项目 | EUI 当前状态 |
| --- | --- |
| [Charts](https://vue.primeuipro.dev/charts) | 已有 EChart 和 EECharts，具备图表能力；本次未做 PRO 图表功能集的逐项比较。 |
| [Scheduler](https://vue.primeuipro.dev/scheduler) | 未找到成套的事件排程、资源分组及拖动调整排程组件；ECalendar/EDatePicker 不等于完整 Scheduler。 |
| [Task Board](https://vue.primeuipro.dev/taskboard) | 未找到带列/泳道和任务拖动流转的成套看板。 |
| [Text Editor](https://vue.primeuipro.dev/texteditor) | 未找到带工具栏、格式命令等功能的成套富文本编辑器；ETextArea、ECodeView 不据此视为等价。 |

官网侧栏的 DataGrid、Sheet、Gantt Chart、Diagram、PDF Viewer 标注 Roadmap，不作为对方已发布且 EUI 缺失的组件计数。设计模板、主题、图标包、Blocks、服务支持及其他站点附加产品也未计入基础组件对照。

## 验证与限制

- 已核对官网基础目录总数、各分类项数、91 项名称唯一性和分类汇总。
- 已阅读本地公开注册、Gallery 目录及关键边界实现；缺失表示未找到对应公共 API，不表示不能用 Qt/QML自行实现。
- 本节记录实施前的只读审计；当前组件和示例变更以本文开头及功能扩展说明为准。
- 未创建分支、提交或推送。
