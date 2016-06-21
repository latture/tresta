#include <QtWidgets>
#include "color_dialog.h"

namespace tresta {
    ColorDialog::ColorDialog(bool userColorsProvided, bool displacementsProvided, QWidget *parent)
            : QDialog(parent),
              useUserColors(false),
              transparencyEnabled(true),
              alphaCutoff(0.0f),
              origColor(QColor::fromRgbF(0.0824f, 0.3961f, 0.7529f, 1.0f)),
              defColor(QColor::fromRgbF(0.7176f, 0.1098f, 0.1098f, 1.0f)) {
        if (displacementsProvided)
            origColor.setAlphaF(0.5);

        setModal(false);

        QVBoxLayout *layout = new QVBoxLayout(this);

        createCheckBox(userColorsProvided);
        createChooseGroupBox(displacementsProvided);
        createSpinBoxLayout();
        createCloseButton();

        layout->addWidget(useUserColorsCheckBox);
        layout->addWidget(chooseGroupBox);
        layout->addWidget(transparencyEnabledCheckBox);
        layout->addLayout(alphaCutoffLayout);
        layout->addWidget(closeButton);

        setLayout(layout);
    }

    const QColor &ColorDialog::getOrigColor() const {
        return origColor;
    }

    const QColor &ColorDialog::getDefColor() const {
        return defColor;
    }

    const bool ColorDialog::getUseUserColors() const {
        return useUserColors;
    }

    const bool ColorDialog::getTransparencyEnabled() const {
        return transparencyEnabled;
    }

    void ColorDialog::setOrigColorAlpha(float alpha) {
        origColor.setAlphaF(alpha);
    }

    void ColorDialog::pickOrigColor() {
        if (transparencyEnabled)
            origColor = QColorDialog::getColor(origColor, this, "Choose Original Color", QColorDialog::ShowAlphaChannel);
        else
            origColor = QColorDialog::getColor(origColor, this, "Choose Original Color");
        emit origColorChanged();
    }

    void ColorDialog::pickDefColor() {
        if (transparencyEnabled)
            defColor = QColorDialog::getColor(defColor, this, "Choose Deformed Color", QColorDialog::ShowAlphaChannel);
        else
            defColor = QColorDialog::getColor(defColor, this, "Choose Deformed Color");
        emit defColorChanged();
    }

    void ColorDialog::setUseUserColors(bool state) {
        if (useUserColors != state) {
            useUserColors = state;
            chooseGroupBox->setEnabled(!useUserColors);
        }
    }

    void ColorDialog::setTransparencyEnabled(bool state) {
        if (transparencyEnabled != state) {
            transparencyEnabled = state;
            emit transparencyEnabledChanged(state);
        }
    }

    void ColorDialog::setAlphaCutoff(double cutoff) {
        alphaCutoff = (float) cutoff;
        emit alphaCutoffChanged(alphaCutoff);
    }

    void ColorDialog::createCheckBox(bool userColorsProvided) {
        useUserColorsCheckBox = new QCheckBox(tr("Use user defined colors"), this);
        useUserColorsCheckBox->setChecked(false);
        connect(useUserColorsCheckBox, SIGNAL(toggled(bool)), this, SLOT(setUseUserColors(bool)));
        useUserColorsCheckBox->setEnabled(userColorsProvided);

        transparencyEnabledCheckBox = new QCheckBox(tr("Enable transparency"), this);
        transparencyEnabledCheckBox->setChecked(false);
        connect(transparencyEnabledCheckBox, SIGNAL(toggled(bool)), this, SLOT(setTransparencyEnabled(bool)));
        transparencyEnabledCheckBox->setEnabled(userColorsProvided);
    }

    void ColorDialog::createChooseGroupBox(bool displacementsProvided) {
        chooseGroupBox = new QGroupBox(this);
        QVBoxLayout *vlayout = new QVBoxLayout(chooseGroupBox);

        origColorButton = new QPushButton("Choose Original Color", this);
        connect(origColorButton, SIGNAL(clicked()), this, SLOT(pickOrigColor()));
        vlayout->addWidget(origColorButton);


        defColorButton = new QPushButton("Choose Deformed Color", this);
        connect(defColorButton, SIGNAL(clicked()), this, SLOT(pickDefColor()));
        defColorButton->setEnabled(displacementsProvided);
        vlayout->addWidget(defColorButton);

        chooseGroupBox->setLayout(vlayout);
    }

    void ColorDialog::createSpinBoxLayout() {
        alphaCutoffSpinBox = new QDoubleSpinBox(this);
        alphaCutoffSpinBox->setMinimum(0.0);
        alphaCutoffSpinBox->setMaximum(1.0);
        alphaCutoffSpinBox->setDecimals(4);
        alphaCutoffSpinBox->setValue(alphaCutoff);
        connect(alphaCutoffSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAlphaCutoff(double)));

        alphaCutoffLabel = new QLabel(this);
        alphaCutoffLabel->setText("Alpha threshold:");

        alphaCutoffLayout = new QHBoxLayout(this);
        alphaCutoffLayout->addWidget(alphaCutoffLabel);
        alphaCutoffLayout->addWidget(alphaCutoffSpinBox);
    }

    void ColorDialog::createCloseButton() {
        closeButton = new QPushButton("Close", this);
        connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    }
} // namespace tresta

