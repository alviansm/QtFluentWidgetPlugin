#include "fluentplaincardcontainerextension.h"
#include "../widget/fluentplaincardwidget.h"
#include <QVBoxLayout>
#include <QDebug>

// FluentPlainCardContainerExtension implementation
FluentPlainCardContainerExtension::FluentPlainCardContainerExtension(FluentPlainCardWidget *widget, QObject *parent)
    : QObject(parent), m_widget(widget)
{
    Q_ASSERT(m_widget);
}

void FluentPlainCardContainerExtension::addWidget(QWidget *widget)
{
    if (!widget || !m_widget) {
        return;
    }

    QLayout *layout = m_widget->layout();
    if (layout) {
        // Set the parent first to ensure proper ownership
        widget->setParent(m_widget);
        layout->addWidget(widget);
    }
}

int FluentPlainCardContainerExtension::count() const
{
    if (!m_widget || !m_widget->layout()) {
        return 0;
    }
    return m_widget->layout()->count();
}

int FluentPlainCardContainerExtension::currentIndex() const
{
    // Simple container doesn't have concept of current index
    // Return the last valid index or -1 if empty
    int c = count();
    return c > 0 ? c - 1 : -1;
}

void FluentPlainCardContainerExtension::insertWidget(int index, QWidget *widget)
{
    if (!widget || !m_widget) {
        return;
    }

    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(m_widget->layout());
    if (layout) {
        // Ensure valid index
        index = qBound(0, index, layout->count());

        // Set parent first
        widget->setParent(m_widget);
        layout->insertWidget(index, widget);
    }
}

void FluentPlainCardContainerExtension::remove(int index)
{
    if (!m_widget || !m_widget->layout()) {
        return;
    }

    QLayout *layout = m_widget->layout();
    if (index >= 0 && index < layout->count()) {
        QLayoutItem *item = layout->takeAt(index);
        if (item) {
            QWidget *widget = item->widget();
            if (widget) {
                // Important: Don't delete the widget here!
                // Qt Designer manages widget lifecycle
                widget->setParent(nullptr);
            }
            // Only delete the layout item, not the widget
            delete item;
        }
    }
}

void FluentPlainCardContainerExtension::setCurrentIndex(int index)
{
    Q_UNUSED(index)
    // Simple container doesn't support current index concept
    // This is mainly for tab widgets, stack widgets, etc.
}

QWidget *FluentPlainCardContainerExtension::widget(int index) const
{
    if (!m_widget || !m_widget->layout()) {
        return nullptr;
    }

    QLayout *layout = m_widget->layout();
    if (index >= 0 && index < layout->count()) {
        QLayoutItem *item = layout->itemAt(index);
        if (item) {
            return item->widget();
        }
    }
    return nullptr;
}

bool FluentPlainCardContainerExtension::canAddWidget() const
{
    // FluentPlainCardWidget can always accept new child widgets
    return m_widget && m_widget->layout();
}

bool FluentPlainCardContainerExtension::canRemove(int index) const
{
    if (!m_widget || !m_widget->layout()) {
        return false;
    }

    // Can remove if the index is valid and there's a widget at that position
    QLayout *layout = m_widget->layout();
    return (index >= 0 &&
            index < layout->count() &&
            layout->itemAt(index) != nullptr &&
            layout->itemAt(index)->widget() != nullptr);
}

// FluentPlainCardExtensionFactory implementation
FluentPlainCardExtensionFactory::FluentPlainCardExtensionFactory(QExtensionManager *parent)
    : QExtensionFactory(parent)
{
}

QObject *FluentPlainCardExtensionFactory::createExtension(QObject *object, const QString &iid, QObject *parent) const
{
    FluentPlainCardWidget *widget = qobject_cast<FluentPlainCardWidget*>(object);

    if (widget && (iid == Q_TYPEID(QDesignerContainerExtension))) {
        return new FluentPlainCardContainerExtension(widget, parent);
    }

    return QExtensionFactory::createExtension(object, iid, parent);
}