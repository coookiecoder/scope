#include "../include/Obj.hpp"

#include <algorithm>

Face::Face() {

}

Face::~Face() {

}

const std::vector<int>& Face::getVerticesIndex() const {
    return vertices_index;
}

const std::vector<int>& Face::getTexturesIndex() const {
    return textures_index;
}

const std::vector<int>& Face::getNormalsIndex() const {
    return normals_index;
}

int Face::getVerticesIndex(const int index) const {
    return vertices_index[index];
}

int Face::getTextureIndex(const int index) const {
    return textures_index[index];
}

int Face::getNormalIndex(const int index) const {
    return normals_index[index];
}

void Face::addVerticesIndex(const int vertices) {
    vertices_index.push_back(vertices);
}

void Face::addTexturesIndex(const int textures) {
    textures_index.push_back(textures);
}

void Face::addNormalsIndex(const int normals) {
    normals_index.push_back(normals);
}

std::ostream& operator<<(std::ostream& os, const Face& face) {
    auto &vertices = face.getVerticesIndex();
    auto &textures = face.getTexturesIndex();
    auto &normals = face.getNormalsIndex();

    for (int index = 0; index < vertices.size(); ++index) {
        os << vertices[index] << "/" << textures[index] << "/" << normals[index];
        if (index != vertices.size() - 1)
            os << " ";
    }

    return os;
}


void Obj::parseVertex(const std::string &line) {
    float x, y, z;
    std::istringstream iss(line);
    std::string type; // for the "v"
    iss >> type >> x >> y >> z;
    vertices.emplace_back(x, y, z);
}

void Obj::parseTexCoord(const std::string &line) {
    float x, y;
    std::istringstream iss(line);
    std::string type; // for the "vt"
    iss >> type >> x >> y;
    texture_coordinates.emplace_back(x, y);
}

void Obj::parseNormal(const std::string &line) {
    float x, y, z;
    std::istringstream iss(line);
    std::string type; // for the "vn"
    iss >> type >> x >> y >> z;
    normals.emplace_back(x, y, z);
}

void Obj::parseFace(const std::string &line) {
    Face face;
    std::istringstream iss(line);
    std::string type; // for the "f"
    iss >> type;

    std::string token;
    while (iss >> token) {
        std::ranges::replace(token, '/', ' ');
        std::istringstream tokenStream(token);
        int vertexIdx, texCoordIdx, normalIdx = 0;

        tokenStream >> vertexIdx >> texCoordIdx >> normalIdx;
        face.addVerticesIndex(vertexIdx);
        face.addTexturesIndex(texCoordIdx);
        face.addNormalsIndex(normalIdx);
    }
    faces.push_back(face);
}


Obj::Obj(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::invalid_argument("File could not be opened");
    }

    std::string line;

    while (getline(file, line)) {
        std::stringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            this->parseVertex(line);
        } else if (type == "vt") {
            this->parseTexCoord(line);
        } else if (type == "vn") {
            this->parseNormal(line);
        } else if (type == "f") {
            this->parseFace(line);
        }
    }
}

Obj::~Obj() {

}

const std::vector<Vector3D<float>>& Obj::getVertices() const {
    return vertices;
}

const std::vector<Vector2D<float>>& Obj::getTextureCoordinates() const {
    return texture_coordinates;
}

const std::vector<Vector3D<float>>& Obj::getNormals() const {
    return normals;
}

const std::vector<Face>& Obj::getFaces() const {
    return faces;
}

std::ostream& operator<<(std::ostream& os, const Obj& obj) {
    auto &vertices = obj.getVertices();
    auto &texture_coordinates = obj.getTextureCoordinates();
    auto &normals = obj.getNormals();
    auto &faces = obj.getFaces();

    os << "Vertices: " << vertices.size() << std::endl;
    for (auto item : vertices)
        os << item << std::endl;
    os << std::endl << "Texture coordinates: " << texture_coordinates.size() << std::endl;
    for (auto item : texture_coordinates)
        os << item << std::endl;
    os << std::endl << "Normals: " << normals.size() << std::endl;
    for (auto item : normals)
        os << item << std::endl;
    os << std::endl << "Faces: " << faces.size() << std::endl;
    for (const auto& item : faces)
        os << item << std::endl;

    return os;
}

glm::vec3 cookie::subtract(const glm::vec3& a, const glm::vec3& b) {
    return glm::vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

glm::vec3 cookie::add(const glm::vec3& a, const glm::vec3& b) {
    return glm::vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

glm::vec3 cookie::normalize(glm::vec3 vector) {
    float len = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
    if (len == 0.0f)
        return glm::vec3(0.0f);
    return glm::vec3(vector.x / len, vector.y / len, vector.z / len);
}

glm::vec3 cookie::cross(glm::vec3 a, glm::vec3 b) {
    return glm::vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

glm::mat4 cookie::rotate(glm::mat4 m, float angle, glm::vec3 axis) {
    axis = cookie::normalize(axis);
    float c = cos(angle);
    float s = sin(angle);
    float t = 1.0f - c;

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    glm::mat4 rot(1.0f);
    rot[0][0] = t*x*x + c;
    rot[0][1] = t*x*y + s*z;
    rot[0][2] = t*x*z - s*y;

    rot[1][0] = t*x*y - s*z;
    rot[1][1] = t*y*y + c;
    rot[1][2] = t*y*z + s*x;

    rot[2][0] = t*x*z + s*y;
    rot[2][1] = t*y*z - s*x;
    rot[2][2] = t*z*z + c;

    return m * rot;
}

glm::mat4 cookie::lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
    glm::vec3 f = cookie::normalize(cookie::subtract(center, eye));
    glm::vec3 s = cookie::normalize(cookie::cross(f, up));
    glm::vec3 u = cookie::cross(s, f);

    glm::mat4 result(1.0f);
    result[0][0] = s.x;
    result[1][0] = s.y;
    result[2][0] = s.z;

    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;

    result[0][2] = -f.x;
    result[1][2] = -f.y;
    result[2][2] = -f.z;

    result[3][0] = -glm::dot(s, eye);
    result[3][1] = -glm::dot(u, eye);
    result[3][2] = glm::dot(f, eye);

    return result;
}

glm::mat4 cookie::perspective(float fov, float aspect, float near, float far) {
    float tanHalfFov = tan(fov / 2.0f);
    glm::mat4 result(0.0f);

    result[0][0] = 1.0f / (aspect * tanHalfFov);
    result[1][1] = 1.0f / tanHalfFov;
    result[2][2] = -(far + near) / (far - near);
    result[2][3] = -1.0f;
    result[3][2] = -(2.0f * far * near) / (far - near);

    return result;
}