#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "window.h"

class QAction;
class QMenu;
class QToolButton;
class QWidget;

namespace tresta {

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    protected:
        void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    private slots:
        void open();
        void plotDeformedPressed();
        void plotOriginalPressed();
        void setScalePressed();
        void setColorPressed();
        void zoomPressed();
        void panPressed();
        void rotatePressed();
        void demoPressed();
        void exportPressed();
        void alphaCutoffPressed();

        void zoomChanged(bool isEnabled);
        void panChanged(bool isEnabled);
        void rotateChanged(bool isEnabled);

        void demoChanged(bool isEnabled);
        void about();
        void clearWindowWidget();

    private:
        void createActions();
        void createMenus();
        void createButtons();
        void createToolBars();
        void createStatusBar();
        void readSettings();
        void writeSettings();
        void createGLWidget(const QString &fileName);
        void sendKey(Qt::Key key);

        QWidget *glWidget = nullptr;
        tresta::Window *glWindow = nullptr;

        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *helpMenu;
        QToolBar *toolBar;
        QToolButton *openButton;
        QToolButton *plotDeformedButton;
        QToolButton *plotOriginalButton;
        QToolButton *zoomButton;
        QToolButton *panButton;
        QToolButton *rotateButton;
        QToolButton *setScaleButton;
        QToolButton *setColorButton;
        QToolButton *demoButton;
        QToolButton *exportButton;
        QToolButton *alphaCutoffButton;
        QAction *openAct;
        QAction *plotDeformedAct;
        QAction *plotOriginalAct;
        QAction *setScaleAct;
        QAction *setColorAct;
        QAction *zoomAct;
        QAction *panAct;
        QAction *rotateAct;
        QAction *demoAct;
        QAction *exportAct;
        QAction *alphaCutoffAct;
        QAction *exitAct;
        QAction *aboutAct;
    };

} // namespace tresta

#endif
