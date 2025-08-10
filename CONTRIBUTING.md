## 🚀 Adding More Widgets

Here's how to add a **MetricWidget** as an example:

### Step 1: Create Widget Implementation

**File: `widgets/metricwidget.h`**
```cpp
#ifndef METRICWIDGET_H
#define METRICWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class MetricWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label WRITE setLabel)
    Q_PROPERTY(QString value READ value WRITE setValue)
    Q_PROPERTY(QString unit READ unit WRITE setUnit)
    Q_PROPERTY(QColor valueColor READ valueColor WRITE setValueColor)
    Q_PROPERTY(bool darkMode READ isDarkMode WRITE setDarkMode)

public:
    explicit MetricWidget(QWidget *parent = nullptr);

    QString label() const { return m_label; }
    void setLabel(const QString &label);

    QString value() const { return m_value; }
    void setValue(const QString &value);

    QString unit() const { return m_unit; }
    void setUnit(const QString &unit);

    QColor valueColor() const { return m_valueColor; }
    void setValueColor(const QColor &color);

    bool isDarkMode() const { return m_darkMode; }
    void setDarkMode(bool dark);

    QSize sizeHint() const override { return QSize(160, 80); }

private:
    void setupUI();
    void updateStyles();

    QLabel *m_labelWidget;
    QLabel *m_valueWidget;
    QLabel *m_unitWidget;
    
    QString m_label;
    QString m_value;
    QString m_unit;
    QColor m_valueColor;
    bool m_darkMode;
};

#endif
```

**File: `widgets/metricwidget.cpp`**
```cpp
#include "metricwidget.h"
#include <QHBoxLayout>

MetricWidget::MetricWidget(QWidget *parent)
    : QWidget(parent)
    , m_label("Metric")
    , m_value("0")
    , m_unit("")
    , m_valueColor(QColor(0, 120, 215))
    , m_darkMode(false)
{
    setupUI();
    updateStyles();
}

void MetricWidget::setupUI()
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 8, 12, 8);
    layout->setSpacing(2);

    m_labelWidget = new QLabel(m_label);
    m_labelWidget->setAlignment(Qt::AlignLeft);

    auto valueLayout = new QHBoxLayout();
    m_valueWidget = new QLabel(m_value);
    m_unitWidget = new QLabel(m_unit);
    
    valueLayout->addWidget(m_valueWidget);
    valueLayout->addWidget(m_unitWidget);
    valueLayout->addStretch();

    layout->addWidget(m_labelWidget);
    layout->addLayout(valueLayout);
    layout->addStretch();
}

void MetricWidget::setLabel(const QString &label)
{
    m_label = label;
    m_labelWidget->setText(label);
}

void MetricWidget::setValue(const QString &value)
{
    m_value = value;
    m_valueWidget->setText(value);
}

void MetricWidget::setUnit(const QString &unit)
{
    m_unit = unit;
    m_unitWidget->setText(unit);
}

void MetricWidget::setValueColor(const QColor &color)
{
    m_valueColor = color;
    updateStyles();
}

void MetricWidget::setDarkMode(bool dark)
{
    m_darkMode = dark;
    updateStyles();
}

void MetricWidget::updateStyles()
{
    QString bgColor = m_darkMode ? "#2d2d2d" : "#ffffff";
    QString borderColor = m_darkMode ? "#404040" : "#e5e5e5";
    QString labelColor = m_darkMode ? "#b0b0b0" : "#666666";
    QString unitColor = m_darkMode ? "#888888" : "#999999";

    setStyleSheet(QString(
        "MetricWidget {"
        "    background-color: %1;"
        "    border: 1px solid %2;"
        "    border-radius: 6px;"
        "}"
    ).arg(bgColor, borderColor));

    m_labelWidget->setStyleSheet(QString("color: %1; font-size: 11px; font-weight: 500;").arg(labelColor));
    m_valueWidget->setStyleSheet(QString("color: %1; font-size: 20px; font-weight: bold;").arg(m_valueColor.name()));
    m_unitWidget->setStyleSheet(QString("color: %1; font-size: 14px; margin-left: 4px;").arg(unitColor));
}
```

### Step 2: Create Plugin for New Widget

**File: `plugin/metricwidgetplugin.h`**
```cpp
#ifndef METRICWIDGETPLUGIN_H
#define METRICWIDGETPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class MetricWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    explicit MetricWidgetPlugin(QObject *parent = nullptr);

    bool isContainer() const override { return false; }
    bool isInitialized() const override { return m_initialized; }
    QIcon icon() const override { return QIcon(":/icons/metricwidget.png"); }
    QString domXml() const override;
    QString group() const override { return "Fluent Widgets"; }
    QString includeFile() const override { return "metricwidget.h"; }
    QString name() const override { return "MetricWidget"; }
    QString toolTip() const override { return "Simple metric display"; }
    QString whatsThis() const override { return "Widget for displaying KPIs with label, value, and unit"; }
    QWidget *createWidget(QWidget *parent) override;
    void initialize(QDesignerFormEditorInterface *) override { m_initialized = true; }

private:
    bool m_initialized = false;
};

#endif
```

**File: `plugin/metricwidgetplugin.cpp`**
```cpp
#include "metricwidgetplugin.h"
#include "../widgets/metricwidget.h"

MetricWidgetPlugin::MetricWidgetPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
}

QWidget *MetricWidgetPlugin::createWidget(QWidget *parent)
{
    return new MetricWidget(parent);
}

QString MetricWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"MetricWidget\" name=\"metricWidget\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect><x>0</x><y>0</y><width>160</width><height>80</height></rect>\n"
           "  </property>\n"
           "  <property name=\"label\"><string>CPU Usage</string></property>\n"
           "  <property name=\"value\"><string>67</string></property>\n"
           "  <property name=\"unit\"><string>%</string></property>\n"
           " </widget>\n"
           "</ui>\n";
}
```

### Step 3: Register New Widget in Collection

**Update: `plugin/fluentwidgetsplugin.cpp`**
```cpp
#include "fluentwidgetsplugin.h"
#include "cardwidgetplugin.h"
#include "metricwidgetplugin.h"  // ← Add this

FluentWidgetsPlugin::FluentWidgetsPlugin(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new CardWidgetPlugin(this));
    m_widgets.append(new MetricWidgetPlugin(this));  // ← Add this
}
```

### Step 4: Update Build Files

**Update `.pro` file**: Uncomment MetricWidget lines
**Update `CMakeLists.txt`**: Uncomment MetricWidget lines

### Step 5: Rebuild and Test

```bash
# Rebuild plugin
qmake FluentWidgetPlugin.pro
nmake clean && nmake release

# Copy to Designer
copy release\fluentwidgetsplugin.dll E:\Qt\6.8.3\msvc2022_64\plugins\designer\
```

**Result**: Qt Designer now shows both **CardWidget** and **MetricWidget** in "Fluent Widgets" group!

## 📋 Development Workflow

1. **Create new widget** in `widgets/` directory
2. **Create plugin class** in `plugin/` directory  
3. **Register in collection** (fluentwidgetsplugin.cpp)
4. **Update build files** (.pro and CMakeLists.txt)
5. **Build and test** in Designer
6. **Document usage** in README