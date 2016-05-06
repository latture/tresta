#ifndef TRESTA_COLOR_DIALOG_H
#define TRESTA_COLOR_DIALOG_H

#include <QDialog>
#include <QColor>

class QCheckBox;
class QDoubleSpinBox;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;

namespace tresta {

    class ColorDialog : public QDialog {
    Q_OBJECT

    signals:
        void origColorChanged();
        void defColorChanged();
        void transparencyEnabledChanged(bool);
        void alphaCutoffChanged(float);

    public slots:
        /**
         * Prompts user to pick the color for the undeformed mesh.
         */
        void pickOrigColor();

        /**
         * Prompts the user to pick the color for the deformed mesh.
         */
        void pickDefColor();

        /**
         * Sets whether custom user colors applied to each element should be used.
         * @param state bool. Whether to use user defined colors.
         */
        void setUseUserColors(bool state);

        /**
         * Sets whether rendering should allow transparency.
         * @param transparencyEnabled bool. Whether transparency should be used.
         */
        void setTransparencyEnabled(bool state);

        /**
         * Sets the alpha threshold for discarding fragments in the shader.\
         * @param cutoff float. Values below this are discarded. Should be on the range `[0, 1]`.
         */
        void setAlphaCutoff(double cutoff);

    public:
        ColorDialog(bool userColorProvided, bool displacementsProvided, QWidget *parent = 0);

        /**
         * Gets the currently stored color to assign to the undeformed mesh.
         * @return color QColor. Copy of the original color.
         */
        const QColor &getOrigColor() const;

        /**
         * Gets the currently stored color to assign to the deformed mesh.
         * @return color QColor. Copy of the deformed color.
         */
        const QColor &getDefColor() const;

        /**
         * Gets the bool that determines whether user supplied colors should be used.
         * @return useUserColors bool. True if user colors should be used.
         */
        const bool getUseUserColors() const;

        /**
         * Gets the bool that determines whether rendering should allow transparency.
         * @return transparencyEnabled bool. True if transparency should be used.
         */
        const bool getTransparencyEnabled() const;

        /**
         * Sets the alpha channel value for the color stored for the undeformed mesh.
         * @param alpha float. Alpha channel value on the range `[0.0, 1.0]`.
         */
        void setOrigColorAlpha(float alpha);

    private:
        /**
         * Creates the group box containing the buttons that prompt the user to choose 
         * colors for either the deformed or undeformed mesh.
         * @param displacementsProvided bool. Whether nodal displacements were provided.
         *        If `false` then there is no deformed mesh to render.
         */
        void createChooseGroupBox(bool displacementsProvided);

        /**
         * Creates the checkbox used to switch between user specified elemental colors 
         * and single colors chosen for the original and deformed meshes.
         * @param userColorsProvided bool. Whether a CSV files was provided that contains
         *        user colors. If `false`, then the checkbox is disabled.
         */
        void createCheckBox(bool userColorsProvided);

        /**
         * Creates the spin box layout used to set the alpha cutoff threshold label and spin box.
         */
        void createSpinBoxLayout();

        /**
         * Creates the button used to close the dialog.
         */
        void createCloseButton();

        bool useUserColors;/**<Whether to use custom defined elemental colors instead of single colors chosen for each mesh.*/
        bool transparencyEnabled;/**<Whether transparency is enabled.*/
        float alphaCutoff;/**<If transparency is enabled, alpha values below this value will be discarded by the shader.*/

        QColor origColor;/**<Color of the undeformed mesh.*/
        QColor defColor;/**<Color of the deformed mesh.*/

        QCheckBox *useUserColorsCheckBox;/**<When checked user defined elemental colors should replace single colors chosen for each mesh.*/
        QCheckBox *transparencyEnabledCheckBox;/**<When checked the mesh will be drawn with transparency enabled.*/
        QGroupBox *chooseGroupBox;/**<Holds buttons used to prompt the user to choose colors for the original and deformed meshes.*/
        QPushButton *origColorButton;/**<Button that prompts the user to choose the color of the original mesh.*/
        QPushButton *defColorButton;/**<Button that prompts the user to choose the color of the deformed mesh.*/
        QPushButton *closeButton;/**<Button that closes the dialog.*/
        QDoubleSpinBox *alphaCutoffSpinBox;/**<Spin box that allows the user to set the alpha cutoff threshold.*/
        QLabel *alphaCutoffLabel;/**<Label displayed beside the alpha cutoff spinbox*/;
        QHBoxLayout *alphaCutoffLayout;/**<Layout that holds the alpha cutoff label and spin box*/
    };

} // namespace tresta

#endif //TRESTA_COLOR_DIALOG_H
