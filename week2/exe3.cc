#include <bits/stdc++.h>

using namespace std;

int Edit_Distance(string &v, string &w);

void populate(string file, string &v, string &w)
{
    ifstream my_file(file);
    assert(my_file.is_open());

    my_file >> v >> w;

    return;
}

int main(int argc, char const *argv[])
{
    string file_name = argv[1], // dataset_248_3.txt
        v, w;
    populate(file_name, v, w);

    int solution = Edit_Distance(v, w);

    cout << solution << '\n';

    return 0;
}

void initialize_score(vector<vector<int>> &s, int row, int col, int indel)
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

    for (int j = 1; j < col; j++)
    {
        s[0][j] += s[0][j - 1] - indel;
    }

    for (int i = 1; i < row; i++)
    {
        s[i][0] += s[i - 1][0] - indel;
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

void Global_Alignment(string v, string w, vector<vector<char>> &Backtrack, int match, int mu, int sigma)
{
    int n = v.size(), m = w.size();
    vector<vector<int>> s;
    initialize_score(s, n + 1, m + 1, sigma);
    initialize_with_char(Backtrack, n + 1, m + 1);

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

    return;
}

void OutputLCS(vector<vector<char>> &Backtrack, string w, string v, int i, int j, vector<char> &Sv, vector<char> &Sw)
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
            Sw.push_back(w[j - 1]);
            j--;
        }

        return;
    }
    else if (j == 0 && i != 0)
    {
        while (i != 0)
        {
            Sv.push_back(v[i - 1]);
            Sw.push_back('-');
            i--;
        }

        return;
    }
    if (Backtrack[i][j] == 'd')
    {
        Sv.push_back(v[i - 1]);
        Sw.push_back('-');
        return OutputLCS(Backtrack, w, v, i - 1, j, Sv, Sw);
    }
    else if (Backtrack[i][j] == 'r')
    {
        Sw.push_back(w[j - 1]);
        Sv.push_back('-');
        return OutputLCS(Backtrack, w, v, i, j - 1, Sv, Sw);
    }
    else
    {
        Sv.push_back(v[i - 1]);
        Sw.push_back(w[j - 1]);
        return OutputLCS(Backtrack, w, v, i - 1, j - 1, Sv, Sw);
    }

    return;
}

int Hamming_Distance(vector<char> &Sv, vector<char> &Sw)
{
    assert(Sv.size() == Sw.size());
    int distance = 0;

    for (int i = 0; i < Sv.size(); i++)
    {
        if (Sv[i] != Sw[i])
            distance++;
    }

    return distance;
}

int Edit_Distance(string &v, string &w)
{
    vector<vector<char>> Backtrack;
    vector<char> solution_v, solution_w;
    int solution;

    Global_Alignment(v, w, Backtrack, 0, 1, 1);
    OutputLCS(Backtrack, w, v, v.size(), w.size(), solution_v, solution_w);
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

    solution = Hamming_Distance(solution_v, solution_w);

    return solution;
}
