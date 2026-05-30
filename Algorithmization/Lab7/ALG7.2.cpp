#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct GoldPile {
    long long cost;
    long long weight;
};

long long getMaxGoldCost(long long m, vector<GoldPile>& piles) {
    sort(piles.begin(), piles.end(), [](GoldPile a, GoldPile b) {
        return a.cost > b.cost;
        });

    long long result = 0;
    for (int i = 0; i < (int)piles.size(); i++) {
        if (m == 0) break;

        long long take = min(m, piles[i].weight);
        result += take * piles[i].cost;
        m -= take;
    }

    return result;
}

int main() {
    long long m;
    int n;
    cin >> m;
    cin >> n;

    vector<GoldPile> piles(n);
    for (int i = 0; i < n; i++) {
        cin >> piles[i].cost >> piles[i].weight;
    }

    cout << getMaxGoldCost(m, piles) << endl;
    return 0;
}
