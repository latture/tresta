#ifndef TRESTA_COLOR_DIALOG_H
#define TRESTA_COLOR_DIALOG_H

#include <QDialog>
#include <QColor>

class QCheckBox;

class QGroupBox;

class QPushButton;

namespace tresta {

    class ColorDialog : public QDialog {
    Q_OBJECT

    public slots:

        void pickOrigColor();

        void pickDefColor();

        void setUseUserColors(bool state);

    public:
        ColorDialog(bool userColorProvided, bool displacementsProvided, QWidget *parent = 0);

        const QColor &getOrigColor() const;

        const QColor &getDefColor() const;

        const bool getUseUserColors() const;

        void setOrigColorAlpha(float alpha);

    private:
        void createActions();

        void createChooseGroupBox(bool displacementsProvided);

        void createCheckBox(bool userColorsProvided);

        void createCloseButton();

        bool useUserColors;

        QColor origColor;
        QColor defColor;

        QCheckBox *useUserColorsCheckBox;
        QGroupBox *chooseGroupBox;
        QPushButton *origColorButton;
        QPushButton *defColorButton;
        QPushButton *closeButton;
    };

} // namespace tresta

#endif //TRESTA_COLOR_DIALOG_H
