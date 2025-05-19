#include <fstream>

#include "../include/MaterialLoader.hpp"

#include <iostream>

MaterialLoader::MaterialLoader() = default;

MaterialLoader::MaterialLoader(const std::vector<std::string>& filepaths) {
    for (const auto& filepath : filepaths) {
        std::ifstream file(filepath);
        std::string line;
        Material current;

        while (std::getline(file, line)) {
            std::stringstream iss(line);
            std::string keyword;
            iss >> keyword;

            if (keyword == "newmtl") {
                if (!current.name.empty()) {
                    materials[materials.size()] = current;
                }
                current = Material();  // Reset current
                iss >> current.name;
            } else if (keyword == "Ka") {
                iss >> current.Ka[0] >> current.Ka[1] >> current.Ka[2];
            } else if (keyword == "Kd") {
                iss >> current.Kd[0] >> current.Kd[1] >> current.Kd[2];
            } else if (keyword == "Ks") {
                iss >> current.Ks[0] >> current.Ks[1] >> current.Ks[2];
            } else if (keyword == "d") {
                iss >> current.d;
            } else if (keyword == "Tr") { // Alternative for transparency
                iss >> current.d;
                current.d = 1.0f - current.d;
            } else if (keyword == "illum") {
                iss >> current.illum;
            } else if (keyword == "map_Kd") {
                iss >> current.map_Kd;
            } else if (keyword == "map_Bump" || keyword == "bump") {
                iss >> current.map_Bump;
            }
        }

        if (!current.name.empty()) {
            materials.push_back(current);
        }
    }
}

MaterialLoader::~MaterialLoader() = default;

const std::vector<Material>& MaterialLoader::getMaterials() const {
    return materials;
}


std::ostream& operator<<(std::ostream& os, const MaterialLoader& materials) {
    os << "Materials: " << materials.getMaterials().size() << std::endl;
    for (auto& item : materials.getMaterials()) {
        os << "Name: " << item.name << std::endl;
        os << "Map Kd: " << item.map_Kd << std::endl;
        os << "Map Bump: " << item.map_Bump << std::endl;
        os << "Ka: " << item.Ka[0] << " " << item.Ka[1] << " " << item.Ka[2] << std::endl;
        os << "Kd: " << item.Kd[0] << " " << item.Kd[1] << " " << item.Kd[2] << std::endl;
        os << "Ks: " << item.Ks[0] << " " << item.Ks[1] << " " << item.Ks[2] << std::endl;
        os << "D: " << item.d << std::endl;
        os << "Illum: " << item.illum << std::endl << std::endl;
    }

    return os;
}
