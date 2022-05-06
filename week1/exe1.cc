#include <bits/stdc++.h>

using namespace std;

const int POSITIVE_INF = numeric_limits<int>::max();

int DPChange(int money, vector<int> &v);

int main(int argc, char const *argv[])
{
    int money = 19083;
    vector<int> v = {11, 5, 3, 1};

    cout << DPChange(money, v) << "\n";

    return 0;
}

int DPChange(int money, vector<int> &coins)
{
    vector<int> MNC; // Minimum Nuber of Coins = MNC
    MNC.push_back(0);

    for (int m = 1; m <= money; m++)
    {
        MNC.push_back(POSITIVE_INF);

        for (auto coin : coins)
        {
            if (m >= coin)
            {
                if (MNC[m - coin] + 1 < MNC[m])
                {
                    MNC[m] = MNC[m - coin] + 1;
                }
            }
        }
    }

    return MNC[money];
}
