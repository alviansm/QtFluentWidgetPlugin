#ifndef FLUENTPLAINCARDCONTAINEREXTENSION_H
#define FLUENTPLAINCARDCONTAINEREXTENSION_H

#include <QDesignerContainerExtension>
#include <QExtensionFactory>
#include <QDesignerFormEditorInterface>

class FluentPlainCardWidget;

class FluentPlainCardContainerExtension : public QObject, public QDesignerContainerExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerContainerExtension)

public:
    explicit FluentPlainCardContainerExtension(FluentPlainCardWidget *widget, QObject *parent = nullptr);

    // QDesignerContainerExtension interface
    void addWidget(QWidget *widget) override;
    int count() const override;
    int currentIndex() const override;
    void insertWidget(int index, QWidget *widget) override;
    void remove(int index) override;
    void setCurrentIndex(int index) override;
    QWidget *widget(int index) const override;
    bool canAddWidget() const override;
    bool canRemove(int index) const override;

private:
    FluentPlainCardWidget *m_widget;
};

class FluentPlainCardExtensionFactory : public QExtensionFactory
{
    Q_OBJECT

public:
    explicit FluentPlainCardExtensionFactory(QExtensionManager *parent = nullptr);

protected:
    QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const override;
};

#endif // FLUENTPLAINCARDCONTAINEREXTENSION_H