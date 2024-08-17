#ifndef SAHARA_MODELWIDGET_H
#define SAHARA_MODELWIDGET_H

#include "Sahara_global.h"

#include <QWidget>
#include <QFileDialog>

#include "scene/model.h"
#include "scene/node/node.h"
#include "widgets/scenewidget/scenewidget.h"
#include "widgets/scenewidget/openglscenewidget.h"
#include "widgets/scenewidget/vulkanscenewidget.h"

namespace Ui {
    class ModelWidget;
}

namespace Sahara {

    class SAHARA_EXPORT ModelWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit ModelWidget(QWidget *parent = nullptr, bool vulkan = false);
            ~ModelWidget();

            void setModel(const QString& path);
            QStringList animationClipNames() const;
            void setAnimationClip(const QString& name);

        signals:
            void initialized() const;

        private:
            Ui::ModelWidget *ui;
            bool _vulkan;
            Model* _model;
            Node* _modelNode;
            SceneWidget* _sceneWidget;

            void showAnimationsUI(const bool visible);

        private slots:
            void sceneWidgetInitialized();
            void dialValueChanged();
            void filePushButtonClicked();
            void animationComboBoxCurrentTextChanged(const QString& text);
    };
}

#endif // SAHARA_MODELWIDGET_H
