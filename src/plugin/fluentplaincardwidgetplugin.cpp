#include "fluentplaincardwidgetplugin.h"
#include "../widget/fluentplaincardwidget.h"
#include <QDesignerFormEditorInterface>
#include <QtPlugin>
#include <QApplication>

FluentPlainCardWidgetPlugin::FluentPlainCardWidgetPlugin(QObject *parent)
    : QObject(parent), m_initialized(false), m_core(nullptr)
{
}

void FluentPlainCardWidgetPlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized)
        return;

    m_core = core;

    // Set application property to help with Designer detection
    if (qApp) {
        qApp->setProperty("inDesigner", true);
    }

    m_initialized = true;
}

bool FluentPlainCardWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *FluentPlainCardWidgetPlugin::createWidget(QWidget *parent)
{
    FluentPlainCardWidget *widget = new FluentPlainCardWidget(parent);

    // Ensure the widget is properly set up for Designer
    if (m_core) {
        widget->setObjectName("fluentPlainCardWidget");
    }

    return widget;
}

QString FluentPlainCardWidgetPlugin::name() const
{
    return "FluentPlainCardWidget";
}

QString FluentPlainCardWidgetPlugin::group() const
{
    return "Fluent Widgets";  // Same group as other fluent widgets
}

QIcon FluentPlainCardWidgetPlugin::icon() const
{
    return QIcon(":/widget_icons/icon-plain-frame-widget.png");  // Use a square/container icon
}

QString FluentPlainCardWidgetPlugin::toolTip() const
{
    return "Plain card container with optional Fluent UI effects";
}

QString FluentPlainCardWidgetPlugin::whatsThis() const
{
    return "FluentPlainCardWidget provides a simple container with optional "
           "hover, shadow, and roundness effects. Perfect for grouping "
           "other widgets with Fluent Design styling.";
}

bool FluentPlainCardWidgetPlugin::isContainer() const
{
    return true;  // This widget acts as a container
}

QString FluentPlainCardWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\" displayname=\"Fluent Plain Card Widget\">\n"
           " <widget class=\"FluentPlainCardWidget\" name=\"fluentPlainCardWidget\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>200</width>\n"
           "    <height>150</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"enableHover\">\n"
           "   <bool>true</bool>\n"
           "  </property>\n"
           "  <property name=\"enableShadow\">\n"
           "   <bool>true</bool>\n"
           "  </property>\n"
           "  <property name=\"enableRoundness\">\n"
           "   <bool>true</bool>\n"
           "  </property>\n"
           "  <property name=\"borderRadius\">\n"
           "   <number>8</number>\n"
           "  </property>\n"
           "  <property name=\"contentMargin\">\n"
           "   <number>16</number>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

QString FluentPlainCardWidgetPlugin::includeFile() const
{
    return "fluentplaincardwidget.h";
}