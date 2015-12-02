#ifndef TRESTA_SETUP_H
#define TRESTA_SETUP_H

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include "containers.h"

namespace tresta {

    /**
     * @brief Constructs a JSON document from the specified file.
     * @details The specified file is opened, and the contents are used to create a JSON document. The contents should
     * be valid JSON.
     *
     * @param config_filename `std::string`. Name of the JSON file to load.
     * @return document `rapidjson::Document`. JSON document.
     */
    rapidjson::Document parseJSONConfig(const std::string &config_filename);

    /**
     * Parses the file indicated by the "nodes" key in `config_doc` into a vector of `tresta::Node`'s.
     *
     * @param config_doc `rapidjson::Document`. Document storing the file name containing the nodal coordinates.
     * @return nodal_coordinates. `std::vector<tresta::Node>`. \f$(x,y,z)\f$ position of each node.
     */
    std::vector<Node> createNodeVecFromJSON(const rapidjson::Document &config_doc);

    /**
     * Parses the files indicated by the "elems" and "props" keys in `config_doc` into a vector of `tresta::Elem`'s.
     *
     * @param config_doc `rapidjson::Document`. Document storing the file names of the csv files that contain
     *                    the node number designations for each element and elemental properties.
     * @return elements. `std::vector<tresta::Elem>`.
     */
    std::vector<Elem> createElemVecFromJSON(const rapidjson::Document &config_doc);

    /**
     * Parses the file indicated by the "displacements" key in `config_doc` into a vector of `tresta::Displacement`'s.
     *
     * @param config_doc `rapidjson::Document`. Document storing the file name of the csv file that contains
     *                    the nodal displacements.
     * @return nodal_displacements. `std::vector<tresta::Displacement>`.
     */
    std::vector<Displacement> createDisplacementVecFromJSON(const rapidjson::Document &config_doc);

    /**
     * Parses the file indicated by the "colors" key in `config_doc` into a vector of `tresta::Color`'s.
     *
     * @param config_doc `rapidjson::Document`. Document storing the file name of the csv file that contains
     *                    the elemental colors.
     * @return colors. `std::vector<tresta::Color>`.
     */
    std::vector<Color> createColorVecFromJSON(const rapidjson::Document &config_doc);

    /**
     * Constructs the deformed elemental positions based on interpolation of nodal displacements.
     *
     * @param nodes `std::vector<tresta::Node>`. Node list: contains the \f$(x, y, z)\f$ position of each node.
     * @param elems `std::vector<tresta::Elem>`. Element list: contains the nodal indices that are connected to form
     *                                           each element as well as the associated elemental properties.
     * @param displacements `std::vector<tresta::Displacement>`. Nodal displacements.
     * @param scale `float`. Multiplier for nodal displacements. All displacements will be scaled by the given value.
     *
     * @return `std::vector<std::vector<tresta::Node>>` Interpolated nodal positions.
     */
    std::vector<std::vector<Node>> createNodeStrips(const std::vector<Node> &nodes,
                                                    const std::vector<Elem> &elems,
                                                    const std::vector<Displacement> &displacements,
                                                    const float scale);

    Job createJobFromJSON(const rapidjson::Document &config_doc);

    Job loadJobFromFilename(const std::string &config_filename);

} // namespace tresta

#endif // TRESTA_SETUP_H
