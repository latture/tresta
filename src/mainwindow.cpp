#include <QtWidgets>
#include <iostream>
#include "mainwindow.h"
#include "setup.h"

namespace tresta {
    MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
    {
        createActions();
        createButtons();
        createMenus();
        createToolBars();
        createStatusBar();

        setWindowIcon(QIcon(":/assets/logo_64x64.png"));

        readSettings();

        setUnifiedTitleAndToolBarOnMac(true);
    }

    MainWindow::~MainWindow() {
        clearWindowWidget();
    }


    void MainWindow::closeEvent(QCloseEvent *event)
    {
        writeSettings();
        event->accept();
    }

    void MainWindow::open()
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty()){
            createGLWidget(fileName);
            statusBar()->showMessage(tr("File loaded"), 2000);
        }
    }

    void MainWindow::plotDeformedPressed()
    {
        sendKey(Qt::Key_D);
    }

    void MainWindow::plotOriginalPressed()
    {
        sendKey(Qt::Key_O);
    }

    void MainWindow::setScalePressed()
    {
        sendKey(Qt::Key_S);
    }

    void MainWindow::setColorPressed() {
        sendKey(Qt::Key_C);
    }

    void MainWindow::zoomPressed()
    {
        sendKey(Qt::Key_Z);
    }

    void MainWindow::panPressed()
    {
        sendKey(Qt::Key_P);
    }

    void MainWindow::rotatePressed()
    {
        sendKey(Qt::Key_R);
    }

    void MainWindow::zoomChanged(bool isEnabled) {
        zoomButton->setChecked(isEnabled);
    }

    void MainWindow::panChanged(bool isEnabled) {
        panButton->setChecked(isEnabled);
    }

    void MainWindow::rotateChanged(bool isEnabled) {
        rotateButton->setChecked(isEnabled);
    }

    void MainWindow::about()
    {
       QString aboutText("The Tresta plots truss structures in the original and deformed configurations "
                                 "based on a cofiguration file written in JSON format.\r\n\n"
                                 "Controls\r\n"
                                 "Left mouse:\t\trotate\r\n"
                                 "Middle mouse:\tzoom\r\n"
                                 "Right mouse:\ttranslate\r\n\n"
                                 "Key O:\ttoggle original shape\r\n"
                                 "Key D:\ttoggle deformed shape\r\n"
                                 "Key Z:\ttoggle zoom (left mouse)\r\n"
                                 "Key P:\ttoggle pan (left mouse)\r\n"
                                 "Key R:\ttoggle rotate (left mouse)\r\n"
                                 "Key S:\tscale deformation\r\n");
        QMessageBox::about(this, tr("About Tresta"), aboutText);
    }

    void MainWindow::clearWindowWidget() {
        if (glWindow) {
            delete glWindow;
        }
        if (glWidget) {
            delete glWidget;
        }
    }

    void MainWindow::createActions()
    {
        openAct = new QAction(QIcon(":/assets/default-document-open.png"), tr("&Open..."), this);
        openAct->setShortcuts(QKeySequence::Open);
        openAct->setStatusTip(tr("Open an existing file"));
        connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

        plotOriginalAct = new QAction(QIcon(":/assets/show-original_32x32.png"), tr("Plot o&riginal shape"), this);
        plotOriginalAct->setStatusTip(tr("Toggle display of the original shape"));
        connect(plotOriginalAct, SIGNAL(triggered()), this, SLOT(plotOriginalPressed()));

        plotDeformedAct = new QAction(QIcon(":/assets/show-deformed_32x32.png"), tr("Plot &deformed shape"), this);
        plotDeformedAct->setStatusTip(tr("Toggle display of the deformed shape"));
        connect(plotDeformedAct, SIGNAL(triggered()), this, SLOT(plotDeformedPressed()));

        setScaleAct = new QAction(QIcon(":/assets/scale_32x32.png"), tr("&Set deformation scale"), this);
        setScaleAct->setStatusTip(tr("Set scale multiplier for deformations"));
        connect(setScaleAct, SIGNAL(triggered()), this, SLOT(setScalePressed()));

        setColorAct = new QAction(QIcon(":/assets/gtk-select-color_32x32.png"), tr("&Choose colors"), this);
        setColorAct->setStatusTip(tr("Choose colors for current scene"));
        connect(setColorAct, SIGNAL(triggered()), this, SLOT(setColorPressed()));

        zoomAct = new QAction(QIcon(":/assets/zoom_32x32.png"), tr("Adjust camera &zoom"), this);
        zoomAct->setStatusTip(tr("Adjust camera zoom"));
        connect(zoomAct, SIGNAL(triggered()), this, SLOT(zoomPressed()));

        panAct = new QAction(QIcon(":/assets/pan_32x32.png"), tr("&Pan camera"), this);
        panAct->setStatusTip(tr("Pan camera"));
        connect(panAct, SIGNAL(triggered()), this, SLOT(panPressed()));

        rotateAct = new QAction(QIcon(":/assets/rotate_32x32.png"), tr("&Rotate camera"), this);
        rotateAct->setStatusTip(tr("Rotate camera"));
        connect(rotateAct, SIGNAL(triggered()), this, SLOT(rotatePressed()));

        exitAct = new QAction(QIcon(":/assets/window-close.png"), tr("E&xit"), this);
        exitAct->setShortcuts(QKeySequence::Quit);
        exitAct->setStatusTip(tr("Exit the application"));
        connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

        aboutAct = new QAction(tr("&About"), this);
        aboutAct->setStatusTip(tr("Show the application's About box"));
        connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    }

    void MainWindow::createMenus()
    {
        fileMenu = menuBar()->addMenu(tr("&File"));
        fileMenu->addAction(openAct);
        fileMenu->addSeparator();
        fileMenu->addAction(exitAct);

        menuBar()->addSeparator();

        editMenu = menuBar()->addMenu(tr("&Edit"));
        editMenu->addAction(plotOriginalAct);
        editMenu->addAction(plotDeformedAct);
        editMenu->addAction(zoomAct);
        editMenu->addAction(panAct);
        editMenu->addAction(rotateAct);
        editMenu->addAction(setScaleAct);

        menuBar()->addSeparator();

        helpMenu = menuBar()->addMenu(tr("&Help"));
        helpMenu->addAction(aboutAct);
    }

    void MainWindow::createButtons() {
        openButton = new QToolButton(this);
        openButton->setDefaultAction(openAct);

        plotDeformedButton = new QToolButton(this);
        plotDeformedButton->setDefaultAction(plotDeformedAct);
        plotDeformedButton->setCheckable(true);

        plotOriginalButton = new QToolButton(this);
        plotOriginalButton->setDefaultAction(plotOriginalAct);
        plotOriginalButton->setCheckable(true);

        zoomButton = new QToolButton(this);
        zoomButton->setDefaultAction(zoomAct);
        zoomButton->setCheckable(true);

        panButton = new QToolButton(this);
        panButton->setDefaultAction(panAct);
        panButton->setCheckable(true);

        rotateButton = new QToolButton(this);
        rotateButton->setDefaultAction(rotateAct);
        rotateButton->setCheckable(true);

        setScaleButton = new QToolButton(this);
        setScaleButton->setDefaultAction(setScaleAct);

        setColorButton = new QToolButton(this);
        setColorButton->setDefaultAction(setColorAct);
    }


    void MainWindow::createToolBars()
    {
        toolBar = addToolBar(tr("Toolbar"));
        toolBar->addWidget(openButton);
        toolBar->addWidget(plotOriginalButton);
        toolBar->addWidget(plotDeformedButton);
        toolBar->addWidget(zoomButton);
        toolBar->addWidget(panButton);
        toolBar->addWidget(rotateButton);
        toolBar->addWidget(setScaleButton);
        toolBar->addWidget(setColorButton);

    }

    void MainWindow::createStatusBar()
    {
        statusBar()->showMessage(tr("Ready"));
    }

    void MainWindow::readSettings()
    {
        QSettings settings("Latture", "Tresta");
        QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
        QSize size = settings.value("size", QSize(640, 480)).toSize();
        resize(size);
        move(pos);
    }

    void MainWindow::writeSettings()
    {
        QSettings settings("Latture", "Tresta");
        settings.setValue("pos", pos());
        settings.setValue("size", size());
    }

    void MainWindow::createGLWidget(const QString &fileName) {
        try{
            Job job = loadJobFromFilename(fileName.toStdString());
            clearWindowWidget();
            glWindow = new Window(job);
            glWidget = QWidget::createWindowContainer(glWindow, this);
            setCentralWidget(glWidget);
            connect(glWindow, SIGNAL(zoomChanged(bool)), this, SLOT(zoomChanged(bool)));
            connect(glWindow, SIGNAL(panChanged(bool)), this, SLOT(panChanged(bool)));
            connect(glWindow, SIGNAL(rotateChanged(bool)), this, SLOT(rotateChanged(bool)));
        }
        catch (std::exception &e) {
            std::cerr << "error: " << e.what() << std::endl;
            QMessageBox::critical(0, QString("Error: Could not construct mesh"), QString(e.what()));
        }

    }

    void MainWindow::sendKey(Qt::Key key) {
        if (glWindow && glWidget){
            QKeyEvent e(QEvent::KeyPress, key, Qt::NoModifier);
            glWindow->handleKeyEvent(&e);
        }
    }

} // namespace tresta
