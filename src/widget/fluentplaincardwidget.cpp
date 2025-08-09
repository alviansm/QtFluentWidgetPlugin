#include "fluentplaincardwidget.h"
#include <QApplication>
#include <QPalette>
#include <QDebug>

FluentPlainCardWidget::FluentPlainCardWidget(QWidget *parent)
    : QWidget(parent)
    , m_shadowEffect(nullptr)
    , m_hoverAnimation(nullptr)
    , m_enableHover(true)
    , m_enableShadow(true)
    , m_enableRoundness(true)
    , m_darkMode(false)
    , m_backgroundColor(QColor(255, 255, 255)) // Default light background
    , m_borderColor(QColor(229, 229, 229))     // Default light border
    , m_borderRadius(8)
    , m_borderWidth(1)
    , m_contentMargin(16)
    , m_isHovered(false)
    , m_hoverOffset(0.0)
    , m_inDesigner(false)
{
    // Detect if we're in Qt Designer
    m_inDesigner = isInDesigner();

    // Only setup effects if not in Designer mode
    if (!m_inDesigner) {
        setupShadowEffect();
        setupHoverAnimation();
        setAttribute(Qt::WA_Hover, true);
    }

    updateStyles();
    setMinimumSize(100, 80);

    // Make sure the widget accepts drops and can be used as a container
    // But DON'T set any layout - let Designer handle layout management
    setAcceptDrops(true);
}

FluentPlainCardWidget::~FluentPlainCardWidget()
{
    // Cleanup is handled automatically by Qt's parent-child relationship
}

bool FluentPlainCardWidget::isInDesigner() const
{
    // Multiple ways to detect Designer mode for reliability
    if (qApp && qApp->property("inDesigner").toBool()) {
        return true;
    }

    // Check for Designer-specific object names in the parent hierarchy
    QWidget *p = parentWidget();
    while (p) {
        QString objName = p->objectName();
        if (objName.contains("qt_designer") ||
            objName.contains("Designer") ||
            objName.contains("FormWindow")) {
            return true;
        }
        p = p->parentWidget();
    }

    // Check application name
    if (qApp && qApp->applicationName().contains("designer", Qt::CaseInsensitive)) {
        return true;
    }

    return false;
}

void FluentPlainCardWidget::setupShadowEffect()
{
    if (m_inDesigner) {
        return; // Skip shadow effects in Designer
    }

    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(12);
    m_shadowEffect->setXOffset(0);
    m_shadowEffect->setYOffset(2);
    m_shadowEffect->setColor(QColor(0, 0, 0, 25));

    updateShadowEffect();
}

void FluentPlainCardWidget::setupHoverAnimation()
{
    if (m_inDesigner) {
        return; // Skip animations in Designer
    }

    m_hoverAnimation = new QPropertyAnimation(this, "pos", this);
    m_hoverAnimation->setDuration(150);
    m_hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);

    connect(m_hoverAnimation, &QPropertyAnimation::finished,
            this, &FluentPlainCardWidget::onHoverAnimationFinished);
}

void FluentPlainCardWidget::updateStyles()
{
    QString cardStyle;

    // Determine colors based on theme
    QColor bgColor = m_backgroundColor;
    QColor borderColor = m_borderColor;

    if (m_darkMode && m_backgroundColor == QColor(255, 255, 255)) {
        // Use default dark colors if still using light defaults
        bgColor = QColor(45, 45, 45);
        borderColor = QColor(64, 64, 64);
    }

    // Build style string - apply contentMargin as padding
    cardStyle = QString(
        "FluentPlainCardWidget {"
        "    background-color: %1;"
        "    border: %2px solid %3;"
        "    padding: %4px;"
        "%5"
        "}"
    ).arg(bgColor.name())
     .arg(m_borderWidth)
     .arg(borderColor.name())
     .arg(m_contentMargin)
     .arg(m_enableRoundness ? QString("    border-radius: %1px;").arg(m_borderRadius) : "");

    setStyleSheet(cardStyle);
}

void FluentPlainCardWidget::updateShadowEffect()
{
    // Avoid setting graphics effects in Designer mode
    if (m_inDesigner) {
        return;
    }

    if (m_enableShadow && m_shadowEffect) {
        setGraphicsEffect(m_shadowEffect);
    } else {
        setGraphicsEffect(nullptr);
    }
}

void FluentPlainCardWidget::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event)

    if (m_inDesigner) {
        return; // Skip hover effects in Designer
    }

    if (m_enableHover && !m_isHovered && m_hoverAnimation && m_shadowEffect) {
        m_isHovered = true;

        // Enhance shadow on hover
        if (m_enableShadow && m_shadowEffect) {
            m_shadowEffect->setBlurRadius(20);
            m_shadowEffect->setYOffset(6);
            m_shadowEffect->setColor(QColor(0, 0, 0, 40));
        }

        // Animate upward movement
        if (m_hoverAnimation && m_hoverAnimation->state() != QAbstractAnimation::Running) {
            QPoint currentPos = pos();
            QPoint targetPos = currentPos + QPoint(0, -3);

            m_hoverAnimation->setStartValue(currentPos);
            m_hoverAnimation->setEndValue(targetPos);
            m_hoverAnimation->start();
        }
    }
}

void FluentPlainCardWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)

    if (m_inDesigner) {
        return; // Skip hover effects in Designer
    }

    if (m_enableHover && m_isHovered && m_hoverAnimation && m_shadowEffect) {
        m_isHovered = false;

        // Reset shadow
        if (m_enableShadow && m_shadowEffect) {
            m_shadowEffect->setBlurRadius(12);
            m_shadowEffect->setYOffset(2);
            m_shadowEffect->setColor(QColor(0, 0, 0, 25));
        }

        // Animate back to original position
        if (m_hoverAnimation && m_hoverAnimation->state() != QAbstractAnimation::Running) {
            QPoint currentPos = pos();
            QPoint targetPos = currentPos + QPoint(0, 3);

            m_hoverAnimation->setStartValue(currentPos);
            m_hoverAnimation->setEndValue(targetPos);
            m_hoverAnimation->start();
        }
    }
}

void FluentPlainCardWidget::onHoverAnimationFinished()
{
    // Animation completed
}

void FluentPlainCardWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// Property implementations
bool FluentPlainCardWidget::isHoverEnabled() const { return m_enableHover; }
void FluentPlainCardWidget::setHoverEnabled(bool enabled)
{
    m_enableHover = enabled;
    if (!m_inDesigner) {
        setAttribute(Qt::WA_Hover, enabled);
    }
}

bool FluentPlainCardWidget::isShadowEnabled() const { return m_enableShadow; }
void FluentPlainCardWidget::setShadowEnabled(bool enabled)
{
    m_enableShadow = enabled;
    updateShadowEffect();
}

bool FluentPlainCardWidget::isRoundnessEnabled() const { return m_enableRoundness; }
void FluentPlainCardWidget::setRoundnessEnabled(bool enabled)
{
    m_enableRoundness = enabled;
    updateStyles();
}

bool FluentPlainCardWidget::isDarkMode() const { return m_darkMode; }
void FluentPlainCardWidget::setDarkMode(bool dark)
{
    m_darkMode = dark;
    updateStyles();
}

QColor FluentPlainCardWidget::backgroundColor() const { return m_backgroundColor; }
void FluentPlainCardWidget::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    updateStyles();
}

QColor FluentPlainCardWidget::borderColor() const { return m_borderColor; }
void FluentPlainCardWidget::setBorderColor(const QColor &color)
{
    m_borderColor = color;
    updateStyles();
}

int FluentPlainCardWidget::borderRadius() const { return m_borderRadius; }
void FluentPlainCardWidget::setBorderRadius(int radius)
{
    m_borderRadius = qMax(0, radius);
    updateStyles();
}

int FluentPlainCardWidget::borderWidth() const { return m_borderWidth; }
void FluentPlainCardWidget::setBorderWidth(int width)
{
    m_borderWidth = qMax(0, width);
    updateStyles();
}

int FluentPlainCardWidget::contentMargin() const { return m_contentMargin; }
void FluentPlainCardWidget::setContentMargin(int margin)
{
    m_contentMargin = qMax(0, margin);
    updateStyles(); // Update the padding in stylesheet
}

QSize FluentPlainCardWidget::sizeHint() const
{
    return QSize(200, 150);
}

QSize FluentPlainCardWidget::minimumSizeHint() const
{
    return QSize(100, 80);
}