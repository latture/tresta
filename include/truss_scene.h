#ifndef TRESTA_TRUSS_SCENE
#define TRESTA_TRUSS_SCENE

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QVector3D>

#include "abstract_scene.h"
#include "containers.h"
#include "color_dialog.h"
#include "cylinder.h"
#include "sphere.h"

class QOpenGLFunctions;

namespace tresta {

    class TrussScene : public AbstractScene
    {
    public:
        /**
         * @brief Constructor
         * @details Builds the vertices for the original and deformed positions
         * as well as the camera position based on the input Job.
         */
        TrussScene(const Job &job);

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
        QOpenGLFunctions *mGLFunc;

        QOpenGLShaderProgram mSphereShader;
        QOpenGLShaderProgram mCylinderShader;

        QMatrix4x4 modelview;
        QMatrix4x4 modelview_inv;
        QMatrix4x4 modelnormal;
        QMatrix4x4 projection;
        std::vector<QMatrix4x4> vertexViewVector;
        std::vector<std::vector<QMatrix4x4>> deformedVertexViewVectors;

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

        void renderCylinder(const QMatrix4x4 &vertexView);
        void renderSphere(const QMatrix4x4 &vertexView);

        QMatrix4x4 buildVertexMatrix(const float angle, const Node &axis, const Node &translation);
        std::vector<QMatrix4x4> buildVertexMatrixVector(const std::vector<Node> &nodes,
                                                        const std::vector<Elem> &elems,
                                                        float x_scale_multiplier,
                                                        float z_scale_multiplier);
        void buildDeformedVertexViewVectors();
        void rebuildNodeStrips();
        void calcCenteringShift();
        void prepareShaders();
        void prepareVertexBuffers();
        void updateModelMatrices(QOpenGLShaderProgram &shader);

        void updateModelColor(const QColor &color, QOpenGLShaderProgram &shader);
        void updateVertexMatrix(const QMatrix4x4 &vertexView, QOpenGLShaderProgram &shader);
        void updateProjectionUniforms(int w, int h, QOpenGLShaderProgram &shader);
        void updateLightPosition(const QVector3D &position, QOpenGLShaderProgram &shader);

    };

} // namespace tresta

#endif // TRESTA_TRUSS_SCENE
