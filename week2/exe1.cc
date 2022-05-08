#include <bits/stdc++.h>
//* Global ALignment Problem (Needleman–Wunsch Algorithm)

using namespace std;

void Global_Alignment(string v, string w, vector<vector<char>> &Backtrack, int &similarity, int match, int mu, int sigma);
void OutputLCS(vector<vector<char>> &Backtrack, string w, string v, int i, int j, vector<char> &solution_v, vector<char> &solution_w, int &match, int &mu, int &sigma);

void populate(string file_name, int &match, int &mismatch, int &indel, string &v, string &w)
{
    ifstream my_file(file_name);
    assert(my_file.is_open());
    my_file >> match >> mismatch >> indel;
    my_file >> w;
    my_file >> v;

    return;
}

int main(int argc, char const *argv[])
{
    string file = argv[1], // dataset_247_3.txt
        v, w;
    int match, mu, sigma, similarity(0);
    vector<vector<char>> Backtrack;
    vector<char> solution_v, solution_w;

    populate(file, match, mu, sigma, v, w);

    Global_Alignment(v, w, Backtrack, similarity, match, mu, sigma);
    OutputLCS(Backtrack, w, v, v.size(), w.size(), solution_v, solution_w, match, mu, sigma);

    reverse(solution_v.begin(), solution_v.end());
    reverse(solution_w.begin(), solution_w.end());

    cout << similarity << "\n";
    for (auto elem : solution_w)
    {
        cout << elem;
    }
    cout << '\n';
    for (auto elem : solution_v)
    {
        cout << elem;
    }
    cout << '\n';

    return 0;
}

void initialize_score(vector<vector<int>> &s, int row, int col, int &indel)
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

    for (int i = 1; i < col; i++)
    {
        s[0][i] += s[0][i - 1] - indel;
    }

    for (int i = 1; i < row; i++)
    {
        s[i][0] += s[i - 1][0] - indel;
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

void Global_Alignment(string v, string w, vector<vector<char>> &Backtrack, int &similarity, int match, int mu, int sigma)
{
    int n = v.size(), m = w.size();
    vector<vector<int>> s;
    initialize_score(s, n + 1, m + 1, sigma);
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
    similarity = s[n][m];

    // display_matrix(s);
    // cout << '\n';
    // display_backtrack(Backtrack);

    return;
}

void OutputLCS(vector<vector<char>> &Backtrack, string w, string v, int i, int j, vector<char> &Sv, vector<char> &Sw, int &match, int &mu, int &sigma)
{
    if (i == 0 && j == 0)
    {
        return;
    }
    else if (i == 0 && j != 0)
    {
        while (j != 0)
        {

            Sv.push_back('-');
            Sw.push_back(w[--j]);
        }

        return;
    }
    else if (j == 0 && i != 0)
    {
        while (i != 0)
        {
            Sv.push_back(v[--i]);
            Sw.push_back('-');
        }

        return;
    }

    if (Backtrack[i][j] == 'd')
    {
        Sv.push_back(v[i - 1]);
        Sw.push_back('-');
        return OutputLCS(Backtrack, w, v, i - 1, j, Sv, Sw, match, mu, sigma);
    }
    else if (Backtrack[i][j] == 'r')
    {
        Sw.push_back(w[j - 1]);
        Sv.push_back('-');
        return OutputLCS(Backtrack, w, v, i, j - 1, Sv, Sw, match, mu, sigma);
    }
    else
    {

        Sv.push_back(v[i - 1]);
        Sw.push_back(w[j - 1]);
        return OutputLCS(Backtrack, w, v, i - 1, j - 1, Sv, Sw, match, mu, sigma);
    }

    return;
}
