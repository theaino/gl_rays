#include "bvh.hpp"

#include "mesh.hpp"
#include <vector>
#include <glm/vec3.hpp>

BVHGenerator::BVHGenerator(int depth) {
	this->depth = depth;
}

void BVHGenerator::generate(Mesh *mesh) {
	triangles.clear();
	triangle_indices.clear();
	nodes.clear();
	t_shader_triangle *shader_triangles = mesh->getShaderTriangles();
	for (int i = 0; i < mesh->triangleCount(); ++i) {
		triangles.push_back(shader_triangles[i]);
	}

	t_bvh root_node;

	root_node.triangle_count = triangles.size();
	root_node.triangle_offset = triangle_indices.size();

	for (int triangle_index = 0; triangle_index < triangles.size(); ++triangle_index) {
		triangle_indices.push_back(triangle_index);
	}

	makeBounds(&root_node);
	nodes.push_back(root_node);

	std::vector<int> current_nodes;
	current_nodes.push_back(0);

	for (int i = 0; i < depth; ++i) {
		std::vector<int> new_nodes;
		for (int node_index : current_nodes) {
			t_bvh *node = &nodes[node_index];
			if (node->triangle_count < 2) {
				continue;
			}
			glm::vec3 bound_size = node->bound_end - node->bound_start;
			int split_side = 0;
			for (int j = 1; j < 3; ++j) {
				if (bound_size[j] > bound_size[split_side]) {
					split_side = j;
				}
			}
			std::vector<int> left_triangles;
			std::vector<int> right_triangles;
			float split_middle = node->bound_start[split_side] + bound_size[split_side] / 2;
			for (int j = 0; j < node->triangle_count; ++j) {
				int triangle_index = triangle_indices[node->triangle_offset + j];
				t_shader_triangle triangle = triangles[triangle_index];
				bool include_left = false;
				bool include_right = false;
				for (glm::vec3 vertex : {triangle.v0, triangle.v1, triangle.v2}) {
					if (vertex[split_side] < split_middle) {
						include_left = true;
					} else {
						include_right = true;
					}
				}
				if (include_left) {
					left_triangles.push_back(triangle_index);
				}
				if (include_right) {
					right_triangles.push_back(triangle_index);
				}
			}

			t_bvh *left_node = new t_bvh();
			t_bvh *right_node = new t_bvh();

			left_node->triangle_offset = triangle_indices.size();
			for (int triangle_index : left_triangles) {
				triangle_indices.push_back(triangle_index);
			}
			left_node->triangle_count = left_triangles.size();

			right_node->triangle_offset = triangle_indices.size();
			for (int triangle_index : right_triangles) {
				triangle_indices.push_back(triangle_index);
			}
			right_node->triangle_count = right_triangles.size();

			makeBounds(left_node);
			makeBounds(right_node);

			node->left_index = nodes.size();
			new_nodes.push_back(nodes.size());
			nodes.push_back(*left_node);
			node->right_index = nodes.size();
			new_nodes.push_back(nodes.size());
			nodes.push_back(*right_node);

			node->has_children = true;
		}

		current_nodes = new_nodes;
	}
}

void BVHGenerator::updateSSBO(SSBO *triangle_ssbo, SSBO *triangle_index_ssbo, SSBO *bvh_ssbo) {
  triangle_ssbo->setData(triangles.data(), sizeof(t_shader_triangle) * triangles.size());
  triangle_index_ssbo->setData(triangle_indices.data(), sizeof(int) * triangle_indices.size());
	bvh_ssbo->setData(nodes.data(), sizeof(t_bvh) * nodes.size());
}

std::vector<t_bvh> BVHGenerator::getNodes() {
	return nodes;
}

void BVHGenerator::makeBounds(t_bvh *node) {
	for (int i = 0; i < node->triangle_count; ++i) {
		int triangle_index = triangle_indices[node->triangle_offset + i];
		t_shader_triangle triangle = triangles[triangle_index];
		for (glm::vec3 vertex : {triangle.v0, triangle.v1, triangle.v2}) {
			if (i == 0) {
				node->bound_start = vertex;
				node->bound_end = vertex;
			}
			for (int j = 0; j < 3; ++j) {
				if (node->bound_start[j] > vertex[j]) {
					node->bound_start[j] = vertex[j];
				}
				if (node->bound_end[j] < vertex[j]) {
					node->bound_end[j] = vertex[j];
				}
			}
		}
	}
}
