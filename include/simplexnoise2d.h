#ifndef SIMPLEXNOISE2D_H
#define SIMPLEXNOISE2D_H

#include <array>
#include <random>

#include <glm/glm.hpp>

#define DIRECTIONS_COUNT 1001

class SimplexNoise2D
{
public:
    template <typename T> requires std::uniform_random_bit_generator<T> SimplexNoise2D(T &gen);
    SimplexNoise2D(std::mt19937_64 &gen);
	double sample(double x, double y);
private:
	std::array<glm::vec2, DIRECTIONS_COUNT> directions;
	static constexpr glm::mat2 skew_matrix = glm::mat2(1.3660254037844386, 0.3660254037844386,  // 1 + (sqrt(3) - 1) / 2, (sqrt(3) - 1)/2
													   0.3660254037844386, 1.3660254037844386); // Skew (x, y) into (x + (x + y) * F, y + (x + y) * F), where F = (sqrt(3) - 1)/2
	static constexpr glm::mat2 unskew_matrix = glm::mat2( 0.7886751345948129, -0.21132486540518713, // 1 - (3 - sqrt(3)) / 6, - (3 - sqrt(3)) / 6
														 -0.21132486540518713, 0.7886751345948129); // Unskew (x, y) into (x - (x + y) * G, y - (x + y) * G), where G = (3 - sqrt(3)) / 6
	static glm::vec2 skew(glm::vec2 vector);
	static glm::vec2 unskew(glm::vec2 vector);
	static unsigned int shuffle(int x);
};

#endif // SIMPLEXNOISE2D_H
