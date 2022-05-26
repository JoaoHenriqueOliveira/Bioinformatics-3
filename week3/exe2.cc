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

    auto edges = MiddleEdge(v, w, middle_index, match, mu, sigma);

    cout << edges[0][0] << " " << edges[0][1] << '\n'
         << edges[1][0] << ' ' << edges[1][1] << '\n';

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

void build_length_i(vector<vector<int>> &from_src, vector<vector<int>> &to_sink, int curr_index, int next_index, int n, int m, vector<vector<int>> &length_i)
{
    vector<int> v1, v2;
    int src_node, sink_node;
    if (m % 2 == 0)
    {
        for (int i = 0; i <= n; i++)
        {
            src_node = from_src[curr_index][i] + to_sink[curr_index][n - i];
            v1.push_back(src_node);
        }

        length_i.push_back(v1);

        for (int i = 0; i <= n; i++)
        {
            sink_node = from_src[next_index][i] + to_sink[next_index][n - i];
            v2.push_back(sink_node);
        }

        length_i.push_back(v2);
    }
    else
    {
        for (int i = 0; i <= n; i++)
        {
            src_node = from_src[curr_index][i] + to_sink[next_index][n - i];
            v1.push_back(src_node);
        }

        length_i.push_back(v1);

        for (int i = 0; i <= n; i++)
        {
            sink_node = from_src[next_index][i] + to_sink[curr_index][n - i];
            v2.push_back(sink_node);
        }

        length_i.push_back(v2);
    }
    return;
}

void display(vector<vector<int>> &v, int curr, int next)
{

    for (int j = 0; j < v[0].size(); j++)
    {
        cout << " " << v[curr][j] << '\t' << v[next][j] << '\n';
    }
    cout << '\n';

    return;
}

vector<vector<int>> MiddleEdge(string &v, string &w, int middle_index, int &match, int &mu, int &indel)
{
    int n = v.size(), m = w.size(), match_score_src(0), match_score_sink(0), curr_index(0), middle_node_src(0), middle_node_sink(0);
    vector<vector<int>> from_src, to_sink, length_i;

    initialize_score(from_src, to_sink, n + 1, indel);

    for (int j = 1; j <= middle_index; j++)
    {
        curr_index = j % 2;
        from_src[curr_index][0] = -j * indel;
        to_sink[curr_index][0] = -j * indel;

        for (int i = 1; i <= n; i++)
        {
            match_score_src = v[i - 1] == w[j - 1] ? match : -mu;
            match_score_sink = v[n - i] == w[m - j] ? match : -mu;

            from_src[curr_index][i] = max(max(from_src[abs(curr_index - 1)][i] - indel, from_src[curr_index][i - 1] - indel),
                                          from_src[abs(curr_index - 1)][i - 1] + match_score_src);
            to_sink[curr_index][i] = max(max(to_sink[abs(curr_index - 1)][i] - indel, to_sink[curr_index][i - 1] - indel),
                                         to_sink[abs(curr_index - 1)][i - 1] + match_score_sink);
        }
    }

    int next_index = abs(curr_index - 1);

    // Align the values of distances accordingly to parity of input string
    if (m % 2 == 0)
    {
        from_src[next_index][0] = -(middle_index + 1) * indel;
        for (int i = 1; i <= n; i++)
        {
            match_score_src = v[i - 1] == w[middle_index] ? match : -mu;
            from_src[next_index][i] = max(max(from_src[curr_index][i] - indel, from_src[next_index][i - 1] - indel),
                                          from_src[curr_index][i - 1] + match_score_src);
        }
    }
    else
    {
        from_src[next_index][0] = -(middle_index + 1) * indel;
        to_sink[next_index][0] = -(middle_index + 1) * indel;
        for (int i = 1; i <= n; i++)
        {
            match_score_src = v[i - 1] == w[middle_index] ? match : -mu;
            match_score_sink = v[n - i] == w[middle_index] ? match : -mu;

            from_src[next_index][i] = max(max(from_src[curr_index][i] - indel, from_src[next_index][i - 1] - indel),
                                          from_src[curr_index][i - 1] + match_score_src);
            to_sink[next_index][i] = max(max(to_sink[curr_index][i] - indel, to_sink[next_index][i - 1] - indel),
                                         to_sink[curr_index][i - 1] + match_score_sink);
        }
    }

    build_length_i(from_src, to_sink, curr_index, next_index, n, m, length_i);

    // display(from_src, curr_index, next_index);
    // display(to_sink, curr_index, next_index);
    // display(length_i, 0, 1);

    middle_node_src = max_element(length_i[0].begin(), length_i[0].end()) - length_i[0].begin();
    middle_node_sink = max_element(length_i[1].begin(), length_i[1].end()) - length_i[1].begin();

    if (middle_node_src == middle_node_sink)
    {
        // cout << "Horz" << '\n';
        return {{middle_node_src, middle_index}, {middle_node_src, middle_index + 1}};
    }
    else if (middle_node_src + 1 == middle_node_sink)
    {
        // cout << "Diag" << '\n';
        return {{middle_node_src, middle_index}, {middle_node_sink, middle_index + 1}};
    }
    else
    {
        // cout << "Vertical" << '\n';
        return {{middle_node_src, middle_index}, {middle_node_src + 1, middle_index}};
    }
}