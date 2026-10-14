#include <cmath>
#include <limits>
#include <QPointer>
#include <QMatrix4x4>
#include <QQuickItem>
#include <QEasingCurve>
#include <QVariantAnimation>

#include "EAnimation.h"

namespace
{
enum class Phase
{
    In,
    Out,
    Attention
};

enum class Motion
{
    Fade,
    Flip,
    Roll,
    Spin,
    Tada,
    Zoom,
    Flash,
    Jello,
    Pulse,
    Scale,
    Shake,
    Slide,
    Swing,
    Bounce,
    Rotate,
    Wobble,
    HeadShake,
    HeartBeat,
    LightSpeed,
    RubberBand
};

struct Effect
{
    const char *name;
    Motion motion;
    Phase phase;
    int x = 0;
    int y = 0;
};

// 前八项保留最初公开列表的顺序，其余按效果系列排列。
const Effect effects[] = {{"fadeIn", Motion::Fade, Phase::In},
                          {"fadeOut", Motion::Fade, Phase::Out},
                          {"slideInUp", Motion::Slide, Phase::In, 0, 1},
                          {"slideInLeft", Motion::Slide, Phase::In, -1},
                          {"scaleIn", Motion::Scale, Phase::In},
                          {"scaleOut", Motion::Scale, Phase::Out},
                          {"shake", Motion::Shake, Phase::Attention, 1},
                          {"pulse", Motion::Pulse, Phase::Attention},
                          {"fadeInUp", Motion::Fade, Phase::In, 0, 1},
                          {"fadeInDown", Motion::Fade, Phase::In, 0, -1},
                          {"fadeInLeft", Motion::Fade, Phase::In, -1},
                          {"fadeInRight", Motion::Fade, Phase::In, 1},
                          {"fadeOutUp", Motion::Fade, Phase::Out, 0, -1},
                          {"fadeOutDown", Motion::Fade, Phase::Out, 0, 1},
                          {"fadeOutLeft", Motion::Fade, Phase::Out, -1},
                          {"fadeOutRight", Motion::Fade, Phase::Out, 1},
                          {"fadeInTopLeft", Motion::Fade, Phase::In, -1, -1},
                          {"fadeInTopRight", Motion::Fade, Phase::In, 1, -1},
                          {"fadeInBottomLeft", Motion::Fade, Phase::In, -1, 1},
                          {"fadeInBottomRight", Motion::Fade, Phase::In, 1, 1},
                          {"fadeOutTopLeft", Motion::Fade, Phase::Out, -1, -1},
                          {"fadeOutTopRight", Motion::Fade, Phase::Out, 1, -1},
                          {"fadeOutBottomLeft", Motion::Fade, Phase::Out, -1, 1},
                          {"fadeOutBottomRight", Motion::Fade, Phase::Out, 1, 1},
                          {"slideInDown", Motion::Slide, Phase::In, 0, -1},
                          {"slideInRight", Motion::Slide, Phase::In, 1},
                          {"slideOutUp", Motion::Slide, Phase::Out, 0, -1},
                          {"slideOutDown", Motion::Slide, Phase::Out, 0, 1},
                          {"slideOutLeft", Motion::Slide, Phase::Out, -1},
                          {"slideOutRight", Motion::Slide, Phase::Out, 1},
                          {"zoomIn", Motion::Zoom, Phase::In},
                          {"zoomOut", Motion::Zoom, Phase::Out},
                          {"zoomInUp", Motion::Zoom, Phase::In, 0, 1},
                          {"zoomInDown", Motion::Zoom, Phase::In, 0, -1},
                          {"zoomInLeft", Motion::Zoom, Phase::In, -1},
                          {"zoomInRight", Motion::Zoom, Phase::In, 1},
                          {"zoomOutUp", Motion::Zoom, Phase::Out, 0, -1},
                          {"zoomOutDown", Motion::Zoom, Phase::Out, 0, 1},
                          {"zoomOutLeft", Motion::Zoom, Phase::Out, -1},
                          {"zoomOutRight", Motion::Zoom, Phase::Out, 1},
                          {"rotateIn", Motion::Rotate, Phase::In},
                          {"rotateOut", Motion::Rotate, Phase::Out},
                          {"rotateInUpLeft", Motion::Rotate, Phase::In, -1, -1},
                          {"rotateInUpRight", Motion::Rotate, Phase::In, 1, -1},
                          {"rotateInDownLeft", Motion::Rotate, Phase::In, -1, 1},
                          {"rotateInDownRight", Motion::Rotate, Phase::In, 1, 1},
                          {"rotateOutUpLeft", Motion::Rotate, Phase::Out, -1, -1},
                          {"rotateOutUpRight", Motion::Rotate, Phase::Out, 1, -1},
                          {"rotateOutDownLeft", Motion::Rotate, Phase::Out, -1, 1},
                          {"rotateOutDownRight", Motion::Rotate, Phase::Out, 1, 1},
                          {"bounce", Motion::Bounce, Phase::Attention},
                          {"bounceIn", Motion::Bounce, Phase::In},
                          {"bounceOut", Motion::Bounce, Phase::Out},
                          {"bounceInUp", Motion::Bounce, Phase::In, 0, 1},
                          {"bounceInDown", Motion::Bounce, Phase::In, 0, -1},
                          {"bounceInLeft", Motion::Bounce, Phase::In, -1},
                          {"bounceInRight", Motion::Bounce, Phase::In, 1},
                          {"bounceOutUp", Motion::Bounce, Phase::Out, 0, -1},
                          {"bounceOutDown", Motion::Bounce, Phase::Out, 0, 1},
                          {"bounceOutLeft", Motion::Bounce, Phase::Out, -1},
                          {"bounceOutRight", Motion::Bounce, Phase::Out, 1},
                          {"flip", Motion::Flip, Phase::Attention, 0, 1},
                          {"flipInX", Motion::Flip, Phase::In, 1},
                          {"flipInY", Motion::Flip, Phase::In, 0, 1},
                          {"flipOutX", Motion::Flip, Phase::Out, 1},
                          {"flipOutY", Motion::Flip, Phase::Out, 0, 1},
                          {"shakeX", Motion::Shake, Phase::Attention, 1},
                          {"shakeY", Motion::Shake, Phase::Attention, 0, 1},
                          {"swing", Motion::Swing, Phase::Attention},
                          {"tada", Motion::Tada, Phase::Attention},
                          {"wobble", Motion::Wobble, Phase::Attention},
                          {"jello", Motion::Jello, Phase::Attention},
                          {"rubberBand", Motion::RubberBand, Phase::Attention},
                          {"flash", Motion::Flash, Phase::Attention},
                          {"heartBeat", Motion::HeartBeat, Phase::Attention},
                          {"headShake", Motion::HeadShake, Phase::Attention},
                          {"spin", Motion::Spin, Phase::Attention},
                          {"rollIn", Motion::Roll, Phase::In, -1},
                          {"rollOut", Motion::Roll, Phase::Out, 1},
                          {"lightSpeedInLeft", Motion::LightSpeed, Phase::In, -1},
                          {"lightSpeedInRight", Motion::LightSpeed, Phase::In, 1},
                          {"lightSpeedOutLeft", Motion::LightSpeed, Phase::Out, -1},
                          {"lightSpeedOutRight", Motion::LightSpeed, Phase::Out, 1}};

const Effect *findEffect(const QString &name)
{
    for (const auto &effect : effects)
    {
        if (name == QLatin1String(effect.name))
            return &effect;
    }
    return nullptr;
}

qreal bounceScale(qreal progress)
{
    if (progress < 0.55)
        return 0.3 + 0.82 * progress / 0.55;
    if (progress < 0.75)
        return 1.12 - 0.2 * (progress - 0.55) / 0.2;
    if (progress < 0.9)
        return 0.92 + 0.12 * (progress - 0.75) / 0.15;
    return 1.04 - 0.04 * (progress - 0.9) / 0.1;
}

qreal bounceOffset(qreal progress)
{
    if (progress < 0.55)
        return 1 - 1.16 * progress / 0.55;
    if (progress < 0.75)
        return -0.16 + 0.24 * (progress - 0.55) / 0.2;
    return 0.08 * (1 - progress) / 0.25;
}

bool readNumber(const QVariantMap &options, const QString &name, qreal fallback, qreal maximum, qreal &result)
{
    result = fallback;
    if (!options.contains(name))
        return true;
    const QVariant value = options.value(name);
    switch (value.userType())
    {
    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::Float:
    case QMetaType::Double:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        result = value.toDouble();
        return std::isfinite(result) && result >= 0 && result <= maximum;
    default:
        return false;
    }
}
}

class EAnimation::Controller : public QQuickTransform
{
public:
    Controller(QQuickItem *target, QObject *parent) : QQuickTransform(parent), m_target(target)
    {
        appendToItem(target);
        m_animation.setEndValue(1.0);
        m_animation.setStartValue(0.0);
        connect(&m_animation,
                &QVariantAnimation::valueChanged,
                this,
                [this](const QVariant &value) { apply(value.toReal()); });
        connect(&m_animation,
                &QVariantAnimation::finished,
                this,
                [this]
                {
                    apply(1);
                    m_transform.setToIdentity();
                    // 退出后保留原透明度，供再次进入或 stop() 恢复。
                    m_changesOpacity = m_effect->phase == Phase::Out;
                    update();
                });
        connect(target,
                &QQuickItem::opacityChanged,
                this,
                [this]
                {
                    if (m_target && !m_writingOpacity)
                        m_opacity = m_target->opacity();
                });
    }

    ~Controller() override { stop(); }

    void stop()
    {
        m_animation.stop();
        m_transform.setToIdentity();
        if (m_target && m_changesOpacity)
            writeOpacity(m_opacity);
        m_changesOpacity = false;
        update();
    }

    void play(const Effect *effect, int duration, qreal distance, QEasingCurve::Type easing)
    {
        stop();
        if (!m_target)
            return;
        m_effect         = effect;
        m_distance       = distance;
        m_opacity        = m_target->opacity();
        m_changesOpacity = effect->phase != Phase::Attention || effect->motion == Motion::Flash;
        m_animation.setDuration(duration);
        m_animation.setEasingCurve(easing);
        apply(0);
        if (m_target)
            m_animation.start();
    }

    void applyTo(QMatrix4x4 *matrix) const override { *matrix *= m_transform; }

private:
    void writeOpacity(qreal opacity)
    {
        // 原生 setter 保留 QML 绑定，并忽略动画自身引发的透明度变化。
        m_writingOpacity = true;
        m_target->setOpacity(opacity);
        m_writingOpacity = false;
    }

    void apply(qreal progress)
    {
        if (!m_target)
            return;
        const qreal pi        = 3.141592653589793;
        const qreal alpha     = qBound(qreal(0), progress, qreal(1));
        const bool exiting    = m_effect->phase == Phase::Out;
        const qreal remaining = exiting ? progress : 1 - progress;
        const QPointF direction(m_effect->x, m_effect->y);
        QPointF offset;
        QPointF origin(m_target->width() / 2, m_target->height() / 2);
        qreal skew    = 0;
        qreal angle   = 0;
        qreal scaleX  = 1;
        qreal scaleY  = 1;
        qreal opacity = exiting ? 1 - alpha : alpha;
        switch (m_effect->motion)
        {
        case Motion::Fade:
            offset = direction * m_distance * remaining * remaining;
            break;
        case Motion::Slide:
            offset = direction * m_distance * remaining;
            break;
        case Motion::Scale:
            scaleX = scaleY = 1 - 0.15 * remaining;
            break;
        case Motion::Zoom:
            scaleX = scaleY = 1 - 0.7 * remaining;
            offset          = direction * m_distance * remaining * remaining;
            break;
        case Motion::Rotate:
            angle = (direction.isNull() ? (exiting ? 90 : -90) : 90 * m_effect->x * m_effect->y) * remaining;
            if (!direction.isNull())
                origin = QPointF(m_effect->x < 0 ? 0 : m_target->width(), m_effect->y < 0 ? 0 : m_target->height());
            break;
        case Motion::Bounce:
            if (m_effect->phase == Phase::Attention)
                offset.setY(-m_distance * std::abs(std::sin(3 * pi * alpha)) * (1 - alpha));
            else if (direction.isNull())
                scaleX = scaleY = bounceScale(exiting ? 1 - alpha : alpha);
            else
                offset = direction * m_distance * bounceOffset(exiting ? 1 - alpha : alpha);
            break;
        case Motion::Flip:
            angle = m_effect->phase == Phase::Attention ? 360 * progress : 90 * remaining;
            break;
        case Motion::Shake:
            offset = direction * m_distance * std::sin(6 * pi * alpha) * (1 - alpha);
            break;
        case Motion::Pulse:
            scaleX = scaleY = 1 + 0.08 * std::sin(pi * alpha);
            break;
        case Motion::Swing:
            origin.setY(0);
            angle = 18 * std::sin(3 * pi * alpha) * (1 - alpha);
            break;
        case Motion::Tada:
            scaleX = scaleY = 1 + 0.1 * std::sin(pi * alpha);
            angle           = 8 * std::sin(10 * pi * alpha) * std::sin(pi * alpha);
            break;
        case Motion::Wobble:
            angle = 10 * std::sin(5 * pi * alpha) * (1 - alpha);
            offset.setX(m_distance * std::sin(5 * pi * alpha) * (1 - alpha));
            break;
        case Motion::Jello:
            skew = 12 * std::sin(6 * pi * alpha) * (1 - alpha);
            break;
        case Motion::RubberBand:
            scaleX = 1 + 0.3 * std::sin(4 * pi * alpha) * (1 - alpha);
            scaleY = 1 / scaleX;
            break;
        case Motion::Flash:
            opacity = 0.25 + 0.75 * std::abs(std::cos(3 * pi * alpha));
            break;
        case Motion::HeartBeat:
            scaleX = scaleY = 1 + 0.16 * std::pow(std::sin(2 * pi * alpha), 2) * (1 - alpha);
            break;
        case Motion::HeadShake:
            angle = 8 * std::sin(4 * pi * alpha) * (1 - alpha);
            offset.setX(m_distance * std::sin(4 * pi * alpha) * (1 - alpha));
            break;
        case Motion::Spin:
            angle = 360 * progress;
            break;
        case Motion::Roll:
            angle  = 120 * m_effect->x * remaining;
            offset = direction * m_distance * remaining;
            break;
        case Motion::LightSpeed:
            skew   = 20 * m_effect->x * remaining;
            offset = direction * m_distance * remaining;
            break;
        }
        m_transform.setToIdentity();
        m_transform.translate(offset.x(), offset.y());
        m_transform.translate(origin.x(), origin.y());
        if (m_effect->motion == Motion::Flip)
            m_transform.rotate(angle, m_effect->x, m_effect->y, 0);
        else
            m_transform.rotate(angle, 0, 0, 1);
        m_transform.scale(scaleX, scaleY);
        QMatrix4x4 shear;
        shear(0, 1) = std::tan(skew * pi / 180);
        if (m_effect->motion == Motion::Jello)
            shear(1, 0) = -shear(0, 1) / 2;
        m_transform *= shear;
        m_transform.translate(-origin.x(), -origin.y());
        if (m_changesOpacity)
            writeOpacity((m_opacity > 0 || exiting || m_effect->phase == Phase::Attention ? m_opacity : 1) * opacity);
        update();
    }

    qreal m_opacity  = 1;
    qreal m_distance = 24;
    QMatrix4x4 m_transform;
    QVariantAnimation m_animation;
    bool m_changesOpacity = false;
    QPointer<QQuickItem> m_target;
    bool m_writingOpacity  = false;
    const Effect *m_effect = nullptr;
};

EAnimation::EAnimation(QObject *parent) : QObject(parent)
{
}

EAnimation::~EAnimation()
{
    const auto controllers = m_controllers;
    m_controllers.clear();
    qDeleteAll(controllers);
}

QStringList EAnimation::effects() const
{
    QStringList names;
    for (const auto &effect : ::effects)
        names.append(QLatin1String(effect.name));
    return names;
}

bool EAnimation::stop(QQuickItem *target)
{
    auto *controller = m_controllers.value(target);
    if (!controller)
        return false;
    controller->stop();
    return true;
}

bool EAnimation::play(QQuickItem *target, const QString &effect, const QVariantMap &options)
{
    const auto *preset = findEffect(effect);
    if (!target || !preset || target->thread() != thread())
        return false;
    for (auto it = options.cbegin(); it != options.cend(); ++it)
    {
        if (it.key() != QStringLiteral("duration") && it.key() != QStringLiteral("distance") &&
            it.key() != QStringLiteral("easing"))
            return false;
    }
    qreal easing;
    qreal distance;
    qreal duration;
    const auto defaultEasing = preset->phase == Phase::Attention || preset->motion == Motion::Bounce
                                   ? QEasingCurve::Linear
                                   : QEasingCurve::OutCubic;
    if (!readNumber(options, QStringLiteral("duration"), 300, std::numeric_limits<int>::max(), duration) ||
        !readNumber(options, QStringLiteral("distance"), 24, 1000000, distance) ||
        !readNumber(options, QStringLiteral("easing"), defaultEasing, QEasingCurve::OutInBounce, easing) ||
        std::floor(duration) != duration || std::floor(easing) != easing)
        return false;
    auto *controller = m_controllers.value(target);
    if (!controller)
    {
        controller = new Controller(target, this);
        m_controllers.insert(target, controller);
        connect(target,
                &QObject::destroyed,
                this,
                [this, target, controller]
                {
                    m_controllers.remove(target);
                    controller->deleteLater();
                });
    }
    controller->play(preset, int(duration), distance, QEasingCurve::Type(int(easing)));
    return true;
}
