#include "calculations.h"
#include <cmath>
#include <limits>
#include <stdexcept>

double calculate(double a, double b, char operation) {
    double result = 0;
    switch (operation) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/':
            if (b == 0) throw std::runtime_error("На ноль делить нельзя.");
            result = a / b;
            break;
        case 'r': {
            if (b < 1 || b != std::floor(b) || b > std::numeric_limits<int>::max())
                throw std::runtime_error("Степень корня должна быть целым числом от 1 до 2147483647.");
            int n = static_cast<int>(b);
            if (a < 0 && n % 2 == 0)
                throw std::runtime_error("Чётный корень из отрицательного числа не определён.");
            result = std::pow(std::fabs(a), 1.0 / n);
            if (a < 0) result = -result;
            break;
        }
        default: throw std::runtime_error("Неизвестная операция.");
    }
    if (!std::isfinite(result)) throw std::runtime_error("Слишком большое число.");
    return result;
}

double tangent(double degrees) {
    double angle = std::remainder(degrees, 180.0);
    if (std::fabs(std::fabs(angle) - 90.0) < 1e-10)
        throw std::runtime_error("Для этого угла тангенс не определён.");
    double pi = std::acos(-1.0);
    return std::tan(angle * pi / 180.0);
}

double decibels(double x) {
    if (x <= 0) throw std::runtime_error("Для вычисления дБ нужно положительное число.");
    return 20.0 * std::log10(x);
}
