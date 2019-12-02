#include "modelwidget.h"
#include "ui_modelwidget.h"

Sahara::ModelWidget::ModelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModelWidget),
    _model(nullptr),
    _modelNode(nullptr)
{
    ui->setupUi(this);

    connect(ui->sceneWidget, &SceneWidget::initialized, this, &ModelWidget::sceneWidgetInitialized);
    connect(ui->sceneWidget, &SceneWidget::initialized, this, &ModelWidget::initialized);
    connect(ui->animationComboBox, &QComboBox::currentTextChanged, this, &ModelWidget::animationComboBoxCurrentTextChanged);
    connect(ui->scaleDial, &QDial::valueChanged, this, &ModelWidget::dialValueChanged);
    connect(ui->xDial, &QDial::valueChanged, this, &ModelWidget::dialValueChanged);
    connect(ui->yDial, &QDial::valueChanged, this, &ModelWidget::dialValueChanged);
    connect(ui->zDial, &QDial::valueChanged, this, &ModelWidget::dialValueChanged);
    connect(ui->filePushButton, &QPushButton::clicked, this, &ModelWidget::filePushButtonClicked);
}

Sahara::ModelWidget::~ModelWidget()
{
    delete ui;
}

void Sahara::ModelWidget::setModel(const QString& path)
{
    if (_modelNode) {
        _modelNode->remove();
        delete _modelNode;
    }

    _model = Model::fromCollada(path);
    float scale = static_cast<float>(ui->scaleDial->value()) / ui->scaleDial->maximum();
    _modelNode = new Node(QFileInfo(path).baseName(), _model, QMatrix4x4(QMatrix3x3() * scale));

    ui->sceneWidget->scene().root().addChild(_modelNode);

    if (_model->animationClipNames().size() > 1) {
        ui->animationComboBox->clear();

        for (const QString& animationClipName : _model->animationClipNames()) {
            ui->animationComboBox->addItem(animationClipName);
        }

        showAnimationsUI(true);
    } else {
        showAnimationsUI(false);
    }
}

void Sahara::ModelWidget::showAnimationsUI(const bool visible)
{
    ui->animationComboBox->setVisible(visible);
    ui->animationLabel->setVisible(visible);
}

void Sahara::ModelWidget::sceneWidgetInitialized()
{
    PointLight* pointLight = new PointLight(QColor(255, 255, 255), 1, 0, 0);
    Node* pointLightNode = new Node("Point Light", pointLight, QMatrix4x4());

    ui->sceneWidget->scene().cameraNode().addChild(pointLightNode);
}

void Sahara::ModelWidget::dialValueChanged()
{
    float x = static_cast<float>(ui->xDial->value()) / ui->xDial->maximum() * 360.0f;
    float y = static_cast<float>(ui->yDial->value()) / ui->yDial->maximum() * 360.0f;
    float z = static_cast<float>(ui->zDial->value()) / ui->zDial->maximum() * 360.0f;

    QMatrix4x4 rotation;
    rotation.rotate(x, {1, 0, 0});
    rotation.rotate(y, {0, 1, 0});
    rotation.rotate(z, {0, 0, 1});

    float scale = static_cast<float>(ui->scaleDial->value()) / ui->scaleDial->maximum();
    _modelNode->setTransform(QMatrix4x4(rotation.toGenericMatrix<3, 3>() * scale));
}

void Sahara::ModelWidget::filePushButtonClicked()
{
    QString path = QFileDialog::getOpenFileName(this,
        tr("Open Collada File"), "",
        tr("Collada documents (*.dae);;All Files (*)"));

    if (path.isEmpty())
        return;

    setModel(path);
}

void Sahara::ModelWidget::animationComboBoxCurrentTextChanged(const QString& text)
{
    _model->setAnimationClip(text);
}
