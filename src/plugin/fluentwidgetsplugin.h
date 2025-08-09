#ifndef FLUENTWIDGETSPLUGIN_H
#define FLUENTWIDGETSPLUGIN_H

#include <QDesignerCustomWidgetCollectionInterface>

class FluentWidgetsPlugin : public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
    FluentWidgetsPlugin(QObject *parent = nullptr);
    QList<QDesignerCustomWidgetInterface*> customWidgets() const override;

private:
    QList<QDesignerCustomWidgetInterface*> m_widgets;
};

#endif // FLUENTWIDGETSPLUGIN_H
