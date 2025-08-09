#ifndef FLUENTBUTTONWIDGET_H
#define FLUENTBUTTONWIDGET_H

#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
#include <QEnterEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QTimer>
#include <QFont>
#include <QIcon>

class FluentButtonWidget : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(ButtonStyle buttonStyle READ buttonStyle WRITE setButtonStyle)
    Q_PROPERTY(ButtonSize buttonSize READ buttonSize WRITE setButtonSize)
    Q_PROPERTY(bool darkMode READ isDarkMode WRITE setDarkMode)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor)
    Q_PROPERTY(bool iconOnly READ isIconOnly WRITE setIconOnly)
    Q_PROPERTY(QString iconResource READ iconResource WRITE setIconResource)

public:
    enum ButtonStyle {
        Primary = 0,
        Secondary,
        Outline,
        Subtle,
        Transparent
    };
    Q_ENUM(ButtonStyle)

    enum ButtonSize {
        Small = 0,
        Medium,
        Large
    };
    Q_ENUM(ButtonSize)

    enum ButtonState {
        Rest = 0,
        Hover,
        Pressed,
        Focused,
        Disabled
    };

    explicit FluentButtonWidget(QWidget *parent = nullptr);
    explicit FluentButtonWidget(const QString &text, QWidget *parent = nullptr);

    // Property getters/setters
    ButtonStyle buttonStyle() const;
    void setButtonStyle(ButtonStyle style);

    ButtonSize buttonSize() const;
    void setButtonSize(ButtonSize size);

    bool isDarkMode() const;
    void setDarkMode(bool dark);

    QColor accentColor() const;
    void setAccentColor(const QColor &color);

    bool isIconOnly() const;
    void setIconOnly(bool iconOnly);

    QString iconResource() const;
    void setIconResource(const QString &resourcePath);

    // Icon methods
    void setIcon(const QPixmap &pixmap);
    void setIcon(const QString &resourcePath);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *event) override;

private slots:
    void onAnimationFinished();
    void onPressAnimationFinished();

private:
    void setupAnimations();
    void updateStyles();
    void updateSizes();
    void updateButtonState();
    void animateToState(ButtonState state);
    void updateIconDisplay();  // Added missing method declaration

    QString getStyleSheet() const;
    QSize getButtonSize() const;

    // Properties
    ButtonStyle m_buttonStyle;
    ButtonSize m_buttonSize;
    bool m_darkMode;
    QColor m_accentColor;
    bool m_iconOnly;
    QString m_iconResource;    // Added missing member variable

    // Icon properties
    QString m_fontIconCharacter;
    QFont m_fontIconFont;
    bool m_useFontIcon;
    QPixmap m_icon;           // Added missing member variable (renamed from m_pixmapIcon)

    // State tracking
    ButtonState m_currentState;
    bool m_isPressed;
    bool m_isHovered;

    // Animations
    QPropertyAnimation *m_hoverAnimation;
    QPropertyAnimation *m_pressAnimation;
    QTimer *m_pressTimer;

    // Animation properties
    qreal m_hoverOpacity;
    qreal m_pressScale;
};

#endif // FLUENTBUTTONWIDGET_H
