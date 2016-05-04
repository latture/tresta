#ifndef TRESTA_PLY_EXPORTER_H
#define TRESTA_PLY_EXPORTER_H

#include "containers.h"
#include "shape.h"

#include <QWidget>
#include <QMatrix4x4>
#include <QProgressDialog>
#include <QMutex>

namespace tresta {

    class PlyExporter : public QWidget
    {
    Q_OBJECT
    public:
        PlyExporter(QWidget* parent=0);
        PlyExporter( const PlyExporter& other ) {};

        void operator() (const QString& fileName, const QString& description, const Shape* shape, const Job& job, const std::vector<QMatrix4x4>& vertexViewVector);

        void exportPly(const QString& fileName, const QString& description, const Shape* shape, const Job& job, const std::vector<QMatrix4x4>& vertexViewVector);

    private:
        bool createPlyVertices(const Shape* shape, const Job& job, const std::vector<QMatrix4x4>& vertexViewVector);
        bool createPlyFaces(const Shape* shape, const Job& job, const std::vector<QMatrix4x4>& vertexViewVector);
        void saveFile(const QString& fileName);

        void handleCancelledExport();
        
        template <typename T>
        void write2DVector(QTextStream & outStream, const std::vector<T> &data);

        std::vector<std::vector<float>> vertices;
        std::vector<std::vector<unsigned int>> faces;

        QProgressDialog* progress;
        QMutex progressMutex;
    };

} // namespace tresta

#endif // TRESTA_PLY_EXPORTER_H