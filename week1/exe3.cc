#include <bits/stdc++.h>

using namespace std;

void LCSBackTrack(string v, string w, vector<vector<char>> &Backtrack);
void OutputLCS(vector<vector<char>> &Backtrack, string v, int i, int j, vector<char> &solution);

int main(int argc, char const *argv[])
{
    string v = "AACCTTGG",
           w = "ACACTGTGA";
    vector<char> solution;
    vector<vector<char>> b;
    LCSBackTrack(v, w, b);
    OutputLCS(b, v, v.size(), w.size(), solution);

    for (auto itr = solution.end() - 1; itr != solution.begin() - 1; itr--)
    {
        cout << *itr;
    }
    cout << '\n';

    return 0;
}

void initialize_with_zero(vector<vector<int>> &s, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        vector<int> v;
        s.push_back(v);
        for (int j = 0; j < col; j++)
        {
            s[i].push_back(0);
        }
    }
    return;
}

void initialize_with_char(vector<vector<char>> &m, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        vector<char> v;
        m.push_back(v);
        for (int j = 0; j < col; j++)
        {
            m[i].push_back('-');
        }
    }
    return;
}

void display_matrix(vector<vector<int>> &m)
{
    for (auto row : m)
    {
        for (auto elem : row)
        {
            cout << elem << "\t";
        }
        cout << "\n\n";
    }

    return;
}

void display_backtrack(vector<vector<char>> &m)
{
    for (auto row : m)
    {
        for (auto elem : row)
        {
            cout << elem << "\t";
        }
        cout << "\n\n";
    }

    return;
}

void LCSBackTrack(string v, string w, vector<vector<char>> &Backtrack)
{
    int n = v.size(), m = w.size();
    vector<vector<int>> s;
    initialize_with_zero(s, n + 1, m + 1);
    initialize_with_char(Backtrack, n + 1, m + 1);

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            int match = 0;
            if (v[i - 1] == w[j - 1])
            {
                match = 1;
            }
            s[i][j] = max(max(s[i - 1][j], s[i][j - 1]), s[i - 1][j - 1] + match);

            if (s[i][j] == s[i - 1][j])
            {
                Backtrack[i][j] = 'd';
            }
            else if (s[i][j] == s[i][j - 1])
            {
                Backtrack[i][j] = 'r';
            }
            else if (s[i][j] == s[i - 1][j - 1] + match)
            {
                Backtrack[i][j] = 'm';
            }
        }
    }
    // display_matrix(s);
    // cout << '\n';
    // display_backtrack(Backtrack);

    return;
}

void OutputLCS(vector<vector<char>> &Backtrack, string v, int i, int j, vector<char> &solution)
{
    if (i == 0 || j == 0)
    {
        return;
    }
    if (Backtrack[i][j] == 'd')
    {
        return OutputLCS(Backtrack, v, i - 1, j, solution);
    }
    else if (Backtrack[i][j] == 'r')
    {
        return OutputLCS(Backtrack, v, i, j - 1, solution);
    }
    else
    {
        solution.push_back(v[i - 1]);
        return OutputLCS(Backtrack, v, i - 1, j - 1, solution);
    }

    return;
}
