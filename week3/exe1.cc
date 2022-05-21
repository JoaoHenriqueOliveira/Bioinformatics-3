#include <bits/stdc++.h>

using namespace std;

const int NEGATIVE_INF = (1 << 31) + 1; // -2^31 +1

void populate(string file, string &v, string &w, int &match, int &mismatch, int &sigma, int &epsilon)
{
    ifstream my_file(file);
    assert(my_file.is_open());
    my_file >> match >> mismatch >> sigma >> epsilon;
    my_file >> v >> w;

    return;
}

void ThreeLevelManhattan(vector<vector<char>> &Backtrack, string &v, string &w, int &match, int &mismatch, int &sgima, int &epsilon, int &similarity);
void OutputLCS(vector<vector<char>> &Backtrack, string w, string v, int i, int j, vector<char> &solution_v, vector<char> &solution_w);

int main(int argc, char const *argv[])
{
    string file_name = argv[1], v, w;
    int match, mu, sigma, epsilon, similarity;

    populate(file_name, v, w, match, mu, sigma, epsilon);

    vector<vector<char>> Backtrack;
    vector<char> solution_v, solution_w;

    ThreeLevelManhattan(Backtrack, v, w, match, mu, sigma, epsilon, similarity);
    OutputLCS(Backtrack, w, v, v.size(), w.size(), solution_v, solution_w);

    reverse(solution_v.begin(), solution_v.end());
    reverse(solution_w.begin(), solution_w.end());

    cout << similarity << '\n';

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

void initialize_lower(vector<vector<int>> &l, int row, int col, int sigma, int epsilon)
{
    for (int i = 0; i < row; i++)
    {
        vector<int> v;
        l.push_back(v);
        for (int j = 0; j < col; j++)
        {
            l[i].push_back(0);
        }
    }

    for (int j = 0; j < col; j++)
    {
        l[0][j] = NEGATIVE_INF;
    }

    for (int i = 1; i < row; i++)
    {
        l[i][0] = -sigma - (i - 1) * epsilon;
    }

    return;
}

void initialize_upper(vector<vector<int>> &u, int row, int col, int sigma, int epsilon)
{
    for (int i = 0; i < row; i++)
    {
        vector<int> v;
        u.push_back(v);
        for (int j = 0; j < col; j++)
        {
            u[i].push_back(0);
        }
    }

    for (int i = 0; i < row; i++)
    {
        u[i][0] = NEGATIVE_INF;
    }

    for (int j = 1; j < col; j++)
    {
        u[0][j] = -sigma - (j - 1) * epsilon;
    }

    return;
}

void initialize_middle(vector<vector<int>> &m, int row, int col, int sigma, int epsilon)
{
    for (int i = 0; i < row; i++)
    {
        vector<int> v;
        m.push_back(v);
        for (int j = 0; j < col; j++)
        {
            m[i].push_back(0);
        }
    }

    for (int j = 1; j < col; j++)
    {
        m[0][j] = -sigma - (j - 1) * epsilon;
    }

    for (int i = 1; i < row; i++)
    {
        m[i][0] = -sigma - (i - 1) * epsilon;
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

void ThreeLevelManhattan(vector<vector<char>> &Backtrack, string &v, string &w, int &match, int &mu, int &sigma, int &epsilon, int &similarity)
{
    int n = v.size(), m = w.size();
    vector<vector<int>> lower, middle, upper;
    int match_score;

    initialize_backtrack(Backtrack, n + 1, m + 1);
    initialize_lower(lower, n + 1, m + 1, sigma, epsilon);
    initialize_upper(upper, n + 1, m + 1, sigma, epsilon);
    initialize_middle(middle, n + 1, m + 1, sigma, epsilon);

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            match_score = v[i - 1] == w[j - 1] ? match : -mu;

            upper[i][j] = max(upper[i][j - 1] - epsilon, middle[i][j - 1] - sigma);
            lower[i][j] = max(lower[i - 1][j] - epsilon, middle[i - 1][j] - sigma);
            middle[i][j] = max(max(lower[i][j], upper[i][j]), middle[i - 1][j - 1] + match_score);

            if (middle[i][j] == upper[i][j])
            {
                Backtrack[i][j] = 'r';
            }
            else if (middle[i][j] == lower[i][j])
            {
                Backtrack[i][j] = 'd';
            }
            else if (middle[i][j] == middle[i - 1][j - 1] + match)
            {
                Backtrack[i][j] = 'm';
            }
        }
    }
    similarity = middle[n][m];

    // display_matrix(lower);
    // cout << "\n\n";
    // display_matrix(middle);
    // cout << "\n\n";
    // display_matrix(upper);
    // cout << '\n';
    // display_backtrack(Backtrack);

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
