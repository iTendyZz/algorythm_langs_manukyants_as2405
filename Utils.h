#pragma once
#include <iostream>
using namespace std;
#define INPUT_LINE(in, str) getline(in>>std::ws, str); \
						std::cerr << str << std::endl

class redirect_output_wrapper
{
	std::ostream& stream;
	std::streambuf* const old_buf;

public:
	redirect_output_wrapper(std::ostream& src)
		: stream(src), old_buf(src.rdbuf()) {
	}

	~redirect_output_wrapper() {
		stream.rdbuf(old_buf);
	}

	void redirect(const std::ostream& dest) const {
		stream.rdbuf(dest.rdbuf());
	}
};

double GetCorrectDiameter();

template <typename T>
T GetCorrectNumber(T min, T max)
{
	T x;
	while ((std::cin >> x).fail()	// https://github.com/papilinatm/cpp_lessons_2020/blob/master/cpp_lessons/utils.h
		|| std::cin.peek() != '\n'	
		|| x < min || x > max)	
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		std::cout << "Type number (" << min << "-" << max << "):";
	}
	std::cerr << x << std::endl;
	return x;
}
