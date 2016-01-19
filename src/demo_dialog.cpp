//
// Created by ryan on 12/14/15.
//

#include "demo_dialog.h"
#include <QtWidgets>

namespace tresta {

    DemoDialog::DemoDialog(QWidget *parent) : saveDirectory(QDir::currentPath()), defaultFilePrefix("frame") {
        setModal(true);
        setMinimumWidth(400);
        setMinimumHeight(200);

        QVBoxLayout *layout = new QVBoxLayout(this);
        createGroupBox();
        layout->addWidget(groupBox);
        setLayout(layout);
    }

    void DemoDialog::pickDirectory() {
        QString dir = QFileDialog::getExistingDirectory(this,
                                                        tr("Choose directory to save frames"),
                                                        saveDirectory,
                                                        QFileDialog::ShowDirsOnly);
        if (!dir.isEmpty()) {
            saveDirectory = dir;
            saveDirLineEdit->setText(saveDirectory);
        }
    }

    const QString &DemoDialog::getDirectory() const {
        return saveDirectory;
    }

    const QString &DemoDialog::getFilenamePrefix() const {
        if (!prefixLineEdit->displayText().isEmpty())
            return prefixLineEdit->displayText();
        else
            return defaultFilePrefix;
    }

    void DemoDialog::createLabel() {
        prefixLabel = new QLabel("File prefix", this);
        prefixLabel->setAlignment(Qt::AlignmentFlag::AlignRight | Qt::AlignmentFlag::AlignVCenter);
    }

    void DemoDialog::createButtons() {
        startButton = new QPushButton("start", this);
        startButton->setStatusTip("Start demo mode and begin saving frames");
        connect(startButton, &QPushButton::clicked, this, &DemoDialog::accept);

        closeButton = new QPushButton("cancel", this);
        closeButton->setStatusTip("Cancel demo mode and close dialog");
        connect(closeButton, &QPushButton::clicked, this, &DemoDialog::reject);

        chooseDirButton = new QPushButton("choose directory", this);
        chooseDirButton->setStatusTip("Choose directory to save files");
        connect(chooseDirButton, &QPushButton::clicked, this, &DemoDialog::pickDirectory);
    }

    void DemoDialog::createLineEdits() {
        prefixLineEdit = new QLineEdit(this);
        prefixLineEdit->setPlaceholderText("Enter file prefix, e.g. frame");

        saveDirLineEdit = new QLineEdit(this);
        saveDirLineEdit->setText(saveDirectory);
    }

    void DemoDialog::createGroupBox() {
        createButtons();
        createLineEdits();
        createLabel();

        groupBox = new QGroupBox(this);
        QGridLayout *glayout = new QGridLayout(this);
        glayout->addWidget(chooseDirButton, 0, 0);
        glayout->addWidget(saveDirLineEdit, 0, 1);

        glayout->addWidget(prefixLabel, 1, 0);
        glayout->addWidget(prefixLineEdit, 1, 1);

        QHBoxLayout *hlayout = new QHBoxLayout(this);
        hlayout->addWidget(startButton);
        hlayout->addWidget(closeButton);
        glayout->addLayout(hlayout, 2, 0, 1, 2);

        groupBox->setLayout(glayout);
    }

} //namespace tresta