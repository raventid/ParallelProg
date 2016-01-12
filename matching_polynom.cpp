#include <iterator>
#include <regex>
#include <algorithm>
#include <string>
#include <iostream>

int main() { 

    std::string poly = "4x^2+3x^1+2x";
    std::regex term("[-+]?(\\d*)x(\\^[0-9])*");

    std::copy(
        std::sregex_token_iterator(poly.begin(), poly.end(), term),
        std::sregex_token_iterator(), 
        std::ostream_iterator<std::string>(std::cout, "\n")
        );

    std::map<int, int> components;
    components.insert(power, coeff);
    return 0;
}
