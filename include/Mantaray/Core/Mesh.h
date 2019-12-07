#include <vector>

#include "Mantaray/Core/Vector.h"

namespace MR {

class Mesh {
    public:
        Mesh();
        ~Mesh();
        void addVertice(Vector2f v);
        void addVertices(Vector2f v[], unsigned int vc);
        void addVertices(std::vector<Vector2f> v);
        void addIndex(int i);
        void clear();
        void uploadMeshData();
        void draw();

    private:
        unsigned int m_VAO, m_VBO;
        std::vector<Vector2f> m_Vertices;

        bool m_UsesIncices = false;
        std::vector<int> m_Indices;
};
}
