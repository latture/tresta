//
// Created by ryan on 12/14/15.
//

#ifndef TRESTA_DEMODIALOG_H
#define TRESTA_DEMODIALOG_H

#include <QDialog>

class QAction;

class QGroupBox;

class QPushButton;

class QLineEdit;

class QLabel;

namespace tresta {

    class DemoDialog : public QDialog {
        Q_OBJECT

    public
        slots:
        /**
         * Prompts user to pick the directory to save the frames to.
         */
        void

        pickDirectory();

    public:
        DemoDialog(QWidget *parent = 0);

        /**
         * Gets the currently stored directory to save frames to.
         * @return directory QString. Current directory.
         */
        const QString &getDirectory() const;

        /**
         * Gets the currently stored directory to save frames to.
         * @return prefix QString. Root prepended to each frame. Files are saved in the form `prefix_%3d.png`.
         */
        const QString &getFilenamePrefix() const;

    private:


        /**
         * Creates the label for the file name prefix line edit.
         */
        void createLabel();

        /**
         * Creates the button used to close the dialog, start demo mode, and choose the save directory.
         */
        void createButtons();

        /**
         * Creates the line edit that holds the filename prefix.
         */
        void createLineEdits();

        /**
         * Creates the group box used to hold the widgets of the dialog.
         */
        void createGroupBox();

        QString saveDirectory;
        /**<Directory to save demo frames to.*/
        QString defaultFilePrefix;
        /**<Prefix to prepend to frames. Files are saved in the form `prefix_%3d.png`.*/

        QLabel *prefixLabel;
        /**<Label for file prefix line edit.*/
        QLineEdit *saveDirLineEdit;
        /**<Holds the directory to save files to.*/
        QLineEdit *prefixLineEdit;
        /**<Holds the filename prefix.*/
        QGroupBox *groupBox;
        /**<Holds the buttons and line edit of the dialog in a managed box.*/
        QPushButton *chooseDirButton;
        /**<Button that prompts the user to choose the save directory.*/
        QPushButton *startButton;
        /**<Button that prompts the user to choose the color of the deformed mesh.*/
        QPushButton *closeButton;/**<Button that closes the dialog.*/
    };

} // namespace tresta


#endif //TRESTA_DEMODIALOG_H
