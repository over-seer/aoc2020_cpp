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

using namespace std;

vector<string> read_ip(string s) {
  vector<string> lines;
  string line;
  ifstream ip(s);
  while(getline(ip,line))
    lines.push_back(line);
  return lines;
}

set<string> get_words(string l) {
    set<string> sset;
    stringstream ss(l);
    string word;
    while(getline(ss,word,' ')) {
        if(word != "" && word != "contains") {
            if(word.back() == ',') word = word.substr(0, word.size() - 1);
            sset.insert(word);
        }
    }
    return sset;
}

auto parse_ip(string fn) {
    map<set<string>,set<string>> result;
    auto ip = read_ip(fn);
    for(string s : ip) {
        stringstream ss(s);
        string l, r, word;
        getline(ss,l,'(');
        getline(ss,r,')');
        result.insert({get_words(l),get_words(r)});
    }
    return result;
}

auto solve(const map<set<string>,set<string>> & ip) {
    map<string,string> i_by_a, a_by_i;
    map<string,set<string>> possible_i_by_a;
    set<string> ingredients, allergens;

    for(const auto & [iset,aset] : ip) {
        for(string i : iset) ingredients.insert(i);
        for(string a : aset) allergens.insert(a);
    }

    for(string a : allergens) possible_i_by_a.insert({a,ingredients});

    while(i_by_a.size() < allergens.size())
    {
        cout << "iteration" << endl;
        for(const auto & [iset,aset] : ip) {
            for(string a : aset) {
                //cout << a << endl;
                auto possible_i = possible_i_by_a.at(a);
                for(string i : possible_i) {
                    if(!iset.contains(i)) {
                        //cout << "erase " << i << endl;
                        possible_i_by_a.at(a).erase(i);
                        //cout << " done" << endl;
                    }
                }
                if(possible_i_by_a.at(a).size() == 1) {
                    //cout << "debug1" << endl;
                    string i = *(possible_i_by_a.at(a).begin());
                    //cout << i << endl;
                    i_by_a[a] = i;
                    a_by_i[i] = a;
                    for(auto & [aa,ii] : possible_i_by_a) ii.erase(i);
                }
            }        
        }
        //for(auto [s1,s2] : i_by_a) cout << s1 << " : " << s2 << endl;
    }

    for(string i : ingredients) {
        if(!a_by_i.contains(i)) {
            a_by_i[i] = "None";
            cout << i <<endl;
        }
    }

    return std::pair(a_by_i, i_by_a);
}

size_t count_ing(const map<set<string>,set<string>> & ip, string i) {
    size_t count = 0;
    for(auto [set1,set2] : ip) {
        if(set1.contains(i)) count++;
    }
    return count;
}

void day21(string fn) {
    auto ip = parse_ip(fn);
    auto [a_by_i, i_by_a] = solve(ip);
    size_t ans = 0;
    for(auto [i,a] : a_by_i) {
        if(a == "None") ans += count_ing(ip,i);
    }
    cout << "aoc 2020 day 21 file " << fn << " part 1 = " << ans << endl;
    cout << "part 2 :\n";
    for(auto [a,i] : i_by_a) {
        cout << i << ",";
    }
    cout << endl;
}

int main() {
    day21("test_input");
    day21("input");
}