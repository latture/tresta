#include "window.h"

#include <QMouseEvent>
#include <QOpenGLContext>
#include <QTimer>
#include <QMessageBox>
#include <QScreen>

#include "glassert.h"

namespace tresta {

    namespace {
        static void infoGL() {
            glCheckError();
            const GLubyte *str;
            std::cout << "OpenGL infos with gl functions" << std::endl;
            str = glGetString(GL_RENDERER);
            std::cout << "Renderer : " << str << std::endl;
            str = glGetString(GL_VENDOR);
            std::cout << "Vendor : " << str << std::endl;
            str = glGetString(GL_VERSION);
            std::cout << "OpenGL Version : " << str << std::endl;
            str = glGetString(GL_SHADING_LANGUAGE_VERSION);
            std::cout << "GLSL Version : " << str << std::endl;
            glCheckError();
        }
    }

    Window::Window(Job &job, QScreen *screen) :
            QWindow(screen),
            mScene(new TrussScene(job)),
            rotatePressed(false),
            keyboardRotate(false),
            translatePressed(false),
            keyboardTranslate(false),
            zoomPressed(false),
            keyboardZoom(false),
            keyboardOverride(false) {
        setSurfaceType(OpenGLSurface);

        create();

        mContext = new QOpenGLContext();
        mContext->create();

        mScene->setContext(mContext);

        printContextInfos();
        initializeGl();

        resize(QSize(640, 480));
        setTitle("tresta");
        setIcon(QIcon(":/assets/logo_64x64.png"));

        connect(this, SIGNAL(widthChanged(int)), this, SLOT(resizeGl()));
        connect(this, SIGNAL(heightChanged(int)), this, SLOT(resizeGl()));

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateScene()));
        timer->start(16);
    }

    void Window::printContextInfos() {
        if (!mContext->isValid())
            throw std::runtime_error("The OpenGL context is invalid!");

        mContext->makeCurrent(this);

        std::cout << "Window format version is: "
        << format().majorVersion() << "."
        << format().minorVersion() << std::endl;

        std::cout << "Context format version is: "
        << mContext->format().majorVersion()
        << "." << mContext->format().minorVersion() << std::endl;
        infoGL();
    }

    void Window::initializeGl() {
        mContext->makeCurrent(this);
        mScene->initialize();
    }

    void Window::paintGl() {
        if (!isExposed()) return;
        mContext->makeCurrent(this);
        mScene->render();
        mContext->swapBuffers(this);
    }

    void Window::resizeGl() {
        mContext->makeCurrent(this);
        mScene->resize(width(), height());
    }

    void Window::updateScene() {
        mScene->update(0.0f);
        paintGl();
    }

    void Window::handleKeyEvent(QKeyEvent *e) {
        keyPressEvent(e);
    }

    void Window::mousePressEvent(QMouseEvent *e) {
        currX = e->x();
        currY = e->y();
        switch (e->button()) {

            case Qt::LeftButton:
                if (keyboardOverride) {
                    if (keyboardTranslate)
                        translatePressed = true;
                    else if (keyboardZoom)
                        zoomPressed = true;
                    else {
                        rotatePressed = true;
                    }
                }
                else {
                    rotatePressed = true;
                }
                break;

            case Qt::RightButton:
                translatePressed = true;
                break;

            case Qt::MiddleButton:
                zoomPressed = true;
                break;
        }
    }

    void Window::mouseMoveEvent(QMouseEvent *e) {
        if (rotatePressed) {
            mScene->setRotate(e->x() - currX, e->y() - currY);
        }
        else if (translatePressed) {
            mScene->setTranslate(e->x() - currX, e->y() - currY);
        }
        else if (zoomPressed) {
            mScene->setZoom(e->x() - currX, e->y() - currY);
        }
        currX = e->x();
        currY = e->y();
    }

    void Window::mouseReleaseEvent(QMouseEvent *e) {
        switch (e->button()) {
            case Qt::LeftButton:
                if (keyboardOverride) {
                    if (keyboardTranslate)
                        translatePressed = false;
                    else if (keyboardZoom)
                        zoomPressed = false;
                    else
                        rotatePressed = false;
                }
                else {
                    rotatePressed = false;
                }
                break;

            case Qt::RightButton:
                translatePressed = false;
                break;

            case Qt::MiddleButton:
                zoomPressed = false;
                break;
        }
    }

    void Window::keyPressEvent(QKeyEvent *e) {
        switch (e->key()) {
            case Qt::Key_R:
                keyboardRotate = !keyboardRotate;
                keyboardTranslate = false;
                keyboardZoom = false;
                keyboardOverride = keyboardRotate;
                emit zoomChanged(keyboardZoom);
                emit panChanged(keyboardTranslate);
                emit rotateChanged(keyboardRotate);
                break;

            case Qt::Key_P:
                keyboardTranslate = !keyboardTranslate;
                keyboardRotate = false;
                keyboardZoom = false;
                keyboardOverride = keyboardTranslate;
                emit zoomChanged(keyboardZoom);
                emit panChanged(keyboardTranslate);
                emit rotateChanged(keyboardRotate);
                break;

            case Qt::Key_Z:
                keyboardZoom = !keyboardZoom;
                keyboardRotate = false;
                keyboardTranslate = false;
                keyboardOverride = keyboardZoom;
                emit zoomChanged(keyboardZoom);
                emit panChanged(keyboardTranslate);
                emit rotateChanged(keyboardRotate);
                break;

            default:
                mScene->handleKeyEvent(e->key());
        }

    }

} // namespace tresta
