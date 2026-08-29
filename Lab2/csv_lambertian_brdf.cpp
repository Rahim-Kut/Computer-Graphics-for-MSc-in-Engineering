#include <cmath>
#include <fstream>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

const float PI = 3.14159265359; 

double lambertian_brdf(glm::vec3 in_direction, glm::vec3 out_direction, glm::vec3 normal){
    double albedo = 1.0;
    return albedo / PI;
}

int main(){
    std::ofstream csv("lambertian_brdf.csv");

    csv << "Angle (degrees),BRDF\n";

    glm::vec3 normal(0.0f, 0.0f, 1.0f);
    glm::vec3 out_direction(0.0f, 0.0f, 1.0f);

    for (int angle = -90; angle <= 90; angle += 45){
        double radians = angle * PI / 180.0;

        glm::vec3 in_direction(
            std::sin(radians),
            0.0f,
            std::cos(radians)
        );

        double value = lambertian_brdf(glm::normalize(in_direction), glm::normalize(out_direction), glm::normalize(normal));

        csv << angle << "," << value << "\n";
    }

    std::cout << "Wrote lambertian_brdf.csv\n";
    return 0;
}
