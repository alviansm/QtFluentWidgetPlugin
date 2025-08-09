#include "fluentmodalwidget.h"
#include <QApplication>
#include <QPalette>
#include <QKeyEvent>
#include <QGraphicsEffect>

FluentModalWidget::FluentModalWidget(QWidget *parent)
    : QWidget(parent)
    , m_imageLabel(nullptr)
    , m_titleLabel(nullptr)
    , m_descriptionLabel(nullptr)
    , m_primaryButton(nullptr)
    , m_secondaryButton(nullptr)
    , m_stepIndicatorWidget(nullptr)
    , m_stepIndicatorLayout(nullptr)
    , m_mainLayout(nullptr)
    , m_contentLayout(nullptr)
    , m_buttonLayout(nullptr)
    , m_shadowEffect(nullptr)
    , m_scaleAnimation(nullptr)
    , m_opacityAnimation(nullptr)
    , m_showAnimation(nullptr)
    , m_hideAnimation(nullptr)
    , m_opacityEffect(nullptr)
    , m_title("Discover Copilot, a whole new way to work")
    , m_description("Explore new ways to work smarter and faster using the power of AI. Copilot in [Word] can help you [get started from scratch], [work from an existing file], [get actionable insights about documents], and more.")
    , m_primaryButtonText("Next")
    , m_secondaryButtonText("Try later")
    , m_currentStep(1)
    , m_totalSteps(5)
    , m_darkMode(false)
    , m_accentColor(QColor(0, 120, 215)) // Default blue
    , m_showNavigationButtons(true)
    , m_isVisible(false)
{
    setupUI();
    setupAnimations();
    setupShadowEffect();
    updateTheme();

    setFixedSize(370, 345);
    setAttribute(Qt::WA_Hover, true);
    setFocusPolicy(Qt::StrongFocus);

    // Initially hidden
    setVisible(false);
}

void FluentModalWidget::setupUI()
{
    // Create main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Setup different areas
    setupImageArea();
    setupContentArea();
    setupButtonArea();
}

void FluentModalWidget::setupImageArea()
{
    // Image area - fills the top portion
    m_imageLabel = new QLabel();
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setFixedSize(370, 160); // Full width, fixed height
    m_imageLabel->setScaledContents(true);
    m_imageLabel->setStyleSheet(
        "QLabel {"
        "    background-color: #f0f4ff;"
        "    border: none;"
        "    border-top-left-radius: 12px;"
        "    border-top-right-radius: 12px;"
        "    color: #6c757d;"
        "    font-size: 11px;"
        "    font-weight: 500;"
        "}"
    );
    m_imageLabel->setText("SWAP WITH IMAGE - 286PX WIDTH MAX\nCreate a screenshot with this layout and then\nread the components.");

    m_mainLayout->addWidget(m_imageLabel);
}

void FluentModalWidget::setupContentArea()
{
    // Content layout with padding
    m_contentLayout = new QVBoxLayout();
    m_contentLayout->setContentsMargins(24, 24, 24, 16);
    m_contentLayout->setSpacing(12);

    // Title label - left aligned
    m_titleLabel = new QLabel(m_title);
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_titleLabel->setWordWrap(true);

    // Description label - left aligned
    m_descriptionLabel = new QLabel(m_description);
    m_descriptionLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_descriptionLabel->setWordWrap(true);

    m_contentLayout->addWidget(m_titleLabel);
    m_contentLayout->addWidget(m_descriptionLabel);
    m_contentLayout->addStretch();

    m_mainLayout->addLayout(m_contentLayout);
}

void FluentModalWidget::setupButtonArea()
{
    // Step indicator above buttons
    setupStepIndicator();

    // Center the step indicator
    QHBoxLayout *stepCenterLayout = new QHBoxLayout();
    stepCenterLayout->setContentsMargins(24, 16, 24, 16);
    stepCenterLayout->addStretch();
    stepCenterLayout->addWidget(m_stepIndicatorWidget);
    stepCenterLayout->addStretch();

    // Button layout with secondary, step indicator, and primary
    m_buttonLayout = new QHBoxLayout();
    m_buttonLayout->setContentsMargins(24, 0, 24, 24);
    m_buttonLayout->setSpacing(16);

    // Secondary button (left)
    m_secondaryButton = new QPushButton(m_secondaryButtonText);
    m_secondaryButton->setFixedHeight(32);
    m_secondaryButton->setMinimumWidth(80);

    // Primary button (right)
    m_primaryButton = new QPushButton(m_primaryButtonText);
    m_primaryButton->setFixedHeight(32);
    m_primaryButton->setMinimumWidth(80);

    m_buttonLayout->addWidget(m_secondaryButton);
    m_buttonLayout->addStretch();
    m_buttonLayout->addWidget(m_primaryButton);

    // Add layouts to main layout
    m_mainLayout->addLayout(stepCenterLayout);
    m_mainLayout->addLayout(m_buttonLayout);

    // Connect button signals
    connect(m_primaryButton, &QPushButton::clicked,
            this, &FluentModalWidget::onPrimaryButtonClicked);
    connect(m_secondaryButton, &QPushButton::clicked,
            this, &FluentModalWidget::onSecondaryButtonClicked);
}

void FluentModalWidget::setupStepIndicator()
{
    m_stepIndicatorWidget = new QWidget();
    m_stepIndicatorLayout = new QHBoxLayout(m_stepIndicatorWidget);
    m_stepIndicatorLayout->setContentsMargins(0, 0, 0, 0);
    m_stepIndicatorLayout->setSpacing(8);

    updateStepIndicator();
}

void FluentModalWidget::setupAnimations()
{
    // Opacity effect for fade animations
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(m_opacityEffect);

    // Scale animation
    m_scaleAnimation = new QPropertyAnimation(this, "geometry", this);
    m_scaleAnimation->setDuration(300);
    m_scaleAnimation->setEasingCurve(QEasingCurve::OutCubic);

    // Opacity animation
    m_opacityAnimation = new QPropertyAnimation(m_opacityEffect, "opacity", this);
    m_opacityAnimation->setDuration(300);
    m_opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);

    // Show animation group
    m_showAnimation = new QParallelAnimationGroup(this);
    m_showAnimation->addAnimation(m_scaleAnimation);
    m_showAnimation->addAnimation(m_opacityAnimation);

    // Hide animation group
    m_hideAnimation = new QParallelAnimationGroup(this);
    m_hideAnimation->addAnimation(m_scaleAnimation);
    m_hideAnimation->addAnimation(m_opacityAnimation);

    connect(m_showAnimation, &QParallelAnimationGroup::finished,
            this, &FluentModalWidget::onShowAnimationFinished);
    connect(m_hideAnimation, &QParallelAnimationGroup::finished,
            this, &FluentModalWidget::onHideAnimationFinished);
}

void FluentModalWidget::setupShadowEffect()
{
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(30);
    m_shadowEffect->setXOffset(0);
    m_shadowEffect->setYOffset(8);
    m_shadowEffect->setColor(QColor(0, 0, 0, 60));
    // Note: Can't apply shadow effect if opacity effect is already applied
}

void FluentModalWidget::updateTheme()
{
    updateStyles();
    updateStepIndicator();
}

void FluentModalWidget::updateStyles()
{
    QString modalStyle;
    QString primaryButtonStyle;
    QString secondaryButtonStyle;

    if (m_darkMode) {
        // Dark mode styles
        modalStyle = QString(
            "FluentModalWidget {"
            "    background-color: #2d2d2d;"
            "    border: 1px solid #404040;"
            "    border-radius: 12px;"
            "}"
        );

        primaryButtonStyle = QString(
            "QPushButton {"
            "    background-color: %1;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 6px;"
            "    font-size: 13px;"
            "    font-weight: 500;"
            "    padding: 6px 12px;"
            "}"
            "QPushButton:hover {"
            "    background-color: %2;"
            "}"
            "QPushButton:pressed {"
            "    background-color: %3;"
            "}"
        ).arg(m_accentColor.name())
         .arg(m_accentColor.lighter(110).name())
         .arg(m_accentColor.darker(110).name());

        secondaryButtonStyle = QString(
            "QPushButton {"
            "    background-color: transparent;"
            "    color: #ffffff;"
            "    border: 1px solid #404040;"
            "    border-radius: 6px;"
            "    font-size: 13px;"
            "    font-weight: 500;"
            "    padding: 6px 12px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #404040;"
            "    border-color: #505050;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #505050;"
            "}"
        );

        m_titleLabel->setStyleSheet("color: #ffffff; font-size: 16px; font-weight: 600; margin-bottom: 4px; text-align: left;");
        m_descriptionLabel->setStyleSheet("color: #b0b0b0; font-size: 13px; font-weight: 400; line-height: 18px; text-align: left;");

    } else {
        // Light mode styles
        modalStyle = QString(
            "FluentModalWidget {"
            "    background-color: #ffffff;"
            "    border: 1px solid #e5e5e5;"
            "    border-radius: 12px;"
            "}"
        );

        primaryButtonStyle = QString(
            "QPushButton {"
            "    background-color: %1;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 6px;"
            "    font-size: 13px;"
            "    font-weight: 500;"
            "    padding: 6px 12px;"
            "}"
            "QPushButton:hover {"
            "    background-color: %2;"
            "}"
            "QPushButton:pressed {"
            "    background-color: %3;"
            "}"
        ).arg(m_accentColor.name())
         .arg(m_accentColor.lighter(110).name())
         .arg(m_accentColor.darker(110).name());

        secondaryButtonStyle = QString(
            "QPushButton {"
            "    background-color: transparent;"
            "    color: #666666;"
            "    border: 1px solid #e5e5e5;"
            "    border-radius: 6px;"
            "    font-size: 13px;"
            "    font-weight: 500;"
            "    padding: 6px 12px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #f8f9fa;"
            "    border-color: #d0d7de;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #f0f0f0;"
            "}"
        );

        m_titleLabel->setStyleSheet("color: #1a1a1a; font-size: 16px; font-weight: 600; margin-bottom: 4px; text-align: left;");
        m_descriptionLabel->setStyleSheet("color: #666666; font-size: 13px; font-weight: 400; line-height: 18px; text-align: left;");
    }

    setStyleSheet(modalStyle);
    if (m_primaryButton) m_primaryButton->setStyleSheet(primaryButtonStyle);
    if (m_secondaryButton) m_secondaryButton->setStyleSheet(secondaryButtonStyle);
}

void FluentModalWidget::updateStepIndicator()
{
    // Clear existing dots
    for (QPushButton* dot : m_stepDots) {
        m_stepIndicatorLayout->removeWidget(dot);
        dot->deleteLater();
    }
    m_stepDots.clear();

    // Create new clickable dots
    for (int i = 1; i <= m_totalSteps; ++i) {
        QPushButton* dot = new QPushButton();
        dot->setFixedSize(8, 8);
        dot->setFlat(true);
        dot->setProperty("step", i); // Store step number

        QString dotStyle;
        if (i == m_currentStep) {
            // Active dot
            dotStyle = QString(
                "QPushButton {"
                "    background-color: %1;"
                "    border-radius: 4px;"
                "    border: none;"
                "}"
                "QPushButton:hover {"
                "    background-color: %2;"
                "}"
            ).arg(m_accentColor.name())
             .arg(m_accentColor.lighter(110).name());
        } else {
            // Inactive dot
            QString inactiveColor = m_darkMode ? "#404040" : "#e5e5e5";
            QString hoverColor = m_darkMode ? "#505050" : "#d0d0d0";
            dotStyle = QString(
                "QPushButton {"
                "    background-color: %1;"
                "    border-radius: 4px;"
                "    border: none;"
                "}"
                "QPushButton:hover {"
                "    background-color: %2;"
                "}"
            ).arg(inactiveColor).arg(hoverColor);
        }

        dot->setStyleSheet(dotStyle);
        m_stepIndicatorLayout->addWidget(dot);
        m_stepDots.append(dot);

        // Connect click signal
        connect(dot, &QPushButton::clicked, this, &FluentModalWidget::onStepDotClicked);
    }
}

void FluentModalWidget::updateImageDisplay()
{
    if (!m_imageLabel) return;

    if (!m_image.isNull()) {
        m_imageLabel->setText("");
        // Scale image to fill the area while maintaining aspect ratio
        QPixmap scaledPixmap = m_image.scaled(370, 160, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        m_imageLabel->setPixmap(scaledPixmap);
        m_imageLabel->setStyleSheet(
            "QLabel {"
            "    background-color: transparent;"
            "    border: none;"
            "    border-top-left-radius: 12px;"
            "    border-top-right-radius: 12px;"
            "}"
        );
    } else {
        m_imageLabel->setText("SWAP WITH IMAGE - 286PX WIDTH MAX\nCreate a screenshot with this layout and then\nread the components.");
        updateStyles(); // This will restore the placeholder styling
    }
}

void FluentModalWidget::showModal()
{
    if (m_isVisible) return;

    setVisible(true);
    m_isVisible = true;

    // Setup initial state for animation
    QRect finalGeometry = geometry();
    QRect startGeometry = finalGeometry;
    startGeometry.moveCenter(finalGeometry.center());
    startGeometry.setSize(QSize(finalGeometry.width() * 0.8, finalGeometry.height() * 0.8));

    m_opacityEffect->setOpacity(0.0);
    setGeometry(startGeometry);

    // Setup animation end values
    m_scaleAnimation->setStartValue(startGeometry);
    m_scaleAnimation->setEndValue(finalGeometry);
    m_opacityAnimation->setStartValue(0.0);
    m_opacityAnimation->setEndValue(1.0);

    m_showAnimation->start();
}

void FluentModalWidget::hideModal()
{
    if (!m_isVisible) return;

    QRect currentGeometry = geometry();
    QRect endGeometry = currentGeometry;
    endGeometry.moveCenter(currentGeometry.center());
    endGeometry.setSize(QSize(currentGeometry.width() * 0.8, currentGeometry.height() * 0.8));

    m_scaleAnimation->setStartValue(currentGeometry);
    m_scaleAnimation->setEndValue(endGeometry);
    m_opacityAnimation->setStartValue(1.0);
    m_opacityAnimation->setEndValue(0.0);

    m_hideAnimation->start();
}

void FluentModalWidget::nextStep()
{
    if (m_currentStep < m_totalSteps) {
        setCurrentStep(m_currentStep + 1);
    }
}

void FluentModalWidget::previousStep()
{
    if (m_currentStep > 1) {
        setCurrentStep(m_currentStep - 1);
    }
}

void FluentModalWidget::goToStep(int step)
{
    setCurrentStep(step);
}

void FluentModalWidget::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event)
    // Could add subtle hover effects here if desired
}

void FluentModalWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    // Reset any hover effects
}

void FluentModalWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void FluentModalWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        hideModal();
        emit modalClosed();
    } else if (event->key() == Qt::Key_Left) {
        previousStep();
    } else if (event->key() == Qt::Key_Right) {
        nextStep();
    } else {
        QWidget::keyPressEvent(event);
    }
}

// Slot implementations
void FluentModalWidget::onPrimaryButtonClicked()
{
    emit primaryButtonClicked();
}

void FluentModalWidget::onSecondaryButtonClicked()
{
    emit secondaryButtonClicked();
}

void FluentModalWidget::onStepDotClicked()
{
    QPushButton* sender = qobject_cast<QPushButton*>(QObject::sender());
    if (sender) {
        int step = sender->property("step").toInt();
        goToStep(step);
    }
}

void FluentModalWidget::onShowAnimationFinished()
{
    // Animation completed
}

void FluentModalWidget::onHideAnimationFinished()
{
    setVisible(false);
    m_isVisible = false;
    emit modalClosed();
}

// Property implementations
QString FluentModalWidget::title() const { return m_title; }
void FluentModalWidget::setTitle(const QString &title)
{
    m_title = title;
    if (m_titleLabel) m_titleLabel->setText(title);
}

QString FluentModalWidget::description() const { return m_description; }
void FluentModalWidget::setDescription(const QString &description)
{
    m_description = description;
    if (m_descriptionLabel) m_descriptionLabel->setText(description);
}

QPixmap FluentModalWidget::image() const { return m_image; }
void FluentModalWidget::setImage(const QPixmap &image)
{
    m_image = image;
    updateImageDisplay();
}

void FluentModalWidget::setImage(const QString &resourcePath)
{
    QPixmap image(resourcePath);
    if (!image.isNull()) {
        setImage(image);
    }
}

QString FluentModalWidget::primaryButtonText() const { return m_primaryButtonText; }
void FluentModalWidget::setPrimaryButtonText(const QString &text)
{
    m_primaryButtonText = text;
    if (m_primaryButton) m_primaryButton->setText(text);
}

QString FluentModalWidget::secondaryButtonText() const { return m_secondaryButtonText; }
void FluentModalWidget::setSecondaryButtonText(const QString &text)
{
    m_secondaryButtonText = text;
    if (m_secondaryButton) m_secondaryButton->setText(text);
}

int FluentModalWidget::currentStep() const { return m_currentStep; }
void FluentModalWidget::setCurrentStep(int step)
{
    int newStep = qBound(1, step, m_totalSteps);
    if (newStep != m_currentStep) {
        m_currentStep = newStep;
        updateStepIndicator();
        emit stepChanged(m_currentStep);
    }
}

int FluentModalWidget::totalSteps() const { return m_totalSteps; }
void FluentModalWidget::setTotalSteps(int steps)
{
    m_totalSteps = qMax(1, steps);
    if (m_currentStep > m_totalSteps) {
        m_currentStep = m_totalSteps;
    }
    updateStepIndicator();
}

bool FluentModalWidget::isDarkMode() const { return m_darkMode; }
void FluentModalWidget::setDarkMode(bool dark)
{
    m_darkMode = dark;
    updateTheme();
}

QColor FluentModalWidget::accentColor() const { return m_accentColor; }
void FluentModalWidget::setAccentColor(const QColor &color)
{
    m_accentColor = color;
    updateStyles();
    updateStepIndicator();
}

bool FluentModalWidget::showNavigationButtons() const { return m_showNavigationButtons; }
void FluentModalWidget::setShowNavigationButtons(bool show)
{
    m_showNavigationButtons = show;
    // Navigation is now handled by clickable step dots and main buttons
    // This property is kept for compatibility but doesn't affect UI anymore
}

QSize FluentModalWidget::sizeHint() const
{
    return QSize(370, 345);
}

QSize FluentModalWidget::minimumSizeHint() const
{
    return QSize(320, 300);
}