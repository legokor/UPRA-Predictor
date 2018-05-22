#pragma once
#include <map>
#include <vector>
#include "vecs.h"

template <typename K, typename V>
V interpolate(const std::pair<K,V>& v1, const std::pair<K,V>& v2, K k) {
    K dk = v2.first - v1.first;
    return v1.second * (double)((k - v1.first)/dk) + v2.second * (double)((v2.first - k)/dk);
}

template <typename K, typename V>
class Dataset {
    std::vector<std::pair<K,V>> data;

public:
    Dataset() {
        data = std::vector<std::pair<K,V>>();
    }

    void addDataPoint(K k, V v) {
        data.push_back(std::make_pair(k, v));
    }


    K getLastKey() {
        return data.back().first;
    }

    V getLast() {
        return data.back().second;
    }

    // TODO: Check if getting data from outside dataset bounds returns nonsense or not
    V getAt(K k) {
        if (k < data.front().first)
            return interpolate(data[0], data[1], k);
        for (size_t i = 0; i < data.size()-1; i++) {
            if (data[i].first < k && k < data[i+1].first)
                return interpolate(data[i], data[i+1], k);
        }
        return interpolate(data[data.size()-2], data[data.size()-1], k);
    }
};
