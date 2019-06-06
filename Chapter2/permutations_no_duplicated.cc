#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

#define SWAP(arr, a, b) do { \
    if ((a) != (b)) { \
        arr[a] ^= arr[b]; arr[b] ^= arr[a]; arr[a] ^= arr[b]; \
    } \
} while (0)

bool need_swap(char*str, int k, int i)
{
    for (int j = k; j < i; j++) {
        if (str[j] == str[i]) {
            return false;
        }
    }

    return true;
}

void perm(char* str, int k, int m, vector<string>& res)
{
    // Found the candidate.
    if (k == m) {
        res.push_back(str);
        return;
    }

    // Exchange str[k] with str[k .. m].
    for (int i = k; i <= m; i++) {
        // Current check value is str[i], if str[k .. i - 1] has char equals to str[i],
        // then str[i] cannot be swapped.
        if (need_swap(str, k, i)) {
            SWAP(str, k, i);
            perm(str, k + 1, m, res);
            SWAP(str, k, i);
        }
    }
}

int main(int argc, char const* argv[])
{
    char str[] = "122";
    vector<string> res;

    perm(str, 0, strlen(str) - 1, res);

    for (vector<string>::iterator it = res.begin(); it != res.end(); it++) {
        cout << *it << endl;
    }

    return 0;
}
