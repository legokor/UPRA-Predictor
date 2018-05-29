#pragma once
#include <algorithm>
#include <map>
#include <vector>

template <typename K, typename V>
V interpolate(const std::pair<K,V>& v1, const std::pair<K,V>& v2, K k) {
    K dk = v2.first - v1.first;
    return v1.second * (double)((k - v1.first)/dk) + v2.second * (double)((v2.first - k)/dk);
}

template <typename K, typename V>
class Dataset {
    std::vector<std::pair<K,V>> data;

public:

    void addDataPoint(K k, V v) {
        auto dpoint = std::make_pair(k,v);
        data.insert(
            std::upper_bound(data.begin(), data.end(), dpoint,
            [] (auto a, auto b) { return a.first < b.first; }),
            dpoint
        );
    }

    bool empty() { return data.empty(); }

    K getLastKey() {
        return data.back().first;
    }

    V getLast() {
        return data.back().second;
    }

    // TODO: Check if getting data from outside dataset bounds returns nonsense or not
    V getAt(K k) {
        //Key smaller than smallest in dataset -> extrapolate
        if (k < data.front().first)
            return interpolate(data[0], data[1], k);

        auto pos = std::upper_bound(data.begin(), data.end(), k, [] (K a, auto b) { return a < b.first; });
        if (pos < data.end())
            return interpolate(*(pos-1), *pos, k);

        //Key larger than largest in dataset -> extrapolate
        return interpolate(data[data.size()-2], data[data.size()-1], k);
    }
};
