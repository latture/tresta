#ifndef TRESTA_ABSTRACT_SCENE_H
#define TRESTA_ABSTRACT_SCENE_H

class QOpenGLContext;

class AbstractScene
{
public:
    AbstractScene() : mContext(0) {}
    virtual ~AbstractScene(){}

    void setContext(QOpenGLContext *context) { mContext = context; }
    QOpenGLContext* context() const { return mContext; }

    virtual void initialize() = 0;

    virtual void update(float t) = 0;

    virtual void render() = 0;

    virtual void resize(int width, int height) = 0;

    virtual void setRotate(int dx, int dy) = 0;

    virtual void setTranslate(int dx, int dy) = 0;

    virtual void setZoom(int dx, int dy) = 0;

    virtual void handleKeyEvent(int key) = 0;

protected:
    QOpenGLContext *mContext;
};

#endif // TRESTA_ABSTRACT_SCENE_H
