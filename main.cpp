#include <bits/stdc++.h>
#include <chrono>
#include <random>
// Livliness

using namespace std;

void printvector(vector<int> &v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";
    }
    cout << endl;
}

bool solve(vector<int> &v, int n, map<vector<int>, bool> &dp, int fno, vector<vector<int>> &st)
{
    // impossible
    if (n < 0)
        return false;
    // base case
    if (n == 0)
    {
        // for stalling the system we need to have no of nodes with decision 1 = no of nodes with decision 0
        bool c1 = v[0] + v[3] == v[1] + v[4];
        // faulty nodes with decision 1 + faulty nodes with decision 0 + faulty nodes who do not respond
        bool c2 = v[0] + v[1] + v[2] == fno;
        // for l1: all loyal nodes should have same decision v[3]*v[4]==0
        bool c3 = v[3] == 0 || v[4] == 0;
        if (c1 && c2 && c3)
        {
            // printvector(v);
            st.push_back(v);
        }
        return c1 && c2 && c3;
    }
    if (dp.find(v) != dp.end())
    {
        return dp[v];
    }
    bool flag = false;
    for (int i = 0; i < 5; i++)
    {
        v[i]++;
        if (solve(v, n - 1, dp, fno, st))
        {
            // return
            flag = true;
        }
        v[i]--;
    }
    return dp[v] = flag;
}

int main()
{
    int serial = 0;
    int fno;
    int nlimit = 15;
    cout << "Enter number of malicious peers: \n";
    cin >> fno;
    // for f faulty nodes how many nodes we require for liveliness
    // ie client should eventually get a response
    for (int i = 1; i < nlimit; i++)
    {
        int n = i + fno;
        map<vector<int>, bool> dp;
        vector<vector<int>> st;
        vector<int> v(5, 0);
        // v[0] = faulty nodes with decision 1
        // v[1] = faulty nodes with decision 0
        // v[2] = faulty nodes that dont respond
        // v[3] = loyal nodes with decision 1
        // v[4] = loyal nodes with decision 0
        bool ans = solve(v, n, dp, fno, st);

        if (ans == true)
        {
            cout << "----------------------------\n";
            cout << "No liveness at n = " << n << endl;
            for (auto vec : st)
            {
                for (int val : vec)
                {
                    cout << val << " ";
                }
                cout << endl;
            }
            serial++;
        }
        else
        {
            cout << "----------------------------\n";
            cout << "liveness at n = " << n << endl;
        }
    }
    return 0;
}