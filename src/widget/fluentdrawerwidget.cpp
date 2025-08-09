#include "fluentdrawerwidget.h"
#include <QApplication>
#include <QScreen>

FluentDrawerWidget::FluentDrawerWidget(QWidget *parent)
    : QWidget(parent)
    , m_drawerFrame(nullptr)
    , m_titleLabel(nullptr)
    , m_closeButton(nullptr)
    , m_contentScrollArea(nullptr)
    , m_contentLabel(nullptr)
    , m_contentWidget(nullptr)
    , m_primaryButton(nullptr)
    , m_secondaryButton(nullptr)
    , m_backButton(nullptr)
    , m_forwardButton(nullptr)
    , m_refreshButton(nullptr)
    , m_moreButton(nullptr)
    , m_mainLayout(nullptr)
    , m_headerLayout(nullptr)
    , m_navigationLayout(nullptr)
    , m_buttonLayout(nullptr)
    , m_drawerLayout(nullptr)
    , m_shadowEffect(nullptr)
    , m_opacityEffect(nullptr)
    , m_slideAnimation(nullptr)
    , m_opacityAnimation(nullptr)
    , m_animationGroup(nullptr)
    , m_title("Title")
    , m_content("")
    , m_primaryButtonText("Primary")
    , m_secondaryButtonText("Secondary")
    , m_darkMode(false)
    , m_accentColor(QColor(0, 120, 215))
    , m_position(Right)
    , m_showNavigationButtons(false)
    , m_modal(true)
    , m_isVisible(false)
    , m_isAnimating(false)
{
    setupUI();
    setupAnimations();
    updateTheme();

    setFixedSize(400, 600);
    setAttribute(Qt::WA_DeleteOnClose, false);

    // Initially hide the widget
    QWidget::hide();
}

void FluentDrawerWidget::setupUI()
{
    // Main layout for the entire widget
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Drawer frame that contains all content
    m_drawerFrame = new QFrame();
    m_drawerFrame->setFrameShape(QFrame::NoFrame);

    // Drawer layout
    m_drawerLayout = new QVBoxLayout(m_drawerFrame);
    m_drawerLayout->setContentsMargins(0, 0, 0, 0);
    m_drawerLayout->setSpacing(0);

    // Header layout
    m_headerLayout = new QHBoxLayout();
    m_headerLayout->setContentsMargins(20, 16, 16, 16);
    m_headerLayout->setSpacing(12);

    // Title label
    m_titleLabel = new QLabel(m_title);
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Close button
    m_closeButton = new QPushButton("×");
    m_closeButton->setFixedSize(32, 32);
    m_closeButton->setFocusPolicy(Qt::NoFocus);
    connect(m_closeButton, &QPushButton::clicked, this, &FluentDrawerWidget::onCloseButtonClicked);

    m_headerLayout->addWidget(m_titleLabel);
    m_headerLayout->addStretch();

    // Navigation buttons (for navigation variant)
    setupNavigationButtons();

    m_headerLayout->addWidget(m_closeButton);

    // Content scroll area - set up as container
    m_contentScrollArea = new QScrollArea();
    m_contentScrollArea->setWidgetResizable(true);
    m_contentScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_contentScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_contentScrollArea->setFrameShape(QFrame::NoFrame);

    // Create a container widget for the content area
    m_contentWidget = new QWidget();
    m_contentWidget->setObjectName("drawerContent");

    // Set up layout for the content widget to accept child widgets
    auto contentLayout = new QVBoxLayout(m_contentWidget);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setSpacing(12);

    // Add placeholder label (will be hidden when actual content is added)
    m_contentLabel = new QLabel("Drop widgets here or set custom content");
    m_contentLabel->setAlignment(Qt::AlignCenter);
    m_contentLabel->setWordWrap(true);
    m_contentLabel->setObjectName("placeholderLabel");
    m_contentLabel->setStyleSheet("color: #999; font-style: italic;");
    contentLayout->addWidget(m_contentLabel);
    contentLayout->addStretch();

    m_contentScrollArea->setWidget(m_contentWidget);

    // Button layout
    m_buttonLayout = new QHBoxLayout();
    m_buttonLayout->setContentsMargins(20, 16, 20, 20);
    m_buttonLayout->setSpacing(12);

    // Primary button
    m_primaryButton = new QPushButton(m_primaryButtonText);
    m_primaryButton->setMinimumHeight(32);
    connect(m_primaryButton, &QPushButton::clicked, this, &FluentDrawerWidget::onPrimaryButtonClicked);

    // Secondary button
    m_secondaryButton = new QPushButton(m_secondaryButtonText);
    m_secondaryButton->setMinimumHeight(32);
    connect(m_secondaryButton, &QPushButton::clicked, this, &FluentDrawerWidget::onSecondaryButtonClicked);

    m_buttonLayout->addStretch();
    m_buttonLayout->addWidget(m_secondaryButton);
    m_buttonLayout->addWidget(m_primaryButton);

    // Add to drawer layout
    m_drawerLayout->addLayout(m_headerLayout);
    m_drawerLayout->addWidget(m_contentScrollArea, 1);
    m_drawerLayout->addLayout(m_buttonLayout);

    // Add drawer frame to main layout
    m_mainLayout->addWidget(m_drawerFrame);

    // Setup shadow effect
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(30);
    m_shadowEffect->setXOffset(0);
    m_shadowEffect->setYOffset(0);
    m_shadowEffect->setColor(QColor(0, 0, 0, 60));
    m_drawerFrame->setGraphicsEffect(m_shadowEffect);

    updateNavigationButtons();
}

void FluentDrawerWidget::setupNavigationButtons()
{
    // Create navigation buttons with icons
    m_backButton = new QPushButton();
    m_backButton->setIcon(QIcon(":/icons/arrow-left.png"));
    m_backButton->setToolTip("Back");

    m_forwardButton = new QPushButton();
    m_forwardButton->setIcon(QIcon(":/icons/arrow-right.png"));
    m_forwardButton->setToolTip("Forward");

    m_refreshButton = new QPushButton();
    m_refreshButton->setIcon(QIcon(":/icons/refresh.png"));
    m_refreshButton->setToolTip("Refresh");

    m_moreButton = new QPushButton();
    m_moreButton->setIcon(QIcon(":/icons/more-horizontal.png"));
    m_moreButton->setToolTip("More options");

    // Style navigation buttons
    QList<QPushButton*> navButtons = {m_backButton, m_forwardButton, m_refreshButton, m_moreButton};
    for (QPushButton* button : navButtons) {
        button->setFixedSize(32, 32);
        button->setFocusPolicy(Qt::NoFocus);
        button->setIconSize(QSize(16, 16));
    }
}

void FluentDrawerWidget::setupAnimations()
{
    // Slide animation
    m_slideAnimation = new QPropertyAnimation(this, "pos", this);
    m_slideAnimation->setDuration(300);
    m_slideAnimation->setEasingCurve(QEasingCurve::OutCubic);

    // Opacity animation
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(m_opacityEffect);

    m_opacityAnimation = new QPropertyAnimation(m_opacityEffect, "opacity", this);
    m_opacityAnimation->setDuration(300);

    // Animation group
    m_animationGroup = new QParallelAnimationGroup(this);
    m_animationGroup->addAnimation(m_slideAnimation);
    m_animationGroup->addAnimation(m_opacityAnimation);

    connect(m_animationGroup, &QParallelAnimationGroup::finished,
            this, &FluentDrawerWidget::onShowAnimationFinished);
}

void FluentDrawerWidget::updateTheme()
{
    updateStyles();
}

void FluentDrawerWidget::updateStyles()
{
    QString drawerStyle;
    QString buttonStyle;
    QString primaryButtonStyle;
    QString closeButtonStyle;
    QString scrollAreaStyle;

    if (m_darkMode) {
        // Dark mode styles
        drawerStyle = QString(
            "QFrame {"
            "    background-color: #2d2d2d;"
            "    border: 1px solid #404040;"
            "    border-radius: 8px;"
            "}"
        );

        buttonStyle = QString(
            "QPushButton {"
            "    background-color: #404040;"
            "    border: 1px solid #555555;"
            "    border-radius: 4px;"
            "    color: #ffffff;"
            "    padding: 8px 16px;"
            "    font-weight: 400;"
            "}"
            "QPushButton:hover {"
            "    background-color: #4a4a4a;"
            "    border-color: #666666;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #353535;"
            "}"
        );

        primaryButtonStyle = QString(
            "QPushButton {"
            "    background-color: %1;"
            "    border: 1px solid %1;"
            "    border-radius: 4px;"
            "    color: #ffffff;"
            "    padding: 8px 16px;"
            "    font-weight: 500;"
            "}"
            "QPushButton:hover {"
            "    background-color: %2;"
            "    border-color: %2;"
            "}"
            "QPushButton:pressed {"
            "    background-color: %3;"
            "}"
        ).arg(m_accentColor.name())
         .arg(m_accentColor.lighter(110).name())
         .arg(m_accentColor.darker(110).name());

        closeButtonStyle = QString(
            "QPushButton {"
            "    background-color: transparent;"
            "    border: none;"
            "    border-radius: 16px;"
            "    color: #b0b0b0;"
            "    font-size: 18px;"
            "    font-weight: normal;"
            "}"
            "QPushButton:hover {"
            "    background-color: #404040;"
            "    color: #ffffff;"
            "}"
        );

        scrollAreaStyle = QString(
            "QScrollArea {"
            "    background-color: transparent;"
            "    border: none;"
            "}"
            "QScrollBar:vertical {"
            "    background-color: #404040;"
            "    width: 12px;"
            "    border-radius: 6px;"
            "}"
            "QScrollBar::handle:vertical {"
            "    background-color: #606060;"
            "    border-radius: 6px;"
            "    min-height: 20px;"
            "}"
            "QScrollBar::handle:vertical:hover {"
            "    background-color: #707070;"
            "}"
        );

        m_titleLabel->setStyleSheet("color: #ffffff; font-size: 16px; font-weight: 600;");
        m_contentLabel->setStyleSheet("color: #b0b0b0; font-size: 14px; line-height: 1.4;");

    } else {
        // Light mode styles
        drawerStyle = QString(
            "QFrame {"
            "    background-color: #ffffff;"
            "    border: 1px solid #e5e5e5;"
            "    border-radius: 8px;"
            "}"
        );

        buttonStyle = QString(
            "QPushButton {"
            "    background-color: #f8f8f8;"
            "    border: 1px solid #d0d0d0;"
            "    border-radius: 4px;"
            "    color: #333333;"
            "    padding: 8px 16px;"
            "    font-weight: 400;"
            "}"
            "QPushButton:hover {"
            "    background-color: #f0f0f0;"
            "    border-color: #c0c0c0;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #e8e8e8;"
            "}"
        );

        primaryButtonStyle = QString(
            "QPushButton {"
            "    background-color: %1;"
            "    border: 1px solid %1;"
            "    border-radius: 4px;"
            "    color: #ffffff;"
            "    padding: 8px 16px;"
            "    font-weight: 500;"
            "}"
            "QPushButton:hover {"
            "    background-color: %2;"
            "    border-color: %2;"
            "}"
            "QPushButton:pressed {"
            "    background-color: %3;"
            "}"
        ).arg(m_accentColor.name())
         .arg(m_accentColor.lighter(110).name())
         .arg(m_accentColor.darker(110).name());

        closeButtonStyle = QString(
            "QPushButton {"
            "    background-color: transparent;"
            "    border: none;"
            "    border-radius: 16px;"
            "    color: #666666;"
            "    font-size: 18px;"
            "    font-weight: normal;"
            "}"
            "QPushButton:hover {"
            "    background-color: #f0f0f0;"
            "    color: #333333;"
            "}"
        );

        scrollAreaStyle = QString(
            "QScrollArea {"
            "    background-color: transparent;"
            "    border: none;"
            "}"
            "QScrollBar:vertical {"
            "    background-color: #f0f0f0;"
            "    width: 12px;"
            "    border-radius: 6px;"
            "}"
            "QScrollBar::handle:vertical {"
            "    background-color: #c0c0c0;"
            "    border-radius: 6px;"
            "    min-height: 20px;"
            "}"
            "QScrollBar::handle:vertical:hover {"
            "    background-color: #a0a0a0;"
            "}"
        );

        m_titleLabel->setStyleSheet("color: #1a1a1a; font-size: 16px; font-weight: 600;");
        m_contentLabel->setStyleSheet("color: #666666; font-size: 14px; line-height: 1.4;");
    }

    m_drawerFrame->setStyleSheet(drawerStyle);
    m_secondaryButton->setStyleSheet(buttonStyle);
    m_primaryButton->setStyleSheet(primaryButtonStyle);
    m_closeButton->setStyleSheet(closeButtonStyle);
    m_contentScrollArea->setStyleSheet(scrollAreaStyle);

    // Apply same button style to navigation buttons
    QList<QPushButton*> navButtons = {m_backButton, m_forwardButton, m_refreshButton, m_moreButton};
    for (QPushButton* button : navButtons) {
        button->setStyleSheet(closeButtonStyle);
    }
}

void FluentDrawerWidget::updateNavigationButtons()
{
    // Remove existing navigation buttons from layout
    QList<QPushButton*> navButtons = {m_backButton, m_forwardButton, m_refreshButton, m_moreButton};
    for (QPushButton* button : navButtons) {
        m_headerLayout->removeWidget(button);
        button->setVisible(false);
    }

    if (m_showNavigationButtons) {
        // Add navigation buttons to header
        int insertIndex = m_headerLayout->count() - 1; // Before close button
        m_headerLayout->insertWidget(insertIndex, m_moreButton);
        m_headerLayout->insertWidget(insertIndex, m_refreshButton);
        m_headerLayout->insertWidget(insertIndex, m_forwardButton);
        m_headerLayout->insertWidget(insertIndex, m_backButton);

        for (QPushButton* button : navButtons) {
            button->setVisible(true);
        }
    }
}

void FluentDrawerWidget::show()
{
    if (m_isVisible || m_isAnimating) return;

    m_isAnimating = true;

    // Update parent geometry
    if (parentWidget()) {
        m_parentGeometry = parentWidget()->geometry();
    } else {
        m_parentGeometry = QApplication::primaryScreen()->geometry();
    }

    updateGeometry();

    // Set initial position (off-screen)
    move(getStartPosition());

    // Show the widget
    QWidget::show();
    raise();

    // Start animation
    animateIn();
}

void FluentDrawerWidget::hide()
{
    if (!m_isVisible || m_isAnimating) return;

    m_isAnimating = true;
    animateOut();
}

void FluentDrawerWidget::toggle()
{
    if (m_isVisible) {
        hide();
    } else {
        show();
    }
}

void FluentDrawerWidget::animateIn()
{
    m_slideAnimation->setStartValue(pos());
    m_slideAnimation->setEndValue(getEndPosition());

    m_opacityAnimation->setStartValue(0.0);
    m_opacityAnimation->setEndValue(1.0);

    m_animationGroup->start();
}

void FluentDrawerWidget::animateOut()
{
    m_slideAnimation->setStartValue(pos());
    m_slideAnimation->setEndValue(getStartPosition());

    m_opacityAnimation->setStartValue(1.0);
    m_opacityAnimation->setEndValue(0.0);

    connect(m_animationGroup, &QParallelAnimationGroup::finished,
            this, &FluentDrawerWidget::onHideAnimationFinished);

    m_animationGroup->start();
}

QPoint FluentDrawerWidget::getStartPosition()
{
    QPoint startPos;

    switch (m_position) {
        case Left:
            startPos = QPoint(m_parentGeometry.left() - width(), m_parentGeometry.top());
            break;
        case Right:
            startPos = QPoint(m_parentGeometry.right(), m_parentGeometry.top());
            break;
        case Top:
            startPos = QPoint(m_parentGeometry.left(), m_parentGeometry.top() - height());
            break;
        case Bottom:
            startPos = QPoint(m_parentGeometry.left(), m_parentGeometry.bottom());
            break;
    }

    return startPos;
}

QPoint FluentDrawerWidget::getEndPosition()
{
    QPoint endPos;

    switch (m_position) {
        case Left:
            endPos = QPoint(m_parentGeometry.left() + 20, m_parentGeometry.top() + 20);
            break;
        case Right:
            endPos = QPoint(m_parentGeometry.right() - width() - 20, m_parentGeometry.top() + 20);
            break;
        case Top:
            endPos = QPoint(m_parentGeometry.left() + 20, m_parentGeometry.top() + 20);
            break;
        case Bottom:
            endPos = QPoint(m_parentGeometry.left() + 20, m_parentGeometry.bottom() - height() - 20);
            break;
    }

    return endPos;
}

void FluentDrawerWidget::updateGeometry()
{
    // Adjust size based on position
    switch (m_position) {
        case Left:
        case Right:
            setFixedSize(400, m_parentGeometry.height() - 40);
            break;
        case Top:
        case Bottom:
            setFixedSize(m_parentGeometry.width() - 40, 300);
            break;
    }
}

void FluentDrawerWidget::setContentWidget(QWidget *widget)
{
    if (!m_contentWidget) return;

    // Get the content layout
    QVBoxLayout* contentLayout = qobject_cast<QVBoxLayout*>(m_contentWidget->layout());
    if (!contentLayout) return;

    // Clear existing content (except placeholder)
    while (contentLayout->count() > 2) { // Keep placeholder and stretch
        QLayoutItem* item = contentLayout->takeAt(1); // Remove item after placeholder
        if (item) {
            if (item->widget()) {
                item->widget()->setParent(nullptr);
            }
            delete item;
        }
    }

    if (widget) {
        // Hide placeholder and add new widget
        m_contentLabel->setVisible(false);
        contentLayout->insertWidget(1, widget); // Insert after placeholder, before stretch
    } else {
        // Show placeholder when no content
        m_contentLabel->setVisible(true);
    }
}

QWidget* FluentDrawerWidget::contentWidget() const
{
    return m_contentWidget;
}

void FluentDrawerWidget::onCloseButtonClicked()
{
    emit closeRequested();
    hide();
}

void FluentDrawerWidget::onPrimaryButtonClicked()
{
    emit primaryButtonClicked();
}

void FluentDrawerWidget::onSecondaryButtonClicked()
{
    emit secondaryButtonClicked();
}

void FluentDrawerWidget::onShowAnimationFinished()
{
    m_isAnimating = false;
    m_isVisible = true;
    emit opened();

    disconnect(m_animationGroup, &QParallelAnimationGroup::finished,
               this, &FluentDrawerWidget::onHideAnimationFinished);
}

void FluentDrawerWidget::onHideAnimationFinished()
{
    m_isAnimating = false;
    m_isVisible = false;
    QWidget::hide();
    emit closed();

    disconnect(m_animationGroup, &QParallelAnimationGroup::finished,
               this, &FluentDrawerWidget::onHideAnimationFinished);
}

void FluentDrawerWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void FluentDrawerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

bool FluentDrawerWidget::event(QEvent *event)
{
    return QWidget::event(event);
}

// Property implementations
QString FluentDrawerWidget::title() const { return m_title; }
void FluentDrawerWidget::setTitle(const QString &title)
{
    m_title = title;
    if (m_titleLabel) m_titleLabel->setText(title);
}

QString FluentDrawerWidget::content() const { return m_content; }
void FluentDrawerWidget::setContent(const QString &content)
{
    m_content = content;
    if (m_contentLabel) {
        if (content.isEmpty()) {
            m_contentLabel->setText("Drop widgets here or set custom content");
            m_contentLabel->setStyleSheet("color: #999; font-style: italic;");
        } else {
            m_contentLabel->setText(content);
            m_contentLabel->setStyleSheet(""); // Reset to normal styling
        }
    }
}

QString FluentDrawerWidget::primaryButtonText() const { return m_primaryButtonText; }
void FluentDrawerWidget::setPrimaryButtonText(const QString &text)
{
    m_primaryButtonText = text;
    if (m_primaryButton) m_primaryButton->setText(text);
}

QString FluentDrawerWidget::secondaryButtonText() const { return m_secondaryButtonText; }
void FluentDrawerWidget::setSecondaryButtonText(const QString &text)
{
    m_secondaryButtonText = text;
    if (m_secondaryButton) m_secondaryButton->setText(text);
}

bool FluentDrawerWidget::isDarkMode() const { return m_darkMode; }
void FluentDrawerWidget::setDarkMode(bool dark)
{
    m_darkMode = dark;
    updateTheme();
}

QColor FluentDrawerWidget::accentColor() const { return m_accentColor; }
void FluentDrawerWidget::setAccentColor(const QColor &color)
{
    m_accentColor = color;
    updateStyles();
}

FluentDrawerWidget::DrawerPosition FluentDrawerWidget::position() const { return m_position; }
void FluentDrawerWidget::setPosition(DrawerPosition pos)
{
    m_position = pos;
}

bool FluentDrawerWidget::showNavigationButtons() const { return m_showNavigationButtons; }
void FluentDrawerWidget::setShowNavigationButtons(bool show)
{
    m_showNavigationButtons = show;
    updateNavigationButtons();
}

bool FluentDrawerWidget::isModal() const { return m_modal; }
void FluentDrawerWidget::setModal(bool modal)
{
    m_modal = modal;
}

QSize FluentDrawerWidget::sizeHint() const
{
    return QSize(400, 600);
}

QSize FluentDrawerWidget::minimumSizeHint() const
{
    return QSize(300, 400);
}