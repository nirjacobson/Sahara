#include "modelwidget.h"
#include "ui_modelwidget.h"

Sahara::ModelWidget::ModelWidget(QWidget *parent, bool vulkan) :
    QWidget(parent),
    ui(new Ui::ModelWidget),
    _model(nullptr),
    _modelNode(nullptr),
    _vulkan(vulkan)
{
    ui->setupUi(this);

    if (!vulkan) {
        OpenGLSceneWidget* widget = new OpenGLSceneWidget(this);
        _sceneWidget = widget;
        layout()->replaceWidget(ui->sceneWidget, widget);
        connect(widget, &OpenGLSceneWidget::initialized, this, &ModelWidget::sceneWidgetInitialized);
        connect(widget, &OpenGLSceneWidget::initialized, this, &ModelWidget::initialized);
    }
#ifdef VULKAN
    else {
        VulkanSceneWidget* widget = new VulkanSceneWidget(this);
        _sceneWidget = widget;
        layout()->replaceWidget(ui->sceneWidget, widget);
        connect(widget, SIGNAL(initialized(void)), this, SLOT(sceneWidgetInitialized(void)));
        connect(widget, SIGNAL(initialized(void)), this, SIGNAL(initialized(void)));
    }
#endif

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

    if (!_vulkan) {
        _model = OpenGLModel::fromCollada(path);
    }
#ifdef VULKAN
    else {
        _model = VulkanModel::fromCollada(dynamic_cast<VulkanRenderer*>(_sceneWidget->renderer()), path);
    }
#endif

    float scale = static_cast<float>(ui->scaleDial->value()) / ui->scaleDial->maximum();
    _modelNode = new Node(QFileInfo(path).baseName(), _model, QMatrix4x4(QMatrix3x3() * scale));

    _sceneWidget->scene().root().addChild(_modelNode);
    dialValueChanged();

    if (_model->animationClipNames().size() > 1) {
        ui->animationComboBox->blockSignals(true);
        ui->animationComboBox->clear();

        for (const QString& animationClipName : _model->animationClipNames()) {
            ui->animationComboBox->addItem(animationClipName);
        }
        ui->animationComboBox->blockSignals(false);

        showAnimationsUI(true);
    } else {
        showAnimationsUI(false);
    }
}

QStringList Sahara::ModelWidget::animationClipNames() const
{
    return _model->animationClipNames();
}

void Sahara::ModelWidget::setAnimationClip(const QString& name)
{
   ui->animationComboBox->blockSignals(true);
   ui->animationComboBox->setCurrentText(name);
   ui->animationComboBox->blockSignals(false);
}

void Sahara::ModelWidget::showAnimationsUI(const bool visible)
{
    ui->animationComboBox->setVisible(visible);
    ui->animationLabel->setVisible(visible);
}

void Sahara::ModelWidget::sceneWidgetInitialized()
{
    QMatrix4x4 cameraTransform;
    cameraTransform.translate({0, 0, 16});
    _sceneWidget->scene().cameraNode().setTransform(cameraTransform);

    PointLight* pointLight = new PointLight("Point Light", QColor(255, 255, 255), 1, 0, 0);
    Node* pointLightNode = new Node("Point Light", pointLight, QMatrix4x4());

    _sceneWidget->scene().cameraNode().addChild(pointLightNode);
}

void Sahara::ModelWidget::dialValueChanged()
{
    float x = static_cast<float>(ui->xDial->value()) / ui->xDial->maximum() * 360.0f;
    float y = static_cast<float>(ui->yDial->value()) / ui->yDial->maximum() * 360.0f;
    float z = static_cast<float>(ui->zDial->value()) / ui->zDial->maximum() * 360.0f;

    QMatrix4x4 rotation;
    rotation.rotate(-x, {1, 0, 0});
    rotation.rotate(-y, {0, 1, 0});
    rotation.rotate(-z, {0, 0, 1});

    float scale = static_cast<float>(ui->scaleDial->value()) / ui->scaleDial->maximum();
    QMatrix4x4 translation;
    float half = (_model->volume().upperVertex().y() - _model->volume().lowerVertex().y()) / 2.0f;
    translation.translate({0, -(half + _model->volume().lowerVertex().y()), 0});
    _modelNode->setTransform(QMatrix4x4(scale * rotation.toGenericMatrix<3, 3>()) * translation);
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
