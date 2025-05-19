#pragma once

#include <string>
#include <sstream>
#include <vector>

struct Material {
    std::string name;
    std::string map_Kd;
    std::string map_Bump;
    float Ka[3] = {};
    float Kd[3] = {};
    float Ks[3] = {};
    float d = 1.0f;
    int illum = 0;
};

class MaterialLoader {
    private:
        std::vector<Material> materials;

    public:
        MaterialLoader();
        MaterialLoader(const std::vector<std::string>& filepaths);
        ~MaterialLoader();

        const std::vector<Material>& getMaterials() const;
};

std::ostream& operator<<(std::ostream& os, const MaterialLoader& obj);