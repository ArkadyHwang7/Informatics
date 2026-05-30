#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool canUseColor(int vertex, int color, const vector<string>& graph, const vector<int>& colors) {
    for (int i = 0; i < (int)graph.size(); i++) {
        if (graph[vertex][i] == '1' && colors[i] == color) {
            return false;
        }
    }

    return true;
}

bool colorGraph(int vertex, int n, int k, const vector<string>& graph, vector<int>& colors) {
    if (vertex == n) return true;

    for (int color = 1; color <= k; color++) {
        if (canUseColor(vertex, color, graph, colors)) {
            colors[vertex] = color;

            if (colorGraph(vertex + 1, n, k, graph, colors)) {
                return true;
            }

            colors[vertex] = 0;
        }
    }

    return false;
}

int main() {
    int n, k;
    cin >> n >> k;

    vector<string> graph(n);
    for (int i = 0; i < n; i++) {
        cin >> graph[i];
    }

    vector<int> colors(n, 0);

    if (colorGraph(0, n, k, graph, colors)) {
        cout << "YES" << endl;
        for (int i = 0; i < n; i++) {
            cout << colors[i] << " ";
        }
        cout << endl;
    }
    else {
        cout << "NO" << endl;
    }

    return 0;
}

