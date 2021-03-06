/* 
 * File:   KNearestKneighbours.h
 * Author: Bogdan Cojocar
 *
 */

#ifndef KNEARESTNEIGHBOURS_H
#define	KNEARESTNEIGHBOURS_H

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <functional>
#include <map>
#include <utility>
#include <vector>

template <typename T, unsigned int N = 2 >
class KNearestNeighbours {
public:

    enum Distance {
        EUCLIDEAN,
        CHEBYSHEV,
        MANHATTAN
    };
    typedef std::array<double, N> Array;
    typedef std::pair<Array, T> Feature;
    typedef std::vector<Feature> FeatureSet;
    typedef std::vector<std::pair<T, double >> Distances;

    KNearestNeighbours();
    ~KNearestNeighbours();

    void computeDistances(FeatureSet& featureSet, const Array& sample);
    T getBestClass();
    void setDistance(Distance distance);
    void SetNumberOfNeighbours(unsigned int number);

private:
    unsigned int numberOfNeighbours;
    Distance distance;
    Distances distances;

    double calculateDistance(const Array& a, const Array& b);
};

template <typename T, unsigned int N>
KNearestNeighbours<T, N>::KNearestNeighbours()
: distance(EUCLIDEAN),
numberOfNeighbours(10) {
}

template <typename T, unsigned int N>
KNearestNeighbours<T, N>::~KNearestNeighbours() {
}

template <typename T, unsigned int N>
void KNearestNeighbours<T, N>::setDistance(Distance dist) {
    distance = dist;
}

template <typename T, unsigned int N>
void KNearestNeighbours<T, N>::SetNumberOfNeighbours(unsigned int number) {
    numberOfNeighbours = number;
}

template <typename T, unsigned int N>
void KNearestNeighbours<T, N>::computeDistances(FeatureSet& featureSet,
        const Array& sample) {
    for (Feature& feature : featureSet) {
        distances.push_back(std::make_pair(feature.second, 
                calculateDistance(feature.first, sample)));
    }
}

template <typename T, unsigned int N>
T KNearestNeighbours<T, N>::getBestClass() {
    auto comp = [](const std::pair<T, double>& lhs, const std::pair<T, double>& rhs) {
                return lhs.second < rhs.second;
            };
    std::sort(distances.begin(), distances.end(), comp);
 
    // Count the number of occurrences of each class 
    std::map<T, unsigned int> countClass;
    for (auto i = 0; i < numberOfNeighbours; ++i) {
        countClass[distances[i].first]++;
    }
    std::pair<T, unsigned int> bestClass(T(), UINT_MAX);
    for (auto& distance : distances) {
        if (distance.second < bestClass.second) {
            bestClass = distance;
        }
    }
    return bestClass.first;
}

template <typename T, unsigned int N>
double KNearestNeighbours<T, N>::calculateDistance(const Array& a,
        const Array& b) {

    std::function<double(double, double) > distance_type_op;
    switch (distance) {
        case EUCLIDEAN:
            distance_type_op = [](double x, double y) {
                return pow(x - y, 2);
            };
            break;
        case CHEBYSHEV:
            distance_type_op = [](double x, double y) {
                return x - y;
            };
            break;
        case MANHATTAN:
            distance_type_op = [](double x, double y) {
                return abs(x - y);
            };
            break;
    }
    Array result;
    std::transform(a.begin(), a.end(), b.begin(), result.begin(), distance_type_op);
    return std::accumulate(result.begin(), result.end(), 0, std::plus<double>());
}

#endif	/* KNEARESTNEIGHBOURS_H */

