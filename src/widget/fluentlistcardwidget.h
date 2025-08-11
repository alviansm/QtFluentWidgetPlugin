#ifndef FLUENTLISTCARDWIDGET_H
#define FLUENTLISTCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QListView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEnterEvent>
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include <QScrollBar>
#include <QAbstractItemDelegate>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>

// Custom delegate for list items
class FluentListCardItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit FluentListCardItemDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setDarkMode(bool dark);
    void setAccentColor(const QColor &color);

private:
    bool m_darkMode;
    QColor m_accentColor;
};

class FluentListCardWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(int itemsPerPage READ itemsPerPage WRITE setItemsPerPage)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage)
    Q_PROPERTY(bool darkMode READ isDarkMode WRITE setDarkMode)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor)
    Q_PROPERTY(bool showPagination READ showPagination WRITE setShowPagination)
    Q_PROPERTY(QPixmap headerIcon READ headerIcon WRITE setHeaderIcon)

public:
    explicit FluentListCardWidget(QWidget *parent = nullptr);
    ~FluentListCardWidget();

    // Property getters/setters
    QString title() const;
    void setTitle(const QString &title);

    int itemsPerPage() const;
    void setItemsPerPage(int count);

    int currentPage() const;
    void setCurrentPage(int page);

    bool isDarkMode() const;
    void setDarkMode(bool dark);

    QColor accentColor() const;
    void setAccentColor(const QColor &color);

    bool showPagination() const;
    void setShowPagination(bool show);

    QPixmap headerIcon() const;
    void setHeaderIcon(const QPixmap &icon);
    void setHeaderIcon(const QString &resourcePath);
    void setHeaderFontIcon(const QString &character, const QFont &font = QFont());

    // Model management
    QStandardItemModel* model() const;
    void setModel(QStandardItemModel *model);

    // Convenience methods for adding items
    void addItem(const QString &text, const QString &badge = QString(), const QColor &badgeColor = QColor());
    void clearItems();

    // Navigation
    int totalPages() const;
    bool hasPreviousPage() const;
    bool hasNextPage() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void previousPage();
    void nextPage();
    void goToPage(int page);

signals:
    void pageChanged(int page);
    void itemClicked(const QModelIndex &index);
    void itemDoubleClicked(const QModelIndex &index);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onPreviousClicked();
    void onNextClicked();
    void onDotClicked();
    void onItemClicked(const QModelIndex &index);
    void onItemDoubleClicked(const QModelIndex &index);
    void onModelDataChanged();

private:
    void setupUI();
    void setupShadowEffect();
    void setupHoverAnimation();
    void updateTheme();
    void updateStyles();
    void updateHeaderIcon();
    void updatePagination();
    void updatePaginationDots();
    void updateListView();
    void updateNavigationButtons();

    // UI Components
    QLabel *m_titleLabel;
    QLabel *m_headerIconLabel;
    QListView *m_listView;
    QWidget *m_paginationWidget;
    QPushButton *m_previousButton;
    QPushButton *m_nextButton;
    QHBoxLayout *m_dotsLayout;
    QList<QPushButton*> m_dotButtons;

    // Layouts
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_headerLayout;
    QHBoxLayout *m_paginationLayout;

    // Effects and animations
    QGraphicsDropShadowEffect *m_shadowEffect;
    QPropertyAnimation *m_hoverAnimation;

    // Data and model
    QStandardItemModel *m_model;
    FluentListCardItemDelegate *m_delegate;

    // Properties
    QString m_title;
    int m_itemsPerPage;
    int m_currentPage;
    bool m_darkMode;
    QColor m_accentColor;
    bool m_showPagination;
    QPixmap m_headerIcon;

    // Header icon properties
    QString m_fontIconCharacter;
    QFont m_fontIconFont;
    bool m_useFontIcon;

    // Animation state
    bool m_isHovered;
};

#endif // FLUENTLISTCARDWIDGET_H
