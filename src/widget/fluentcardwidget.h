#ifndef FLUENTCARDWIDGET_H
#define FLUENTCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEnterEvent>
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include <QFont>

class FluentCardWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString value READ value WRITE setValue)
    Q_PROPERTY(QString progressLabel READ progressLabel WRITE setProgressLabel)
    Q_PROPERTY(int progressValue READ progressValue WRITE setProgressValue)
    Q_PROPERTY(QPixmap icon READ icon WRITE setIcon)
    Q_PROPERTY(bool darkMode READ isDarkMode WRITE setDarkMode)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor)

public:
    explicit FluentCardWidget(QWidget *parent = nullptr);

    // Property getters/setters
    QString title() const;
    void setTitle(const QString &title);

    QString value() const;
    void setValue(const QString &value);

    QString progressLabel() const;
    void setProgressLabel(const QString &label);

    int progressValue() const;
    void setProgressValue(int value);

    QPixmap icon() const;
    void setIcon(const QPixmap &icon);
    void setIcon(const QString &resourcePath);  // Load from resource
    void setFontIcon(const QString &character, const QFont &font = QFont());  // Font-based icon

    bool isDarkMode() const;
    void setDarkMode(bool dark);

    QColor accentColor() const;
    void setAccentColor(const QColor &color);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onHoverAnimationFinished();

private:
    void setupUI();
    void setupShadowEffect();
    void setupHoverAnimation();
    void updateTheme();
    void updateStyles();
    void updateIconDisplay();

    // UI Components
    QLabel *m_titleLabel;
    QLabel *m_valueLabel;
    QLabel *m_progressLabel;
    QLabel *m_iconLabel;
    QProgressBar *m_progressBar;

    // Layouts
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_headerLayout;

    // Effects and animations
    QGraphicsDropShadowEffect *m_shadowEffect;
    QPropertyAnimation *m_hoverAnimation;

    // Properties
    QString m_title;
    QString m_value;
    QString m_progressLabelText;
    int m_progressValue;
    QPixmap m_icon;
    bool m_darkMode;
    QColor m_accentColor;

    // Icon properties
    QString m_fontIconCharacter;
    QFont m_fontIconFont;
    bool m_useFontIcon;

    // Animation state
    bool m_isHovered;
    qreal m_hoverOffset;
};

#endif // FLUENTCARDWIDGET_H