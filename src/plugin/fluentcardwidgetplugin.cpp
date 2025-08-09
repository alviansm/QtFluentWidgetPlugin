#include "fluentcardwidgetplugin.h"
#include "../widget/fluentcardwidget.h"
#include <QtPlugin>

FluentCardWidgetPlugin::FluentCardWidgetPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
}

void FluentCardWidgetPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core)
    if (m_initialized)
        return;
    m_initialized = true;
}

bool FluentCardWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *FluentCardWidgetPlugin::createWidget(QWidget *parent)
{
    return new FluentCardWidget(parent);
}

QString FluentCardWidgetPlugin::name() const
{
    return "FluentCardWidget";
}

QString FluentCardWidgetPlugin::group() const
{
    return "Fluent Widgets";  // Updated group name
}

QIcon FluentCardWidgetPlugin::icon() const
{
    return QIcon(":/widget_icons/icon-card-widget.png");
}

QString FluentCardWidgetPlugin::toolTip() const
{
    return "Modern card widget with Fluent UI design";
}

QString FluentCardWidgetPlugin::whatsThis() const
{
    return "FluentCardWidget provides a card-style display with title, value, "
           "progress bar, and icon. Supports dark/light themes and hover animations.";
}

bool FluentCardWidgetPlugin::isContainer() const
{
    return false;
}

QString FluentCardWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\" displayname=\"Fluent Card Widget\">\n"
           " <widget class=\"FluentCardWidget\" name=\"fluentCardWidget\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>280</width>\n"
           "    <height>140</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"title\">\n"
           "   <string>VESSEL AVAILABILITY</string>\n"
           "  </property>\n"
           "  <property name=\"value\">\n"
           "   <string>92.4%</string>\n"
           "  </property>\n"
           "  <property name=\"progressLabel\">\n"
           "   <string>Operational Status</string>\n"
           "  </property>\n"
           "  <property name=\"progressValue\">\n"
           "   <number>92</number>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

QString FluentCardWidgetPlugin::includeFile() const
{
    return "fluentcardwidget.h";
}
