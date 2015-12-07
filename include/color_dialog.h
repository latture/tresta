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
         * @return useUserColors bool. Trus if user colors should be used.
         */
        const bool getUseUserColors() const;

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
         * Creates the button used to close the dialog.
         */
        void createCloseButton();

        bool useUserColors;/**<Whether to use custom defined elemental colors instead of single colors chosen for each mesh.*/

        QColor origColor;/**<Color of the undeformed mesh.*/
        QColor defColor;/**<Color of the deformed mesh.*/

        QCheckBox *useUserColorsCheckBox;/**<When checked user defined elemental colors should replace single colors chosen for each mesh.*/
        QGroupBox *chooseGroupBox;/**<Holds buttons used to prompt the user to choose colors for the original and deformed meshes.*/
        QPushButton *origColorButton;/**<Button that prompts the user to choose the color of the original mesh.*/
        QPushButton *defColorButton;/**<Button that prompts the user to choose the color of the deformed mesh.*/
        QPushButton *closeButton;/**<Button that closes the dialog.*/
    };

} // namespace tresta

#endif //TRESTA_COLOR_DIALOG_H
