#include <bits/stdc++.h>

using namespace std;

const int SIGMA = 5;

pair<int, int> Local_Global_Alignment(string &v, string &w, vector<vector<char>> &Backtrack, map<char, map<char, int>> &PAM250, int &similarity);
void OutputLCS(vector<vector<char>> &Backtrack, string w, string v, int i, int j, vector<char> &Sv, vector<char> &Sw);

void populate(string file, string &v, string &w)
{
    ifstream my_file(file);
    assert(my_file.is_open());
    my_file >> v >> w;

    return;
}

void build_PAM250(map<char, map<char, int>> &PAM250, string file = "PAM250.txt")
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
            PAM250[chr] = tmp;
        }
        break;
    }

    while (!my_file.eof())
    {
        for (auto elem : PAM250)
        {
            char protein = elem.first;
            int tmp;
            for (auto var : PAM250)
            {
                my_file >> tmp;
                PAM250[protein][var.first] = tmp;
            }
        }
    }

    return;
}

int main(int argc, char const *argv[])
{
    string file_name = argv[1], v, w;
    int similarity(0);
    map<char, map<char, int>> PAM250;
    vector<vector<char>> Backtrack;

    vector<char> solution_v, solution_w;

    populate(file_name, v, w);
    build_PAM250(PAM250);

    auto max_coord = Local_Global_Alignment(v, w, Backtrack, PAM250, similarity);
    int i_max = max_coord.first, j_max = max_coord.second;

    OutputLCS(Backtrack, w, v, i_max, j_max, solution_v, solution_w);
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

    return;
}

pair<int, int> Local_Global_Alignment(string &v, string &w, vector<vector<char>> &Backtrack, map<char, map<char, int>> &PAM250, int &similarity)
{
    int n = v.size(), m = w.size();
    vector<vector<int>> s;
    initialize_score(s, n + 1, m + 1);
    initialize_backtrack(Backtrack, n + 1, m + 1);
    int max_val = numeric_limits<int>::min(), i_max, j_max;

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            char chr1 = v[i - 1], chr2 = w[j - 1];
            int match_score = PAM250[chr1][chr2];
            s[i][j] = max(max(s[i - 1][j] - SIGMA, s[i][j - 1] - SIGMA),
                          max(0, s[i - 1][j - 1] + match_score));

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
                Backtrack[i][j] = 'M'; // match/mismatch
            }
            else
            {
                Backtrack[i][j] = 's'; // source
            }

            if (s[i][j] >= max_val)
            {
                max_val = s[i][j];
                i_max = i;
                j_max = j;
            }
        }
    }

    similarity = s[i_max][j_max];

    // display_matrix(s);
    // cout << '\n';
    // display_backtrack(Backtrack);

    return {i_max, j_max};
}

void OutputLCS(vector<vector<char>> &Backtrack, string w, string v, int i, int j, vector<char> &Sv, vector<char> &Sw)
{
    if (Backtrack[i][j] == 's')
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
