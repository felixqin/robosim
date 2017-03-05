#include <cassert>
#include "config.h"


static const Real robot_vertices[] = {
	0, 0, 0.15,
	0.0779423, 0.045, 0.15,
	0.0561141, 0.0703648, 0.15,
	0.026528, 0.0860016, 0.15,
	-0.00672571, 0.0897483, 0.15,
	-0.0390495, 0.0810872, 0.15,
	-0.0659747, 0.0612156, 0.15,
	-0.0837786, 0.0328807, 0.15,
	-0.09, 1.35896e-008, 0.15,
	-0.0837786, -0.0328807, 0.15,
	-0.0659747, -0.0612155, 0.15,
	-0.0390496, -0.0810872, 0.15,
	-0.00672572, -0.0897483, 0.15,
	0.026528, -0.0860016, 0.15,
	0.0561141, -0.0703648, 0.15,
	0.0779423, -0.045, 0.15,
	0.0779423, 0.045, 0.035,
	0.0561141, 0.0703648, 0.065,
	0.026528, 0.0860016, 0.065,
	-0.00672571, 0.0897483, 0.035,
	-0.0390495, 0.0810872, 0.035,
	-0.0659747, 0.0612156, 0.065,
	-0.0837786, 0.0328807, 0.065,
	-0.09, 1.35896e-008, 0.035,
	-0.0837786, -0.0328807, 0.065,
	-0.0659747, -0.0612155, 0.065,
	-0.0390496, -0.0810872, 0.035,
	-0.00672572, -0.0897483, 0.035,
	0.026528, -0.0860016, 0.065,
	0.0561141, -0.0703648, 0.065,
	0.0779423, -0.045, 0.035,
	0.0779423, -0.045, 0.06,
	0.0779423, 0.045, 0.06,
	-0.00672571, 0.0897483, 0.01,
	-0.0390495, 0.0810872, 0.01,
	-0.09, 1.35896e-008, 0.01,
	-0.09, 1.35896e-008, 0.035,
	-0.0390496, -0.0810872, 0.01,
	-0.00672572, -0.0897483, 0.01
};

static const unsigned int robot_indices[] = {
	0,1,2,
	0,2,3,
	0,3,4,
	0,4,5,
	0,5,6,
	0,6,7,
	0,7,8,
	0,8,9,
	0,9,10,
	0,10,11,
	0,11,12,
	0,12,13,
	0,13,14,
	0,14,15,
	0,15,1,
	1,16,2,
	2,16,17,
	2,17,3,
	3,17,18,
	3,18,4,
	4,18,19,
	4,19,5,
	5,19,20,
	5,20,6,
	6,20,21,
	6,21,7,
	7,21,22,
	7,22,8,
	8,22,23,
	8,23,9,
	9,23,24,
	9,24,10,
	10,24,25,
	10,25,11,
	11,25,26,
	11,26,12,
	12,26,27,
	12,27,13,
	13,27,28,
	13,28,14,
	14,28,29,
	14,29,15,
	15,29,30,
	15,31,32,
	15,32,1,
	19,33,20,
	19,34,20,
	23,35,24,
	22,36,23,
	26,37,27,
	26,38,27
};

static const Real robot_normals[] = {
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0.757972, 0.652287, 0,
	0.757972, 0.652287, 0,
	0.467268, 0.884115, 0,
	0.467268, 0.884115, 0,
	0.111965, 0.993712, 0,
	0.111965, 0.993712, 0,
	-0.258819, 0.965926, 0,
	-0.258819, 0.965926, 0,
	-0.59382, 0.804598, 0,
	-0.59382, 0.804598, 0,
	-0.846724, 0.532032, 0,
	-0.846724, 0.532032, 0,
	-0.982566, 0.185912, 0,
	-0.982566, 0.185912, 0,
	-0.982567, -0.185911, -0,
	-0.982567, -0.185911, 0,
	-0.846724, -0.532032, -0,
	-0.846724, -0.532032, 0,
	-0.59382, -0.804598, -0,
	-0.59382, -0.804598, 0,
	-0.258819, -0.965926, -0,
	-0.258819, -0.965926, 0,
	0.111964, -0.993712, 0,
	0.111964, -0.993712, 0,
	0.467268, -0.884115, 0,
	0.467268, -0.884115, 0,
	0.757972, -0.652288, 0,
	0.757972, -0.652288, 0,
	1, -8.27842e-008, 0,
	1, -8.27842e-008, 0,
	-0.258819, 0.965926, 0,
	-0.258819, 0.965926, 0,
	-0.982567, -0.185911, -0,
	0, 0, 0,
	-0.258819, -0.965926, -0,
	-0.258819, -0.965926, 0
};

void load_default_trimesh(std::vector<Real>& vertices,std::vector<unsigned int>& indices,std::vector<Real>& normals)
{
        size_t vertices_count = sizeof(robot_vertices)/sizeof(Real);
        vertices.clear();
        std::copy(robot_vertices,robot_vertices+vertices_count,
                std::back_inserter(vertices));

        size_t indices_count = sizeof(robot_indices)/sizeof(unsigned int);
        indices.clear();
        std::copy(robot_indices,robot_indices+indices_count,
                std::back_inserter(indices));

        size_t normals_count = sizeof(robot_normals)/sizeof(Real);
        assert(indices_count==normals_count);
        normals.clear();
        std::copy(robot_normals,robot_normals+normals_count,
                std::back_inserter(normals));
}
