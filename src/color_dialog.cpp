#include <QtWidgets>
#include "color_dialog.h"

namespace tresta {
    ColorDialog::ColorDialog(bool userColorsProvided, bool displacementsProvided, QWidget *parent)
            : QDialog(parent),
              useUserColors(false),
              origColor(QColor::fromRgbF(0.0824f, 0.3961f, 0.7529f, 1.0f)),
              defColor(QColor::fromRgbF(0.7176f, 0.1098f, 0.1098f, 1.0f)) {
        if (displacementsProvided)
            origColor.setAlphaF(0.5);

        setModal(true);

        QVBoxLayout *layout = new QVBoxLayout(this);

        createCheckBox(userColorsProvided);
        layout->addWidget(useUserColorsCheckBox);

        createChooseGroupBox(displacementsProvided);
        layout->addWidget(chooseGroupBox);

        createCloseButton();
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

    void ColorDialog::setOrigColorAlpha(float alpha) {
        origColor.setAlphaF(alpha);
    }

    void ColorDialog::pickOrigColor() {
        origColor = QColorDialog::getColor(origColor, this, "Choose Original Color", QColorDialog::ShowAlphaChannel);
        emit origColorChanged();
    }

    void ColorDialog::pickDefColor() {
        defColor = QColorDialog::getColor(defColor, this, "Choose Deformed Color", QColorDialog::ShowAlphaChannel);
        emit defColorChanged();
    }

    void ColorDialog::setUseUserColors(bool state) {
        if (useUserColors != state) {
            useUserColors = state;
            chooseGroupBox->setEnabled(!useUserColors);
        }
    }

    void ColorDialog::createCheckBox(bool userColorsProvided) {
        useUserColorsCheckBox = new QCheckBox(tr("Use user defined colors"), this);
        useUserColorsCheckBox->setChecked(false);
        connect(useUserColorsCheckBox, SIGNAL(toggled(bool)), this, SLOT(setUseUserColors(bool)));
        useUserColorsCheckBox->setEnabled(userColorsProvided);
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

    void ColorDialog::createCloseButton() {
        closeButton = new QPushButton("Close", this);
        connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    }

} // namespace tresta

