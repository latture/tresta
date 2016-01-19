#ifndef TRESTA_WINDOW_H
#define TRESTA_WINDOW_H

#include <QWindow>

#include "truss_scene.h"
#include "containers.h"
#include "demo_dialog.h"

class QMouseEvent;
class QOpenGLContext;

namespace tresta {

    class Window : public QWindow
    {
        Q_OBJECT
    public:
        explicit Window(Job &job, QScreen *screen = 0);
        ~Window() {};

    public slots:
        void handleKeyEvent(QKeyEvent *e);

    signals:
        void rotateChanged(bool isEnabled);
        void zoomChanged(bool isEnabled);
        void panChanged(bool isEnabled);

        void demoChanged(bool isEnabled);

    protected slots:
        void resizeGl();
        void paintGl();

        void demo();
        void updateScene();
        void mousePressEvent(QMouseEvent * e);
        void mouseMoveEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);
        void keyPressEvent(QKeyEvent *);

    private:
        QOpenGLContext *mContext;
        QScopedPointer<TrussScene> mScene;
        QString demoSaveDirectory;
        DemoDialog demoDialog;

        bool rotatePressed;
        bool keyboardRotate;
        bool translatePressed;
        bool keyboardTranslate;
        bool zoomPressed;
        bool keyboardZoom;
        bool keyboardOverride;
        bool demoMode;

        int currX, currY;
        int currDemoFrame;

        void printContextInfos();
        void initializeGl();
    };

} // namespace tresta

#endif // TRESTA_WINDOW_H
