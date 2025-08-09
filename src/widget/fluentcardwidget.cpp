#include "fluentcardwidget.h"
#include <QApplication>
#include <QPalette>

FluentCardWidget::FluentCardWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(nullptr)
    , m_valueLabel(nullptr)
    , m_progressLabel(nullptr)
    , m_iconLabel(nullptr)
    , m_progressBar(nullptr)
    , m_mainLayout(nullptr)
    , m_headerLayout(nullptr)
    , m_shadowEffect(nullptr)
    , m_hoverAnimation(nullptr)
    , m_title("TITLE")
    , m_value("0.0%")
    , m_progressLabelText("Status")
    , m_progressValue(0)
    , m_darkMode(false)
    , m_accentColor(QColor(0, 120, 215)) // Default blue
    , m_fontIconCharacter("⚓") // Anchor symbol as default
    , m_fontIconFont("Segoe UI", 16)
    , m_useFontIcon(true)
    , m_isHovered(false)
    , m_hoverOffset(0.0)
{
    setupUI();
    setupShadowEffect();
    setupHoverAnimation();
    updateTheme();

    setFixedSize(280, 140);
    setAttribute(Qt::WA_Hover, true);
}

void FluentCardWidget::setupUI()
{
    // Create main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(20, 16, 20, 16);
    m_mainLayout->setSpacing(8);

    // Header layout with title and icon
    m_headerLayout = new QHBoxLayout();
    m_headerLayout->setContentsMargins(0, 0, 0, 0);

    // Title label
    m_titleLabel = new QLabel(m_title);
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Icon label
    m_iconLabel = new QLabel();
    m_iconLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_iconLabel->setFixedSize(24, 24);

    // Set default font-based icon
    updateIconDisplay();

    m_headerLayout->addWidget(m_titleLabel);
    m_headerLayout->addStretch();
    m_headerLayout->addWidget(m_iconLabel);

    // Value label (main display)
    m_valueLabel = new QLabel(m_value);
    m_valueLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Progress label
    m_progressLabel = new QLabel(m_progressLabelText);
    m_progressLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Progress bar
    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(m_progressValue);
    m_progressBar->setTextVisible(false);
    m_progressBar->setFixedHeight(6);

    // Add widgets to main layout
    m_mainLayout->addLayout(m_headerLayout);
    m_mainLayout->addWidget(m_valueLabel);
    m_mainLayout->addWidget(m_progressLabel);
    m_mainLayout->addWidget(m_progressBar);
    m_mainLayout->addStretch();
}

void FluentCardWidget::setupShadowEffect()
{
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(15);
    m_shadowEffect->setXOffset(0);
    m_shadowEffect->setYOffset(2);
    m_shadowEffect->setColor(QColor(0, 0, 0, 30));
    setGraphicsEffect(m_shadowEffect);
}

void FluentCardWidget::setupHoverAnimation()
{
    m_hoverAnimation = new QPropertyAnimation(this, "pos", this);
    m_hoverAnimation->setDuration(200);
    m_hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);

    connect(m_hoverAnimation, &QPropertyAnimation::finished,
            this, &FluentCardWidget::onHoverAnimationFinished);
}

void FluentCardWidget::updateTheme()
{
    updateStyles();
}

void FluentCardWidget::updateStyles()
{
    QString cardStyle;
    QString progressStyle;

    if (m_darkMode) {
        // Dark mode styles
        cardStyle = QString(
            "FluentCardWidget {"
            "    background-color: #2d2d2d;"
            "    border: 1px solid #404040;"
            "    border-radius: 8px;"
            "}"
        );

        progressStyle = QString(
            "QProgressBar {"
            "    background-color: #404040;"
            "    border: none;"
            "    border-radius: 3px;"
            "}"
            "QProgressBar::chunk {"
            "    background-color: %1;"
            "    border-radius: 3px;"
            "}"
        ).arg(m_accentColor.name());

        m_titleLabel->setStyleSheet("color: #b0b0b0; font-size: 11px; font-weight: 500; letter-spacing: 0.5px;");
        m_valueLabel->setStyleSheet("color: #ffffff; font-size: 32px; font-weight: 300; margin: 4px 0px;");
        m_progressLabel->setStyleSheet("color: #b0b0b0; font-size: 12px; font-weight: 400;");

    } else {
        // Light mode styles
        cardStyle = QString(
            "FluentCardWidget {"
            "    background-color: #ffffff;"
            "    border: 1px solid #e5e5e5;"
            "    border-radius: 8px;"
            "}"
        );

        progressStyle = QString(
            "QProgressBar {"
            "    background-color: #f0f0f0;"
            "    border: none;"
            "    border-radius: 3px;"
            "}"
            "QProgressBar::chunk {"
            "    background-color: %1;"
            "    border-radius: 3px;"
            "}"
        ).arg(m_accentColor.name());

        m_titleLabel->setStyleSheet("color: #666666; font-size: 11px; font-weight: 500; letter-spacing: 0.5px;");
        m_valueLabel->setStyleSheet("color: #1a1a1a; font-size: 32px; font-weight: 300; margin: 4px 0px;");
        m_progressLabel->setStyleSheet("color: #666666; font-size: 12px; font-weight: 400;");
    }

    setStyleSheet(cardStyle);
    m_progressBar->setStyleSheet(progressStyle);

    // Update icon colors based on theme
    updateIconDisplay();
}

void FluentCardWidget::updateIconDisplay()
{
    if (!m_iconLabel) return;

    if (m_useFontIcon) {
        // Set font-based icon
        m_iconLabel->setPixmap(QPixmap()); // Clear pixmap
        m_iconLabel->setText(m_fontIconCharacter);
        m_iconLabel->setFont(m_fontIconFont);
        m_iconLabel->setScaledContents(false);

        // Set color based on theme
        QString iconColor = m_darkMode ? "#b0b0b0" : "#666666";
        m_iconLabel->setStyleSheet(QString("color: %1;").arg(iconColor));

    } else {
        // Set pixmap-based icon
        m_iconLabel->setText("");
        m_iconLabel->setStyleSheet("");
        m_iconLabel->setScaledContents(true);

        if (!m_icon.isNull()) {
            m_iconLabel->setPixmap(m_icon.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

void FluentCardWidget::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event)

    if (!m_isHovered) {
        m_isHovered = true;

        // Animate shadow and position
        m_shadowEffect->setBlurRadius(25);
        m_shadowEffect->setYOffset(8);
        m_shadowEffect->setColor(QColor(0, 0, 0, 50));

        // Animate upward movement
        QPoint currentPos = pos();
        QPoint targetPos = currentPos + QPoint(0, -4);

        m_hoverAnimation->setStartValue(currentPos);
        m_hoverAnimation->setEndValue(targetPos);
        m_hoverAnimation->start();
    }
}

void FluentCardWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)

    if (m_isHovered) {
        m_isHovered = false;

        // Reset shadow
        m_shadowEffect->setBlurRadius(15);
        m_shadowEffect->setYOffset(2);
        m_shadowEffect->setColor(QColor(0, 0, 0, 30));

        // Animate back to original position
        QPoint currentPos = pos();
        QPoint targetPos = currentPos + QPoint(0, 4);

        m_hoverAnimation->setStartValue(currentPos);
        m_hoverAnimation->setEndValue(targetPos);
        m_hoverAnimation->start();
    }
}

void FluentCardWidget::onHoverAnimationFinished()
{
    // Animation finished
}

void FluentCardWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// Property implementations
QString FluentCardWidget::title() const { return m_title; }
void FluentCardWidget::setTitle(const QString &title)
{
    m_title = title;
    if (m_titleLabel) m_titleLabel->setText(title.toUpper());
}

QString FluentCardWidget::value() const { return m_value; }
void FluentCardWidget::setValue(const QString &value)
{
    m_value = value;
    if (m_valueLabel) m_valueLabel->setText(value);
}

QString FluentCardWidget::progressLabel() const { return m_progressLabelText; }
void FluentCardWidget::setProgressLabel(const QString &label)
{
    m_progressLabelText = label;
    if (m_progressLabel) m_progressLabel->setText(label);
}

int FluentCardWidget::progressValue() const { return m_progressValue; }
void FluentCardWidget::setProgressValue(int value)
{
    m_progressValue = qBound(0, value, 100);
    if (m_progressBar) m_progressBar->setValue(m_progressValue);
}

QPixmap FluentCardWidget::icon() const { return m_icon; }
void FluentCardWidget::setIcon(const QPixmap &icon)
{
    m_icon = icon;
    m_useFontIcon = false;
    updateIconDisplay();
}

void FluentCardWidget::setIcon(const QString &resourcePath)
{
    QPixmap icon(resourcePath);
    if (!icon.isNull()) {
        setIcon(icon);
    }
}

void FluentCardWidget::setFontIcon(const QString &character, const QFont &font)
{
    m_fontIconCharacter = character;
    m_fontIconFont = font.family().isEmpty() ? QFont("Segoe UI", 16) : font;
    m_useFontIcon = true;
    updateIconDisplay();
}

bool FluentCardWidget::isDarkMode() const { return m_darkMode; }
void FluentCardWidget::setDarkMode(bool dark)
{
    m_darkMode = dark;
    updateTheme();
}

QColor FluentCardWidget::accentColor() const { return m_accentColor; }
void FluentCardWidget::setAccentColor(const QColor &color)
{
    m_accentColor = color;
    updateStyles();
}

QSize FluentCardWidget::sizeHint() const
{
    return QSize(280, 140);
}

QSize FluentCardWidget::minimumSizeHint() const
{
    return QSize(200, 120);
}