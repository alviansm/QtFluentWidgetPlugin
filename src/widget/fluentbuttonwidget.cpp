#include "fluentbuttonwidget.h"
#include <QApplication>
#include <QPalette>
#include <QFontMetrics>

FluentButtonWidget::FluentButtonWidget(QWidget *parent)
    : QPushButton(parent)
    , m_buttonStyle(Primary)
    , m_buttonSize(Medium)
    , m_darkMode(false)
    , m_accentColor(QColor(0, 120, 215)) // Default blue
    , m_iconOnly(false)
    , m_iconResource("")              // Fixed: Now properly declared in header
    , m_fontIconCharacter("")
    , m_fontIconFont("Segoe UI", 12)
    , m_useFontIcon(false)
    , m_currentState(Rest)
    , m_isPressed(false)
    , m_isHovered(false)
    , m_hoverOpacity(1.0)
    , m_pressScale(1.0)
{
    setText("Button");
    setupAnimations();
    updateSizes();
    updateStyles();

    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_Hover, true);
}

FluentButtonWidget::FluentButtonWidget(const QString &text, QWidget *parent)
    : FluentButtonWidget(parent)
{
    setText(text);
}

void FluentButtonWidget::setupAnimations()
{
    // Hover animation
    m_hoverAnimation = new QPropertyAnimation(this, "geometry", this);
    m_hoverAnimation->setDuration(150);
    m_hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);

    // Press animation
    m_pressAnimation = new QPropertyAnimation(this, "geometry", this);
    m_pressAnimation->setDuration(100);
    m_pressAnimation->setEasingCurve(QEasingCurve::OutCubic);

    // Press timer for visual feedback
    m_pressTimer = new QTimer(this);
    m_pressTimer->setSingleShot(true);
    m_pressTimer->setInterval(100);

    connect(m_hoverAnimation, &QPropertyAnimation::finished,
            this, &FluentButtonWidget::onAnimationFinished);
    connect(m_pressAnimation, &QPropertyAnimation::finished,
            this, &FluentButtonWidget::onPressAnimationFinished);
    connect(m_pressTimer, &QTimer::timeout,
            this, &FluentButtonWidget::updateButtonState);
}

void FluentButtonWidget::updateStyles()
{
    QString styleSheet = getStyleSheet();
    setStyleSheet(styleSheet);

    // Update icon display
    updateIconDisplay();

    update();
}

void FluentButtonWidget::updateSizes()
{
    QSize buttonSize = getButtonSize();
    setFixedSize(buttonSize);

    // Update font size based on button size
    QFont currentFont = font();
    switch (m_buttonSize) {
    case Small:
        currentFont.setPointSize(12);
        break;
    case Medium:
        currentFont.setPointSize(14);
        break;
    case Large:
        currentFont.setPointSize(16);
        break;
    }
    setFont(currentFont);
}

void FluentButtonWidget::updateIconDisplay()
{
    if (m_iconOnly) {
        // For icon-only buttons, display just the icon
        if (!m_icon.isNull()) {
            QPushButton::setIcon(QIcon(m_icon));
            setText("");
        }
    } else {
        // For text buttons with icons, show both
        if (!m_icon.isNull()) {
            QPushButton::setIcon(QIcon(m_icon));
        } else {
            QPushButton::setIcon(QIcon()); // Clear icon
        }
    }
}

QString FluentButtonWidget::getStyleSheet() const
{
    QString baseStyle;
    QString hoverStyle;
    QString pressedStyle;
    QString disabledStyle;
    QString focusStyle;

    // Color values based on theme
    QString bgColor, textColor, borderColor;
    QString hoverBgColor, hoverTextColor, hoverBorderColor;
    QString pressedBgColor, pressedTextColor, pressedBorderColor;
    QString disabledBgColor, disabledTextColor, disabledBorderColor;
    QString focusBorderColor;

    if (m_darkMode) {
        // Dark theme colors
        switch (m_buttonStyle) {
        case Primary:
            bgColor = m_accentColor.name();
            textColor = "#ffffff";
            borderColor = m_accentColor.name();
            hoverBgColor = m_accentColor.lighter(110).name();
            hoverTextColor = "#ffffff";
            hoverBorderColor = hoverBgColor;
            pressedBgColor = m_accentColor.darker(110).name();
            pressedTextColor = "#ffffff";
            pressedBorderColor = pressedBgColor;
            break;
        case Secondary:
            bgColor = "#404040";
            textColor = "#ffffff";
            borderColor = "#404040";
            hoverBgColor = "#4a4a4a";
            hoverTextColor = "#ffffff";
            hoverBorderColor = "#4a4a4a";
            pressedBgColor = "#363636";
            pressedTextColor = "#ffffff";
            pressedBorderColor = "#363636";
            break;
        case Outline:
            bgColor = "transparent";
            textColor = "#ffffff";
            borderColor = "#6d6d6d";
            hoverBgColor = "#404040";
            hoverTextColor = "#ffffff";
            hoverBorderColor = "#6d6d6d";
            pressedBgColor = "#363636";
            pressedTextColor = "#ffffff";
            pressedBorderColor = "#6d6d6d";
            break;
        case Subtle:
            bgColor = "transparent";
            textColor = "#ffffff";
            borderColor = "transparent";
            hoverBgColor = "#404040";
            hoverTextColor = "#ffffff";
            hoverBorderColor = "transparent";
            pressedBgColor = "#363636";
            pressedTextColor = "#ffffff";
            pressedBorderColor = "transparent";
            break;
        case Transparent:
            bgColor = "transparent";
            textColor = m_accentColor.name();
            borderColor = "transparent";
            hoverBgColor = "rgba(255, 255, 255, 0.05)";
            hoverTextColor = m_accentColor.lighter(110).name();
            hoverBorderColor = "transparent";
            pressedBgColor = "rgba(255, 255, 255, 0.03)";
            pressedTextColor = m_accentColor.darker(110).name();
            pressedBorderColor = "transparent";
            break;
        }
        focusBorderColor = m_accentColor.lighter(120).name();
        disabledBgColor = "#2d2d2d";
        disabledTextColor = "#6d6d6d";
        disabledBorderColor = "#2d2d2d";
    } else {
        // Light theme colors
        switch (m_buttonStyle) {
        case Primary:
            bgColor = m_accentColor.name();
            textColor = "#ffffff";
            borderColor = m_accentColor.name();
            hoverBgColor = m_accentColor.darker(110).name();
            hoverTextColor = "#ffffff";
            hoverBorderColor = hoverBgColor;
            pressedBgColor = m_accentColor.darker(120).name();
            pressedTextColor = "#ffffff";
            pressedBorderColor = pressedBgColor;
            break;
        case Secondary:
            bgColor = "#f3f2f1";
            textColor = "#323130";
            borderColor = "#f3f2f1";
            hoverBgColor = "#edebe9";
            hoverTextColor = "#323130";
            hoverBorderColor = "#edebe9";
            pressedBgColor = "#e1dfdd";
            pressedTextColor = "#323130";
            pressedBorderColor = "#e1dfdd";
            break;
        case Outline:
            bgColor = "transparent";
            textColor = "#323130";
            borderColor = "#8a8886";
            hoverBgColor = "#f3f2f1";
            hoverTextColor = "#323130";
            hoverBorderColor = "#8a8886";
            pressedBgColor = "#edebe9";
            pressedTextColor = "#323130";
            pressedBorderColor = "#8a8886";
            break;
        case Subtle:
            bgColor = "transparent";
            textColor = "#323130";
            borderColor = "transparent";
            hoverBgColor = "#f3f2f1";
            hoverTextColor = "#323130";
            hoverBorderColor = "transparent";
            pressedBgColor = "#edebe9";
            pressedTextColor = "#323130";
            pressedBorderColor = "transparent";
            break;
        case Transparent:
            bgColor = "transparent";
            textColor = m_accentColor.name();
            borderColor = "transparent";
            hoverBgColor = "rgba(0, 0, 0, 0.03)";
            hoverTextColor = m_accentColor.darker(110).name();
            hoverBorderColor = "transparent";
            pressedBgColor = "rgba(0, 0, 0, 0.05)";
            pressedTextColor = m_accentColor.darker(120).name();
            pressedBorderColor = "transparent";
            break;
        }
        focusBorderColor = m_accentColor.name();
        disabledBgColor = "#f3f2f1";
        disabledTextColor = "#a19f9d";
        disabledBorderColor = "#f3f2f1";
    }

    // Border radius based on size
    QString borderRadius;
    switch (m_buttonSize) {
    case Small:
        borderRadius = "4px";
        break;
    case Medium:
        borderRadius = "4px";
        break;
    case Large:
        borderRadius = "6px";
        break;
    }

    baseStyle = QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: %2;"
        "    border: 1px solid %3;"
        "    border-radius: %4;"
        "    padding: 0px;"
        "    font-weight: 500;"
        "    text-align: center;"
        "}"
    ).arg(bgColor, textColor, borderColor, borderRadius);

    hoverStyle = QString(
        "QPushButton:hover {"
        "    background-color: %1;"
        "    color: %2;"
        "    border-color: %3;"
        "}"
    ).arg(hoverBgColor, hoverTextColor, hoverBorderColor);

    pressedStyle = QString(
        "QPushButton:pressed {"
        "    background-color: %1;"
        "    color: %2;"
        "    border-color: %3;"
        "}"
    ).arg(pressedBgColor, pressedTextColor, pressedBorderColor);

    disabledStyle = QString(
        "QPushButton:disabled {"
        "    background-color: %1;"
        "    color: %2;"
        "    border-color: %3;"
        "}"
    ).arg(disabledBgColor, disabledTextColor, disabledBorderColor);

    focusStyle = QString(
        "QPushButton:focus {"
        "    outline: 2px solid %1;"
        "    outline-offset: 2px;"
        "}"
    ).arg(focusBorderColor);

    return baseStyle + hoverStyle + pressedStyle + disabledStyle + focusStyle;
}

QSize FluentButtonWidget::getButtonSize() const
{
    if (m_iconOnly) {
        switch (m_buttonSize) {
        case Small:
            return QSize(24, 24);
        case Medium:
            return QSize(32, 32);
        case Large:
            return QSize(40, 40);
        }
    } else {
        QFontMetrics fm(font());
        QString buttonText = text();
        int textWidth = fm.horizontalAdvance(buttonText);
        int padding = 0;
        int height = 0;

        switch (m_buttonSize) {
        case Small:
            padding = 16;
            height = 24;
            break;
        case Medium:
            padding = 20;
            height = 32;
            break;
        case Large:
            padding = 24;
            height = 40;
            break;
        }

        int width = qMax(textWidth + padding, height * 2);
        return QSize(width, height);
    }
    return QSize(100, 32);
}

void FluentButtonWidget::updateButtonState()
{
    ButtonState newState = Rest;

    if (!isEnabled()) {
        newState = Disabled;
    } else if (m_isPressed) {
        newState = Pressed;
    } else if (m_isHovered) {
        newState = Hover;
    } else if (hasFocus()) {
        newState = Focused;
    }

    if (newState != m_currentState) {
        m_currentState = newState;
        animateToState(newState);
    }
}

void FluentButtonWidget::animateToState(ButtonState state)
{
    // Simple state change without complex animations for now
    // Can be enhanced with more sophisticated animations later
    updateStyles();
}

// Event handlers
void FluentButtonWidget::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event)
    m_isHovered = true;
    updateButtonState();
    QPushButton::enterEvent(event);
}

void FluentButtonWidget::leaveEvent(QEvent *event)
{
    m_isHovered = false;
    updateButtonState();
    QPushButton::leaveEvent(event);
}

void FluentButtonWidget::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    updateButtonState();
    QPushButton::mousePressEvent(event);
}

void FluentButtonWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    updateButtonState();
    QPushButton::mouseReleaseEvent(event);
}

void FluentButtonWidget::focusInEvent(QFocusEvent *event)
{
    updateButtonState();
    QPushButton::focusInEvent(event);
}

void FluentButtonWidget::focusOutEvent(QFocusEvent *event)
{
    updateButtonState();
    QPushButton::focusOutEvent(event);
}

void FluentButtonWidget::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
}

void FluentButtonWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::EnabledChange) {
        updateButtonState();
    }
    QPushButton::changeEvent(event);
}

void FluentButtonWidget::onAnimationFinished()
{
    // Animation completed
}

void FluentButtonWidget::onPressAnimationFinished()
{
    // Press animation completed
}

// Property implementations
FluentButtonWidget::ButtonStyle FluentButtonWidget::buttonStyle() const
{
    return m_buttonStyle;
}

void FluentButtonWidget::setButtonStyle(ButtonStyle style)
{
    if (m_buttonStyle != style) {
        m_buttonStyle = style;
        updateStyles();
    }
}

FluentButtonWidget::ButtonSize FluentButtonWidget::buttonSize() const
{
    return m_buttonSize;
}

void FluentButtonWidget::setButtonSize(ButtonSize size)
{
    if (m_buttonSize != size) {
        m_buttonSize = size;
        updateSizes();
        updateStyles();
    }
}

bool FluentButtonWidget::isDarkMode() const
{
    return m_darkMode;
}

void FluentButtonWidget::setDarkMode(bool dark)
{
    if (m_darkMode != dark) {
        m_darkMode = dark;
        updateStyles();
    }
}

QColor FluentButtonWidget::accentColor() const
{
    return m_accentColor;
}

void FluentButtonWidget::setAccentColor(const QColor &color)
{
    if (m_accentColor != color) {
        m_accentColor = color;
        updateStyles();
    }
}

bool FluentButtonWidget::isIconOnly() const
{
    return m_iconOnly;
}

void FluentButtonWidget::setIconOnly(bool iconOnly)
{
    if (m_iconOnly != iconOnly) {
        m_iconOnly = iconOnly;
        updateSizes();
        updateStyles();
    }
}

QString FluentButtonWidget::iconResource() const
{
    return m_iconResource;
}

void FluentButtonWidget::setIconResource(const QString &resourcePath)
{
    if (m_iconResource != resourcePath) {
        m_iconResource = resourcePath;
        if (!resourcePath.isEmpty()) {
            QPixmap pixmap(resourcePath);
            if (!pixmap.isNull()) {
                m_icon = pixmap;
                updateIconDisplay();
            }
        } else {
            m_icon = QPixmap();
            updateIconDisplay();
        }
    }
}

void FluentButtonWidget::setIcon(const QPixmap &pixmap)
{
    m_icon = pixmap;
    m_iconResource = ""; // Clear resource path when setting pixmap directly
    updateIconDisplay();
}

void FluentButtonWidget::setIcon(const QString &resourcePath)
{
    setIconResource(resourcePath);
}

QSize FluentButtonWidget::sizeHint() const
{
    return getButtonSize();
}

QSize FluentButtonWidget::minimumSizeHint() const
{
    if (m_iconOnly) {
        return QSize(24, 24);
    }
    return QSize(60, 24);
}
