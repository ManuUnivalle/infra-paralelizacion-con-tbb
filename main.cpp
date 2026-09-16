#include <iostream>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <vector>
#include <chrono>

using namespace std::chrono;
using namespace std;
const int VECTOR_SIZE = 10000000;
const long VALUE = 2;

vector<long> v(VECTOR_SIZE);
vector<long> u(VECTOR_SIZE);
vector<long> w(VECTOR_SIZE);

void fillVector(vector<long>& v, long value) {
    tbb::parallel_for(
    tbb::blocked_range<size_t>(0, VECTOR_SIZE),
    [&](const tbb::blocked_range<size_t>& r) {
        for (size_t i = r.begin(); i != r.end(); ++i) {
            v[i] = value;
        }
    }
    );
}

long suma_total(vector<long>& w) {
    long resultado = tbb::parallel_reduce(
    tbb::blocked_range<size_t>(0, VECTOR_SIZE),
    0L,
    [&](const tbb::blocked_range<size_t>& r, long parcial) {
        for (size_t i = r.begin(); i != r.end(); ++i) {
            parcial += w[i];
        }
        return parcial;
    },
    [](long x, long y) -> long {
            return x + y;
        }
    );
    
    return resultado;
}


void producto(vector<long>& v, vector<long>& u, vector<long>& w) {
    tbb::parallel_for(
    tbb::blocked_range<size_t>(0, VECTOR_SIZE),
    [&](const tbb::blocked_range<size_t>& r) {
        for (size_t i = r.begin(); i != r.end(); ++i) {
            w[i] = v[i] * u[i];
        }
    }
    );
}

int main() { 

    fillVector(v, VALUE);
    fillVector(u, VALUE);

    long result = 0;
    auto start = high_resolution_clock::now();
    producto(v, u, w);
    result = suma_total(w);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    printf("Suma: %ld\n", result);
    printf("Tiempo de ejecucion: %lld ms\n", duration.count());
    
    return 0; 
}
