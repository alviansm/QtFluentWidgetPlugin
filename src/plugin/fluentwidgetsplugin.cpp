#include "fluentwidgetsplugin.h"

#include "fluentbuttonwidgetplugin.h"
#include "fluentcardwidgetplugin.h"
#include "fluentdrawerwidgetplugin.h"
#include "fluentgraphcardwidgetplugin.h"
#include "fluentlistcardwidgetplugin.h"
#include "fluentmessagebarwidgetplugin.h"
#include "fluentmodalwidgetplugin.h"
#include "fluentplaincardwidgetplugin.h"

// #include "metricwidgetplugin.h"  // Add future widgets here

FluentWidgetsPlugin::FluentWidgetsPlugin(QObject *parent)
    : QObject(parent)
{
    // Register all individual widget plugins
    m_widgets.append(new FluentCardWidgetPlugin(this));
    m_widgets.append(new FluentButtonWidgetPlugin(this));
    m_widgets.append(new FluentDrawerWidgetPlugin(this));
    m_widgets.append(new FluentGraphCardWidgetPlugin(this));
    m_widgets.append(new FluentListCardWidgetPlugin(this));
    m_widgets.append(new FluentMessageBarWidgetPlugin(this));
    m_widgets.append(new FluentModalWidgetPlugin(this));
    m_widgets.append(new FluentPlainCardWidgetPlugin(this));

    // m_widgets.append(new MetricWidgetPlugin(this));  // Add future widgets here
}

QList<QDesignerCustomWidgetInterface*> FluentWidgetsPlugin::customWidgets() const
{
    return m_widgets;
}
