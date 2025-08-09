#ifndef FLUENTDRAWERWIDGETPLUGIN_H
#define FLUENTDRAWERWIDGETPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class FluentDrawerWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    explicit FluentDrawerWidgetPlugin(QObject *parent = nullptr);

    bool isContainer() const override;
    bool isInitialized() const override;
    QIcon icon() const override;
    QString domXml() const override;
    QString group() const override;
    QString includeFile() const override;
    QString name() const override;
    QString toolTip() const override;
    QString whatsThis() const override;
    QWidget *createWidget(QWidget *parent) override;
    void initialize(QDesignerFormEditorInterface *core) override;

private:
    bool m_initialized;
};

#endif // FLUENTDRAWERWIDGETPLUGIN_H