#include "fluentbuttonwidgetplugin.h"
#include "../widget/fluentbuttonwidget.h"
#include <QtPlugin>

FluentButtonWidgetPlugin::FluentButtonWidgetPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
}

void FluentButtonWidgetPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core)
    if (m_initialized)
        return;
    m_initialized = true;
}

bool FluentButtonWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *FluentButtonWidgetPlugin::createWidget(QWidget *parent)
{
    return new FluentButtonWidget(parent);
}

QString FluentButtonWidgetPlugin::name() const
{
    return "FluentButtonWidget";
}

QString FluentButtonWidgetPlugin::group() const
{
    return "Fluent Widgets";
}

QIcon FluentButtonWidgetPlugin::icon() const
{
    return QIcon(":/widget_icons/icon-button-widget.png");
}

QString FluentButtonWidgetPlugin::toolTip() const
{
    return "Modern Fluent UI style button with multiple variants";
}

QString FluentButtonWidgetPlugin::whatsThis() const
{
    return "FluentButtonWidget provides a modern button with Fluent Design System styling. "
           "Supports Primary, Secondary, Outline, Subtle, and Transparent styles with "
           "proper hover, press, and focus states. Includes dark/light theme support, "
           "custom accent colors, and icon support.";
}

bool FluentButtonWidgetPlugin::isContainer() const
{
    return false;
}

QString FluentButtonWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\" displayname=\"Fluent Button Widget\">\n"
           " <widget class=\"FluentButtonWidget\" name=\"fluentButton\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>100</width>\n"
           "    <height>32</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"text\">\n"
           "   <string>Button</string>\n"
           "  </property>\n"
           "  <property name=\"buttonStyle\">\n"
           "   <enum>FluentButtonWidget::Primary</enum>\n"
           "  </property>\n"
           "  <property name=\"buttonSize\">\n"
           "   <enum>FluentButtonWidget::Medium</enum>\n"
           "  </property>\n"
           "  <property name=\"darkMode\">\n"
           "   <bool>false</bool>\n"
           "  </property>\n"
           "  <property name=\"iconOnly\">\n"
           "   <bool>false</bool>\n"
           "  </property>\n"
           "  <property name=\"iconResource\">\n"
           "   <string></string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

QString FluentButtonWidgetPlugin::includeFile() const
{
    return "fluentbuttonwidget.h";
}
