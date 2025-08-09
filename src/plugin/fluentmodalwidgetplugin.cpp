#include "fluentmodalwidgetplugin.h"
#include "../widget/fluentmodalwidget.h"
#include <QtPlugin>

FluentModalWidgetPlugin::FluentModalWidgetPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
}

void FluentModalWidgetPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core)
    if (m_initialized)
        return;
    m_initialized = true;
}

bool FluentModalWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *FluentModalWidgetPlugin::createWidget(QWidget *parent)
{
    auto *widget = new FluentModalWidget(parent);
    // Pre-configure for designer visibility
    widget->showModal();
    return widget;
}

QString FluentModalWidgetPlugin::name() const
{
    return "FluentModalWidget";
}

QString FluentModalWidgetPlugin::group() const
{
    return "Fluent Widgets";
}

QIcon FluentModalWidgetPlugin::icon() const
{
    return QIcon(":/widget_icons/icon-modal-widget.png"); // You'll need to add this icon to your resources
}

QString FluentModalWidgetPlugin::toolTip() const
{
    return "Modern modal dialog widget with Fluent UI design";
}

QString FluentModalWidgetPlugin::whatsThis() const
{
    return "FluentModalWidget provides a modal dialog with image area, title, description, "
           "step indicators, and action buttons. Supports dark/light themes, animations, "
           "and step-based navigation.";
}

bool FluentModalWidgetPlugin::isContainer() const
{
    return false;
}

QString FluentModalWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\" displayname=\"Fluent Modal Widget\">\n"
           " <widget class=\"FluentModalWidget\" name=\"fluentModalWidget\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>370</width>\n"
           "    <height>345</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"title\">\n"
           "   <string>Discover Copilot, a whole new way to work</string>\n"
           "  </property>\n"
           "  <property name=\"description\">\n"
           "   <string>Explore new ways to work smarter and faster using the power of AI. Copilot in [Word] can help you [get started from scratch], [work from an existing file], [get actionable insights about documents], and more.</string>\n"
           "  </property>\n"
           "  <property name=\"primaryButtonText\">\n"
           "   <string>Next</string>\n"
           "  </property>\n"
           "  <property name=\"secondaryButtonText\">\n"
           "   <string>Try later</string>\n"
           "  </property>\n"
           "  <property name=\"currentStep\">\n"
           "   <number>1</number>\n"
           "  </property>\n"
           "  <property name=\"totalSteps\">\n"
           "   <number>5</number>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

QString FluentModalWidgetPlugin::includeFile() const
{
    return "fluentmodalwidget.h";
}