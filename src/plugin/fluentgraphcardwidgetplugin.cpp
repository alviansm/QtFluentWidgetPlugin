#include "fluentgraphcardwidgetplugin.h"
#include "../widget/fluentgraphcardwidget.h"
#include <QtPlugin>

FluentGraphCardWidgetPlugin::FluentGraphCardWidgetPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
}

void FluentGraphCardWidgetPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core)
    if (m_initialized)
        return;
    m_initialized = true;
}

bool FluentGraphCardWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *FluentGraphCardWidgetPlugin::createWidget(QWidget *parent)
{
    return new FluentGraphCardWidget(parent);
}

QString FluentGraphCardWidgetPlugin::name() const
{
    return "FluentGraphCardWidget";
}

QString FluentGraphCardWidgetPlugin::group() const
{
    return "Fluent Widgets";
}

QIcon FluentGraphCardWidgetPlugin::icon() const
{
    return QIcon(":/widget_icons/icon-graph-widget.png");
}

QString FluentGraphCardWidgetPlugin::toolTip() const
{
    return "Modern graph card widget with multiple chart types and Fluent UI design";
}

QString FluentGraphCardWidgetPlugin::whatsThis() const
{
    return "FluentGraphCardWidget provides a card-style container for displaying various chart types "
           "including line, bar, pie, scatter, and area charts. Features customizable theming, "
           "animations, and integrates Qt Charts for professional data visualization.";
}

bool FluentGraphCardWidgetPlugin::isContainer() const
{
    return false;
}

QString FluentGraphCardWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\" displayname=\"Fluent Graph Card Widget\">\n"
           " <widget class=\"FluentGraphCardWidget\" name=\"fluentGraphCardWidget\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>400</width>\n"
           "    <height>300</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"title\">\n"
           "   <string>Chart Title</string>\n"
           "  </property>\n"
           "  <property name=\"subtitle\">\n"
           "   <string>Chart Description</string>\n"
           "  </property>\n"
           "  <property name=\"graphType\">\n"
           "   <enum>FluentGraphCardWidget::LineChart</enum>\n"
           "  </property>\n"
           "  <property name=\"showLegend\">\n"
           "   <bool>true</bool>\n"
           "  </property>\n"
           "  <property name=\"showGrid\">\n"
           "   <bool>true</bool>\n"
           "  </property>\n"
           "  <property name=\"animated\">\n"
           "   <bool>true</bool>\n"
           "  </property>\n"
           "  <property name=\"dataSource\">\n"
           "   <string>Sample Data</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

QString FluentGraphCardWidgetPlugin::includeFile() const
{
    return "fluentgraphcardwidget.h";
}
