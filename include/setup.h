#ifndef TRESTA_SETUP_H
#define TRESTA_SETUP_H

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include "containers.h"

namespace tresta {

    rapidjson::Document parseJSONConfig(const std::string &config_filename);

    /**
     * Parses the file indicated by the "nodes" key in `config_doc` into a vector of `fea::Node`'s.
     *
     * @param config_doc `rapidjson::Document`. Document storing the file name containing the nodal coordinates.
     * @return Nodal coordinates. `std::vector<Node>`.
     */
    std::vector<Node> createNodeVecFromJSON(const rapidjson::Document &config_doc);

    /**
     * Parses the files indicated by the "elems" and "props" keys in `config_doc` into a vector of `fea::Elem`'s.
     *
     * @param config_doc `rapidjson::Document`. Document storing the file names of the csv files that contain
     *                    the node number designations for each element and elemental properties.
     * @return Elements. `std::vector<Elem>`.
     */
    std::vector<Elem> createElemVecFromJSON(const rapidjson::Document &config_doc);

    std::vector<Displacement> createDisplacementVecFromJSON(const rapidjson::Document &config_doc);

    // std::vector<Node> createSingleNodeStrip(const Node &node1, const Node &node2, const Node &normal, )

    std::vector<std::vector<Node>> createNodeStrips(const std::vector<Node> &nodes,
                                                    const std::vector<Elem> &elems,
                                                    const std::vector<Displacement> &displacements,
                                                    const float scale);

    Job createJobFromJSON(const rapidjson::Document &config_doc);

    Job loadJobFromFilename(const std::string &config_filename);

} // namespace tresta

#endif // TRESTA_SETUP_H
