#pragma once

#include <QString>

namespace EPalette
{
namespace Seed
{
inline const QString Info    = "#909399";
inline const QString Danger  = "#f56c6c";
inline const QString Primary = "#409eff";
inline const QString Success = "#67c23a";
inline const QString Warning = "#e6a23c";
}

namespace Neutral
{
inline const QString N0   = "#ffffff";
inline const QString N50  = "#f5f7fa";
inline const QString N100 = "#ebeef5";
inline const QString N200 = "#e4e7ed";
inline const QString N300 = "#dcdfe6";
inline const QString N400 = "#c0c4cc";
inline const QString N500 = "#a8abb2";
inline const QString N600 = "#909399";
inline const QString N700 = "#6c6e72";
inline const QString N800 = "#4c4d4f";
inline const QString N850 = "#414243";
inline const QString N900 = "#303133";
inline const QString N950 = "#141414";
}

namespace Base
{
inline const QString White       = Neutral::N0;
inline const QString Black       = Neutral::N950;
inline const QString Transparent = "#00000000";
}

namespace Theme
{
inline const QString Info    = Seed::Info;
inline const QString Danger  = Seed::Danger;
inline const QString Primary = Seed::Primary;
inline const QString Success = Seed::Success;
inline const QString Warning = Seed::Warning;
}
}
