#include <iostream>
#include <vector>

using namespace std;

void foo(const string& str, int cur_idx, string& candidate, bool v[], vector<string>& res)
{
    if (cur_idx == str.length()) {
        // Check duplicated.
        for (vector<string>::iterator it = res.begin(); it != res.end(); it++) {
            if (*it == candidate) {
                return;
            }
        }
        res.push_back(candidate);
        return;
    }

    for (int i = 0; i < str.length(); i++) {
        if (!v[i]) {
            v[i] = true;
            candidate += str[i];
            foo(str, cur_idx + 1, candidate, v, res);
            candidate.erase(candidate.end() - 1);
            v[i] = false;
        }
    }
}

int main(int argc, char const* argv[])
{
    string str("12345");
    vector<string> res;
    string candidate;
    bool* v = new bool[str.length()]();

    foo(str, 0, candidate, v, res);

    for (vector<string>::iterator it = res.begin(); it != res.end(); it++) {
        cout << *it << endl;
    }
    cout << "The total size is: " << res.size() << endl;

    delete [] v;

    return 0;
}