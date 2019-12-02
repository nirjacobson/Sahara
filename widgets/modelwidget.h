#ifndef SAHARA_MODELWIDGET_H
#define SAHARA_MODELWIDGET_H

#include <QWidget>

#include "scene/asset/pointlight.h"
#include "scene/model.h"
#include "scene/node/node.h"

namespace Ui {
    class ModelWidget;
}

namespace Sahara {

    class ModelWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit ModelWidget(QWidget *parent = nullptr);
            ~ModelWidget();

            void setModel(const QString& path);
            QStringList animationClipNames() const;
            void setAnimationClip(const QString& name);

        signals:
            void initialized() const;

        private:
            Ui::ModelWidget *ui;
            Model* _model;
            Node* _modelNode;

            void showAnimationsUI(const bool visible);

        private slots:
            void sceneWidgetInitialized();
            void dialValueChanged();
            void filePushButtonClicked();
            void animationComboBoxCurrentTextChanged(const QString& text);
    };
}

#endif // SAHARA_MODELWIDGET_H
