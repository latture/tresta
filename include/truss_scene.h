#ifndef TRESTA_TRUSS_SCENE
#define TRESTA_TRUSS_SCENE

#include <QMatrix4x4>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QVector3D>

#include "abstract_scene.h"
#include "containers.h"
#include "color_dialog.h"
#include "cylinder.h"
#include "sphere.h"

namespace tresta {

    class TrussScene : public QObject, public AbstractScene
            {
    Q_OBJECT
    public slots:
        /**
         * Sets the vertex buffer object containing the mesh's original color to the current original color
         * specified by the color dialog.
         */
        void updateOrigColorBuffer();

        /**
         * Sets the vertex buffer object containing the mesh's deformed color to the current deformed color
         * specified by the color dialog.
         */
        void updateDefColorBuffer();

        void updateTransparencyEnabled(bool);

        void updateAlphaCutoff(float);

    public:
        /**
         * @brief Constructor
         * @details Builds the vertices for the original and deformed positions
         * as well as the camera position based on the input Job.
         */
        TrussScene(const Job &job, QObject *parent = nullptr);

        /**
         * @brief Initializes OpenGL functions, vertex buffers, and rendering properties.
         */
        void initialize();

        /**
         * @brief Updates the scene based on change in time.
         * @param t Change in time.
         */
        void update(float t);

        /**
         * Renders the original and deformed positions of the Job.
         */
        void render();

        /** Render in demo mode.
         *
         * @param frameNumber int. Current frame on the range `[0, 359]`. If a numbers `>359` will be wrapped around, i.e. `frameNumber %= 360`.
         */
        void demo(int frameNumber);

        /**
         * Updates the projection uniform variables for the shaders and sets the viewport size.
         * @param width  Window width.
         * @param height Window height
         */
        void resize(int width, int height);

        /**
         * Looks for keys S, O, or D to either set the deformation scale, toggle rendering
         * the original shape, and toggle the deformed shape, respectively.
         * @param key [description]
         */
        void handleKeyEvent(int key);

        /**
         * Sets the camera rotation based on mouse movement.
         * @param dx Change in x-direction of mouse.
         * @param dy Change in y-direction of mouse.
         */
        void setRotate(int dx, int dy);

        /**
         * Sets the camera translation based on mouse movement.
         * @param dx Change in x-direction of mouse.
         * @param dy Change in y-direction of mouse.
         */
        void setTranslate(int dx, int dy);

        /**
         * Sets the camera zoom based on mouse movement.
         * @param dx Change in x-direction of mouse.
         * @param dy Change in y-direction of mouse.
         */
        void setZoom(int dx, int dy);


        /**
         * Recalcualtes the deformed positions based on the input deformation scale.
         * @param scale Multiplier for deformations.
         */
        void setDeformationScale(float scale);

        /**
         * Returns the current multiplier on deformations.
         * @return Deformation scale
         */
        float getDeformationScale() const;

    private:
        QOpenGLFunctions_3_3_Core *mGLFunc;

        QOpenGLShaderProgram mSphereShader;
        QOpenGLShaderProgram mCylinderShader;

        QMatrix4x4 modelview;
        QMatrix4x4 modelview_inv;
        QMatrix4x4 modelnormal;
        QMatrix4x4 projection;
        std::vector<QMatrix4x4> vertexViewVector;
        std::vector<QMatrix4x4> deformedVertexViewVector;

        std::vector<QOpenGLBuffer> vertexViewColBuffers;
        std::vector<QOpenGLBuffer> defVertexViewColBuffers;
        std::vector<std::string> vertexViewColNames = {"vertexViewCol1", "vertexViewCol2", "vertexViewCol3", "vertexViewCol4"};

        QOpenGLBuffer origColorBuffer;
        QOpenGLBuffer defColorBuffer;
        QOpenGLBuffer userColorBuffer;

        QVector3D camera_rot;
        QVector3D camera_trans;

        QVector3D camera_rot_lag;
        QVector3D camera_trans_lag;

        Node global_min_pos;
        Node global_max_pos;
        Node global_centering_shift;

        Job job;

        Sphere sphere;
        Cylinder cylinder;

        ColorDialog colorDialog;

        float time;
        float camera_z0;
        float deformation_scale;
        const float camera_inertia;
        bool shadersInitialized;
        bool renderOriginal;
        bool renderDeformed;
        bool displacementsProvided;

        void setCamera(float tx, float ty, float tz, float rx, float ry, float rz);

        QMatrix4x4 buildVertexMatrix(const float angle, const Node &axis, const Node &translation);
        std::vector<QMatrix4x4> buildVertexMatrixVector(const std::vector<Node> &nodes,
                                                        const std::vector<Elem> &elems,
                                                        float x_scale_multiplier,
                                                        float z_scale_multiplier);
        void buildDeformedVertexViewVector();
        void rebuildNodeStrips();
        void calcCenteringShift();
        void exportJob();
        void prepareShaders();
        void createVertexViewBuffers(const std::vector<QMatrix4x4>& viewVector, std::vector<QOpenGLBuffer>& viewBuffers);
        void setColorBuffer(const std::vector<QColor> &colors, QOpenGLBuffer &buffer);
        void setVertexColor(QOpenGLBuffer &colorBuffer, GLuint divisor);
        void bindColBuffer(std::vector<QOpenGLBuffer> &colBuffer);
        void releaseColBuffer(std::vector<QOpenGLBuffer> &colBuffer);
        void prepareVertexBuffers();
        void updateModelMatrices(QOpenGLShaderProgram &shader);

        void updateProjectionUniforms(int w, int h, QOpenGLShaderProgram &shader);
        void setAlphaCutoff(float cutoff, QOpenGLShaderProgram &shader);
    };

} // namespace tresta

#endif // TRESTA_TRUSS_SCENE
