#include <bits/stdc++.h>

using namespace std;

int NEGATIVE_INF = numeric_limits<int>::min();

void populate_graph(unordered_map<int, vector<pair<int, int>>> &graph, string file_name, int &s, int &t);
vector<int> top_sort(unordered_map<int, vector<pair<int, int>>> &graph, int &s);
unordered_map<int, int> DP_top_order(unordered_map<int, vector<pair<int, int>>> &graph, vector<int> &top_order, unordered_map<int, int> &prev);
void backtrack(vector<int> &path, int curr, int src, unordered_map<int, int> &prev);

int main(int argc, char const *argv[])
{
    string file_name = argv[1]; // dataset_245_7.txt, graph.txt;
    int s, t;
    unordered_map<int, vector<pair<int, int>>> graph;
    vector<int> top_order, path;
    unordered_map<int, int> prev;

    populate_graph(graph, file_name, s, t);

    top_order = top_sort(graph, s);

    unordered_map<int, int> distance = DP_top_order(graph, top_order, prev);

    backtrack(path, t, s, prev);

    cout << distance[t] << '\n';

    for (int i = path.size() - 1; i >= 0; i--)
    {
        cout << path[i] << ' ';
    }
    cout << '\n';

    return 0;
}

void display(unordered_map<int, vector<pair<int, int>>> &graph)
{
    int i = 1;
    for (auto elem : graph)
    {
        cout << i++ << "\t" << elem.first << "->" << '\t';

        for (auto tmp : elem.second)
        {
            cout << "(" << tmp.first << ", " << tmp.second << ")" << ' ';
        }

        cout << '\n';
    }

    return;
}

void populate_graph(unordered_map<int, vector<pair<int, int>>> &graph, string file_name, int &s, int &t)
{
    int node_s, node_t, weight;
    ifstream my_file(file_name);
    assert(my_file.is_open());
    my_file >> s >> t;
    vector<pair<int, int>> v1, v2;
    graph.insert({s, v1});
    graph.insert({t, v2});

    // cout << "Source: " << s << " Target: " << t << '\n';
    //! "my_file" should not end with '\n'
    while (!my_file.eof())
    {
        my_file >> node_s >> node_t >> weight;
        auto it = graph.find(node_s);
        if (it == graph.end())
        {
            vector<pair<int, int>> v;
            graph.insert({node_s, v});
        }
        graph[node_s].push_back({node_t, weight});
    }

    // display(graph);

    return;
}

void dfs(unordered_map<int, vector<pair<int, int>>> &graph, vector<int> &top_order, int at, unordered_map<int, bool> &visited)
{
    visited[at] = true;
    for (auto elem : graph[at])
    {
        int node = elem.first;
        if (!visited[node])
        {
            dfs(graph, top_order, node, visited);
        }
    }

    top_order.push_back(at);

    return;
}

vector<int> top_sort(unordered_map<int, vector<pair<int, int>>> &graph, int &s)
{
    vector<int> top_order;

    unordered_map<int, bool> visited;
    for (auto elem : graph)
    {
        visited[elem.first] = false;
    }

    dfs(graph, top_order, s, visited);
    reverse(top_order.begin(), top_order.end());

    return top_order;
}

unordered_map<int, int> DP_top_order(unordered_map<int, vector<pair<int, int>>> &graph, vector<int> &top_order, unordered_map<int, int> &prev)
{
    unordered_map<int, int> distance;

    for (auto node : top_order)
    {
        distance[node] = NEGATIVE_INF;
    }
    int n = top_order.size(), src = top_order[0];
    distance[src] = 0;

    for (int i = 1; i < n; i++)
    {
        int node = top_order[i];
        vector<pair<int, int>> pred;
        for (int j = 0; j < i; j++)
        {
            int tmp_node = top_order[j];
            for (auto neighbors : graph[tmp_node])
            {
                if (neighbors.first == node)
                {
                    int w = neighbors.second;
                    pred.push_back({tmp_node, w});
                }
            }
        }

        for (auto pred_node : pred)
        {
            if (distance[pred_node.first] + pred_node.second > distance[node])
            {
                distance[node] = distance[pred_node.first] + pred_node.second;
                prev[node] = pred_node.first;
            }
        }
    }

    return distance;
}

void backtrack(vector<int> &path, int curr, int src, unordered_map<int, int> &prev)
{
    do
    {
        path.push_back(curr);
        curr = prev[curr];
    } while (curr != src);
    path.push_back(src);

    return;
}