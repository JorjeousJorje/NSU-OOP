#include <vector>
#include <random>
#include "optimized_copy.hpp"

int main()
{
    std::vector<double> v1(10, 4.3);
    std::vector<int> v2(10);
    //next line of code will cause WARNING, because we convert double to int, but
	//this copy function is designed with awareness of this WARNING
    optimized::copy(v1.begin(), v1.end(), v2.begin());
}