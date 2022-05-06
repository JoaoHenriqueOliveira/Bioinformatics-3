#include <bits/stdc++.h>
using namespace std;

int ManhattanTouristProblem(int n, int m, vector<vector<int>> &Down, vector<vector<int>> &Right);
void display_matrix(vector<vector<int>> &m);
void populate(string name, int &n, int &m, vector<vector<int>> &Down, vector<vector<int>> &Right);

int main(int argc, char const *argv[])
{
    string file_name = argv[1]; // "dataset_261_10.txt"
    int n, m;
    vector<vector<int>> Down, Right;
    populate(file_name, n, m, Down, Right);

    int result = ManhattanTouristProblem(n, m, Down, Right);
    cout << "Result: " << result << "\n";

    return 0;
}

void populate(string name, int &n, int &m, vector<vector<int>> &Down, vector<vector<int>> &Right)
{
    ifstream my_file(name);
    assert(my_file.is_open());
    my_file >> n;
    my_file >> m;

    for (int i = 0; i < n; i++)
    {
        int tmp;
        vector<int> v;
        Down.push_back(v);
        for (int j = 0; j < m + 1; j++)
        {
            my_file >> tmp;
            Down[i].push_back(tmp);
        }
    }

    char aux;
    my_file >> aux;

    for (int i = 0; i < n + 1; i++)
    {
        int tmp;
        vector<int> v;
        Right.push_back(v);
        for (int j = 0; j < m; j++)
        {
            my_file >> tmp;
            Right[i].push_back(tmp);
        }
    }

    return;
}

void initialize_matrix(int row, int col, vector<vector<int>> &m)
{
    for (int i = 0; i < row + 1; i++)
    {
        vector<int> v(col + 1);
        m.push_back(v);
    }
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

int ManhattanTouristProblem(int n, int m, vector<vector<int>> &Down, vector<vector<int>> &Right)
{
    vector<vector<int>> result;
    initialize_matrix(n, m, result);

    for (int i = 1; i < n + 1; i++) // initialize 1st col
    {
        result[i][0] = Down[i - 1][0] + result[i - 1][0];
    }

    for (int j = 1; j < m + 1; j++) // initialize 1st row
    {
        result[0][j] = Right[0][j - 1] + result[0][j - 1];
    }

    for (int row = 1; row < n + 1; row++)
    {
        for (int col = 1; col < m + 1; col++)
        {
            result[row][col] = max(result[row][col - 1] + Right[row][col - 1], result[row - 1][col] + Down[row - 1][col]);
        }
    }
    display_matrix(result);

    return result[n][m];
}