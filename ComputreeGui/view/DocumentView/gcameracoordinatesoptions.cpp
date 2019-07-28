#include "gcameracoordinatesoptions.h"
#include "ui_gcameracoordinatesoptions.h"

GCameraCoordinatesOptions::GCameraCoordinatesOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GCameraCoordinatesOptions)
{
    ui->setupUi(this);

    _camController = nullptr;
    m_updateInProgress = false;

    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->label_9->setVisible(false);
    ui->label_10->setVisible(false);

    ui->doubleSpinBoxRX->setVisible(false);
    ui->doubleSpinBoxRY->setVisible(false);
    ui->doubleSpinBoxRZ->setVisible(false);
    ui->doubleSpinBoxRW->setVisible(false);
}

GCameraCoordinatesOptions::~GCameraCoordinatesOptions()
{
    delete ui;
}

void GCameraCoordinatesOptions::setCameraController(const DM_GraphicsViewCamera *camController)
{
    disconnectCamController(_camController);

    _camController = (DM_GraphicsViewCamera*)camController;

    updateView(_camController);

    connectCamController(_camController);
}

void GCameraCoordinatesOptions::showEvent(QShowEvent *ev)
{
    updateView();

    QWidget::showEvent(ev);
}

void GCameraCoordinatesOptions::disconnectCamController(DM_GraphicsViewCamera *cc)
{
    if(cc != nullptr)
        disconnect(cc, nullptr, this, nullptr);
}

void GCameraCoordinatesOptions::connectCamController(DM_GraphicsViewCamera *cc)
{
    if(cc != nullptr)
    {
        connect(cc, SIGNAL(coordinatesChanged()), this, SLOT(updateView()), Qt::DirectConnection);
    }
}

void GCameraCoordinatesOptions::updateView(DM_GraphicsViewCamera *cc)
{
    if(cc != nullptr && !m_updateInProgress)
    {
        double q0, q1, q2, q3;
        cc->getOrientation(q0, q1, q2, q3);

        ui->doubleSpinBoxX->setValue(cc->x());
        ui->doubleSpinBoxY->setValue(cc->y());
        ui->doubleSpinBoxZ->setValue(cc->z());
        ui->doubleSpinBoxCX->setValue(cc->cx());
        ui->doubleSpinBoxCY->setValue(cc->cy());
        ui->doubleSpinBoxCZ->setValue(cc->cz());
        ui->doubleSpinBoxRX->setValue(q0);
        ui->doubleSpinBoxRY->setValue(q1);
        ui->doubleSpinBoxRZ->setValue(q2);
        ui->doubleSpinBoxRW->setValue(q3);
    }
}

void GCameraCoordinatesOptions::updateView()
{
    if(isVisible())
        updateView(_camController);
}

void GCameraCoordinatesOptions::on_pushButtonRAZHome_clicked()
{
    if(_camController != nullptr)
        _camController->homePosition();
}

void GCameraCoordinatesOptions::on_pushButtonValid_clicked()
{
    if(_camController != nullptr)
    {        
        m_updateInProgress = true;
        _camController->setSceneCenter(ui->doubleSpinBoxCX->value(),
                                       ui->doubleSpinBoxCY->value(),
                                       ui->doubleSpinBoxCZ->value(),
                                       false);

        _camController->setPosition(ui->doubleSpinBoxX->value(),
                                    ui->doubleSpinBoxY->value(),
                                    ui->doubleSpinBoxZ->value(),
                                    false);

        _camController->setOrientation(ui->doubleSpinBoxRX->value(),
                                       ui->doubleSpinBoxRY->value(),
                                       ui->doubleSpinBoxRZ->value(),
                                       ui->doubleSpinBoxRW->value(),
                                       true);
        m_updateInProgress = false;
    }
}
