#include "MathUtils.h"

const double MathUtils::PI = 3.14159265358979323846;

//1 -> horario | -1 -> anti-horario | 0 -> colinear
int MathUtils::GetOrientation(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
    float r1 = (p2.y - p1.y) * (p3.x - p2.x); // p0 e p1
    float r2 = (p3.y - p2.y) * (p2.x - p1.x); // p1 e p2

    //colinear
    if (r1 == r2) return 0;

    //sentido horario
    if (r1 > r2)
    {
        return 1;
    }

    //sentido anti-horario
    return -1;
}
bool MathUtils::HasInterception(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4)
{
    if (GetOrientation(p1, p2, p3) != GetOrientation(p1, p2, p4))
    {
        if (GetOrientation(p3, p4, p1) != GetOrientation(p3, p4, p2))
        {
            return true;
        }
    }
    return false;
}

glm::vec2 MathUtils::FindIntersection(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2) {
    double a1 = q1.y - p1.y;
    double b1 = p1.x - q1.x;
    double c1 = a1 * (p1.x) + b1 * (p1.y);

    double a2 = q2.y - p2.y;
    double b2 = p2.x - q2.x;
    double c2 = a2 * (p2.x) + b2 * (p2.y);

    double determinant = a1 * b2 - a2 * b1;

    if (determinant == 0) 
    {
        return glm::vec2(FLT_MAX, FLT_MAX);
    }
    else 
    {
        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        return glm::vec2(x, y);
    }
}

//Baseado no uso da função tangente hiperbólica
double MathUtils::AnimationSmoothTime(double value)
{
    return tanh(value);
}