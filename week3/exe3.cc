#include <bits/stdc++.h>
//* Global Alignment problem using linear space complexity

using namespace std;

void LinearSpaceAlignment(string &v, string &w, int top, int bottom, int left, int right, int match, int mu, int sigma, vector<char> &path);

void OutputAlignment(vector<char> &path, string &v, string &w, int &match, int &mu, int &sigma);

vector<vector<int>> MiddleEdges(string &v, string &w, int top, int bottom, int col, int match, int mu, int indel);

void populate(string file, string &v, string &w, int &match, int &mu, int &sigma)
{
    fstream my_file(file);
    assert(my_file.is_open());
    my_file >> match >> mu >> sigma;
    my_file >> w >> v;

    return;
}

int main(int argc, char const *argv[])
{
    string file_name = argv[1], v, w;
    int match, mu, sigma; // match reward, mismatch penalty, indel penalty
    vector<char> path;

    populate(file_name, v, w, match, mu, sigma);

    LinearSpaceAlignment(v, w, 0, v.size(), 0, w.size(), match, mu, sigma, path);

    OutputAlignment(path, v, w, match, mu, sigma);

    return 0;
}

void OutputAlignment(vector<char> &path, string &v, string &w, int &match, int &mu, int &sigma)
{
    int v_i(0), w_i(0), similarity(0);
    vector<char> solution_v, solution_w;

    for (int i = 0; i < path.size(); i++)
    {
        if (path[i] == 'm')
        {
            similarity += v[v_i] == w[w_i] ? match : -mu;

            solution_v.push_back(v[v_i++]);
            solution_w.push_back(w[w_i++]);
        }
        else if (path[i] == 'r')
        {
            similarity += -sigma;

            solution_v.push_back('-');
            solution_w.push_back(w[w_i++]);
        }
        else if (path[i] == 'd')
        {
            similarity += -sigma;

            solution_v.push_back(v[v_i++]);
            solution_w.push_back('-');
        }
    }

    cout << similarity << '\n';

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

    return;
}

void initialize_score(vector<vector<int>> &from_src, vector<vector<int>> &to_sink, int row, int indel)
{
    vector<int> v1(row, 0), v2(row, 0);
    from_src.push_back(v1);
    from_src.push_back(v2);

    to_sink.push_back(v1);
    to_sink.push_back(v2);

    from_src[1][0] = -indel;
    to_sink[1][0] = -indel;

    for (int i = 1; i < row; i++)
    {
        from_src[0][i] -= i * indel;
        to_sink[0][i] -= i * indel;
    }

    return;
}

void FromSrc(string &v, string &w, vector<vector<int>> &from_src, int col, int &match, int &mu, int &indel)
{
    int curr_col, prev_col, n = v.size(), m = w.size(), match_score;

    assert(col >= 0 && col <= m);

    for (int j = 1; j <= col; j++)
    {
        curr_col = j % 2;
        prev_col = abs(curr_col - 1);
        from_src[curr_col][0] = -j * indel;

        for (int i = 1; i <= n; i++)
        {
            match_score = v[i - 1] == w[j - 1] ? match : -mu;
            from_src[curr_col][i] = max(max(from_src[prev_col][i] - indel, from_src[curr_col][i - 1] - indel),
                                        from_src[prev_col][i - 1] + match_score);
        }
    }

    return;
}

void ToSink(string &v, string &w, vector<vector<int>> &to_sink, int col, int &match, int &mu, int &indel)
{
    int curr_col, prev_col, n = v.size(), m = w.size(), match_score;
    assert(col >= 0 && col <= m);

    for (int j = 1; j <= m - col; j++)
    {
        curr_col = j % 2;
        prev_col = abs(curr_col - 1);
        to_sink[curr_col][0] = -j * indel;

        for (int i = 1; i <= n; i++)
        {
            match_score = v[n - i] == w[m - j] ? match : -mu;
            to_sink[curr_col][i] = max(max(to_sink[prev_col][i] - indel, to_sink[curr_col][i - 1] - indel),
                                       to_sink[prev_col][i - 1] + match_score);
        }
    }

    return;
}

vector<vector<int>> MiddleColumn(string &v, string &w, int col, int &match, int &mu, int &indel)
{
    int n = v.size(), m = w.size();
    vector<int> length_i(n + 1, 0);
    vector<vector<int>> from_src, to_sink;
    initialize_score(from_src, to_sink, n + 1, indel);

    FromSrc(v, w, from_src, col, match, mu, indel); // from_src[col % 2] -> middle column
    ToSink(v, w, to_sink, col, match, mu, indel);   // to_sink[(m - col) % 2] -> middle column

    for (int i = 0; i <= n; i++)
    {
        length_i[i] = from_src[col % 2][i] + to_sink[(m - col) % 2][n - i];
    }

    return {length_i, from_src[col % 2]};
}

vector<vector<int>> MiddleEdges(string &v, string &w, int top, int bottom, int col, int match, int mu, int indel)
{
    int n = v.size(), m = w.size();

    assert(col < m);

    vector<vector<int>> cols_src = MiddleColumn(v, w, col, match, mu, indel), cols_tgt = MiddleColumn(v, w, col + 1, match, mu, indel);
    vector<int> middle_col_src = cols_src[0], score_src = cols_src[1],
                middle_col_tgt = cols_tgt[0], score_tgt = cols_tgt[1];

    int node_src = max_element(middle_col_src.begin(), middle_col_src.end()) - middle_col_src.begin();
    int node_tgt = max_element(middle_col_tgt.begin(), middle_col_tgt.end()) - middle_col_tgt.begin();
    int max_value = middle_col_src[node_src], match_score;

    // Local search of edge within [top, bottom] x [col, col + 1]

    for (int i = top; i <= bottom; i++)
    {
        if (middle_col_src[i] == max_value && middle_col_tgt[i] == max_value)
        {
            // return Horz edge
            return {{i, col}, {i, col + 1}};
        }
        else if (middle_col_src[i] == max_value && middle_col_tgt[i + 1] == max_value)
        {
            // possible Diag edge
            match_score = v[i] == w[col] ? match : -mu;
            if (score_src[i] + match_score == score_tgt[i + 1])
            {
                // Diag edge
                return {{i, col}, {i + 1, col + 1}};
            }
        }
    }

    // return Vertical edge
    return {{bottom, col}, {bottom + 1, col}};
}

char solve(vector<vector<int>> &edge)
{
    int a_i = edge[0][0], a_j = edge[0][1], b_i = edge[1][0], b_j = edge[1][1];

    if (a_i == b_i && a_j + 1 == b_j)
    {
        return 'r';
    }
    else if (b_j == a_j && a_i + 1 == b_i)
    {
        return 'd';
    }
    else if (a_i + 1 == b_i && a_j + 1 == b_j)
    {
        return 'm';
    }
    else
    {
        return '?';
    }
}

void LinearSpaceAlignment(string &v, string &w, int top, int bottom, int left, int right, int match, int mu, int sigma, vector<char> &path)
{
    if (left == right)
    {
        int times = bottom - top, i(0);

        while (i < times)
        {
            path.push_back('d');
            i++;
        }

        return;
    }

    if (top == bottom)
    {
        int times = right - left, i(0);

        while (i < times)
        {
            path.push_back('r');
            i++;
        }

        return;
    }

    int middle_col = (left + right) / 2;
    auto edge = MiddleEdges(v, w, top, bottom, middle_col, match, mu, sigma);
    int midNode = edge[0][0];

    LinearSpaceAlignment(v, w, top, midNode, left, middle_col, match, mu, sigma, path);

    char direction = solve(edge);
    path.push_back(direction);

    switch (direction)
    {
    case 'm':
        middle_col++;
        midNode++;
        break;
    case 'r':
        middle_col++;
        break;
    case 'd':
        midNode++;
        break;
    default:
        cout << "ERROR: " << direction << '\n';
        break;
    }

    LinearSpaceAlignment(v, w, midNode, bottom, middle_col, right, match, mu, sigma, path);

    return;
}