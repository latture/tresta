#ifndef TRESTA_CONTAINERS_H
#define TRESTA_CONTAINERS_H

#include <Eigen/Core>
#include <QColor>
#include <vector>

namespace tresta {

    /**
     * @brief A node that describes a mesh. Uses Eigen's predefined Vector class for added functionality.
     * @details See the Eigen documentation on the Vector3d class for more options of what can be done with `Nodes`. \n
     * Examples of constucting a `Node` at \f$(x, y, z)=(0,1,2)\f$:
     * @code
     * // specify values on constuction
     * fea::Node n1(1.0, 2.0, 3.0);
     *
     * // construct a Node then insert values
     * fea::Node n2;
     * n2 << 0.0, 1.0, 2.0;
     * @endcode
     */
    typedef Eigen::Vector3f Node;

    /**
     * @brief Nodal displacement.
     * @details Displacements are given in the form `[dx, dy, dz, rx, ry, rz]`,
     * where `d_` specifies translational displacement along the listed axis, and `r_` specifies rotational displacement
     * along the listed axis.
     */
    typedef Eigen::Matrix<float, 6, 1> Displacement;

    /**
     * @brief The set of properties associated with an element.
     * @details The properties must define a vector pointing along the beam elements local y-axis.
     *
     * @code
     * float nx = 1000.0;
     * float ny = 100.0;
     * float nz = 100.0;
     * fea::Props props(nx, ny, nz);
     * @endcode
     */
    struct Props {
        Eigen::Vector3f normal_vec;/**<Vector normal to element (size==3). Direction should be parallel to the beam element's local y-axis.*/

        Props() { };/**<Default constuctor*/

        /**
         * @brief Constructor
         * @details Allows properties to be set upon initialization.
         *
         * @param[in] nx float. x-component of vector parallel to the beam's local y-axis.
         * @param[in] ny float. y-component of vector parallel to the beam's local y-axis.
         * @param[in] nz float. z-component of vector parallel to the beam's local y-axis.
         */
        Props(float nx, float ny, float nz) {
            normal_vec << nx, ny, nz;
            normal_vec.normalize();
        };
    };

    /**
     * @brief An element of the mesh. Contains the indices of the two `fea::Node`'s that form the element as well
     * as the properties of the element given by the `fea::Props` struct.
     */
    struct Elem {
        Eigen::Vector2i node_numbers;/**<The indices of the node list that define the element.*/
        Props props;/**<The set of properties to associate with the element.*/

        /**
        * @brief Default Constructor
        */
        Elem() { };

        /**
         * @brief Constructor
         * @details Constructor if the node indices are passed independently. Assumes 2 nodes per element.
         *
         * @param[in] node1 unsigned int. The indices of first node associate with the element.
         * @param[in] node2 unsigned int. The indices of second node associate with the element.
         * @param[in] props Props. The element's properties.
         */
        Elem(unsigned int node1, unsigned int node2, const Props &_props) : props(_props) {
            node_numbers << node1, node2;
        }
    };

    /**
     * @brief Contains all the required information to render a mesh.
     * @details A job holds information on the node and element lists as well as any nodal displacements, deformed
     * element shapes, and elemental colors.
     */
    struct Job {

        Job() {};
        Job(const std::vector<Node> &nodes,
            const std::vector<Elem> &elems,
            const std::vector<Displacement> &displacements,
            const std::vector<std::vector<Node>> &node_strips,
            const std::vector<QColor> &colors) :
                nodes(nodes),
                elems(elems),
                displacements(displacements),
                node_strips(node_strips),
                colors(colors) {
            if (colors.size() > 0) {
                assert(elems.size() == colors.size() && "Elements and colors are not the same length.");
            }
            if (displacements.size() > 0) {
                assert(displacements.size() == nodes.size() && "Nodes and displacements are not the same length.");
            }
        }

        std::vector<Node> nodes;
        /**<List of nodal coordinates specifying \f$(x, y, z)\f$ positions.*/
        std::vector<Elem> elems;
        /**<List of elements specifying which nodal indices are connected with an element
                                            as well as the associated elemental properties.*/
        std::vector<Displacement> displacements;
        /**<List of nodal displacements to apply to the nodes*/
        std::vector<std::vector<Node>> node_strips;
        /**<Interpolated deformed nodal coordinates.*/
        std::vector<QColor> colors;/**<Color to render each element.*/
    };

    enum DOF {
        /**
         * Displacement along the global x-axis.
         */
        DISPLACEMENT_X,

        /**
         * Displacement along the global y-axis.
         */
        DISPLACEMENT_Y,

        /**
         * Displacement along the global z-axis.
         */
        DISPLACEMENT_Z,

        /**
         * Rotation about the global x-axis.
         */
        ROTATION_X,

        /**
         * Rotation about the global y-axis.
         */
        ROTATION_Y,

        /**
         * Rotation about the global z-axis.
         */
        ROTATION_Z,
        /**
         * Number of degrees of freedom per node.
         */
        NUM_DOFS
    };
} // namespace tresta

#endif // TRESTA_CONTAINERS_H
