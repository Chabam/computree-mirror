#ifndef GLOGWIDGET_H
#define GLOGWIDGET_H

#include "ct_log/abstract/ct_abstractloglistener.h"

#include <QWidget>
#include <QMutex>
#include <QHash>
#include <QColor>
#include <QTimer>
#include <QQueue>
#include <QPropertyAnimation>

namespace Ui {
class GLogWidget;
}

class GLogWidget : public QWidget, public CT_AbstractLogListener
{
    Q_OBJECT
    Q_PROPERTY(int animationRotationValue READ animationRotationValue WRITE setAnimationRotationValue)

public:
    explicit GLogWidget(QWidget *parent = 0);
    ~GLogWidget();

    int animationRotationValue() const;

public slots:
    void addMessage(const int &severity, const int &type, const QString &s, const QString &filter = "");
    void setAnimationRotationValue(int p);

private:
    Ui::GLogWidget              *ui;
    QQueue<QPair<int, QString> > m_messages;
    QMutex                      m_mutex;
    QHash<int, QColor>          m_colors;
    QTimer                      m_timerCheckNewMessagesToDisplay;
    QPropertyAnimation          m_progressAnimation;
    int                         m_animationRotationValue;

    const static QString        BEGIN_HMTL;
    const static QString        END_HMTL;

private slots:
    void checkNewMessagesToDisplay();
    void on_pushButtonClear_clicked();
};

#endif // GLOGWIDGET_H
