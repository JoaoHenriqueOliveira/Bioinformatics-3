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
    int match, mu, sigma, middle_index; // match reward, mismatch penalty, indel penalty

    populate(file_name, v, w, match, mu, sigma);

    middle_index = w.size() / 2; // w.size() % 2 == 0 ? w.size() / 2 : 1 + w.size() / 2;

    auto edges = MiddleEdge(v, w, middle_index, match, mu, sigma);

    cout << edges[0][0] << " " << edges[0][1] << '\n'
         << edges[1][0] << ' ' << edges[1][1] << '\n';

    return 0;
}

void initialize_score(vector<vector<int>> &score, int n, int indel)
{
    vector<int> v1(n, 0), v2(n, 0);
    score.push_back(v1);
    score.push_back(v2);

    score[1][0] = -indel;

    for (int i = 1; i < n; i++)
    {
        score[0][i] -= i * indel;
    }

    return;
}

vector<vector<int>> MiddleEdge(string &v, string &w, int middle_index, int &match, int &mu, int &indel)
{
    int n = v.size(), m = w.size(), match_score(0), curr_index(0), max_index(0);
    vector<vector<int>> score;

    initialize_score(score, n + 1, indel);

    for (int j = 1; j <= middle_index; j++)
    {
        curr_index = j % 2;
        score[curr_index][0] = -j * indel;

        for (int i = 1; i <= n; i++)
        {
            match_score = v[i - 1] == w[j - 1] ? match : -mu;

            score[curr_index][i] = max(max(score[abs(curr_index - 1)][i] - indel, score[curr_index][i - 1] - indel),
                                       score[abs(curr_index - 1)][i - 1] + match_score);
        }
    }

    max_index = max_element(score[curr_index].begin(), score[curr_index].end()) - score[curr_index].begin(); // first max argval of vector

    int next_index = abs(curr_index - 1);

    score[next_index][0] = -(middle_index + 1) * indel;

    for (int i = 1; i <= n; i++)
    {
        match_score = v[i - 1] == w[middle_index] ? match : -mu;

        score[next_index][i] = max(max(score[curr_index][i] - indel, score[next_index][i - 1] - indel),
                                   score[curr_index][i - 1] + match_score);
    }

    int diag_score = score[next_index][max_index + 1], horz_score = score[next_index][max_index], vertical_score = score[curr_index][max_index + 1];

    if (vertical_score >= diag_score && vertical_score >= horz_score)
    {
        return {{max_index, middle_index}, {max_index + 1, middle_index}};
    }
    else if (horz_score >= diag_score && horz_score >= vertical_score)
    {
        return {{max_index, middle_index}, {max_index, middle_index + 1}};
    }
    else
    {
        return {{max_index, middle_index}, {max_index + 1, middle_index + 1}};
    }
}