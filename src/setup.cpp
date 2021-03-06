#include "boost/format.hpp"
#include "csv_parser.h"
#include <Eigen/Geometry>
#include "setup.h"

namespace tresta {

    namespace {
        template <typename T>
        void createVectorFromJSON(const rapidjson::Document &config_doc,
                                  const std::string &variable,
                                  std::vector<std::vector<T>> &data) {
            if (!config_doc.HasMember(variable.c_str())) {
                throw std::runtime_error(
                    (boost::format("Configuration file does not have requested member variable %s.") % variable).str()
                );
            }

            if (!config_doc[variable.c_str()].IsString()) {
                throw std::runtime_error(
                        (boost::format("Value associated with variable %s is not a string.") % variable).str()
                );
            }
            CSVParser csv;
            std::string filename(config_doc[variable.c_str()].GetString());
            csv.parseToVector(filename, data);
            if (data.size() == 0) {
                throw std::runtime_error(
                    (boost::format("No data was loaded for variable %s.") % variable).str()
                );
            }
        }

        void updateTransforms(const Node &nx, const Node &ny,
                              Eigen::Matrix<float, 12, 12, Eigen::RowMajor> &Aelem,
                              Eigen::Matrix<float, 3, 3, Eigen::RowMajor> &transform_components) {
            // calculate the unit normal vector in local z direction
            Node nz;
            nz = nx.cross(ny);
            nz.normalize();

            // update rotation matrix
            Aelem(0, 0) = nx(0);
            Aelem(0, 1) = nx(1);
            Aelem(0, 2) = nx(2);
            Aelem(1, 0) = ny(0);
            Aelem(1, 1) = ny(1);
            Aelem(1, 2) = ny(2);
            Aelem(2, 0) = nz(0);
            Aelem(2, 1) = nz(1);
            Aelem(2, 2) = nz(2);

            Aelem(3, 3) = nx(0);
            Aelem(3, 4) = nx(1);
            Aelem(3, 5) = nx(2);
            Aelem(4, 3) = -ny(0);
            Aelem(4, 4) = -ny(1);
            Aelem(4, 5) = -ny(2);
            Aelem(5, 3) = nz(0);
            Aelem(5, 4) = nz(1);
            Aelem(5, 5) = nz(2);

            Aelem(6, 6) = nx(0);
            Aelem(6, 7) = nx(1);
            Aelem(6, 8) = nx(2);
            Aelem(7, 6) = ny(0);
            Aelem(7, 7) = ny(1);
            Aelem(7, 8) = ny(2);
            Aelem(8, 6) = nz(0);
            Aelem(8, 7) = nz(1);
            Aelem(8, 8) = nz(2);

            Aelem(9, 9) = nx(0);
            Aelem(9, 10) = nx(1);
            Aelem(9, 11) = nx(2);
            Aelem(10, 9) = -ny(0);
            Aelem(10, 10) = -ny(1);
            Aelem(10, 11) = -ny(2);
            Aelem(11, 9) = nz(0);
            Aelem(11, 10) = nz(1);
            Aelem(11, 11) = nz(2);

            // update tranformation components
            transform_components << nx(0), nx(1), nx(2),
                                    ny(0), ny(1), ny(2),
                                    nz(0), nz(1), nz(2);
        };
    }

    rapidjson::Document parseJSONConfig(const std::string &config_filename) {
        rapidjson::Document config_doc;

        FILE* config_file_ptr = fopen(config_filename.c_str(), "r");

        if (!config_file_ptr) {
            throw std::runtime_error(
                (boost::format("Cannot open configuration input file %s.") % config_filename).str()
            );
        }
        char readBuffer[65536];
        rapidjson::FileReadStream config_stream(config_file_ptr, readBuffer, sizeof(readBuffer));
        config_doc.ParseStream(config_stream);
        fclose(config_file_ptr);

        if (!config_doc.IsObject()) {
            throw std::runtime_error(
                (boost::format("Invalid configuration file: %s") % config_filename).str()
            );
        }

        return config_doc;
    }

    std::vector<Node> createNodeVecFromJSON(const rapidjson::Document &config_doc) {
        std::vector< std::vector< double > > nodes_vec;
        try {
            createVectorFromJSON(config_doc, "nodes", nodes_vec);
        }
        catch (std::runtime_error& e) {
            throw;
        }

        std::vector<Node> nodes_out(nodes_vec.size());
        Node n;

        for(size_t i = 0; i < nodes_vec.size(); ++i) {

            if (nodes_vec[i].size() != 3) {
                throw std::runtime_error(
                    (boost::format("Row %d in nodes does not specify x, y and z coordinates.") % i).str()
                );
            }
            n << nodes_vec[i][0], nodes_vec[i][1], nodes_vec[i][2];
            nodes_out[i] = n;
        }
        return nodes_out;
    }

    std::vector<Elem> createElemVecFromJSON(const rapidjson::Document &config_doc) {
        std::vector< std::vector< unsigned int > > elems_vec;
        std::vector< std::vector< float > > props_vec;

        createVectorFromJSON(config_doc, "elems", elems_vec);
        createVectorFromJSON(config_doc, "props", props_vec);

        if (elems_vec.size() != props_vec.size()) {
            throw std::runtime_error("The number of rows in elems did not match props.");
        }

        std::vector<Elem> elems_out(elems_vec.size());
        Props p;
        size_t nx_idx, ny_idx, nz_idx;
        for(size_t i = 0; i < elems_vec.size(); ++i) {
            if (elems_vec[i].size() != 2) {
                throw std::runtime_error(
                    (boost::format("Row %d in elems does not specify 2 nodal indices [nn1,nn2].") % i).str()
                );
            }
            if (props_vec[i].size() < 3) {
                throw std::runtime_error(
                    (boost::format("Row %d in props does not specify at least 3 property values "
                                   "[..., nx, ny, nz]") % i).str()
                );
            }
            nx_idx = props_vec[i].size() - 3;
            ny_idx = props_vec[i].size() - 2;
            nz_idx = props_vec[i].size() - 1;
            p.normal_vec <<  props_vec[i][nx_idx],  props_vec[i][ny_idx],  props_vec[i][nz_idx];
            elems_out[i] = Elem(elems_vec[i][0], elems_vec[i][1], p);
        }
        return elems_out;
    }

    std::vector<Displacement> createDisplacementVecFromJSON(const rapidjson::Document &config_doc) {
        std::vector< std::vector< float > > disp_vec;

        if (config_doc.HasMember("displacements")) {
            createVectorFromJSON(config_doc, "displacements", disp_vec);
        }

        std::vector<Displacement> disp_out(disp_vec.size());

        for(size_t i = 0; i < disp_vec.size(); ++i) {
            if (disp_vec[i].size() != 6) {
                throw std::runtime_error(
                    (boost::format("Row %d in displacements does not specify x, y and z translations and rotations.") % i).str()
                );
            }
            disp_out[i] << disp_vec[i][0], disp_vec[i][1], disp_vec[i][2],
                    disp_vec[i][3], disp_vec[i][4], disp_vec[i][5];
        }
        return disp_out;
    }

    std::vector<QColor> createColorVecFromJSON(const rapidjson::Document &config_doc) {
        std::vector<std::vector<float> > color_vec;

        if (config_doc.HasMember("colors")) {
            createVectorFromJSON(config_doc, "colors", color_vec);
        }

        std::vector<QColor> color_out(color_vec.size());

        for (size_t i = 0; i < color_vec.size(); ++i) {
            if (color_vec[i].size() != 4) {
                throw std::runtime_error(
                    (boost::format("Row %d in colors does not specify [R, G, B, A] values.") % i).str()
                );
            }
            color_out[i] = QColor::fromRgbF(color_vec[i][0], color_vec[i][1], color_vec[i][2], color_vec[i][3]);
        }
        return color_out;
    }

    std::vector<std::vector<Node>> createNodeStrips(const std::vector<Node> &nodes,
                                                    const std::vector<Elem> &elems,
                                                    const std::vector<Displacement> &displacements,
                                                    const float scale) {

        if (displacements.size() != nodes.size()) {
            throw std::runtime_error(
                (boost::format("Number of rows in displacements (%d) do not match the number number of nodes (%d).") % displacements.size() % nodes.size()).str()
            );
        }

        const unsigned int num_interp_points = 5;
        unsigned int nn1, nn2;
        float element_length;
        Node nx, ny, node1, node2;
        std::vector<std::vector<Node>> node_strips_out(elems.size(), std::vector<Node>(num_interp_points));
        Eigen::Matrix<float, num_interp_points, 1> ip_1, ip_2, ip_3, ip_1_inv, tmp1, tmp2, tmp3, tmp4;
        Eigen::Matrix<float, 12, 12, Eigen::RowMajor> transform_matrix;
        Eigen::Matrix<float, 3, 3, Eigen::RowMajor> transform_components, inv_transform_components;
        Eigen::Matrix<float, 12, 1> global_disp, local_disp;
        Eigen::Matrix<float, num_interp_points, 3, Eigen::RowMajor> local_pos_strip, global_pos_strip, global_combined;
        Eigen::Matrix<float, 3, num_interp_points, Eigen::RowMajor> local_disp_strip, global_disp_strip;

        transform_matrix.setZero();

      	ip_1.setLinSpaced(num_interp_points, 0.0f, 1.0f);
        ip_1_inv = 1.0f - ip_1.array();
        ip_2 = ip_1.cwiseProduct(ip_1);
        ip_3 = ip_2.cwiseProduct(ip_1);

        tmp1 = 1.0f - 3.0f * ip_2.array() + 2.0f * ip_3.array();
        tmp2 = ip_1 - 2.0f * ip_2 + ip_3;
        tmp3 = 3.0f * ip_2 - 2.0f * ip_3;
        tmp4 = -ip_2 + ip_3;

        for (unsigned int i = 0; i < elems.size(); ++i){
            nn1 = elems[i].node_numbers[0];
            nn2 = elems[i].node_numbers[1];

            node1 = nodes[nn1];
            node2 = nodes[nn2];

            // calculate normal vector along element length
            nx = node2 - node1;
            element_length = nx.norm();
            nx /= element_length;

            ny << elems[i].props.normal_vec;
            ny.normalize();

            // update global to local transformation matrix & components
            updateTransforms(nx, ny, transform_matrix, transform_components);
            inv_transform_components = transform_components.inverse();

            global_disp.block<6, 1>(0, 0) = displacements[nn1];
            global_disp.block<6, 1>(6, 0) = displacements[nn2];

            local_disp = transform_matrix * global_disp;

            // interpolate displacements between nodal end points
            local_disp_strip.row(0) = local_disp(0) * ip_1_inv + local_disp(6) * ip_1;
            local_disp_strip.row(1) = local_disp(1) * tmp1 + element_length * local_disp(5) * tmp2 + local_disp(7) * tmp3 + element_length * local_disp(11) * tmp4;
            local_disp_strip.row(2) = local_disp(2) * tmp1 + element_length * local_disp(4) * tmp2 + local_disp(8) * tmp3 + element_length * local_disp(10) * tmp4;

            // transform displacements from local to global space
            global_disp_strip = inv_transform_components * local_disp_strip;

            // calculate the original position of the interpolated strip of points
            global_pos_strip = ip_1_inv * node1.transpose() + ip_1 * node2.transpose();

            global_combined = global_pos_strip + scale * global_disp_strip.transpose();

            for (unsigned int j = 0; j < num_interp_points; ++j)
            {
                node_strips_out[i][j] << global_combined(j, 0), global_combined(j, 1), global_combined(j, 2);
            }
        }

        return node_strips_out;
    }

    Job createJobFromJSON(const rapidjson::Document &config_doc) {
        std::vector<Node> nodes = createNodeVecFromJSON(config_doc);
        std::vector<Elem> elems = createElemVecFromJSON(config_doc);
        std::vector<Displacement> disp = createDisplacementVecFromJSON(config_doc);
        std::vector<QColor> colors = createColorVecFromJSON(config_doc);
        std::vector<std::vector<Node>> node_strips;
        if (disp.size() > 0) {
            node_strips = createNodeStrips(nodes, elems, disp, 1.0f);
        }
        if (colors.size() > 0 && elems.size() != colors.size()) {
            throw std::runtime_error(
                (boost::format("Number of rows in colors (%d) do not match the number number of elements (%d).") % colors.size() % elems.size()).str()
            );
        }

        return Job(nodes, elems, disp, node_strips, colors);
    }

    Job loadJobFromFilename(const std::string &config_filename) {
        rapidjson::Document config_doc = parseJSONConfig(config_filename);
        return createJobFromJSON(config_doc);
    }

} // namespace tresta
