#include "fluentlistcardwidgetplugin.h"
#include "../widget/fluentlistcardwidget.h"
#include <QtPlugin>

FluentListCardWidgetPlugin::FluentListCardWidgetPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
}

void FluentListCardWidgetPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core)
    if (m_initialized)
        return;
    m_initialized = true;
}

bool FluentListCardWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *FluentListCardWidgetPlugin::createWidget(QWidget *parent)
{
    return new FluentListCardWidget(parent);
}

QString FluentListCardWidgetPlugin::name() const
{
    return "FluentListCardWidget";
}

QString FluentListCardWidgetPlugin::group() const
{
    return "Fluent Widgets";
}

QIcon FluentListCardWidgetPlugin::icon() const
{
    return QIcon(":/widget_icons/icon-card_list-widget.png");
}

QString FluentListCardWidgetPlugin::toolTip() const
{
    return "Modern list card widget with pagination and Fluent UI design";
}

QString FluentListCardWidgetPlugin::whatsThis() const
{
    return "FluentListCardWidget provides a card-style list display with title, paginated items, "
           "and navigation controls. Supports dark/light themes and hover animations. "
           "Use setModel() to provide data or addItem() for simple items.";
}

bool FluentListCardWidgetPlugin::isContainer() const
{
    return false;
}

QString FluentListCardWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\" displayname=\"Fluent List Card Widget\">\n"
           " <widget class=\"FluentListCardWidget\" name=\"fluentListCardWidget\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>380</width>\n"
           "    <height>320</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"title\">\n"
           "   <string>Certificates</string>\n"
           "  </property>\n"
           "  <property name=\"itemsPerPage\">\n"
           "   <number>4</number>\n"
           "  </property>\n"
           "  <property name=\"currentPage\">\n"
           "   <number>0</number>\n"
           "  </property>\n"
           "  <property name=\"showPagination\">\n"
           "   <bool>true</bool>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

QString FluentListCardWidgetPlugin::includeFile() const
{
    return "fluentlistcardwidget.h";
}