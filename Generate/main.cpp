#include <random>
#include <fstream>
#include <cstring>

int main(int argc, char * argv[]) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(97, 122);
	std::uniform_int_distribution<> count_dis(10, 50);

	int size = std::atoi(argv[2]) * 1024 * 1024;

	std::ofstream out(argv[1]);

	while (size > 0) {
		size_t count = count_dis(gen) + 2;
		count = (size - count > 0) ? count : size;
		for (size_t i = 2; i != count; ++i) {
			out << static_cast<char>(dis(gen));
		}
		out << '\n';
		size -= count;
	}
	return 0;
}