#ifndef FLUENTDRAWERWIDGET_H
#define FLUENTDRAWERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QFrame>
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include <QEasingCurve>

class FluentDrawerWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString content READ content WRITE setContent)
    Q_PROPERTY(QString primaryButtonText READ primaryButtonText WRITE setPrimaryButtonText)
    Q_PROPERTY(QString secondaryButtonText READ secondaryButtonText WRITE setSecondaryButtonText)
    Q_PROPERTY(bool darkMode READ isDarkMode WRITE setDarkMode)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor)
    Q_PROPERTY(DrawerPosition position READ position WRITE setPosition)
    Q_PROPERTY(bool showNavigationButtons READ showNavigationButtons WRITE setShowNavigationButtons)
    Q_PROPERTY(bool modal READ isModal WRITE setModal)

public:
    enum DrawerPosition {
        Left,
        Right,
        Top,
        Bottom
    };
    Q_ENUM(DrawerPosition)

    explicit FluentDrawerWidget(QWidget *parent = nullptr);

    // Property getters/setters
    QString title() const;
    void setTitle(const QString &title);

    QString content() const;
    void setContent(const QString &content);

    QString primaryButtonText() const;
    void setPrimaryButtonText(const QString &text);

    QString secondaryButtonText() const;
    void setSecondaryButtonText(const QString &text);

    bool isDarkMode() const;
    void setDarkMode(bool dark);

    QColor accentColor() const;
    void setAccentColor(const QColor &color);

    DrawerPosition position() const;
    void setPosition(DrawerPosition pos);

    bool showNavigationButtons() const;
    void setShowNavigationButtons(bool show);

    bool isModal() const;
    void setModal(bool modal);

    // Content management
    void setContentWidget(QWidget *widget);
    QWidget* contentWidget() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void show();
    void hide();
    void toggle();

signals:
    void primaryButtonClicked();
    void secondaryButtonClicked();
    void closeRequested();
    void opened();
    void closed();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

private slots:
    void onCloseButtonClicked();
    void onPrimaryButtonClicked();
    void onSecondaryButtonClicked();
    void onShowAnimationFinished();
    void onHideAnimationFinished();

private:
    void setupUI();
    void setupNavigationButtons();
    void setupAnimations();
    void updateTheme();
    void updateStyles();
    void updateNavigationButtons();
    void updateButtonsVisibility();
    void animateIn();
    void animateOut();
    void updateGeometry();
    QPoint getStartPosition();
    QPoint getEndPosition();

    // UI Components
    QFrame *m_drawerFrame;
    QLabel *m_titleLabel;
    QPushButton *m_closeButton;
    QScrollArea *m_contentScrollArea;
    QLabel *m_contentLabel;
    QWidget *m_contentWidget;
    QPushButton *m_primaryButton;
    QPushButton *m_secondaryButton;

    // Navigation buttons (for navigation variant)
    QPushButton *m_backButton;
    QPushButton *m_forwardButton;
    QPushButton *m_refreshButton;
    QPushButton *m_moreButton;

    // Layouts
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_headerLayout;
    QHBoxLayout *m_navigationLayout;
    QHBoxLayout *m_buttonLayout;
    QVBoxLayout *m_drawerLayout;

    // Effects and animations
    QGraphicsDropShadowEffect *m_shadowEffect;
    QGraphicsOpacityEffect *m_opacityEffect;
    QPropertyAnimation *m_slideAnimation;
    QPropertyAnimation *m_opacityAnimation;
    QParallelAnimationGroup *m_animationGroup;

    // Properties
    QString m_title;
    QString m_content;
    QString m_primaryButtonText;
    QString m_secondaryButtonText;
    bool m_darkMode;
    QColor m_accentColor;
    DrawerPosition m_position;
    bool m_showNavigationButtons;
    bool m_modal;

    // State
    bool m_isVisible;
    bool m_isAnimating;
    QRect m_parentGeometry;
};

#endif // FLUENTDRAWERWIDGET_H
