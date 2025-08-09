#ifndef FLUENTMESSAGEBARWIDGET_H
#define FLUENTMESSAGEBARWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QStyleOption>
#include <QTimer>

class FluentMessageBarWidget : public QWidget
{
    Q_OBJECT
    Q_ENUMS(MessageType)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString message READ message WRITE setMessage)
    Q_PROPERTY(MessageType messageType READ messageType WRITE setMessageType)
    Q_PROPERTY(bool dismissible READ isDismissible WRITE setDismissible)
    Q_PROPERTY(bool showActions READ showActions WRITE setShowActions)
    Q_PROPERTY(QString actionText READ actionText WRITE setActionText)
    Q_PROPERTY(QString secondaryActionText READ secondaryActionText WRITE setSecondaryActionText)
    Q_PROPERTY(bool darkMode READ isDarkMode WRITE setDarkMode)
    Q_PROPERTY(int autoHideDelay READ autoHideDelay WRITE setAutoHideDelay)

public:
    enum MessageType {
        Info,
        Success,
        Warning,
        Error
    };

    explicit FluentMessageBarWidget(QWidget *parent = nullptr);

    // Property getters/setters
    QString title() const;
    void setTitle(const QString &title);

    QString message() const;
    void setMessage(const QString &message);

    MessageType messageType() const;
    void setMessageType(MessageType type);

    bool isDismissible() const;
    void setDismissible(bool dismissible);

    bool showActions() const;
    void setShowActions(bool show);

    QString actionText() const;
    void setActionText(const QString &text);

    QString secondaryActionText() const;
    void setSecondaryActionText(const QString &text);

    bool isDarkMode() const;
    void setDarkMode(bool dark);

    int autoHideDelay() const;
    void setAutoHideDelay(int delay);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    // Public methods
    void show();
    void hide();
    void showMessage(const QString &title, const QString &message, MessageType type = Info);

signals:
    void dismissed();
    void actionClicked();
    void secondaryActionClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onDismissClicked();
    void onActionClicked();
    void onSecondaryActionClicked();
    void onAutoHideTimer();
    void onShowAnimationFinished();
    void onHideAnimationFinished();

private:
    void setupUI();
    void updateStyles();
    void updateIcon();
    void updateActionButtons();
    void startShowAnimation();
    void startHideAnimation();

    QString getIconPath() const;
    QColor getTypeColor() const;
    QString getTypeBackgroundColor() const;
    QString getTypeTextColor() const;

    // UI Components
    QHBoxLayout *m_mainLayout;
    QVBoxLayout *m_contentLayout;
    QHBoxLayout *m_buttonLayout;

    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QLabel *m_messageLabel;
    QPushButton *m_actionButton;
    QPushButton *m_secondaryActionButton;
    QPushButton *m_dismissButton;

    // Effects and animations
    QGraphicsOpacityEffect *m_opacityEffect;
    QPropertyAnimation *m_showAnimation;
    QPropertyAnimation *m_hideAnimation;
    QTimer *m_autoHideTimer;

    // Properties
    QString m_title;
    QString m_messageText;
    MessageType m_messageType;
    bool m_dismissible;
    bool m_showActionButtons;
    QString m_actionText;
    QString m_secondaryActionText;
    bool m_darkMode;
    int m_autoHideDelay; // milliseconds, 0 = no auto hide

    // State
    bool m_isVisible;
};

#endif // FLUENTMESSAGEBARWIDGET_H