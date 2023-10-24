#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <set>

using namespace std;

vector<string> read_ip(string s) {
  vector<string> lines;
  string line;
  ifstream ip(s);
  while(getline(ip,line))
    lines.push_back(line);
  return lines;
}


string str_instructions(const vector<array<int,2>> & in) {
    string sin;
    for(auto [i,j] : in) {
        if(j == 0)       sin += (i == 1 ? "e" : "w");
        else if(j == -1) sin += (i == 1 ? "ne" : "nw");
        else if(j == 1)  sin += (i == 0 ? "se" : "sw");
    }
    return sin;;
}

void print_instructions(const vector<array<int,2>> & in) {
    auto sin = str_instructions(in);
    for(auto s : sin) cout << s;
    cout << endl;
}

vector<vector<array<int,2>>> parse(string fn) {
    auto ip = read_ip(fn);
    vector<vector<array<int,2>>> result;
    for(string line : ip) {
        vector<array<int,2>> instructions;
        while(!line.empty()) {
            if(line.starts_with("ne")) {
                instructions.push_back({1,-1});
                line = line.substr(2);
            }
            else if(line.starts_with("nw")) {
                instructions.push_back({0,-1});
                line = line.substr(2);
            }
            else if(line.starts_with("se")) {
                instructions.push_back({0,1});
                line = line.substr(2);
            }
            else if(line.starts_with("sw")) {
                instructions.push_back({-1,1});
                line = line.substr(2);
            }
            else if(line.starts_with("e")) {
                instructions.push_back({1,0});
                line = line.substr(1);
            }
            else if(line.starts_with("w")) {
                instructions.push_back({-1,0});
                line = line.substr(1);
            }
        }
        result.push_back(instructions);
    }
    for(size_t i = 0; i < result.size(); i++) {
        if(ip[i] != str_instructions(result[i])) {
            cout << "ERROR!!!!";
            exit(1);
        }
    }
    return result;
} 

set<array<int,2>> part1(string fn) {
    auto ip = parse(fn);
    //for(auto line : ip) print_instructions(line);
    set<array<int,2>> blacks;
    for(auto line : ip) {
        array<int,2> ij = {0,0};
        int & i = ij[0];
        int & j = ij[1];
        for(auto [di,dj] : line) {
            i += di;
            j += dj;
        }
        auto it = blacks.find(ij);
        if(it != blacks.end()) blacks.erase(it);
        else blacks.insert(it,ij);
        //if(blacks.contains(ij)) blacks.erase(ij);
        //else blacks.insert(ij);
    }
    cout << "aoc 2020 day 24 part 1 file " << fn << " ans = " << blacks.size() << endl;
    return blacks;
}

array<array<int,2>,6> neighbours(const array<int,2> & ij) {
    array<array<int,2>,6>  ds{{{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0}}};
    auto nbrs = ds;
    for(auto & d : nbrs) {
        d[0] += ij[0];
        d[1] += ij[1];
    }
    return nbrs;
}

bool still_black(set<array<int,2>> blacks, const array<int,2> & ij0) {
    size_t count = 0;
    for(auto ij : neighbours(ij0)) {
        if(blacks.contains(ij)) count++;
        if(count > 2) break;
    }
    return count == 0 || count > 2;
}

bool becomes_black(set<array<int,2>> blacks, const array<int,2> & known_black_ij, const array<int,2> & ij0) {
    size_t count = 1;
    for(auto ij : neighbours(ij0)) {
        if(ij != known_black_ij) {
            if(blacks.contains(ij)) count++;
            if(count > 2) break;
        }
    }
    return count == 2;
}

auto process(set<array<int,2>> blacks) {
    set<array<int,2>> new_blacks;
    for(auto ij : blacks) {
        if(still_black(blacks,ij)) {
            new_blacks.insert(ij);
        }
        for(auto ij_prime : neighbours(ij)) {
            if(!blacks.contains(ij_prime)) {
                if(becomes_black(blacks,ij,ij_prime)) {
                    new_blacks.insert(ij_prime);
                }
            }
        }
    }
    return new_blacks;  
}

void part2(string fn) {
    auto blacks = part1(fn);
    
    for(size_t i = 0; i < 100; i++) {
        blacks = process(blacks);
        //cout << i+1 << " : " << blacks.size() << endl;
    }
    cout << "aoc 2020 day 24 part 2 file " << fn << " ans = " << blacks.size() << endl;

}

int main() {
    part2("test_input");
    part2("input");
    return 0;
}
