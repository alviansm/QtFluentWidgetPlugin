#include "fluentlistcardwidget.h"
#include <QApplication>
#include <QPalette>
#include <QScrollBar>
#include <QResizeEvent>

// FluentListCardItemDelegate Implementation
FluentListCardItemDelegate::FluentListCardItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , m_darkMode(false)
    , m_accentColor(QColor(0, 120, 215))
{
}

void FluentListCardItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = option.rect;
    rect.adjust(16, 4, -16, -4);

    // Background
    QColor bgColor = m_darkMode ? QColor(45, 45, 45) : QColor(255, 255, 255);
    if (option.state & QStyle::State_MouseOver) {
        bgColor = m_darkMode ? QColor(55, 55, 55) : QColor(245, 245, 245);
    }

    painter->fillRect(option.rect, bgColor);

    // Main text
    QString mainText = index.data(Qt::DisplayRole).toString();
    QColor textColor = m_darkMode ? QColor(255, 255, 255) : QColor(26, 26, 26);

    QFont mainFont = option.font;
    mainFont.setPointSize(13);
    mainFont.setWeight(QFont::Normal);

    painter->setFont(mainFont);
    painter->setPen(textColor);

    QRect textRect = rect;
    textRect.setRight(textRect.right() - 100); // Leave space for badge

    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, mainText);

    // Badge (if available)
    QString badgeText = index.data(Qt::UserRole).toString();
    if (!badgeText.isEmpty()) {
        QColor badgeColor = index.data(Qt::UserRole + 1).value<QColor>();
        if (!badgeColor.isValid()) {
            badgeColor = m_accentColor;
        }

        QFont badgeFont = option.font;
        badgeFont.setPointSize(11);
        badgeFont.setWeight(QFont::Medium);

        QFontMetrics badgeFm(badgeFont);
        QSize badgeSize = badgeFm.size(0, badgeText);
        badgeSize.setWidth(badgeSize.width() + 16);
        badgeSize.setHeight(22);

        QRect badgeRect;
        badgeRect.setSize(badgeSize);
        badgeRect.moveCenter(QPoint(rect.right() - badgeSize.width()/2 - 8, rect.center().y()));

        // Draw badge background
        painter->setBrush(badgeColor);
        painter->setPen(Qt::NoPen);
        painter->drawRoundedRect(badgeRect, 11, 11);

        // Draw badge text
        painter->setPen(Qt::white);
        painter->setFont(badgeFont);
        painter->drawText(badgeRect, Qt::AlignCenter, badgeText);
    }

    // Bottom border
    QColor borderColor = m_darkMode ? QColor(64, 64, 64) : QColor(229, 229, 229);
    painter->setPen(QPen(borderColor, 1));
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());

    painter->restore();
}

QSize FluentListCardItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(200, 48);
}

void FluentListCardItemDelegate::setDarkMode(bool dark)
{
    m_darkMode = dark;
}

void FluentListCardItemDelegate::setAccentColor(const QColor &color)
{
    m_accentColor = color;
}

// FluentListCardWidget Implementation
FluentListCardWidget::FluentListCardWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(nullptr)
    , m_headerIconLabel(nullptr)
    , m_listView(nullptr)
    , m_paginationWidget(nullptr)
    , m_previousButton(nullptr)
    , m_nextButton(nullptr)
    , m_dotsLayout(nullptr)
    , m_mainLayout(nullptr)
    , m_headerLayout(nullptr)
    , m_paginationLayout(nullptr)
    , m_shadowEffect(nullptr)
    , m_hoverAnimation(nullptr)
    , m_model(nullptr)
    , m_delegate(nullptr)
    , m_title("Certificates")
    , m_itemsPerPage(4)
    , m_currentPage(0)
    , m_darkMode(false)
    , m_accentColor(QColor(0, 120, 215))
    , m_showPagination(true)
    , m_fontIconCharacter("📋")
    , m_fontIconFont("Segoe UI", 16)
    , m_useFontIcon(true)
    , m_isHovered(false)
{
    // Create default model
    m_model = new QStandardItemModel(this);

    // Create delegate
    m_delegate = new FluentListCardItemDelegate(this);

    setupUI();
    setupShadowEffect();
    setupHoverAnimation();
    updateTheme();

    setFixedSize(380, 320);
    setAttribute(Qt::WA_Hover, true);

    // Add some default items for demonstration
    addItem("Safety Certificate", "2 days", QColor(220, 53, 69));
    addItem("Radio License", "45 days", QColor(255, 193, 7));
    addItem("Load Line Certificate", "8 months", QColor(0, 123, 255));
    addItem("Classification Survey", "2.5 years", QColor(0, 123, 255));
    addItem("Port State Control", "6 months", QColor(40, 167, 69));
}

FluentListCardWidget::~FluentListCardWidget()
{
}

void FluentListCardWidget::setupUI()
{
    // Create main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(20, 16, 20, 16);
    m_mainLayout->setSpacing(12);

    // Header layout with title and icon
    m_headerLayout = new QHBoxLayout();
    m_headerLayout->setContentsMargins(0, 0, 0, 0);

    // Header icon
    m_headerIconLabel = new QLabel();
    m_headerIconLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_headerIconLabel->setFixedSize(24, 24);

    // Title label
    m_titleLabel = new QLabel(m_title);
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    updateHeaderIcon();

    m_headerLayout->addWidget(m_headerIconLabel);
    m_headerLayout->addSpacing(8);
    m_headerLayout->addWidget(m_titleLabel);
    m_headerLayout->addStretch();

    // List view
    m_listView = new QListView();
    m_listView->setModel(m_model);
    m_listView->setItemDelegate(m_delegate);
    m_listView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listView->setFrameStyle(QFrame::NoFrame);
    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listView->setFocusPolicy(Qt::NoFocus);

    // Connect list view signals
    connect(m_listView, &QListView::clicked, this, &FluentListCardWidget::onItemClicked);
    connect(m_listView, &QListView::doubleClicked, this, &FluentListCardWidget::onItemDoubleClicked);

    // Pagination widget
    m_paginationWidget = new QWidget();
    m_paginationLayout = new QHBoxLayout(m_paginationWidget);
    m_paginationLayout->setContentsMargins(0, 8, 0, 0);
    m_paginationLayout->setSpacing(8);

    // Previous button
    m_previousButton = new QPushButton("‹");
    m_previousButton->setFixedSize(32, 32);
    m_previousButton->setFocusPolicy(Qt::NoFocus);
    connect(m_previousButton, &QPushButton::clicked, this, &FluentListCardWidget::onPreviousClicked);

    // Dots layout
    m_dotsLayout = new QHBoxLayout();
    m_dotsLayout->setSpacing(4);

    // Next button
    m_nextButton = new QPushButton("›");
    m_nextButton->setFixedSize(32, 32);
    m_nextButton->setFocusPolicy(Qt::NoFocus);
    connect(m_nextButton, &QPushButton::clicked, this, &FluentListCardWidget::onNextClicked);

    m_paginationLayout->addWidget(m_previousButton);
    m_paginationLayout->addStretch();
    m_paginationLayout->addLayout(m_dotsLayout);
    m_paginationLayout->addStretch();
    m_paginationLayout->addWidget(m_nextButton);

    // Add widgets to main layout
    m_mainLayout->addLayout(m_headerLayout);
    m_mainLayout->addWidget(m_listView, 1);
    m_mainLayout->addWidget(m_paginationWidget);

    // Connect model signals
    connect(m_model, &QStandardItemModel::dataChanged, this, &FluentListCardWidget::onModelDataChanged);
    connect(m_model, &QStandardItemModel::rowsInserted, this, &FluentListCardWidget::onModelDataChanged);
    connect(m_model, &QStandardItemModel::rowsRemoved, this, &FluentListCardWidget::onModelDataChanged);

    updatePagination();
}

void FluentListCardWidget::setupShadowEffect()
{
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(15);
    m_shadowEffect->setXOffset(0);
    m_shadowEffect->setYOffset(2);
    m_shadowEffect->setColor(QColor(0, 0, 0, 30));
    setGraphicsEffect(m_shadowEffect);
}

void FluentListCardWidget::setupHoverAnimation()
{
    m_hoverAnimation = new QPropertyAnimation(this, "pos", this);
    m_hoverAnimation->setDuration(200);
    m_hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);
}

void FluentListCardWidget::updateTheme()
{
    updateStyles();
    if (m_delegate) {
        m_delegate->setDarkMode(m_darkMode);
        m_delegate->setAccentColor(m_accentColor);
    }
    if (m_listView) {
        m_listView->update();
    }
}

void FluentListCardWidget::updateStyles()
{
    QString cardStyle;
    QString buttonStyle;

    if (m_darkMode) {
        // Dark mode styles
        cardStyle = QString(
            "FluentListCardWidget {"
            "    background-color: #2d2d2d;"
            "    border: 1px solid #404040;"
            "    border-radius: 8px;"
            "}"
        );

        buttonStyle = QString(
            "QPushButton {"
            "    background-color: #404040;"
            "    border: 1px solid #555555;"
            "    border-radius: 16px;"
            "    color: #ffffff;"
            "    font-size: 16px;"
            "    font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "    background-color: #505050;"
            "    border-color: %1;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #353535;"
            "}"
            "QPushButton:disabled {"
            "    background-color: #2d2d2d;"
            "    border-color: #333333;"
            "    color: #666666;"
            "}"
        ).arg(m_accentColor.name());

        m_titleLabel->setStyleSheet("color: #ffffff; font-size: 16px; font-weight: 600;");
        m_listView->setStyleSheet("QListView { background-color: transparent; border: none; }");

    } else {
        // Light mode styles
        cardStyle = QString(
            "FluentListCardWidget {"
            "    background-color: #ffffff;"
            "    border: 1px solid #e5e5e5;"
            "    border-radius: 8px;"
            "}"
        );

        buttonStyle = QString(
            "QPushButton {"
            "    background-color: #f8f9fa;"
            "    border: 1px solid #dee2e6;"
            "    border-radius: 16px;"
            "    color: #495057;"
            "    font-size: 16px;"
            "    font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "    background-color: #e9ecef;"
            "    border-color: %1;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #dee2e6;"
            "}"
            "QPushButton:disabled {"
            "    background-color: #ffffff;"
            "    border-color: #e9ecef;"
            "    color: #adb5bd;"
            "}"
        ).arg(m_accentColor.name());

        m_titleLabel->setStyleSheet("color: #1a1a1a; font-size: 16px; font-weight: 600;");
        m_listView->setStyleSheet("QListView { background-color: transparent; border: none; }");
    }

    setStyleSheet(cardStyle);

    if (m_previousButton) m_previousButton->setStyleSheet(buttonStyle);
    if (m_nextButton) m_nextButton->setStyleSheet(buttonStyle);

    // Update dot buttons
    QString dotStyle = QString(
        "QPushButton {"
        "    background-color: %1;"
        "    border: none;"
        "    border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "    background-color: %2;"
        "}"
        "QPushButton[active=\"true\"] {"
        "    background-color: %3;"
        "}"
    ).arg(
        m_darkMode ? "#555555" : "#dee2e6",
        m_darkMode ? "#666666" : "#ced4da",
        m_accentColor.name()
    );

    for (QPushButton *dot : m_dotButtons) {
        dot->setStyleSheet(dotStyle);
    }

    updateHeaderIcon();
}

void FluentListCardWidget::updateHeaderIcon()
{
    if (!m_headerIconLabel) return;

    if (m_useFontIcon) {
        m_headerIconLabel->setPixmap(QPixmap());
        m_headerIconLabel->setText(m_fontIconCharacter);
        m_headerIconLabel->setFont(m_fontIconFont);
        m_headerIconLabel->setScaledContents(false);

        QString iconColor = m_darkMode ? "#b0b0b0" : "#666666";
        m_headerIconLabel->setStyleSheet(QString("color: %1;").arg(iconColor));
    } else {
        m_headerIconLabel->setText("");
        m_headerIconLabel->setStyleSheet("");
        m_headerIconLabel->setScaledContents(true);

        if (!m_headerIcon.isNull()) {
            m_headerIconLabel->setPixmap(m_headerIcon.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

void FluentListCardWidget::updatePagination()
{
    updateListView();
    updatePaginationDots();
    updateNavigationButtons();
}

void FluentListCardWidget::updatePaginationDots()
{
    // Clear existing dots
    for (QPushButton *dot : m_dotButtons) {
        m_dotsLayout->removeWidget(dot);
        dot->deleteLater();
    }
    m_dotButtons.clear();

    if (!m_showPagination) return;

    int pages = totalPages();
    for (int i = 0; i < pages; ++i) {
        QPushButton *dot = new QPushButton();
        dot->setFixedSize(8, 8);
        dot->setFocusPolicy(Qt::NoFocus);
        dot->setProperty("page", i);
        dot->setProperty("active", i == m_currentPage);

        connect(dot, &QPushButton::clicked, this, &FluentListCardWidget::onDotClicked);

        m_dotsLayout->addWidget(dot);
        m_dotButtons.append(dot);
    }

    // Update styles for dots
    updateStyles();
}

void FluentListCardWidget::updateListView()
{
    if (!m_model) return;

    // Hide all items first
    for (int i = 0; i < m_model->rowCount(); ++i) {
        m_listView->setRowHidden(i, true);
    }

    // Show items for current page
    int startIndex = m_currentPage * m_itemsPerPage;
    int endIndex = qMin(startIndex + m_itemsPerPage, m_model->rowCount());

    for (int i = startIndex; i < endIndex; ++i) {
        m_listView->setRowHidden(i, false);
    }
}

void FluentListCardWidget::updateNavigationButtons()
{
    if (!m_showPagination) {
        if (m_paginationWidget) m_paginationWidget->setVisible(false);
        return;
    }

    if (m_paginationWidget) m_paginationWidget->setVisible(true);

    if (m_previousButton) m_previousButton->setEnabled(hasPreviousPage());
    if (m_nextButton) m_nextButton->setEnabled(hasNextPage());
}

void FluentListCardWidget::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event)

    if (!m_isHovered) {
        m_isHovered = true;
        m_shadowEffect->setBlurRadius(25);
        m_shadowEffect->setYOffset(8);
        m_shadowEffect->setColor(QColor(0, 0, 0, 50));

        QPoint currentPos = pos();
        QPoint targetPos = currentPos + QPoint(0, -4);
        m_hoverAnimation->setStartValue(currentPos);
        m_hoverAnimation->setEndValue(targetPos);
        m_hoverAnimation->start();
    }
}

void FluentListCardWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)

    if (m_isHovered) {
        m_isHovered = false;
        m_shadowEffect->setBlurRadius(15);
        m_shadowEffect->setYOffset(2);
        m_shadowEffect->setColor(QColor(0, 0, 0, 30));

        QPoint currentPos = pos();
        QPoint targetPos = currentPos + QPoint(0, 4);
        m_hoverAnimation->setStartValue(currentPos);
        m_hoverAnimation->setEndValue(targetPos);
        m_hoverAnimation->start();
    }
}

void FluentListCardWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void FluentListCardWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateListView();
}

// Slots
void FluentListCardWidget::onPreviousClicked()
{
    previousPage();
}

void FluentListCardWidget::onNextClicked()
{
    nextPage();
}

void FluentListCardWidget::onDotClicked()
{
    QPushButton *dot = qobject_cast<QPushButton*>(sender());
    if (dot) {
        int page = dot->property("page").toInt();
        goToPage(page);
    }
}

void FluentListCardWidget::onItemClicked(const QModelIndex &index)
{
    // Convert to actual model index considering pagination
    int actualRow = m_currentPage * m_itemsPerPage + index.row();
    QModelIndex actualIndex = m_model->index(actualRow, 0);
    emit itemClicked(actualIndex);
}

void FluentListCardWidget::onItemDoubleClicked(const QModelIndex &index)
{
    // Convert to actual model index considering pagination
    int actualRow = m_currentPage * m_itemsPerPage + index.row();
    QModelIndex actualIndex = m_model->index(actualRow, 0);
    emit itemDoubleClicked(actualIndex);
}

void FluentListCardWidget::onModelDataChanged()
{
    updatePagination();
}

// Property implementations
QString FluentListCardWidget::title() const { return m_title; }
void FluentListCardWidget::setTitle(const QString &title)
{
    m_title = title;
    if (m_titleLabel) m_titleLabel->setText(title);
}

int FluentListCardWidget::itemsPerPage() const { return m_itemsPerPage; }
void FluentListCardWidget::setItemsPerPage(int count)
{
    m_itemsPerPage = qMax(1, count);
    updatePagination();
}

int FluentListCardWidget::currentPage() const { return m_currentPage; }
void FluentListCardWidget::setCurrentPage(int page)
{
    int newPage = qBound(0, page, totalPages() - 1);
    if (newPage != m_currentPage) {
        m_currentPage = newPage;
        updatePagination();
        emit pageChanged(m_currentPage);
    }
}

bool FluentListCardWidget::isDarkMode() const { return m_darkMode; }
void FluentListCardWidget::setDarkMode(bool dark)
{
    m_darkMode = dark;
    updateTheme();
}

QColor FluentListCardWidget::accentColor() const { return m_accentColor; }
void FluentListCardWidget::setAccentColor(const QColor &color)
{
    m_accentColor = color;
    updateTheme();
}

bool FluentListCardWidget::showPagination() const { return m_showPagination; }
void FluentListCardWidget::setShowPagination(bool show)
{
    m_showPagination = show;
    updateNavigationButtons();
    updatePaginationDots();
}

QPixmap FluentListCardWidget::headerIcon() const { return m_headerIcon; }
void FluentListCardWidget::setHeaderIcon(const QPixmap &icon)
{
    m_headerIcon = icon;
    m_useFontIcon = false;
    updateHeaderIcon();
}

void FluentListCardWidget::setHeaderIcon(const QString &resourcePath)
{
    QPixmap icon(resourcePath);
    if (!icon.isNull()) {
        setHeaderIcon(icon);
    }
}

void FluentListCardWidget::setHeaderFontIcon(const QString &character, const QFont &font)
{
    m_fontIconCharacter = character;
    m_fontIconFont = font.family().isEmpty() ? QFont("Segoe UI", 16) : font;
    m_useFontIcon = true;
    updateHeaderIcon();
}

QStandardItemModel* FluentListCardWidget::model() const { return m_model; }
void FluentListCardWidget::setModel(QStandardItemModel *model)
{
    if (m_model && m_model->parent() == this) {
        m_model->deleteLater();
    }

    m_model = model;
    if (m_listView) {
        m_listView->setModel(m_model);
    }

    if (m_model) {
        connect(m_model, &QStandardItemModel::dataChanged, this, &FluentListCardWidget::onModelDataChanged);
        connect(m_model, &QStandardItemModel::rowsInserted, this, &FluentListCardWidget::onModelDataChanged);
        connect(m_model, &QStandardItemModel::rowsRemoved, this, &FluentListCardWidget::onModelDataChanged);
    }

    updatePagination();
}

void FluentListCardWidget::addItem(const QString &text, const QString &badge, const QColor &badgeColor)
{
    if (!m_model) return;

    QStandardItem *item = new QStandardItem(text);
    item->setData(badge, Qt::UserRole);
    item->setData(badgeColor, Qt::UserRole + 1);
    item->setEditable(false);

    m_model->appendRow(item);
}

void FluentListCardWidget::clearItems()
{
    if (m_model) {
        m_model->clear();
        m_currentPage = 0;
        updatePagination();
    }
}

int FluentListCardWidget::totalPages() const
{
    if (!m_model || m_itemsPerPage <= 0) return 1;
    return qMax(1, (m_model->rowCount() + m_itemsPerPage - 1) / m_itemsPerPage);
}

bool FluentListCardWidget::hasPreviousPage() const
{
    return m_currentPage > 0;
}

bool FluentListCardWidget::hasNextPage() const
{
    return m_currentPage < totalPages() - 1;
}

void FluentListCardWidget::previousPage()
{
    if (hasPreviousPage()) {
        setCurrentPage(m_currentPage - 1);
    }
}

void FluentListCardWidget::nextPage()
{
    if (hasNextPage()) {
        setCurrentPage(m_currentPage + 1);
    }
}

void FluentListCardWidget::goToPage(int page)
{
    setCurrentPage(page);
}

QSize FluentListCardWidget::sizeHint() const
{
    return QSize(380, 320);
}

QSize FluentListCardWidget::minimumSizeHint() const
{
    return QSize(300, 250);
}