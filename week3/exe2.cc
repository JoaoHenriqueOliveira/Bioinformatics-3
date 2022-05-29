#include <bits/stdc++.h>
//* Middle Edge in Linear Space Problem (for protein strings)

using namespace std;

vector<vector<int>> MiddleEdge(string &v, string &w, int middle_index, int &match, int &mu, int &indel);

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
    int match, mu, sigma, middle_index;
    populate(file_name, v, w, match, mu, sigma);

    middle_index = w.size() / 2;

    auto edge = MiddleEdge(v, w, middle_index, match, mu, sigma);

    cout << edge[0][0] << ' ' << edge[0][1] << '\n'
         << edge[1][0] << ' ' << edge[1][1] << '\n';

    return 0;
}

void initialize_score(vector<vector<int>> &from_src, vector<vector<int>> &to_sink, int n, int indel)
{
    vector<int> v1(n, 0), v2(n, 0);
    from_src.push_back(v1);
    from_src.push_back(v2);

    to_sink.push_back(v1);
    to_sink.push_back(v2);

    from_src[1][0] = -indel;
    to_sink[1][0] = -indel;

    for (int i = 1; i < n; i++)
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

vector<int> MiddleColumn(string &v, string &w, int col, int &match, int &mu, int &indel)
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

    return length_i;
}

vector<vector<int>> MiddleEdge(string &v, string &w, int col, int &match, int &mu, int &indel)
{
    int n = v.size(), m = w.size(), match_score_src(0), match_score_sink(0), curr_index(0), middle_node_src(0), middle_node_sink(0);

    assert(col < m);

    vector<int> middle_col_src = MiddleColumn(v, w, col, match, mu, indel),
                middle_col_tgt = MiddleColumn(v, w, col + 1, match, mu, indel);

    int middle_node = max_element(middle_col_src.begin(), middle_col_src.end()) - middle_col_src.begin();

    if (middle_col_src[middle_node] == middle_col_tgt[middle_node + 1])
    {
        // Diag edge
        return {{middle_node, col}, {middle_node + 1, col + 1}};
    }
    else if (middle_col_src[middle_node] == middle_col_tgt[middle_node])
    {
        // Horz edge
        return {{middle_node, col}, {middle_node, col + 1}};
    }
    else
    {
        // Vertical edge
        return {{middle_node, col}, {middle_node + 1, col}};
    }
}
