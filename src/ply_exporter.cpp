#include "ply_exporter.h"
#include <QApplication>
#include <QFileDialog>
#include <QMutexLocker>
#include <QMessageBox>
#include <QPushButton>
#include <iostream>
#include <qtconcurrentrun.h>

namespace tresta {

    PlyExporter::PlyExporter(QWidget* parent) : QWidget(parent),
                                                progressMutex()
    {
    }

    void PlyExporter::operator()(const QString &fileName, const QString& description, 
                                 const Shape *shape, const Job &job,
                                 const std::vector<QMatrix4x4> &vertexViewVector) {
        exportPly(fileName, description, shape, job, vertexViewVector);
    }

    void PlyExporter::exportPly(const QString& fileName, const QString& description, const Shape* shape, const Job& job, const std::vector<QMatrix4x4>& vertexViewVector) {

        if (!fileName.isEmpty()) {
            progress = new QProgressDialog(this);
            progress->setModal(true);
            progress->setMaximum(100);
            QString labelText = description + QString("\nStep 1 of 3: Building vertices...");
            progress->setLabelText(tr(labelText.toStdString().c_str()));
            progress->show();

            QFuture<bool> t1 = QtConcurrent::run(this, &PlyExporter::createPlyVertices, shape, job, vertexViewVector);
            if (!t1.result()) {
                handleCancelledExport();
                return;
            }
            labelText = description + QString("\nStep 2 of 3: Building faces...");
            progress->setLabelText(tr(labelText.toStdString().c_str()));
            QFuture<bool> t2 = QtConcurrent::run(this, &PlyExporter::createPlyFaces, shape, job, vertexViewVector);
            if (!t2.result()) {
                handleCancelledExport();
                return;
            }

            progress->setValue(0);
            progress->setMaximum(0);
            labelText = description + QString("\nStep 3 of 3: Saving file...");
            progress->setLabelText(tr(labelText.toStdString().c_str()));
            QList<QPushButton *> L = progress->findChildren<QPushButton *>();
            L.at(0)->hide();
            L.at(0)->show();
            qApp->processEvents();

            QFuture<void> t3 = QtConcurrent::run(this, &PlyExporter::saveFile, fileName);
            t3.waitForFinished();

            progress->close();
        }                
    }

    bool PlyExporter::createPlyVertices(const Shape* shape, const Job& job, const std::vector<QMatrix4x4>& vertexViewVector) {
        const size_t vals_per_pt = 3;
        const size_t pts_per_shape = shape->vertices.size() / vals_per_pt;
        std::vector< std::vector< float > > tmp_vertices(vertexViewVector.size() * pts_per_shape, std::vector<float>(2 * vals_per_pt));
        QVector4D originalPoint, transformedPoint, originalNormal, transformedNormal;
        originalPoint.setW(1.0f);
        originalNormal.setW(0.0f);
        int new_value, current_value = 0;
        float value_divisor_inv = 1.0f / ((float) (vertexViewVector.size())) * 100.0f;

        // for each instanced shape
        for (size_t i = 0; i < vertexViewVector.size(); ++i) {
            // for each vertex in the shape
            for (size_t j = 0; j < pts_per_shape; ++j) {
                // copy original shape vertex and normal
                for (size_t k = 0; k < vals_per_pt; ++k) {
                    originalPoint[k] = shape->vertices[vals_per_pt * j + k];
                     originalNormal[k] = shape->normals[vals_per_pt * j + k];
                }

                // transform into global position
                transformedPoint = vertexViewVector[i] * originalPoint;
                transformedNormal = vertexViewVector[i] * originalNormal;
                transformedNormal.normalize();

                // store result in output vector
                for (size_t k = 0; k < vals_per_pt; ++k) {
                    tmp_vertices[i * pts_per_shape + j][k] = transformedPoint[k];
                     tmp_vertices[i * pts_per_shape + j][k + vals_per_pt] = transformedNormal[k];
                }
            }

            QMutexLocker ml(&progressMutex);
            new_value = (int) ((float) (i) * value_divisor_inv);
            if (new_value > current_value) {
                current_value = new_value;
                progress->setValue(current_value);
            }
            qApp->processEvents();
            if (progress->wasCanceled()) {
                return false;
            }
        }
        tmp_vertices.swap(vertices);
        return true;
    }

    bool PlyExporter::createPlyFaces(const Shape* shape, const Job& job, const std::vector<QMatrix4x4>& vertexViewVector) {
        const unsigned int ind_per_face = 3;
        const unsigned int pts_per_vert = 3;
        const size_t indices_per_shape = shape->indices.size();
        const size_t faces_per_shape = indices_per_shape / ind_per_face;
        const size_t pts_per_shape = shape->vertices.size() / pts_per_vert;
        std::vector< std::vector< unsigned int > > tmp_faces(vertexViewVector.size() * faces_per_shape, std::vector<unsigned int>(1 + ind_per_face));
        int new_value, current_value = 0;
        float value_divisor_inv = 1.0f / ((float) (vertexViewVector.size())) * 100.0f;

        // for each instanced shape
        for (size_t i = 0; i < vertexViewVector.size(); ++i) {
            // for each face in the shape
            for (int j = 0; j < faces_per_shape; ++j) {
                // set the number of indices on the face
                tmp_faces[faces_per_shape * i + j][0] = ind_per_face;

                // transform index into global space
                for (size_t k = 0; k < ind_per_face; ++k) {
                    tmp_faces[faces_per_shape * i + j][k + 1] = pts_per_shape * i + shape->indices[ind_per_face * j + k];
                }
            }

            QMutexLocker ml(&progressMutex);
            new_value = (int) ((float) (i) * value_divisor_inv);
            if (new_value > current_value) {
                current_value = new_value;
                progress->setValue(current_value);
            }
            qApp->processEvents();
            if (progress->wasCanceled()) {
                return false;
            }
        }
        tmp_faces.swap(faces);
        return true;
    }

    void PlyExporter::saveFile(const QString &fileName) {
        QFile outputFile(fileName);
        outputFile.open(QIODevice::WriteOnly | QIODevice::Text);

        if (!outputFile.isOpen()) {
            QString err_what = QString("Unable to open file %1").arg(fileName);
            throw std::runtime_error(err_what.toStdString().c_str());
        }

        QTextStream outStream(&outputFile);
        QString header = QString("ply\n"
                                 "format ascii 1.0\n"
                                 "comment Made by tresta\n"
                                 "element vertex %1\n"
                                 "property float x\n"
                                 "property float y\n"
                                 "property float z\n"
                                 "property float nx\n"
                                 "property float ny\n"
                                 "property float nz\n"
                                 "element face %2\n"
                                 "property list uchar int vertex_indices\n"
                                 "end_header\n").arg(QString::number(vertices.size()), QString::number(faces.size()));

        outStream << header;
        write2DVector(outStream, vertices);
        write2DVector(outStream, faces);

        outputFile.close();
    }

    void PlyExporter::handleCancelledExport() {
        progress->close();
        QMessageBox::warning(0, QString("Ply exporter"), QString("Mesh exported cancelled."));

    }

    template <typename T>
    void PlyExporter::write2DVector(QTextStream & outStream, const std::vector<T> &data) {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data[i].size(); ++j) {
                outStream << data[i][j];
                if (j < data[i].size() - 1)
                    outStream << " ";
                else
                    outStream << "\n";
            }
        }
    }
} // namespace tresta