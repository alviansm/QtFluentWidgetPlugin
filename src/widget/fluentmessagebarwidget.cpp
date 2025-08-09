#include "fluentmessagebarwidget.h"
#include <QApplication>
#include <QPalette>

FluentMessageBarWidget::FluentMessageBarWidget(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_contentLayout(nullptr)
    , m_buttonLayout(nullptr)
    , m_iconLabel(nullptr)
    , m_titleLabel(nullptr)
    , m_messageLabel(nullptr)
    , m_actionButton(nullptr)
    , m_secondaryActionButton(nullptr)
    , m_dismissButton(nullptr)
    , m_opacityEffect(nullptr)
    , m_showAnimation(nullptr)
    , m_hideAnimation(nullptr)
    , m_autoHideTimer(nullptr)
    , m_title("Descriptive title")
    , m_messageText("Message providing information to the user with actionable insights.")
    , m_messageType(Info)
    , m_dismissible(true)
    , m_showActionButtons(true)
    , m_actionText("Action")
    , m_secondaryActionText("Action")
    , m_darkMode(false)
    , m_autoHideDelay(0)
    , m_isVisible(false)
{
    setupUI();
    updateStyles();
    updateIcon();
    updateActionButtons();

    setFixedHeight(68);
    setMinimumWidth(400);
}

void FluentMessageBarWidget::setupUI()
{
    // Create main horizontal layout
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(16, 12, 16, 12);
    m_mainLayout->setSpacing(12);

    // Icon label
    m_iconLabel = new QLabel();
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->setFixedSize(20, 20);
    m_iconLabel->setScaledContents(true);

    // Content layout (vertical)
    m_contentLayout = new QVBoxLayout();
    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(4);

    // Title label
    m_titleLabel = new QLabel(m_title);
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Message label
    m_messageLabel = new QLabel(m_messageText);
    m_messageLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_messageLabel->setWordWrap(true);

    m_contentLayout->addWidget(m_titleLabel);
    m_contentLayout->addWidget(m_messageLabel);

    // Button layout (horizontal)
    m_buttonLayout = new QHBoxLayout();
    m_buttonLayout->setContentsMargins(0, 0, 0, 0);
    m_buttonLayout->setSpacing(8);

    // Action buttons
    m_actionButton = new QPushButton(m_actionText);
    m_actionButton->setFlat(true);
    m_actionButton->setCursor(Qt::PointingHandCursor);
    connect(m_actionButton, &QPushButton::clicked, this, &FluentMessageBarWidget::onActionClicked);

    m_secondaryActionButton = new QPushButton(m_secondaryActionText);
    m_secondaryActionButton->setFlat(true);
    m_secondaryActionButton->setCursor(Qt::PointingHandCursor);
    connect(m_secondaryActionButton, &QPushButton::clicked, this, &FluentMessageBarWidget::onSecondaryActionClicked);

    // Dismiss button
    m_dismissButton = new QPushButton();
    m_dismissButton->setFlat(true);
    m_dismissButton->setFixedSize(24, 24);
    m_dismissButton->setCursor(Qt::PointingHandCursor);
    m_dismissButton->setToolTip("Dismiss");
    m_dismissButton->setIcon(QIcon(":/icons/Dismiss.png"));
    m_dismissButton->setIconSize(QSize(12, 12));
    connect(m_dismissButton, &QPushButton::clicked, this, &FluentMessageBarWidget::onDismissClicked);

    m_buttonLayout->addWidget(m_actionButton);
    m_buttonLayout->addWidget(m_secondaryActionButton);
    m_buttonLayout->addStretch();
    m_buttonLayout->addWidget(m_dismissButton);

    // Add all to main layout
    m_mainLayout->addWidget(m_iconLabel);
    m_mainLayout->addLayout(m_contentLayout, 1);
    m_mainLayout->addLayout(m_buttonLayout);

    // Setup animations
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(1.0);
    setGraphicsEffect(m_opacityEffect);

    m_showAnimation = new QPropertyAnimation(m_opacityEffect, "opacity", this);
    m_showAnimation->setDuration(300);
    m_showAnimation->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_showAnimation, &QPropertyAnimation::finished, this, &FluentMessageBarWidget::onShowAnimationFinished);

    m_hideAnimation = new QPropertyAnimation(m_opacityEffect, "opacity", this);
    m_hideAnimation->setDuration(200);
    m_hideAnimation->setEasingCurve(QEasingCurve::InCubic);
    connect(m_hideAnimation, &QPropertyAnimation::finished, this, &FluentMessageBarWidget::onHideAnimationFinished);

    // Auto-hide timer
    m_autoHideTimer = new QTimer(this);
    m_autoHideTimer->setSingleShot(true);
    connect(m_autoHideTimer, &QTimer::timeout, this, &FluentMessageBarWidget::onAutoHideTimer);
}

void FluentMessageBarWidget::updateStyles()
{
    QString backgroundColor = getTypeBackgroundColor();
    QString textColor = getTypeTextColor();
    QColor typeColor = getTypeColor();

    // Main widget style
    QString widgetStyle = QString(
        "FluentMessageBarWidget {"
        "    background-color: %1;"
        "    border-left: 4px solid %2;"
        "    border-radius: 4px;"
        "}"
    ).arg(backgroundColor, typeColor.name());

    setStyleSheet(widgetStyle);

    // Title style
    QString titleStyle = QString(
        "QLabel {"
        "    color: %1;"
        "    font-size: 14px;"
        "    font-weight: 600;"
        "    background: transparent;"
        "    border: none;"
        "}"
    ).arg(textColor);
    m_titleLabel->setStyleSheet(titleStyle);

    // Message style
    QString messageStyle = QString(
        "QLabel {"
        "    color: %1;"
        "    font-size: 13px;"
        "    font-weight: 400;"
        "    background: transparent;"
        "    border: none;"
        "}"
    ).arg(textColor);
    m_messageLabel->setStyleSheet(messageStyle);

    // Icon style
    QString iconStyle = QString(
        "QLabel {"
        "    color: %1;"
        "    background: transparent;"
        "    border: none;"
        "}"
    ).arg(typeColor.name());
    m_iconLabel->setStyleSheet(iconStyle);

    // Action button styles
    QString actionButtonStyle = QString(
        "QPushButton {"
        "    color: %1;"
        "    background: transparent;"
        "    border: 1px solid %1;"
        "    border-radius: 4px;"
        "    padding: 6px 12px;"
        "    font-size: 13px;"
        "    font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "    background-color: %1;"
        "    color: %2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: %1;"
        "    color: %2;"
        "}"
    ).arg(typeColor.name(), backgroundColor);

    m_actionButton->setStyleSheet(actionButtonStyle);
    m_secondaryActionButton->setStyleSheet(actionButtonStyle);

    // Dismiss button style
    QString dismissStyle = QString(
        "QPushButton {"
        "    background: transparent;"
        "    border: none;"
        "    border-radius: 12px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(0, 0, 0, 0.1);"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(0, 0, 0, 0.2);"
        "}"
    );
    m_dismissButton->setStyleSheet(dismissStyle);
}

void FluentMessageBarWidget::updateIcon()
{
    if (m_iconLabel) {
        QString iconPath = getIconPath();
        QPixmap iconPixmap(iconPath);

        if (!iconPixmap.isNull()) {
            m_iconLabel->setPixmap(iconPixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            // Fallback to empty if icon not found
            m_iconLabel->clear();
        }
    }
}

void FluentMessageBarWidget::updateActionButtons()
{
    if (m_actionButton && m_secondaryActionButton) {
        m_actionButton->setVisible(m_showActionButtons);
        m_secondaryActionButton->setVisible(m_showActionButtons);

        m_actionButton->setText(m_actionText);
        m_secondaryActionButton->setText(m_secondaryActionText);
    }

    if (m_dismissButton) {
        m_dismissButton->setVisible(m_dismissible);
    }
}

QString FluentMessageBarWidget::getIconPath() const
{
    switch (m_messageType) {
        case Info:    return ":/icons/message_bar-info.png";
        case Success: return ":/icons/message_bar-success.png";
        case Warning: return ":/icons/message_bar-warning.png";
        case Error:   return ":/icons/message_bar-danger.png";
        default:      return ":/icons/message_bar-info.png";
    }
}

QColor FluentMessageBarWidget::getTypeColor() const
{
    if (m_darkMode) {
        switch (m_messageType) {
            case Info:    return QColor(96, 165, 250);   // Blue
            case Success: return QColor(52, 211, 153);   // Green
            case Warning: return QColor(251, 191, 36);   // Yellow
            case Error:   return QColor(248, 113, 113);  // Red
            default:      return QColor(96, 165, 250);
        }
    } else {
        switch (m_messageType) {
            case Info:    return QColor(59, 130, 246);   // Blue
            case Success: return QColor(34, 197, 94);    // Green
            case Warning: return QColor(245, 158, 11);   // Yellow/Orange
            case Error:   return QColor(239, 68, 68);    // Red
            default:      return QColor(59, 130, 246);
        }
    }
}

QString FluentMessageBarWidget::getTypeBackgroundColor() const
{
    if (m_darkMode) {
        switch (m_messageType) {
            case Info:    return "#1e3a5f";  // Dark blue
            case Success: return "#1e5f3a";  // Dark green
            case Warning: return "#5f4a1e";  // Dark yellow
            case Error:   return "#5f1e1e";  // Dark red
            default:      return "#1e3a5f";
        }
    } else {
        switch (m_messageType) {
            case Info:    return "#eff6ff";  // Light blue
            case Success: return "#f0fdf4";  // Light green
            case Warning: return "#fffbeb";  // Light yellow
            case Error:   return "#fef2f2";  // Light red
            default:      return "#eff6ff";
        }
    }
}

QString FluentMessageBarWidget::getTypeTextColor() const
{
    if (m_darkMode) {
        return "#ffffff";
    } else {
        switch (m_messageType) {
            case Info:    return "#1e40af";  // Dark blue
            case Success: return "#15803d";  // Dark green
            case Warning: return "#a16207";  // Dark yellow
            case Error:   return "#dc2626";  // Dark red
            default:      return "#1e40af";
        }
    }
}

void FluentMessageBarWidget::startShowAnimation()
{
    m_opacityEffect->setOpacity(0.0);
    m_showAnimation->setStartValue(0.0);
    m_showAnimation->setEndValue(1.0);
    m_showAnimation->start();
}

void FluentMessageBarWidget::startHideAnimation()
{
    m_hideAnimation->setStartValue(1.0);
    m_hideAnimation->setEndValue(0.0);
    m_hideAnimation->start();
}

// Property implementations
QString FluentMessageBarWidget::title() const { return m_title; }
void FluentMessageBarWidget::setTitle(const QString &title)
{
    m_title = title;
    if (m_titleLabel) m_titleLabel->setText(title);
}

QString FluentMessageBarWidget::message() const { return m_messageText; }
void FluentMessageBarWidget::setMessage(const QString &message)
{
    m_messageText = message;
    if (m_messageLabel) m_messageLabel->setText(message);
}

FluentMessageBarWidget::MessageType FluentMessageBarWidget::messageType() const { return m_messageType; }
void FluentMessageBarWidget::setMessageType(MessageType type)
{
    m_messageType = type;
    updateIcon();
    updateStyles();
}

bool FluentMessageBarWidget::isDismissible() const { return m_dismissible; }
void FluentMessageBarWidget::setDismissible(bool dismissible)
{
    m_dismissible = dismissible;
    updateActionButtons();
}

bool FluentMessageBarWidget::showActions() const { return m_showActionButtons; }
void FluentMessageBarWidget::setShowActions(bool show)
{
    m_showActionButtons = show;
    updateActionButtons();
}

QString FluentMessageBarWidget::actionText() const { return m_actionText; }
void FluentMessageBarWidget::setActionText(const QString &text)
{
    m_actionText = text;
    updateActionButtons();
}

QString FluentMessageBarWidget::secondaryActionText() const { return m_secondaryActionText; }
void FluentMessageBarWidget::setSecondaryActionText(const QString &text)
{
    m_secondaryActionText = text;
    updateActionButtons();
}

bool FluentMessageBarWidget::isDarkMode() const { return m_darkMode; }
void FluentMessageBarWidget::setDarkMode(bool dark)
{
    m_darkMode = dark;
    updateStyles();
}

int FluentMessageBarWidget::autoHideDelay() const { return m_autoHideDelay; }
void FluentMessageBarWidget::setAutoHideDelay(int delay)
{
    m_autoHideDelay = delay;
}

void FluentMessageBarWidget::show()
{
    QWidget::show();
    m_isVisible = true;
    startShowAnimation();

    if (m_autoHideDelay > 0) {
        m_autoHideTimer->start(m_autoHideDelay);
    }
}

void FluentMessageBarWidget::hide()
{
    if (m_isVisible) {
        m_isVisible = false;
        m_autoHideTimer->stop();
        startHideAnimation();
    }
}

void FluentMessageBarWidget::showMessage(const QString &title, const QString &message, MessageType type)
{
    setTitle(title);
    setMessage(message);
    setMessageType(type);
    show();
}

// Slot implementations
void FluentMessageBarWidget::onDismissClicked()
{
    hide();
    emit dismissed();
}

void FluentMessageBarWidget::onActionClicked()
{
    emit actionClicked();
}

void FluentMessageBarWidget::onSecondaryActionClicked()
{
    emit secondaryActionClicked();
}

void FluentMessageBarWidget::onAutoHideTimer()
{
    hide();
}

void FluentMessageBarWidget::onShowAnimationFinished()
{
    // Show animation completed
}

void FluentMessageBarWidget::onHideAnimationFinished()
{
    QWidget::hide();
}

void FluentMessageBarWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QSize FluentMessageBarWidget::sizeHint() const
{
    return QSize(400, 68);
}

QSize FluentMessageBarWidget::minimumSizeHint() const
{
    return QSize(300, 68);
}