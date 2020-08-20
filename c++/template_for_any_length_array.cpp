/* Template for any length array.
 *
 * Compile with: g++ template_for_any_length_array.cpp -W -Wall
 */

#include <iostream>

using namespace std;

template <int _len_>
void foo(int (&arr)[_len_]) {
    for (int i = 0; i < _len_; ++i) {
        cout << arr[i] << endl;
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7};

    foo(arr);

    return EXIT_SUCCESS;
}
