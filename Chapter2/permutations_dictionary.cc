#include <iostream>
#include <vector>

using namespace std;

int comp_char(const void* arg1, const void* arg2)
{
    return *((char*)arg1) - *((char*)arg2);
}

int main(int argc, char const* argv[])
{
    if (argc != 2) {
        cerr << "usage: " << argv[0] << " <str>" << endl;
        return 1;
    }

    char* str = strdup(argv[1]);
    int len = strlen(str);
    // Sort str.
    qsort(str, len, sizeof(char), comp_char);
    cout << "Sorted string is: " << str << endl;

    vector<string> res;
    for ( ; ; ) {
        // Save one candidate.
        res.push_back(str);

        int i = len - 2;
        // Found A[i] < A[i+1] pair.
        while (i >= 0) {
            if (str[i] < str[i+1]) {
                break;
            }
            --i;
        }
        if (i < 0) {
            break;
        }

        // Found the smallest value in A[i+1 .. n-1] bigger than A[i].
#if 0
        // Not quick enough.
        int k = i + 1;
        for (int j = i + 2; j < len; j++) {
            if (str[j] > str[i] && str[j] < str[k]) {
                k = j;
            }
        }
#else
        // 仔细思考一下，大于str[i]的最小元素一定从字符串的尾部出现的第1个大于str[i]的元素。
        int k = len - 1;
        while (k >= i + 1 && str[k] <= str[i]) {
            k--;
        }
#endif

        // Swap str[i] with str[k].
        char t = str[i];
        str[i] = str[k];
        str[k] = t;

        // Reverse str[i+1 .. n-1]
        for (int j = i + 1, k = len - 1; j < k; j++, k--) {
            int t = str[j];
            str[j] = str[k];
            str[k] = t;
        }
    }

    cout << "size is : " << res.size() << endl;
    for (vector<string>::iterator it = res.begin(); it != res.end(); it++) {
        cout << *it << endl;
    }

    free(str);

    return 0;
}
