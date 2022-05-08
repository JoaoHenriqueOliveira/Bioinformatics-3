#include <bits/stdc++.h>
//* Fitting Alignment Problem

using namespace std;
const int SIGMA = 1;

void build_BLOSUM62(map<char, map<char, int>> &BLOSUM62, string file = "BLOSUM62.txt")
{
    ifstream my_file(file);
    assert(my_file.is_open());
    char i;

    for (string line; getline(my_file, line);)
    {
        for (auto chr : line)
        {
            if (chr == ' ')
                continue;
            map<char, int> tmp;
            BLOSUM62[chr] = tmp;
        }
        break;
    }

    while (!my_file.eof())
    {
        for (auto elem : BLOSUM62)
        {
            char protein = elem.first;
            int tmp;
            for (auto var : BLOSUM62)
            {
                my_file >> tmp;
                BLOSUM62[protein][var.first] = tmp;
            }
        }
    }

    // for (auto elem : BLOSUM62)
    // {
    //     char prt1 = elem.first;
    //     for (auto var : elem.second)
    //     {
    //         cout << "(" << prt1 << ", " << var.first << ") = " << var.second << '\t';
    //     }
    //     cout << '\n';
    // }

    return;
}

void populate(string file, string &v, string &w)
{
    ifstream my_file(file);
    assert(my_file.is_open());

    my_file >> v >> w;

    return;
}

pair<int, int> Fitting_Alignment(string &v, string &w, vector<vector<char>> &Backtrack, map<char, map<char, int>> &BLOSUM62, int &similarity);
void OutputLCS(vector<vector<char>> &Backtrack, string w, string v, int i, int j, vector<char> &Sv, vector<char> &Sw);

int main(int argc, char const *argv[])
{
    string file_name = argv[1], // dataset_248_5.txt
        v, w;
    map<char, map<char, int>> BLOSUM62;
    vector<vector<char>> Backtrack;
    vector<char> solution_v, solution_w;
    int similarity(0);

    populate(file_name, v, w);
    build_BLOSUM62(BLOSUM62);

    auto coord = Fitting_Alignment(v, w, Backtrack, BLOSUM62, similarity);
    int i_fit = coord.first, j_fit = coord.second;

    OutputLCS(Backtrack, w, v, i_fit, j_fit, solution_v, solution_w);
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

    // Only first row is penalized
    for (int j = 1; j < col; j++)
    {
        s[0][j] += s[0][j - 1] - SIGMA;
    }

    return;
}

pair<int, int> Fitting_Alignment(string &v, string &w, vector<vector<char>> &Backtrack, map<char, map<char, int>> &BLOSUM62, int &similarity)
{
    int n = v.size(), m = w.size();
    vector<vector<int>> s;
    initialize_score(s, n + 1, m + 1);
    initialize_backtrack(Backtrack, n + 1, m + 1);
    int max_val = numeric_limits<int>::min(), i_max;

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            char protein1 = v[i - 1], protein2 = w[j - 1];
            int match_score = BLOSUM62[protein1][protein2];
            s[i][j] = max(max(s[i - 1][j] - SIGMA, s[i][j - 1] - SIGMA), s[i - 1][j - 1] + match_score);

            if (s[i][j] == s[i][j - 1] - SIGMA)
            {
                Backtrack[i][j] = 'r'; // right
            }
            else if (s[i][j] == s[i - 1][j] - SIGMA)
            {
                Backtrack[i][j] = 'd'; // down
            }
            else if (s[i][j] == s[i - 1][j - 1] + match_score)
            {
                Backtrack[i][j] = 'm'; // match/mismatch
            }
        }
    }

    for (int i = 0; i <= n; i++)
    {
        if (s[i][m] > max_val)
        {
            max_val = s[i][m];
            i_max = i;
        }
    }

    similarity = max_val;

    // display_matrix(s);
    // cout << '\n';
    // display_backtrack(Backtrack);

    return {i_max, m};
}

void OutputLCS(vector<vector<char>> &Backtrack, string w, string v, int i, int j, vector<char> &Sv, vector<char> &Sw)
{
    if (j == 0)
    {
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
