#include "fluentgraphcardwidget.h"
#include <QApplication>
#include <QPalette>
#include <QRandomGenerator>

// Qt Charts includes
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QPieSeries>
#include <QScatterSeries>
#include <QAreaSeries>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QLegend>

FluentGraphCardWidget::FluentGraphCardWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(nullptr)
    , m_subtitleLabel(nullptr)
    , m_chartView(nullptr)
    , m_mainLayout(nullptr)
    , m_headerLayout(nullptr)
    , m_shadowEffect(nullptr)
    , m_hoverAnimation(nullptr)
    , m_title("Chart Title")
    , m_subtitle("Chart Description")
    , m_graphType(LineChart)
    , m_showLegend(true)
    , m_showGrid(true)
    , m_darkMode(false)
    , m_accentColor(QColor(0, 120, 215))
    , m_dataSource("Sample Data")
    , m_animated(true)
    , m_dataModel(nullptr)
    , m_xColumn(0)
    , m_seriesNamesColumn(-1)
    , m_isHovered(false)
    , m_hoverOffset(0.0)
    , m_hoveredPointIndex(-1)
    , m_chartInitialized(false)
{
    setupUI();
    setupChart();
    m_chartInitialized = true;
    setupShadowEffect();
    setupHoverAnimation();

    // Load sample data first, then apply theme
    loadSampleData();
    updateTheme();

    setFixedSize(400, 300);
    setAttribute(Qt::WA_Hover, true);
    setMouseTracking(true);
}

FluentGraphCardWidget::~FluentGraphCardWidget()
{
    if (m_dataModel) {
        disconnect(m_dataModel, nullptr, this, nullptr);
    }
}

void FluentGraphCardWidget::setupUI()
{
    // Create main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(20, 16, 20, 16);
    m_mainLayout->setSpacing(12);

    // Header layout for title and subtitle
    m_headerLayout = new QVBoxLayout();
    m_headerLayout->setContentsMargins(0, 0, 0, 0);
    m_headerLayout->setSpacing(4);

    // Title label
    m_titleLabel = new QLabel(m_title);
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Subtitle label
    m_subtitleLabel = new QLabel(m_subtitle);
    m_subtitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    m_headerLayout->addWidget(m_titleLabel);
    m_headerLayout->addWidget(m_subtitleLabel);

    // Add header to main layout
    m_mainLayout->addLayout(m_headerLayout);
}

void FluentGraphCardWidget::setupChart()
{
    // Create chart
    QChart *chart = new QChart();
    chart->setAnimationOptions(m_animated ? QChart::AllAnimations : QChart::NoAnimation);

    // Create chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(200);
    m_chartView = chartView; // Store as QWidget*

    // Add chart view to main layout
    m_mainLayout->addWidget(m_chartView, 1);
}

void FluentGraphCardWidget::setupShadowEffect()
{
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(15);
    m_shadowEffect->setXOffset(0);
    m_shadowEffect->setYOffset(2);
    m_shadowEffect->setColor(QColor(0, 0, 0, 30));
    setGraphicsEffect(m_shadowEffect);
}

void FluentGraphCardWidget::setupHoverAnimation()
{
    m_hoverAnimation = new QPropertyAnimation(this, "pos", this);
    m_hoverAnimation->setDuration(200);
    m_hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);

    connect(m_hoverAnimation, &QPropertyAnimation::finished,
            this, &FluentGraphCardWidget::onHoverAnimationFinished);
}

void FluentGraphCardWidget::updateTheme()
{
    updateStyles();
    applyChartTheme();
}

void FluentGraphCardWidget::updateStyles()
{
    QString cardStyle;

    if (m_darkMode) {
        // Dark mode styles
        cardStyle = QString(
            "FluentGraphCardWidget {"
            "    background-color: #2d2d2d;"
            "    border: 1px solid #404040;"
            "    border-radius: 8px;"
            "}"
        );

        m_titleLabel->setStyleSheet("color: #ffffff; font-size: 16px; font-weight: 600;");
        m_subtitleLabel->setStyleSheet("color: #b0b0b0; font-size: 12px; font-weight: 400;");

    } else {
        // Light mode styles
        cardStyle = QString(
            "FluentGraphCardWidget {"
            "    background-color: #ffffff;"
            "    border: 1px solid #e5e5e5;"
            "    border-radius: 8px;"
            "}"
        );

        m_titleLabel->setStyleSheet("color: #1a1a1a; font-size: 16px; font-weight: 600;");
        m_subtitleLabel->setStyleSheet("color: #666666; font-size: 12px; font-weight: 400;");
    }

    setStyleSheet(cardStyle);
}

void FluentGraphCardWidget::applyChartTheme()
{
    QChartView *chartView = qobject_cast<QChartView*>(m_chartView);
    if (!chartView) return;

    QChart *chart = chartView->chart();
    if (!chart) return;

    if (m_darkMode) {
        chart->setTheme(QChart::ChartThemeDark);
        chart->setBackgroundBrush(QBrush(QColor(45, 45, 45)));
        chartView->setStyleSheet("QChartView { background-color: #2d2d2d; border: none; }");
    } else {
        chart->setTheme(QChart::ChartThemeLight);
        chart->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
        chartView->setStyleSheet("QChartView { background-color: #ffffff; border: none; }");
    }

    // Configure legend
    if (chart->legend()) {
        chart->legend()->setVisible(m_showLegend);
        chart->legend()->setAlignment(Qt::AlignBottom);
    }

    updateChart();
}

void FluentGraphCardWidget::updateChart()
{
    QChartView *chartView = qobject_cast<QChartView*>(m_chartView);
    if (!chartView) return;

    QChart *chart = chartView->chart();
    if (!chart) return;

    // Safely remove existing series
    safelyRemoveAllSeries();

    // Ensure we have data before creating charts
    if (m_seriesData.isEmpty()) {
        loadSampleData();
    }

    // Create appropriate chart type with error handling
    try {
        switch (m_graphType) {
            case LineChart:
                createLineChart();
                break;
            case BarChart:
                createBarChart();
                break;
            case PieChart:
                createPieChart();
                break;
            case ScatterChart:
                createScatterChart();
                break;
            case AreaChart:
                createAreaChart();
                break;
        }
    } catch (...) {
        // Fallback to line chart if creation fails
        if (m_graphType != LineChart) {
            m_graphType = LineChart;
            createLineChart();
        }
    }

    // Update chart properties
    chart->setAnimationOptions(m_animated ? QChart::AllAnimations : QChart::NoAnimation);
}

void FluentGraphCardWidget::createLineChart()
{
    QChartView *chartView = qobject_cast<QChartView*>(m_chartView);
    if (!chartView) return;

    QChart *chart = chartView->chart();
    if (!chart) return;

    for (auto it = m_seriesData.constBegin(); it != m_seriesData.constEnd(); ++it) {
        QLineSeries *series = new QLineSeries();
        series->setName(it.key());

        for (const QPointF &point : it.value()) {
            series->append(point);
        }

        chart->addSeries(series);
    }

    chart->createDefaultAxes();

    // Configure axes
    if (!chart->axes(Qt::Horizontal).isEmpty()) {
        QValueAxis *axisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());
        if (axisX) {
            axisX->setGridLineVisible(m_showGrid);
        }
    }

    if (!chart->axes(Qt::Vertical).isEmpty()) {
        QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
        if (axisY) {
            axisY->setGridLineVisible(m_showGrid);
        }
    }
}

void FluentGraphCardWidget::createBarChart()
{
    QChartView *chartView = qobject_cast<QChartView*>(m_chartView);
    if (!chartView) return;

    QChart *chart = chartView->chart();
    if (!chart) return;

    QBarSeries *barSeries = new QBarSeries();

    for (auto it = m_seriesData.constBegin(); it != m_seriesData.constEnd(); ++it) {
        QBarSet *barSet = new QBarSet(it.key());

        for (const QPointF &point : it.value()) {
            barSet->append(point.y());
        }

        barSeries->append(barSet);
    }

    chart->addSeries(barSeries);
    chart->createDefaultAxes();

    // Set categories for bar chart
    if (!m_categories.isEmpty() && !chart->axes(Qt::Horizontal).isEmpty()) {
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(m_categories);
        chart->setAxisX(axisX, barSeries);
    }
}

void FluentGraphCardWidget::createPieChart()
{
    QChartView *chartView = qobject_cast<QChartView*>(m_chartView);
    if (!chartView) return;

    QChart *chart = chartView->chart();
    if (!chart || m_seriesData.isEmpty()) return;

    QPieSeries *pieSeries = new QPieSeries();

    // Use first series data for pie chart
    auto firstSeries = m_seriesData.constBegin();
    for (int i = 0; i < firstSeries.value().size() && i < m_categories.size(); ++i) {
        QString label = m_categories[i];
        double value = firstSeries.value()[i].y();
        pieSeries->append(label, value);
    }

    // Highlight first slice
    if (pieSeries->count() > 0) {
        QPieSlice *firstSlice = pieSeries->slices().at(0);
        firstSlice->setExploded(true);
        firstSlice->setLabelVisible(true);
    }

    chart->addSeries(pieSeries);
}

void FluentGraphCardWidget::createScatterChart()
{
    QChartView *chartView = qobject_cast<QChartView*>(m_chartView);
    if (!chartView) return;

    QChart *chart = chartView->chart();
    if (!chart) return;

    for (auto it = m_seriesData.constBegin(); it != m_seriesData.constEnd(); ++it) {
        QScatterSeries *series = new QScatterSeries();
        series->setName(it.key());
        series->setMarkerSize(8.0);

        for (const QPointF &point : it.value()) {
            series->append(point);
        }

        chart->addSeries(series);
    }

    chart->createDefaultAxes();
}

void FluentGraphCardWidget::createAreaChart()
{
    QChartView *chartView = qobject_cast<QChartView*>(m_chartView);
    if (!chartView) return;

    QChart *chart = chartView->chart();
    if (!chart) return;

    // Validate data availability
    if (m_seriesData.isEmpty()) {
        loadSampleData();
        if (m_seriesData.isEmpty()) {
            return; // Still no data, abort
        }
    }

    // Get the first series with valid data
    auto firstSeries = m_seriesData.constBegin();
    if (firstSeries == m_seriesData.constEnd() || firstSeries.value().isEmpty()) {
        return; // No valid data
    }

    // Create the upper boundary line series
    QLineSeries *upperSeries = new QLineSeries();
    upperSeries->setName(firstSeries.key());

    // Add points with validation
    for (const QPointF &point : firstSeries.value()) {
        // Validate point data
        if (std::isfinite(point.x()) && std::isfinite(point.y())) {
            upperSeries->append(point);
        }
    }

    // Only create area series if we have valid points
    if (upperSeries->count() == 0) {
        delete upperSeries;
        return;
    }

    // Create lower boundary (baseline at 0) if we want a filled area from zero
    QLineSeries *lowerSeries = new QLineSeries();
    for (const QPointF &point : firstSeries.value()) {
        if (std::isfinite(point.x())) {
            lowerSeries->append(QPointF(point.x(), 0));
        }
    }

    // Create area series with both upper and lower boundaries
    QAreaSeries *areaSeries = nullptr;
    if (lowerSeries->count() > 0) {
        areaSeries = new QAreaSeries(upperSeries, lowerSeries);
    } else {
        delete lowerSeries;
        areaSeries = new QAreaSeries(upperSeries);
    }

    if (!areaSeries) {
        delete upperSeries;
        return;
    }

    areaSeries->setName(firstSeries.key());

    // Set area color with transparency
    QColor areaColor = m_accentColor;
    areaColor.setAlpha(100); // Make it semi-transparent
    areaSeries->setBrush(QBrush(areaColor));

    // Set border color
    QPen pen(m_accentColor);
    pen.setWidth(2);
    areaSeries->setPen(pen);

    // Add series to chart
    chart->addSeries(areaSeries);

    // Create axes after adding series
    chart->createDefaultAxes();

    // Configure axes if they exist
    auto horizontalAxes = chart->axes(Qt::Horizontal);
    auto verticalAxes = chart->axes(Qt::Vertical);

    for (auto axis : horizontalAxes) {
        if (QValueAxis *valueAxis = qobject_cast<QValueAxis*>(axis)) {
            valueAxis->setGridLineVisible(m_showGrid);
        }
    }

    for (auto axis : verticalAxes) {
        if (QValueAxis *valueAxis = qobject_cast<QValueAxis*>(axis)) {
            valueAxis->setGridLineVisible(m_showGrid);
        }
    }
}

void FluentGraphCardWidget::loadSampleData()
{
    clearData();

    // Add sample categories
    m_categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
    m_seriesNames.clear();

    // Generate sample data based on chart type
    switch (m_graphType) {
        case LineChart:
        case ScatterChart:
        case AreaChart: {
            QList<QPointF> series1, series2;
            for (int i = 0; i < 6; ++i) {
                series1.append(QPointF(i, QRandomGenerator::global()->bounded(20, 80)));
                series2.append(QPointF(i, QRandomGenerator::global()->bounded(10, 60)));
            }
            m_seriesData["Revenue"] = series1;
            m_seriesData["Expenses"] = series2;
            m_seriesNames << "Revenue" << "Expenses";
            break;
        }
        case BarChart: {
            QList<QPointF> series1, series2;
            for (int i = 0; i < 6; ++i) {
                series1.append(QPointF(i, QRandomGenerator::global()->bounded(10, 50)));
                series2.append(QPointF(i, QRandomGenerator::global()->bounded(5, 30)));
            }
            m_seriesData["Sales"] = series1;
            m_seriesData["Target"] = series2;
            m_seriesNames << "Sales" << "Target";
            break;
        }
        case PieChart: {
            QList<QPointF> series1;
            series1.append(QPointF(0, 30));
            series1.append(QPointF(1, 25));
            series1.append(QPointF(2, 20));
            series1.append(QPointF(3, 15));
            series1.append(QPointF(4, 10));
            m_seriesData["Distribution"] = series1;
            m_categories.clear();
            m_categories << "Category A" << "Category B" << "Category C" << "Category D" << "Category E";
            break;
        }
    }

    refreshChart();
}

// Property implementations
QString FluentGraphCardWidget::title() const { return m_title; }
void FluentGraphCardWidget::setTitle(const QString &title)
{
    m_title = title;
    if (m_titleLabel) m_titleLabel->setText(title);
}

QString FluentGraphCardWidget::subtitle() const { return m_subtitle; }
void FluentGraphCardWidget::setSubtitle(const QString &subtitle)
{
    m_subtitle = subtitle;
    if (m_subtitleLabel) m_subtitleLabel->setText(subtitle);
}

FluentGraphCardWidget::GraphType FluentGraphCardWidget::graphType() const { return m_graphType; }
void FluentGraphCardWidget::setGraphType(GraphType type)
{
    if (m_graphType == type) return;

    m_graphType = type;

    // Only update if the widget is fully initialized
    if (m_chartInitialized && m_chartView) {
        // Ensure we have appropriate data for the new chart type
        if (m_seriesData.isEmpty() ||
            (type == PieChart && m_categories.isEmpty())) {
            loadSampleData();
        }
        updateChart();
    }
}

bool FluentGraphCardWidget::showLegend() const { return m_showLegend; }
void FluentGraphCardWidget::setShowLegend(bool show)
{
    m_showLegend = show;
    applyChartTheme();
}

bool FluentGraphCardWidget::showGrid() const { return m_showGrid; }
void FluentGraphCardWidget::setShowGrid(bool show)
{
    m_showGrid = show;
    updateChart();
}

bool FluentGraphCardWidget::isDarkMode() const { return m_darkMode; }
void FluentGraphCardWidget::setDarkMode(bool dark)
{
    m_darkMode = dark;
    updateTheme();
}

QColor FluentGraphCardWidget::accentColor() const { return m_accentColor; }
void FluentGraphCardWidget::setAccentColor(const QColor &color)
{
    m_accentColor = color;
    updateChart();
}

QString FluentGraphCardWidget::dataSource() const { return m_dataSource; }
void FluentGraphCardWidget::setDataSource(const QString &source)
{
    m_dataSource = source;

    // If we have a data model, try to load from it
    if (m_dataModel) {
        loadDataFromModel();
    } else {
        // Fallback to sample data
        loadSampleData();
    }
}

bool FluentGraphCardWidget::isAnimated() const { return m_animated; }
void FluentGraphCardWidget::setAnimated(bool animated)
{
    m_animated = animated;
    QChartView *chartView = qobject_cast<QChartView*>(m_chartView);
    if (chartView) {
        QChart *chart = chartView->chart();
        if (chart) {
            chart->setAnimationOptions(animated ? QChart::AllAnimations : QChart::NoAnimation);
        }
    }
}

void FluentGraphCardWidget::addDataPoint(const QString &category, double value)
{
    if (!m_categories.contains(category)) {
        m_categories.append(category);
    }

    // Add to default series
    if (!m_seriesData.contains("Data")) {
        m_seriesData["Data"] = QList<QPointF>();
        m_seriesNames << "Data";
    }

    int index = m_categories.indexOf(category);
    m_seriesData["Data"].append(QPointF(index, value));
}

void FluentGraphCardWidget::addDataSeries(const QString &name, const QList<QPointF> &points)
{
    m_seriesData[name] = points;
    if (!m_seriesNames.contains(name)) {
        m_seriesNames << name;
    }
}

void FluentGraphCardWidget::clearData()
{
    m_seriesData.clear();
    m_categories.clear();
    m_seriesNames.clear();
}

void FluentGraphCardWidget::refreshChart()
{
    updateChart();
}

// Data Model Support Implementation
void FluentGraphCardWidget::setDataModel(QAbstractItemModel *model)
{
    if (m_dataModel) {
        disconnect(m_dataModel, nullptr, this, nullptr);
    }

    m_dataModel = model;

    if (m_dataModel) {
        connect(m_dataModel, &QAbstractItemModel::dataChanged, this, &FluentGraphCardWidget::onModelDataChanged);
        connect(m_dataModel, &QAbstractItemModel::modelReset, this, &FluentGraphCardWidget::onModelReset);
        connect(m_dataModel, &QAbstractItemModel::rowsInserted, this, &FluentGraphCardWidget::onModelDataChanged);
        connect(m_dataModel, &QAbstractItemModel::rowsRemoved, this, &FluentGraphCardWidget::onModelDataChanged);

        loadDataFromModel();
    } else {
        loadSampleData();
    }
}

QAbstractItemModel *FluentGraphCardWidget::dataModel() const
{
    return m_dataModel;
}

void FluentGraphCardWidget::setXColumn(int column)
{
    m_xColumn = column;
    if (m_dataModel) {
        loadDataFromModel();
    }
}

void FluentGraphCardWidget::setYColumns(const QList<int> &columns)
{
    m_yColumns = columns;
    if (m_dataModel) {
        loadDataFromModel();
    }
}

void FluentGraphCardWidget::setSeriesNamesColumn(int column)
{
    m_seriesNamesColumn = column;
    if (m_dataModel) {
        loadDataFromModel();
    }
}

void FluentGraphCardWidget::loadDataFromModel()
{
    if (!m_dataModel) {
        loadSampleData();
        return;
    }

    clearData();

    int rowCount = m_dataModel->rowCount();
    if (rowCount == 0 || m_yColumns.isEmpty()) {
        loadSampleData();
        return;
    }

    // Load categories from X column
    for (int row = 0; row < rowCount; ++row) {
        QModelIndex xIndex = m_dataModel->index(row, m_xColumn);
        QString category = m_dataModel->data(xIndex).toString();
        if (!category.isEmpty()) {
            m_categories.append(category);
        }
    }

    // Load data series from Y columns
    for (int i = 0; i < m_yColumns.size(); ++i) {
        int yColumn = m_yColumns[i];
        QString seriesName;

        // Get series name from header or dedicated column
        if (m_seriesNamesColumn >= 0 && i < m_dataModel->rowCount()) {
            QModelIndex nameIndex = m_dataModel->index(i, m_seriesNamesColumn);
            seriesName = m_dataModel->data(nameIndex).toString();
        }

        if (seriesName.isEmpty()) {
            seriesName = m_dataModel->headerData(yColumn, Qt::Horizontal).toString();
            if (seriesName.isEmpty()) {
                seriesName = QString("Series %1").arg(i + 1);
            }
        }

        QList<QPointF> points;
        for (int row = 0; row < rowCount; ++row) {
            QModelIndex yIndex = m_dataModel->index(row, yColumn);
            bool ok;
            double value = m_dataModel->data(yIndex).toDouble(&ok);
            if (ok) {
                points.append(QPointF(row, value));
            }
        }

        if (!points.isEmpty()) {
            m_seriesData[seriesName] = points;
            m_seriesNames.append(seriesName);
        }
    }

    refreshChart();
}

void FluentGraphCardWidget::onModelDataChanged()
{
    loadDataFromModel();
}

void FluentGraphCardWidget::onModelReset()
{
    loadDataFromModel();
}

// Interactive Features Implementation
QPair<QString, int> FluentGraphCardWidget::findDataPointAt(const QPoint &pos) const
{
    QChartView *chartView = qobject_cast<QChartView*>(m_chartView);
    if (!chartView || m_seriesData.isEmpty()) {
        return QPair<QString, int>(QString(), -1);
    }

    QChart *chart = chartView->chart();
    if (!chart) return QPair<QString, int>(QString(), -1);

    // Convert widget coordinates to chart coordinates
    QPointF chartPos = chart->mapFromScene(chartView->mapToScene(pos));

    // Check each series for nearby points
    for (auto it = m_seriesData.constBegin(); it != m_seriesData.constEnd(); ++it) {
        const QString &seriesName = it.key();
        const QList<QPointF> &points = it.value();

        // Find corresponding series in chart
        QList<QAbstractSeries*> allSeries = chart->series();
        for (QAbstractSeries *abstractSeries : allSeries) {
            if (abstractSeries->name() == seriesName) {
                // Check if this is a line series for point detection
                if (QLineSeries *lineSeries = qobject_cast<QLineSeries*>(abstractSeries)) {
                    QList<QPointF> chartPoints = lineSeries->points();
                    for (int i = 0; i < chartPoints.size(); ++i) {
                        QPointF scenePoint = chart->mapToPosition(chartPoints[i], abstractSeries);
                        if (isPointNearMouse(scenePoint, pos, 10)) {
                            return QPair<QString, int>(seriesName, i);
                        }
                    }
                }
                // Check scatter series
                else if (QScatterSeries *scatterSeries = qobject_cast<QScatterSeries*>(abstractSeries)) {
                    QList<QPointF> chartPoints = scatterSeries->points();
                    for (int i = 0; i < chartPoints.size(); ++i) {
                        QPointF scenePoint = chart->mapToPosition(chartPoints[i], abstractSeries);
                        if (isPointNearMouse(scenePoint, pos, 10)) {
                            return QPair<QString, int>(seriesName, i);
                        }
                    }
                }
                break;
            }
        }
    }

    return QPair<QString, int>(QString(), -1);
}

void FluentGraphCardWidget::showDataPointTooltip(const QPoint &pos, const QString &series, int pointIndex, double value)
{
    QString tooltipText;

    if (pointIndex >= 0 && pointIndex < m_categories.size()) {
        tooltipText = QString("%1\n%2: %3")
                     .arg(m_categories[pointIndex])
                     .arg(series)
                     .arg(QString::number(value, 'f', 2));
    } else {
        tooltipText = QString("%1: %2")
                     .arg(series)
                     .arg(QString::number(value, 'f', 2));
    }

    QToolTip::showText(mapToGlobal(pos), tooltipText, this);
}

bool FluentGraphCardWidget::isPointNearMouse(const QPointF &chartPoint, const QPoint &mousePos, int tolerance) const
{
    return (QPointF(mousePos) - chartPoint).manhattanLength() <= tolerance;
}

void FluentGraphCardWidget::enterEvent(QEnterEvent *event)
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

void FluentGraphCardWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)

    // Reset hover state when mouse leaves the widget
    if (m_hoveredPointIndex >= 0) {
        m_hoveredSeries.clear();
        m_hoveredPointIndex = -1;
        QToolTip::hideText();
        emit chartHoverLeft();
        setCursor(Qt::ArrowCursor);
    }

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

void FluentGraphCardWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Find clicked data point
        QPair<QString, int> hitResult = findDataPointAt(event->pos());

        if (!hitResult.first.isEmpty() && hitResult.second >= 0) {
            // Clicked on a data point
            const QString &seriesName = hitResult.first;
            int pointIndex = hitResult.second;

            if (m_seriesData.contains(seriesName) && pointIndex < m_seriesData[seriesName].size()) {
                double value = m_seriesData[seriesName][pointIndex].y();
                emit dataPointClicked(seriesName, pointIndex, value);
            }
        }

        // Also emit the general chart clicked signal
        emit chartClicked("Chart", event->pos());
    }
    QWidget::mousePressEvent(event);
}

void FluentGraphCardWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Find hovered data point
    QPair<QString, int> hitResult = findDataPointAt(event->pos());

    if (!hitResult.first.isEmpty() && hitResult.second >= 0) {
        // Hovering over a data point
        const QString &seriesName = hitResult.first;
        int pointIndex = hitResult.second;

        // Check if this is a new hover target
        if (seriesName != m_hoveredSeries || pointIndex != m_hoveredPointIndex) {
            m_hoveredSeries = seriesName;
            m_hoveredPointIndex = pointIndex;

            if (m_seriesData.contains(seriesName) && pointIndex < m_seriesData[seriesName].size()) {
                double value = m_seriesData[seriesName][pointIndex].y();

                // Show tooltip
                showDataPointTooltip(event->pos(), seriesName, pointIndex, value);

                // Emit hover signal
                emit dataPointHovered(seriesName, pointIndex, value);

                // Change cursor to indicate interactivity
                setCursor(Qt::PointingHandCursor);
            }
        }
    } else {
        // Not hovering over any data point
        if (m_hoveredPointIndex >= 0) {
            // We were hovering before, now we're not
            m_hoveredSeries.clear();
            m_hoveredPointIndex = -1;

            // Hide tooltip
            QToolTip::hideText();

            // Emit hover left signal
            emit chartHoverLeft();

            // Reset cursor
            setCursor(Qt::ArrowCursor);
        }
    }

    QWidget::mouseMoveEvent(event);
}

void FluentGraphCardWidget::onHoverAnimationFinished()
{
    // Animation finished
}

void FluentGraphCardWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void FluentGraphCardWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // Chart view will automatically resize with the widget
}

QSize FluentGraphCardWidget::sizeHint() const
{
    return QSize(400, 300);
}

QSize FluentGraphCardWidget::minimumSizeHint() const
{
    return QSize(300, 250);
}

void FluentGraphCardWidget::safelyRemoveAllSeries()
{
    QChartView *chartView = qobject_cast<QChartView*>(m_chartView);
    if (!chartView) return;

    QChart *chart = chartView->chart();
    if (!chart) return;

    // Get all series before removing them
    QList<QAbstractSeries*> seriesList = chart->series();

    // Remove each series individually to avoid issues
    for (QAbstractSeries *series : seriesList) {
        if (series) {
            chart->removeSeries(series);
            series->deleteLater(); // Schedule for deletion
        }
    }
}