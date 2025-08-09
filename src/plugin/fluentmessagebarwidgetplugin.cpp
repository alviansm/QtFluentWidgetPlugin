#include "fluentmessagebarwidgetplugin.h"
#include "../widget/fluentmessagebarwidget.h"
#include <QtPlugin>

FluentMessageBarWidgetPlugin::FluentMessageBarWidgetPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
}

void FluentMessageBarWidgetPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core)
    if (m_initialized)
        return;
    m_initialized = true;
}

bool FluentMessageBarWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *FluentMessageBarWidgetPlugin::createWidget(QWidget *parent)
{
    return new FluentMessageBarWidget(parent);
}

QString FluentMessageBarWidgetPlugin::name() const
{
    return "FluentMessageBarWidget";
}

QString FluentMessageBarWidgetPlugin::group() const
{
    return "Fluent Widgets";
}

QIcon FluentMessageBarWidgetPlugin::icon() const
{
    return QIcon(":/widget_icons/icon-message_bar-widget.png");
}

QString FluentMessageBarWidgetPlugin::toolTip() const
{
    return "Modern message bar widget with Fluent UI design";
}

QString FluentMessageBarWidgetPlugin::whatsThis() const
{
    return "FluentMessageBarWidget provides a notification bar with different message types "
           "(Info, Success, Warning, Error), customizable actions, and dismiss functionality. "
           "Supports dark/light themes and smooth animations.";
}

bool FluentMessageBarWidgetPlugin::isContainer() const
{
    return false;
}

QString FluentMessageBarWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\" displayname=\"Fluent Message Bar Widget\">\n"
           " <widget class=\"FluentMessageBarWidget\" name=\"fluentMessageBarWidget\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>500</width>\n"
           "    <height>68</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"title\">\n"
           "   <string>Descriptive title</string>\n"
           "  </property>\n"
           "  <property name=\"message\">\n"
           "   <string>Message providing information to the user with actionable insights.</string>\n"
           "  </property>\n"
           "  <property name=\"messageType\">\n"
           "   <enum>FluentMessageBarWidget::Info</enum>\n"
           "  </property>\n"
           "  <property name=\"dismissible\">\n"
           "   <bool>true</bool>\n"
           "  </property>\n"
           "  <property name=\"showActions\">\n"
           "   <bool>true</bool>\n"
           "  </property>\n"
           "  <property name=\"actionText\">\n"
           "   <string>Action</string>\n"
           "  </property>\n"
           "  <property name=\"secondaryActionText\">\n"
           "   <string>Action</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

QString FluentMessageBarWidgetPlugin::includeFile() const
{
    return "fluentmessagebarwidget.h";
}