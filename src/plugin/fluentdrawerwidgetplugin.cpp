#include "fluentdrawerwidgetplugin.h"
#include "../widget/fluentdrawerwidget.h"
#include <QtPlugin>

FluentDrawerWidgetPlugin::FluentDrawerWidgetPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
}

void FluentDrawerWidgetPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core)
    if (m_initialized)
        return;
    m_initialized = true;
}

bool FluentDrawerWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *FluentDrawerWidgetPlugin::createWidget(QWidget *parent)
{
    return new FluentDrawerWidget(parent);
}

QString FluentDrawerWidgetPlugin::name() const
{
    return "FluentDrawerWidget";
}

QString FluentDrawerWidgetPlugin::group() const
{
    return "Fluent Widgets";
}

QIcon FluentDrawerWidgetPlugin::icon() const
{
    return QIcon(":/widget_icons/icon-drawer-widget.png");
}

QString FluentDrawerWidgetPlugin::toolTip() const
{
    return "Fluent UI drawer/modal widget with animations";
}

QString FluentDrawerWidgetPlugin::whatsThis() const
{
    return "FluentDrawerWidget provides a slide-in panel with title, content area, "
           "action buttons, and smooth animations. Supports multiple positions "
           "(left, right, top, bottom), dark/light themes, and optional navigation buttons.";
}

bool FluentDrawerWidgetPlugin::isContainer() const
{
    return true;  // Allows dropping other widgets into the content area
}

QString FluentDrawerWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\" displayname=\"Fluent Drawer Widget\">\n"
           " <widget class=\"FluentDrawerWidget\" name=\"fluentDrawerWidget\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>400</width>\n"
           "    <height>600</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"title\">\n"
           "   <string>Settings</string>\n"
           "  </property>\n"
           "  <property name=\"content\">\n"
           "   <string>Configure your application settings and preferences.</string>\n"
           "  </property>\n"
           "  <property name=\"primaryButtonText\">\n"
           "   <string>Save</string>\n"
           "  </property>\n"
           "  <property name=\"secondaryButtonText\">\n"
           "   <string>Cancel</string>\n"
           "  </property>\n"
           "  <property name=\"position\">\n"
           "   <enum>FluentDrawerWidget::Right</enum>\n"
           "  </property>\n"
           "  <property name=\"showNavigationButtons\">\n"
           "   <bool>false</bool>\n"
           "  </property>\n"
           "  <property name=\"modal\">\n"
           "   <bool>true</bool>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

QString FluentDrawerWidgetPlugin::includeFile() const
{
    return "fluentdrawerwidget.h";
}