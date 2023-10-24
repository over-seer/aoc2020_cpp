#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <numeric>
#include <deque>
#include <list>

using namespace std;

const string test_input = "389125467";
const string input = "871369452";

auto parse(string s) {
    vector<int> result;
    for(char c : s) {
        result.push_back(c - '0');
    }
    return result;
}

map<int,int> make_map(vector<int> v, int n) {
    for(int i : v) cout << i; cout << endl;
    map<int,int> cups;

    int nv = static_cast<int>(v.size());

    for(int i = 0; i < nv - 1; i++) {
        cups[v[i]] = v[i + 1];
    }
    if(n > nv) {
        cups[v.back()] = nv + 1;
    } else {
        cups[v.back()] = v.front();
    }
    for(int i = nv + 1; i <= n; i++) {
        cups[i] = i == n ? v.front() : i + 1;
    }

    //for(auto p : cups) cout << p.first << "->" << p.second << endl; 

    return cups;
}

void move_range(
    map<int,int> & cups, 
    int dest, 
    decltype(cups.begin()) it_current,
    decltype(cups.begin()) it0, 
    decltype(cups.begin()) it1
) {
    int after_the_range = it1->second;
    auto it = cups.find(dest);
    int dest2 = it->second;
    it->second = it0->first;
    it1->second = dest2;

    it_current->second = after_the_range;
}

auto get_next_three(map<int,int> & cups, decltype(cups.begin()) it_current) {
    int current = it_current->first;
    auto it1 = cups.find(it_current->second);
    auto it2 = cups.find(it1->second);
    auto it3 = cups.find(it2->second);
    return array<decltype(it1),3>({it1,it2,it3});
}

auto move_cups(map<int,int> & cups, decltype(cups.begin()) it_current) {
    int current = it_current->first;
    auto [it1,it2,it3] = get_next_three(cups,it_current);
    int i1 = it1->first;
    int i2 = it2->first;
    int i3 = it3->first;
    int dest = current == 1 ? cups.size() : current - 1;
    bool is_dest_ok = false;
    while(!is_dest_ok) {
        if(dest == i1 || dest == i2 || dest == i3) {
            dest--;
            if(dest == 0) {
                dest = cups.size();
            }
        } else {
            is_dest_ok = true;
        }
    }
    
    //cout << current << "," << i1 << i2 << i3 << " -> " << dest << "  ";
    move_range(cups,dest,it_current,it1,it3);
    return cups.find(it_current->second);
}

void print_cups(const map<int,int> & cups) {
    auto it = cups.find(1);
    for(int i = 0; i < 9; i++) {
        cout << it->first;
        it = cups.find(it->second);
    }
    cout << endl;
}

void part1(string sip) {
    auto vip = parse(sip);
    auto cups = make_map(vip,9);
    auto it = cups.find(vip.front());
    for(int i = 0; i < 100; i++) {  
        it = move_cups(cups,it);
    }

    print_cups(cups);

}

void part2(string sip, int ncups, int nmoves) {
    auto vip = parse(sip);
    auto cups = make_map(vip, ncups);

    //for(auto cup : cups) cout << cup.first << "->" << cup.second << endl;
    auto it = cups.find(vip.front());
    for(int i = 0; i < nmoves; i++) {
        // /print_cups(cups);
        it = move_cups(cups,it);
    }
    auto it_first = cups.find(1);
    auto [it1,it2,it3] = get_next_three(cups,it_first);
    int i1 = it1->first;
    int i2 = it2->first;
    int i3 = it3->first;

    //print_cups(cups);
    cout << "\n1, " << i1 << ", " << i2 << endl;
    uint64_t u1 = i1;
    uint64_t u2 = i2;
    uint64_t ans = u1*u2;

    cout << "\naoc 2020 day 23 part 2 for input " << sip << " ans = " << ans << endl;
}

int main() {
    part1(test_input);
    part1(input);
    part2(test_input,1000000,10000000);
    part2(input,1000000,10000000);
    return 0;
}