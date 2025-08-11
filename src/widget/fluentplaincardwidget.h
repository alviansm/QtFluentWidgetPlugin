#ifndef FLUENTPLAINCARDWIDGET_H
#define FLUENTPLAINCARDWIDGET_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEnterEvent>
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include <QColor>

class FluentPlainCardWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool enableHover READ isHoverEnabled WRITE setHoverEnabled)
    Q_PROPERTY(bool enableShadow READ isShadowEnabled WRITE setShadowEnabled)
    Q_PROPERTY(bool enableRoundness READ isRoundnessEnabled WRITE setRoundnessEnabled)
    Q_PROPERTY(bool darkMode READ isDarkMode WRITE setDarkMode)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(int contentMargin READ contentMargin WRITE setContentMargin)

public:
    explicit FluentPlainCardWidget(QWidget *parent = nullptr);
    ~FluentPlainCardWidget();

    // Property getters/setters
    bool isHoverEnabled() const;
    void setHoverEnabled(bool enabled);

    bool isShadowEnabled() const;
    void setShadowEnabled(bool enabled);

    bool isRoundnessEnabled() const;
    void setRoundnessEnabled(bool enabled);

    bool isDarkMode() const;
    void setDarkMode(bool dark);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QColor borderColor() const;
    void setBorderColor(const QColor &color);

    int borderRadius() const;
    void setBorderRadius(int radius);

    int borderWidth() const;
    void setBorderWidth(int width);

    int contentMargin() const;
    void setContentMargin(int margin);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    // Helper method for Designer detection
    bool isInDesigner() const;

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onHoverAnimationFinished();

private:
    void setupShadowEffect();
    void setupHoverAnimation();
    void updateStyles();
    void updateShadowEffect();

    // Effects and animations
    QGraphicsDropShadowEffect *m_shadowEffect;
    QPropertyAnimation *m_hoverAnimation;

    // Properties
    bool m_enableHover;
    bool m_enableShadow;
    bool m_enableRoundness;
    bool m_darkMode;
    QColor m_backgroundColor;
    QColor m_borderColor;
    int m_borderRadius;
    int m_borderWidth;
    int m_contentMargin;

    // Animation state
    bool m_isHovered;
    qreal m_hoverOffset;
    bool m_inDesigner;
};

#endif // FLUENTPLAINCARDWIDGET_H
