#pragma once

#include "mesh.hpp"
#include <vector>
#include <glm/vec3.hpp>

typedef struct bvh {
	bool has_children;
	int left_index;
	int right_index;
	int triangle_count;
	int triangle_offset;
	alignas(16) glm::vec3 bound_start;
	alignas(16) glm::vec3 bound_end;
} t_bvh;

class BVHGenerator {
public:
	BVHGenerator(int depth);
	void generate(Mesh *mesh);
	void updateSSBO(SSBO *triangle_ssbo, SSBO *triangle_index_ssbo, SSBO *bvh_ssbo);
	std::vector<t_bvh> getNodes();

private:
	std::vector<t_bvh> nodes;
	std::vector<t_shader_triangle> triangles;
	std::vector<int> triangle_indices;
	int depth;

	void makeBounds(t_bvh *node);
};
