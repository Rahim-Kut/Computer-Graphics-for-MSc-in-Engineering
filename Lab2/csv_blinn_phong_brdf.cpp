#include <cmath>
#include <fstream>
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

const double PI = 3.14159265359;

double blinn_phong_brdf(glm::vec3 in_direction, glm::vec3 out_direction, glm::vec3 normal){
    double kL = 0.9;
    double kg = 0.1;
    double s = 100.0;

    double L = 1.0;
    double g = 1.0;

    glm::vec3 half_vector = glm::normalize(in_direction + out_direction);

    double diffuse = kL * L / PI;

    double n_dot_h = glm::dot(normal, half_vector);
    n_dot_h = std::max(n_dot_h, 0.0);

    double glossy = kg * g * ((s + 8.0) / (8.0 * PI)) * std::pow(n_dot_h, s);

    return diffuse + glossy;
}

int main()
{
    std::ofstream csv("blinn_phong_brdf.csv");

    csv << "angle_degrees,brdf\n";

    glm::vec3 normal(0.0f, 0.0f, 1.0f);
    glm::vec3 out_direction(0.0f, 0.0f, 1.0f);

    for (int angle = -90; angle <= 90; angle += 1)
    {
        double radians = angle * PI / 180.0;

        glm::vec3 in_direction(
            std::sin(radians),
            0.0f,
            std::cos(radians)
        );

        double value = blinn_phong_brdf(
            glm::normalize(in_direction),
            glm::normalize(out_direction),
            glm::normalize(normal)
        );

        csv << angle << "," << value << "\n";
    }

    std::cout << "Wrote blinn_phong_brdf.csv\n";

    return 0;
}
