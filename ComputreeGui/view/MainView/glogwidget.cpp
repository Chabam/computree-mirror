#include "glogwidget.h"
#include "ui_glogwidget.h"

#include "interfaces.h"

#include <QElapsedTimer>

const QString GLogWidget::BEGIN_HMTL = "<html>";
const QString GLogWidget::END_HMTL = "</html>";

GLogWidget::GLogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GLogWidget)
{
    ui->setupUi(this);

    m_colors.insert(LogInterface::trace, Qt::black);
    m_colors.insert(LogInterface::debug, Qt::darkGray);
    m_colors.insert(LogInterface::info, QColor(0, 162, 232));
    m_colors.insert(LogInterface::warning, QColor(228, 160, 3));
    m_colors.insert(LogInterface::error, Qt::red);

    m_progressAnimation.setTargetObject(this);
    m_progressAnimation.setPropertyName("animationRotationValue");
    m_progressAnimation.setStartValue(0);
    m_progressAnimation.setEndValue(360);
    m_progressAnimation.setDuration(500);
    m_progressAnimation.setLoopCount(-1);

    m_animationRotationValue = 0;

    m_timerCheckNewMessagesToDisplay.setInterval(333);
    m_timerCheckNewMessagesToDisplay.setSingleShot(false);

    connect(&m_timerCheckNewMessagesToDisplay, SIGNAL(timeout()), this, SLOT(checkNewMessagesToDisplay()), Qt::QueuedConnection);

    ui->plainTextEdit->setReadOnly(true);
    ui->plainTextEdit->setMaximumBlockCount(200);

    m_timerCheckNewMessagesToDisplay.start();
}

GLogWidget::~GLogWidget()
{
    delete ui;
}

int GLogWidget::animationRotationValue() const
{
    return m_animationRotationValue;
}

void GLogWidget::addMessage(const int &severity, const int &type, const QString &s, const QString &filter)
{
    Q_UNUSED(type)
    Q_UNUSED(filter)

    m_mutex.lock();
    m_messages.enqueue(qMakePair(severity, s));
    m_mutex.unlock();
}

void GLogWidget::setAnimationRotationValue(int p)
{
    m_animationRotationValue = p;

    QPixmap pix(":/Icones/Icones/loader.png");
    pix = pix.transformed(QTransform().translate(pix.width(), pix.height()).rotate(p));

    ui->labelNewMessagesLoadInProgress->setPixmap(pix);
}

void GLogWidget::checkNewMessagesToDisplay()
{
    QElapsedTimer timer;
    timer.start();

    QPair<int, QString> pair;
    bool ok = true;

    int size = 0;

    while(ok) {

        m_mutex.lock();

        size = m_messages.size();

        if(size > 0)
            pair = m_messages.dequeue();
        else
            ok = false;

        m_mutex.unlock();

        if(ok) {
            --size;
            QColor col = m_colors.value(pair.first, Qt::black);
            QString txt = pair.second.replace("\n", "<br>");
            ui->plainTextEdit->appendHtml(BEGIN_HMTL + "<font color=\"" + col.name() + "\">" + txt + "</font>" + END_HMTL);
        }

        if(timer.elapsed() > 100)
            ok = false;
    }

    if(size > 0)
        m_progressAnimation.start();
    else {
        m_progressAnimation.stop();
        ui->labelNewMessagesLoadInProgress->setPixmap(QPixmap(":/Icones/Icones/valid.png"));
    }
}

void GLogWidget::on_pushButtonClear_clicked()
{
    ui->plainTextEdit->clear();
}
