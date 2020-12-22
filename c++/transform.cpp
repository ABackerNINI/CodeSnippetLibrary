/* Simple example of using std::transform function. */

#include <string>
#include <iostream>
#include <algorithm>

int main() {
    std::string str = "abc";
    /* Convert all lowercase letters to uppercase */
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::toupper(c); });

    std::cout << str << std::endl;

    return 0;
}
