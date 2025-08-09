#ifndef FLUENTGRAPHCARDWIDGET_H
#define FLUENTGRAPHCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEnterEvent>
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include <QAbstractItemModel>
#include <QToolTip>
#include <QMouseEvent>

// Forward declarations for Qt Charts
QT_BEGIN_NAMESPACE
class QChart;
class QChartView;
class QAbstractSeries;
QT_END_NAMESPACE

class FluentGraphCardWidget : public QWidget
{
    Q_OBJECT
    Q_ENUMS(GraphType)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)
    Q_PROPERTY(GraphType graphType READ graphType WRITE setGraphType)
    Q_PROPERTY(bool showLegend READ showLegend WRITE setShowLegend)
    Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid)
    Q_PROPERTY(bool darkMode READ isDarkMode WRITE setDarkMode)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor)
    Q_PROPERTY(QString dataSource READ dataSource WRITE setDataSource)
    Q_PROPERTY(bool animated READ isAnimated WRITE setAnimated)

public:
    enum GraphType {
        LineChart,
        BarChart,
        PieChart,
        ScatterChart,
        AreaChart
    };

    explicit FluentGraphCardWidget(QWidget *parent = nullptr);
    virtual ~FluentGraphCardWidget();

    // Property getters/setters
    QString title() const;
    void setTitle(const QString &title);

    QString subtitle() const;
    void setSubtitle(const QString &subtitle);

    GraphType graphType() const;
    void setGraphType(GraphType type);

    bool showLegend() const;
    void setShowLegend(bool show);

    bool showGrid() const;
    void setShowGrid(bool show);

    bool isDarkMode() const;
    void setDarkMode(bool dark);

    QColor accentColor() const;
    void setAccentColor(const QColor &color);

    QString dataSource() const;
    void setDataSource(const QString &source);

    bool isAnimated() const;
    void setAnimated(bool animated);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    // Data management methods
    void addDataPoint(const QString &category, double value);
    void addDataSeries(const QString &name, const QList<QPointF> &points);
    void clearData();
    void loadSampleData();

    // Model data methods
    void setDataModel(QAbstractItemModel *model);
    QAbstractItemModel *dataModel() const;
    void setXColumn(int column);
    void setYColumns(const QList<int> &columns);
    void setSeriesNamesColumn(int column);
    void loadDataFromModel();

public slots:
    void refreshChart();

signals:
    void chartClicked(const QString &series, const QPointF &point);
    void dataPointClicked(const QString &series, int pointIndex, double value);
    void dataPointHovered(const QString &series, int pointIndex, double value);
    void chartHoverLeft();
    void legendClicked(const QString &series);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onHoverAnimationFinished();
    void onModelDataChanged();
    void onModelReset();

private:
    void setupUI();
    void setupChart();
    void setupShadowEffect();
    void setupHoverAnimation();
    void updateTheme();
    void updateStyles();
    void updateChart();
    void createLineChart();
    void createBarChart();
    void createPieChart();
    void createScatterChart();
    void createAreaChart();
    void applyChartTheme();
    void safelyRemoveAllSeries();

    // Interactive methods
    QPair<QString, int> findDataPointAt(const QPoint &pos) const;
    void showDataPointTooltip(const QPoint &pos, const QString &series, int pointIndex, double value);
    bool isPointNearMouse(const QPointF &chartPoint, const QPoint &mousePos, int tolerance = 8) const;

    // UI Components
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QWidget *m_chartView;

    // Layouts
    QVBoxLayout *m_mainLayout;
    QVBoxLayout *m_headerLayout;

    // Effects and animations
    QGraphicsDropShadowEffect *m_shadowEffect;
    QPropertyAnimation *m_hoverAnimation;

    // Properties
    QString m_title;
    QString m_subtitle;
    GraphType m_graphType;
    bool m_showLegend;
    bool m_showGrid;
    bool m_darkMode;
    QColor m_accentColor;
    QString m_dataSource;
    bool m_animated;

    // Data storage
    QMap<QString, QList<QPointF>> m_seriesData;
    QStringList m_categories;
    QStringList m_seriesNames;

    // Data model support
    QAbstractItemModel *m_dataModel;
    int m_xColumn;
    QList<int> m_yColumns;
    int m_seriesNamesColumn;

    // Animation state
    bool m_isHovered;
    qreal m_hoverOffset;

    // Interactive state
    QString m_hoveredSeries;
    int m_hoveredPointIndex;

    // Chart initialization state
    bool m_chartInitialized;
};

#endif // FLUENTGRAPHCARDWIDGET_H
