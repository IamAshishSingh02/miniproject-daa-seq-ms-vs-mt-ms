#include "../include/mergesort.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
int main(){for(auto v:{std::vector<int>{},std::vector<int>{1},std::vector<int>{3,1,2},std::vector<int>{1,1,-2,4,0},std::vector<int>{5,4,3,2,1}}){auto e=v;mergesort::sequential(e);assert(std::is_sorted(e.begin(),e.end()));auto p=v;mergesort::parallel(p,4);assert(e==p);}std::cout<<"C++ merge sort tests passed\n";}
