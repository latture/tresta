#include "truss_scene.h"
#include <Eigen/Geometry>
#include <QOpenGLFunctions>
#include <QInputDialog>
#include <QtWidgets/qmessagebox.h>
#include "glassert.h"
#include "setup.h"

namespace tresta {

    TrussScene::TrussScene(const Job &_job)
            : mSphereShader(),
              mCylinderShader(),
              job(_job),
              colorDialog(_job.colors.size() > 0, _job.displacements.size() > 0),
              time(0.0f),
              deformation_scale(1.0),
              camera_inertia(0.1f),
              shadersInitialized(false),
              renderOriginal(true),
              renderDeformed(true),
              displacementsProvided(false) {
        if (job.displacements.size() > 0) {
            displacementsProvided = true;
        }
        else {
            renderDeformed = false;
        }

        calcCenteringShift();
        camera_z0 = -1.75f * std::abs(global_max_pos.z());
        camera_rot = QVector3D(30.0f, -30.0f, 0.0f);
        camera_trans = QVector3D(0.0f, 0.0f, camera_z0);

        camera_rot_lag = QVector3D(0.0f, 0.0f, 0.0f);
        camera_trans_lag = QVector3D(0.0f, 0.0f, camera_z0);

        modelview.setToIdentity();
        modelview_inv.setToIdentity();
        modelnormal.setToIdentity();
        projection.setToIdentity();

        sphere.initialize();
        cylinder.initialize();

        vertexViewVector = buildVertexMatrixVector(job.nodes, job.elems, 1.0f, 1.0f);

        buildDeformedVertexViewVectors();
    }

    void TrussScene::initialize() {
        mGLFunc = new QOpenGLFunctions();
        mGLFunc->initializeOpenGLFunctions();
        mGLFunc->glEnable(GL_DEPTH_TEST);
        mGLFunc->glEnable(GL_BLEND);
        mGLFunc->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float red = 1.0f;
        float green = 1.0f;
        float blue = 1.0f;

        mGLFunc->glClearColor(red, green, blue, 0.0);

        prepareShaders();
        prepareVertexBuffers();

        float val = 10.0f * camera_z0;
        QVector3D lightPos(-val, val, -val);
        updateLightPosition(lightPos, mSphereShader);
        updateLightPosition(lightPos, mCylinderShader);
    }

    void TrussScene::update(float t) {
        Q_UNUSED(t);
    }

    void TrussScene::render() {
        glAssert(mGLFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        for (short c = 0; c < 3; ++c) {
            camera_trans_lag[c] += (camera_trans[c] - camera_trans_lag[c]) * camera_inertia;
            camera_rot_lag[c] += (camera_rot[c] - camera_rot_lag[c]) * camera_inertia;
        };

        modelview.setToIdentity();
        modelview.translate(camera_trans_lag);
        modelview.rotate(camera_rot_lag[0], 1.0, 0.0, 0.0);
        modelview.rotate(camera_rot_lag[1], 0.0, 1.0, 0.0);
        modelview.rotate(camera_rot_lag[2], 0.0, 0.0, 1.0);
        modelview_inv = modelview.inverted();
        modelnormal = modelview_inv.transposed();

        updateModelMatrices(mCylinderShader);
        cylinder.mVAO.bind();

        if (!renderOriginal || !renderDeformed)
            mGLFunc->glEnable(GL_CULL_FACE);
        else {
            mGLFunc->glDisable(GL_CULL_FACE);
        }

        if (renderDeformed) {
            updateModelColor(colorDialog.getDefColor(), mCylinderShader);
            for (size_t i = 0; i < deformedVertexViewVectors.size(); ++i) {
                for (size_t j = 0; j < deformedVertexViewVectors[i].size(); ++j) {
                    renderCylinder(deformedVertexViewVectors[i][j]);
                }
            }
        }

        if (renderOriginal) {
//            QColor color = QColor::fromRgbF(0.0824, 0.3961, 0.7529, 1.0);
//            if (renderDeformed && displacementsProvided) {
//                color.setAlphaF(0.5);
//            }

            updateModelColor(colorDialog.getOrigColor(), mCylinderShader);
            for (size_t i = 0; i < job.elems.size(); ++i) {
                renderCylinder(vertexViewVector[i]);
            }
        }

        // updateModelMatrices(mSphereShader);
        // updateModelColor(color, mSphereShader);
        // sphere.mVAO.bind();
        // QMatrix4x4 vertexView;
        // for (size_t i = 0; i < job.nodes.size(); ++i) {
        //     vertexView.setToIdentity();
        //     vertexView.translate(job.nodes[i].x(), job.nodes[i].y(), -job.nodes[i].z());
        //     renderSphere(vertexView);
        // }

        glCheckError();
    }

    void TrussScene::resize(int width, int height) {
        updateProjectionUniforms(width, height, mSphereShader);
        updateProjectionUniforms(width, height, mCylinderShader);
        glAssert(mGLFunc->glViewport(0, 0, width, height));
    }

    void TrussScene::handleKeyEvent(int key) {
        switch (key) {
            case Qt::Key_S:
                if (displacementsProvided) {
                    deformation_scale = (float) QInputDialog::getDouble(0, QString("Choose deformation scale"), 0,
                                                                        (double) deformation_scale, 1.0e-4, 1.0e8, 4);
                    rebuildNodeStrips();
                    buildDeformedVertexViewVectors();
                }
                else {
                    QMessageBox::warning(0, QString("Warning"),
                                         QString("Cannot select scale.\nNo displacements provided."));
                }

                break;

            case Qt::Key_O:
                renderOriginal = !renderOriginal;
                break;

            case Qt::Key_D:
                if (displacementsProvided)
                    renderDeformed = !renderDeformed;
                else
                    QMessageBox::warning(0, QString("Warning"),
                                         QString("Cannot toggle deformed shape.\nNo displacements provided."));
                break;

            case Qt::Key_C:
                colorDialog.exec();
                break;

            default:
                break;
        }
    }

    void TrussScene::setRotate(int dx, int dy) {
        camera_rot[0] += ((float) dy) / 5.0f;
        camera_rot[1] += ((float) dx) / 5.0f;
    }

    void TrussScene::setTranslate(int dx, int dy) {
        camera_trans[0] += ((float) dx) / 100.0f;
        camera_trans[1] -= ((float) dy) / 100.0f;
    }

    void TrussScene::setZoom(int dx, int dy) {
        Q_UNUSED(dx);
        camera_trans[2] += (((float) dy) / 100.0f) * 0.5f * std::fabs(camera_trans[2]);
    }

    void TrussScene::setDeformationScale(float scale) {
        deformation_scale = scale;
        rebuildNodeStrips();
        buildDeformedVertexViewVectors();
    }

    float TrussScene::getDeformationScale() const {
        return deformation_scale;
    }

    void TrussScene::renderCylinder(const QMatrix4x4 &vertexView) {
        updateVertexMatrix(vertexView, mCylinderShader);
        mGLFunc->glDrawElements(GL_TRIANGLES, cylinder.indices.size(), GL_UNSIGNED_SHORT, 0);
    }

    void TrussScene::renderSphere(const QMatrix4x4 &vertexView) {
        updateVertexMatrix(vertexView, mSphereShader);
        mGLFunc->glDrawElements(GL_TRIANGLES, sphere.indices.size(), GL_UNSIGNED_SHORT, 0);
    }

    QMatrix4x4 TrussScene::buildVertexMatrix(const float angle, const Node &axis, const Node &translation) {
        const float c = std::cos(angle);
        const float s = std::sin(angle);
        const float oc = 1.0f - c;
        const float x = axis.x();
        const float y = axis.y();
        const float z = axis.z();

        return QMatrix4x4(c + x * x * oc, x * y * oc - z * s, x * z * oc + y * s, translation.x(),
                          y * x * oc + z * s, c + y * y * oc, y * z * oc - x * s, translation.y(),
                          -(z * x * oc - y * s), -(z * y * oc + x * s), -(c + z * z * oc), -translation.z(),
                          0.0f, 0.0f, 0.0f, 1.0f);
    }

    std::vector<QMatrix4x4> TrussScene::buildVertexMatrixVector(const std::vector<Node> &nodes,
                                                                const std::vector<Elem> &elems,
                                                                float x_scale_multiplier,
                                                                float z_scale_multiplier) {
        std::vector<QMatrix4x4> vector_out;
        vector_out.reserve(elems.size());
        int nn1, nn2;
        float angle, length;
        Node node1, node2, dn, rot_axis, y_axis, z_axis;
        QMatrix4x4 current_matrix;
        y_axis << 0.0f, 1.0f, 0.0f;
        z_axis << 0.0f, 0.0f, 1.0f;

        for (size_t i = 0; i < elems.size(); ++i) {
            nn1 = elems[i].node_numbers[0];
            nn2 = elems[i].node_numbers[1];

            node1 = nodes[nn1];
            node2 = nodes[nn2];

            dn = node2 - node1;
            length = dn.norm();

            if (fabs(dn.x()) < 1.0e-5f && fabs(dn.z()) < 1.0e-5f) {
                Node axis;
                axis << 0.0f, 0.0f, 1.0f;
                if (dn.y() < 0.0f) {
                    current_matrix = buildVertexMatrix(180.0f, z_axis, node1 - global_centering_shift);
                }
                else {
                    current_matrix = buildVertexMatrix(0.0f, z_axis, node1 - global_centering_shift);
                }
            }
            else {
                rot_axis = y_axis.cross(dn);
                rot_axis.normalize();
                angle = std::acos(dn.dot(y_axis) / length);
                current_matrix = buildVertexMatrix(angle, rot_axis, node1 - global_centering_shift);
            }

            current_matrix.scale(x_scale_multiplier, length, z_scale_multiplier);
            vector_out.push_back(current_matrix);
        }
        return vector_out;
    }

    void TrussScene::buildDeformedVertexViewVectors() {
        deformedVertexViewVectors.clear();
        if (job.node_strips.size() > 0) {
            deformedVertexViewVectors.reserve(job.node_strips.size());
            const size_t num_interp_points = job.node_strips[0].size();
            std::vector<Elem> def_elems(num_interp_points - 1);

            for (size_t i = 0; i < def_elems.size(); ++i) {
                def_elems[i].node_numbers << i, i + 1;
            }

            for (size_t i = 0; i < job.node_strips.size(); ++i) {
                deformedVertexViewVectors.push_back(
                        buildVertexMatrixVector(job.node_strips[i], def_elems, 0.99, 0.99)
                );
            }
        }
    }

    void TrussScene::rebuildNodeStrips() {
        if (job.displacements.size() > 0)
            job.node_strips = createNodeStrips(job.nodes, job.elems, job.displacements, deformation_scale);
    }

    void TrussScene::calcCenteringShift() {
        float max_val = std::numeric_limits<float>::max();
        float min_val = std::numeric_limits<float>::lowest();
        global_max_pos << min_val, min_val, min_val;
        global_min_pos << max_val, max_val, max_val;
        for (size_t i = 0; i < job.nodes.size(); ++i) {
            for (size_t j = 0; j < job.nodes[i].size(); ++j) {
                if (job.nodes[i][j] > global_max_pos[j]) {
                    global_max_pos[j] = job.nodes[i][j];
                }
                if (job.nodes[i][j] < global_min_pos[j]) {
                    global_min_pos[j] = job.nodes[i][j];
                }
            }
        }
        global_centering_shift = global_max_pos - global_min_pos;
        global_centering_shift /= 2.0f;
    }

    void TrussScene::prepareShaders() {
        if (!mSphereShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":assets/shaders/blinn_phong.vert")) {
            qCritical() << "Error adding sphere vertex shader.";
        }

        if (!mSphereShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":assets/shaders/blinn_phong.frag")) {
            qCritical() << "Error adding sphere fragment shader.";
        }
        if (!mSphereShader.link()) {
            qCritical() << "Error linking sphere shader.";
        }

        if (!mCylinderShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":assets/shaders/blinn_phong.vert")) {
            qCritical() << "Error adding cylinder vertex shader.";
        }
        if (!mCylinderShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":assets/shaders/blinn_phong.frag")) {
            qCritical() << "Error adding cylinder fragment shader.";
        }
        if (!mCylinderShader.link()) {
            qCritical() << "Error linking cylinder shader.";
        }

        glCheckError();

        shadersInitialized = true;
    }

    void TrussScene::prepareVertexBuffers() {
        sphere.prepareVertexBuffers();

        sphere.mVAO.bind();
        mSphereShader.bind();

        sphere.mVertexPositionBuffer.bind();
        mSphereShader.enableAttributeArray("vertexPosition");
        mSphereShader.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

        sphere.mVertexNormalBuffer.bind();
        mSphereShader.enableAttributeArray("vertexNormal");
        mSphereShader.setAttributeBuffer("vertexNormal", GL_FLOAT, 0, 3);

        cylinder.prepareVertexBuffers();

        cylinder.mVAO.bind();
        mCylinderShader.bind();

        cylinder.mVertexPositionBuffer.bind();
        mCylinderShader.enableAttributeArray("vertexPosition");
        mCylinderShader.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

        cylinder.mVertexNormalBuffer.bind();
        mCylinderShader.enableAttributeArray("vertexNormal");
        mCylinderShader.setAttributeBuffer("vertexNormal", GL_FLOAT, 0, 3);

        cylinder.mVAO.release();
        glCheckError();
    }

    void TrussScene::updateModelMatrices(QOpenGLShaderProgram &shader) {
        assert(shadersInitialized);

        shader.bind();
        shader.setUniformValue("modelview", modelview);
        shader.setUniformValue("modelview_inv", modelview.inverted());
        shader.setUniformValue("modelnormal", modelnormal);
    }

    void TrussScene::updateModelColor(const QColor &color, QOpenGLShaderProgram &shader) {
        assert(shadersInitialized);

        shader.bind();
        shader.setUniformValue("vertexColor", color);
    }

    void TrussScene::updateVertexMatrix(const QMatrix4x4 &vertexView, QOpenGLShaderProgram &shader) {
        assert(shadersInitialized);

        shader.bind();
        shader.setUniformValue("vertexView", vertexView);
    }

    void TrussScene::updateProjectionUniforms(int w, int h, QOpenGLShaderProgram &shader) {
        assert(shadersInitialized);

        shader.bind();
        projection.setToIdentity();
        projection.perspective(60.0f, (float) w / (float) h, 0.01, 1000.0);
        shader.setUniformValue("projection", projection);
    }

    void TrussScene::updateLightPosition(const QVector3D &position, QOpenGLShaderProgram &shader) {
        assert(shadersInitialized);

        shader.bind();
        shader.setUniformValue("lightPos", position);
    }

} // namespace tresta
