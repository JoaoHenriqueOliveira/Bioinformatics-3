#include <bits/stdc++.h>
//* Overlap Alignment Problem

using namespace std;

pair<int, int> Overlap_Alignment(string v, string w, vector<vector<char>> &Backtrack, int &similarity, int match, int mu, int sigma);
void OutputLCS(vector<vector<char>> &Backtrack, string w, string v, int i, int j, vector<char> &Sv, vector<char> &Sw);

void populate(string file, string &v, string &w, int &match, int &mu, int &sigma)
{
    ifstream my_file(file);
    assert(my_file.is_open());

    my_file >> match >> mu >> sigma;
    my_file >> v >> w;
}

int main(int argc, char const *argv[])
{
    string file_name = argv[1], // data_exe5.txt
        v, w;
    int match, mu, sigma, similarity(0);
    vector<vector<char>> Backtrack;
    vector<char> solution_v, solution_w;

    populate(file_name, v, w, match, mu, sigma);

    auto coord = Overlap_Alignment(v, w, Backtrack, similarity, match, mu, sigma);
    int i_overlap = coord.first, j_overlap = coord.second;

    OutputLCS(Backtrack, w, v, i_overlap, j_overlap, solution_v, solution_w);
    reverse(solution_v.begin(), solution_v.end());
    reverse(solution_w.begin(), solution_w.end());

    cout << similarity << "\n";
    for (auto elem : solution_v)
    {
        cout << elem;
    }
    cout << '\n';
    for (auto elem : solution_w)
    {
        cout << elem;
    }
    cout << '\n';

    return 0;
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

void initialize_backtrack(vector<vector<char>> &m, int row, int col)
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

void initialize_score(vector<vector<int>> &s, int row, int col)
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

    //* First row and col aren't penalized as any place can be the starting point

    return;
}

pair<int, int> Overlap_Alignment(string v, string w, vector<vector<char>> &Backtrack, int &similarity, int match, int mu, int sigma)
{
    int n = v.size(), m = w.size(), max_row = numeric_limits<int>::min(), max_col = numeric_limits<int>::min(), i_max, j_max;
    vector<vector<int>> s;
    pair<int, int> p;
    initialize_score(s, n + 1, m + 1);
    initialize_backtrack(Backtrack, n + 1, m + 1);

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            int match_score = v[i - 1] == w[j - 1] ? match : -mu;
            s[i][j] = max(max(s[i - 1][j] - sigma, s[i][j - 1] - sigma), s[i - 1][j - 1] + match_score);

            if (s[i][j] == s[i][j - 1] - sigma)
            {
                Backtrack[i][j] = 'r';
            }
            else if (s[i][j] == s[i - 1][j] - sigma)
            {
                Backtrack[i][j] = 'd';
            }
            else if (s[i][j] == s[i - 1][j - 1] + match)
            {
                Backtrack[i][j] = 'M';
            }
            else if (s[i][j] == s[i - 1][j - 1] - mu)
            {
                Backtrack[i][j] = 'm';
            }
        }
    }

    for (int i = 0; i <= n; i++)
    {
        if (s[i][m] > max_col)
        {
            max_col = s[i][m];
            i_max = i;
        }
    }

    for (int j = 0; j <= m; j++)
    {
        if (s[n][j] > max_row)
        {
            max_row = s[n][j];
            j_max = j;
        }
    }

    if (max_row > max_col)
    {
        similarity = max_row;
        p = {n, j_max};
    }
    else
    {
        similarity = max_col;
        p = {i_max, m};
    }

    // display_matrix(s);
    // cout << '\n';
    // display_backtrack(Backtrack);

    return p;
}

void OutputLCS(vector<vector<char>> &Backtrack, string w, string v, int i, int j, vector<char> &Sv, vector<char> &Sw)
{
    if (i == 0 || j == 0)
    {
        return;
    }

    if (Backtrack[i][j] == 'd')
    {
        Sv.push_back(v[--i]);
        Sw.push_back('-');
        return OutputLCS(Backtrack, w, v, i, j, Sv, Sw);
    }
    else if (Backtrack[i][j] == 'r')
    {
        Sw.push_back(w[--j]);
        Sv.push_back('-');
        return OutputLCS(Backtrack, w, v, i, j, Sv, Sw);
    }
    else
    {
        Sv.push_back(v[--i]);
        Sw.push_back(w[--j]);
        return OutputLCS(Backtrack, w, v, i, j, Sv, Sw);
    }

    return;
}