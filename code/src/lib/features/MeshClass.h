#ifndef __MESH_H__
#define __MESH_H__

#include "MacroClass.h"

#include <glm/vec3.hpp> // glm::vec3

#include <vector>
#include <fstream>

namespace SceneEditor {

	class Mesh {
	public:
		static std::pair<std::vector<glm::vec3>, std::vector<int>>
			read(const std::string& path) {
			std::ifstream infile(path, std::ifstream::binary);

			std::string line;
			std::getline(infile, line);

			int n_vertex, n_face, n_edge, tmp;
			infile >> n_vertex >> n_face >> n_edge;
			std::vector<glm::vec3> vertices(n_vertex);
			std::vector<int> indices(n_face * 3);
			for (int i = 0; i < n_vertex; ++i) {
				infile >> vertices[i][0] >> vertices[i][1] >> vertices[i][2];
			}
			for (int i = 0; i < n_face; ++i) {
				infile >> tmp;
				for (int j = 0; j < 3; ++j) {
					infile >> indices[3 * i + j];
				}
			}
			return { vertices, indices };
		}
	};
}

#endif // __MESH_H__
