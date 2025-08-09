#ifndef FLUENTMODALWIDGET_H
#define FLUENTMODALWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QEnterEvent>
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include <QFont>
#include <QPixmap>

class FluentModalWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QPixmap image READ image WRITE setImage)
    Q_PROPERTY(QString primaryButtonText READ primaryButtonText WRITE setPrimaryButtonText)
    Q_PROPERTY(QString secondaryButtonText READ secondaryButtonText WRITE setSecondaryButtonText)
    Q_PROPERTY(int currentStep READ currentStep WRITE setCurrentStep)
    Q_PROPERTY(int totalSteps READ totalSteps WRITE setTotalSteps)
    Q_PROPERTY(bool darkMode READ isDarkMode WRITE setDarkMode)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor)
    Q_PROPERTY(bool showNavigationButtons READ showNavigationButtons WRITE setShowNavigationButtons)

public:
    explicit FluentModalWidget(QWidget *parent = nullptr);

    // Property getters/setters
    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    QPixmap image() const;
    void setImage(const QPixmap &image);
    void setImage(const QString &resourcePath);

    QString primaryButtonText() const;
    void setPrimaryButtonText(const QString &text);

    QString secondaryButtonText() const;
    void setSecondaryButtonText(const QString &text);

    int currentStep() const;
    void setCurrentStep(int step);

    int totalSteps() const;
    void setTotalSteps(int steps);

    bool isDarkMode() const;
    void setDarkMode(bool dark);

    QColor accentColor() const;
    void setAccentColor(const QColor &color);

    bool showNavigationButtons() const;
    void setShowNavigationButtons(bool show);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    // Public methods
    void showModal();
    void hideModal();
    void nextStep();
    void previousStep();
    void goToStep(int step);

signals:
    void primaryButtonClicked();
    void secondaryButtonClicked();
    void stepChanged(int step);
    void modalClosed();

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onPrimaryButtonClicked();
    void onSecondaryButtonClicked();
    void onStepDotClicked();
    void onShowAnimationFinished();
    void onHideAnimationFinished();

private:
    void setupUI();
    void setupImageArea();
    void setupContentArea();
    void setupButtonArea();
    void setupStepIndicator();
    void setupAnimations();
    void setupShadowEffect();
    void updateTheme();
    void updateStyles();
    void updateStepIndicator();
    void updateImageDisplay();

    // UI Components
    QLabel *m_imageLabel;
    QLabel *m_titleLabel;
    QLabel *m_descriptionLabel;
    QPushButton *m_primaryButton;
    QPushButton *m_secondaryButton;
    QWidget *m_stepIndicatorWidget;
    QHBoxLayout *m_stepIndicatorLayout;

    // Layouts
    QVBoxLayout *m_mainLayout;
    QVBoxLayout *m_contentLayout;
    QHBoxLayout *m_buttonLayout;

    // Effects and animations
    QGraphicsDropShadowEffect *m_shadowEffect;
    QPropertyAnimation *m_scaleAnimation;
    QPropertyAnimation *m_opacityAnimation;
    QParallelAnimationGroup *m_showAnimation;
    QParallelAnimationGroup *m_hideAnimation;
    QGraphicsOpacityEffect *m_opacityEffect;

    // Properties
    QString m_title;
    QString m_description;
    QPixmap m_image;
    QString m_primaryButtonText;
    QString m_secondaryButtonText;
    int m_currentStep;
    int m_totalSteps;
    bool m_darkMode;
    QColor m_accentColor;
    bool m_showNavigationButtons;

    // State
    bool m_isVisible;
    QList<QPushButton*> m_stepDots;
};

#endif // FLUENTMODALWIDGET_H