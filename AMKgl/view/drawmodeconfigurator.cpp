#include "drawmodeconfigurator.h"
#include "ui_drawmodeconfigurator.h"

#include "renderer/drawmodeaccessor.h"
#include "tools/drawmodeconverter.h"

#include <QPushButton>
#include <QHBoxLayout>

DrawModeConfigurator::DrawModeConfigurator(QWidget *parent) :
    ConfigurableWidget(parent),
    ui(new Ui::DrawModeConfigurator)
{
    ui->setupUi(this);

    m_dmAccessor = NULL;

    initUI();
}

DrawModeConfigurator::~DrawModeConfigurator()
{
    delete ui;
}

void DrawModeConfigurator::setDrawModeAccessor(const DrawModeAccessor *ps)
{
    m_dmAccessor = (DrawModeAccessor*)ps;

    updateUI();
}

void DrawModeConfigurator::updateScene()
{
    if(m_dmAccessor != NULL) {
        int n = ui->tableWidget->rowCount()-1;

        for(int i=0; i<n; ++i) {
            QComboBox* cb = (QComboBox*)ui->tableWidget->cellWidget(i+1, 1);
            DrawMode m = DrawMode(cb->currentData().toInt());

            m_dmAccessor->setDrawModeToUse(Scene::ObjectType(i), m);
        }
    }
}

bool DrawModeConfigurator::checkCanApply()
{
    return true;
}

bool DrawModeConfigurator::apply()
{
    updateScene();
    return true;
}

void DrawModeConfigurator::initUI()
{
    ui->tableWidget->clear();

    int n = (int)Scene::NumberOfElements;

    ui->tableWidget->setRowCount(n+1);
    ui->tableWidget->setColumnCount(2);

    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Type d'objets") << tr("Mode de dessin préféré"));

    for(int i=0; i<n; ++i) {
        QTableWidgetItem *item = new QTableWidgetItem(Scene::objectTypeToString((Scene::ObjectType)i));
        ui->tableWidget->setItem(i+1, 0, item);
        ui->tableWidget->setCellWidget(i+1, 1, createNewComboBox());
    }

    QTableWidgetItem *item = new QTableWidgetItem(tr("All"));
    ui->tableWidget->setItem(0, 0, item);
    ui->tableWidget->setCellWidget(0, 1, createNewAllModeWidget());

    ui->tableWidget->resizeColumnsToContents();
}

void DrawModeConfigurator::updateUI()
{
    if(m_dmAccessor != NULL) {
        int n = ui->tableWidget->rowCount()-1;

        for(int i=0; i<n; ++i) {
            DrawMode m = m_dmAccessor->getDrawModeToUse((Scene::ObjectType)i);

            QComboBox* cb = (QComboBox*)ui->tableWidget->cellWidget(i+1, 1);

            int index;
            if((index = cb->findData((int)m)) != -1)
                cb->setCurrentIndex(index);
        }
    }
}

QComboBox* DrawModeConfigurator::createNewComboBox() const
{
    QComboBox* cb = new QComboBox();

    for(int i=int(DM_BASIC); i<=int(DM_VAO); ++i)
        cb->addItem(DrawModeConverter::drawModeToQString(DrawMode(i)), i);

    return cb;
}

QWidget* DrawModeConfigurator::createNewAllModeWidget() const
{
    QWidget* w = new QWidget();
    QHBoxLayout* wl = new QHBoxLayout(w);
    wl->setContentsMargins(0, 0, 0, 0);

    for(int i=int(DM_BASIC); i<=int(DM_VAO); ++i) {
        QPushButton* button = new QPushButton(DrawModeConverter::drawModeToQString(DrawMode(i)));
        button->setObjectName(QString().setNum(i));

        wl->addWidget(button);

        connect(button, SIGNAL(clicked(bool)), this, SLOT(pushButtonDrawModeToAllClicked()));
    }

    return w;
}

void DrawModeConfigurator::pushButtonDrawModeToAllClicked()
{
    QPushButton* button = (QPushButton*)sender();
    setDrawModeToAll(button->objectName().toInt());
}

void DrawModeConfigurator::setDrawModeToAll(int drawModeIndex)
{
    int n = ui->tableWidget->rowCount()-1;

    for(int i=0; i<n; ++i) {
        QComboBox* cb = (QComboBox*)ui->tableWidget->cellWidget(i+1, 1);

        cb->setCurrentIndex(cb->findData(drawModeIndex));
    }
}
