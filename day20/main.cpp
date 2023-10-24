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
#include "n_array.h"

using namespace std;
using namespace n_array;

template<class T>
auto rot90(const T in) {
    auto out = in;
    size_t N = in.size();
    for(size_t i = 0; i < N; i++)
        for(size_t j = 0; j < N; j++)
            out[i][j] = in[N - 1 - j][i];
    return out;
}

template<class T>
auto flip(const T in) {
    auto out = in;
    size_t N = in.size();
    for(size_t i = 0; i < N; i++)
        for(size_t j = 0; j < N; j++)
            out[i][N - 1 - j] = in[i][j];
    return out;
}

struct Pic {
    array<array<char,8>,8> p;
    array<array<char,10>,4> edges;
    Pic() {}
    Pic(const array<array<char,10>,10> & ip) {
        for(size_t i = 0; i < 10; i++)
        {
            top()[i]   = ip[0][i];
            bttm()[i]  = ip[9][i];
            left()[i]  = ip[i][0];
            right()[i] = ip[i][9];
        }
        for(size_t i = 0; i < 8; i++)
            for(size_t j = 0; j < 8; j++)
                p[i][j] = ip[i+1][j+1];
    }
    array<char,10> & top() {return edges[0];}
    array<char,10> & bttm() {return edges[1];}
    array<char,10> & left() {return edges[2];}
    array<char,10> & right() {return edges[3];}
    const array<char,10> & top() const {return edges[0];}
    const array<char,10> & bttm() const {return edges[1];}
    const array<char,10> & left() const {return edges[2];}
    const array<char,10> & right() const {return edges[3];}
};

auto rot90(const Pic & in) {
    auto out = in;
    out.p = rot90<decltype(in.p)>(in.p);
    for(size_t i = 0; i < 10; i++) {
        out.right()[i] = in.top()[i];
        out.bttm()[i]  = in.right()[9 - i];
        out.left()[i]  = in.bttm()[i];
        out.top()[i]   = in.left()[9 - i];
    }
    return out;
}

auto flip(const Pic & in) {
    auto out = in;
    out.p = flip<decltype(in.p)>(in.p);
    for(size_t i = 0; i < 10; i++) {
        out.top()[i]  = in.top()[9 - i];
        out.bttm()[i] = in.bttm()[9 - i];
        out.left() = in.right();
        out.right() = in.left();
    }
    return out;
}

template<class T>
auto orient(T pic, bool is_flip, size_t nrot) {
    if(is_flip) pic = flip(pic);
    for(size_t i = 0; i < nrot; i++) 
        pic = rot90(pic);
    return pic;
}

vector<string> read_ip(string s){
  vector<string> lines;
  string line;
  ifstream ip(s);
  while(getline(ip,line))
    lines.push_back(line);
  return lines;
}

map<size_t,Pic> parse_ip(string fn) {
    map<size_t,Pic> result;
    array<array<char,10>,10> pic;
    auto ip = read_ip(fn);
    size_t irow = 0;
    size_t id = 0;
    for(auto s : ip) {
        if(s[0] == 'T') {
            irow = 0;
            id = stoul(s.substr(5,4));
        } else if(!s.empty()) {
            for(size_t icol = 0; icol < 10; icol++) {
                pic[irow][icol] = s[icol];
            }
            irow++;
            if(irow == 10) result[id] = pic;
        }
    }
    return result;
}

void add_to_edge_counter(map<array<char,10>,size_t> & counter, const Pic & pic) {
    for(auto e1 : pic.edges) {
        auto e2 = e1;
        reverse(e2.begin(),e2.end());
        for(const auto & e : {e1,e2}) {
            if(auto it = counter.find(e); it == counter.end()) {
                counter[e] = 1;
            } else {
                ++(it->second);
            }
        }
    }
}

void print(const array<char,10> & l) {
    for(char c : l) cout << c; 
    cout << " ";
}

size_t num_outer_edges(const Pic & pic, const map<array<char,10>,size_t> & edge_counter) {
    size_t n = 0;
    for(auto e : pic.edges) {
        if(edge_counter.at(e) == 1) n++;
    }
    return n;
}

const set<pair<bool,size_t>> trans = {{true,0u},{true,1u},{true,2u},{true,3u},{false,0u},{false,1u},{false,2u},{false,3u}}; 

void populate_subsection(vector<string> & big, array<array<char,8>,8> small, size_t x, size_t y) {
    for(size_t i = 0; i < 8; i++) {
        for(size_t j = 0; j < 8; j++) {
            big[x * 8 + i][y * 8 + j] = small[i][j];
        }
    }
}

const vector<string> sdragon = {
"                  # ",
"#    ##    ##    ###",
" #  #  #  #  #  #   ",
};

auto get_dragon_pts() {
    set<pair<size_t,size_t>> result;
    for(size_t i = 0; i < sdragon.size(); i++) {
        for(size_t j = 0; j < sdragon[i].size(); j++) {
            if(sdragon[i][j] == '#') 
             {
                result.insert({i,j});
                cout << i << "," << j << endl;
             }
        }
    }
    return result;
}

const auto dragon = get_dragon_pts();

size_t look_for_dragons(vector<string> pic) {
    const size_t ni_dragon = sdragon.size(); 
    const size_t nj_dragon = sdragon[0].size(); 
    const size_t ni = pic.size();
    const size_t nj = pic[0].size();

    for(size_t i = 0; i < ni - ni_dragon + 1; i++) {
        for(size_t j = 0; j < nj - nj_dragon + 1; j++) {
            bool found = true;
            for(auto [di,dj] : dragon) {
                size_t ii = i + di;
                size_t jj = j + dj;
                if(!(pic[ii][jj] == '#' || pic[ii][jj] == 'O')) {
                    found = false;
                    break;
                }
            }
            if(found) {
                for(auto [di,dj] : dragon) {
                    size_t ii = i + di;
                    size_t jj = j + dj;
                    pic[ii][jj] = 'O';
                }
            }
        }
    }

    size_t nsea = 0;
    bool hic_sunt_dracones = false;
    for(const string & s : pic) {
        for(char c : s) {
            if(c == '#') {
                nsea++;
            } else if(c == 'O') {
                hic_sunt_dracones = true;
            }
        }
    }

    if(hic_sunt_dracones) {
        cout << "HIC DUNT DRACONES!\n";
        for(const string & s : pic) cout << s << endl;
    }

    return nsea;
}

void day20(string fn) {
    auto ip = parse_ip(fn);
    map<array<char,10>,size_t> edge_counter;
    set<size_t> ids;
    for(const auto & [id, pic] : ip) {
        add_to_edge_counter(edge_counter,pic);
        ids.insert(id);
    }

    set<size_t> corner_ids;
    set<size_t> edge_ids;
    set<size_t> inner_ids;
    for(const auto & [id, pic] : ip) {
        size_t n_outer = num_outer_edges(pic,edge_counter);
        if(n_outer == 2) corner_ids.insert(id);
        else if (n_outer == 1) edge_ids.insert(id);
        else if (n_outer == 0) inner_ids.insert(id);
        else cout << "ERROR - some pics seem to have more than two outer edges!" << endl;
    }
    size_t part1 = 1;
    for(auto id : corner_ids) part1 *= id;
    cout << "aoc2020 day 20 part1 for file " << fn << " answer = " << part1 << endl;

    size_t n;
    for(n = 1; n < 1000; n++) if(n * n == ip.size()) break;
    map<pair<size_t,size_t>,Pic> pic_layout;
    vector<string> big_pic(n*8,string(n*8,'.'));

    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < n; j++) {
            bool found = false;
            if(i == 0 && j == 0) {
                size_t id0 = *corner_ids.begin();
                ids.erase(id0);
                cout << i << " " << j << " " << id0 << endl;
                //orient top left corner
                for(auto [b,n] : trans) {
                    Pic pic = orient<Pic>(ip.at(id0),b,n);
                    if(edge_counter.at(pic.top()) == 1 && edge_counter.at(pic.left()) == 1) {
                        pic_layout[{0u,0u}] = pic;
                        populate_subsection(big_pic,pic.p,i,j);
                        found = true;
                        break;
                    }
                }
            } else if (i == 0) {
                //top row based on left-hand existing neighbour
                const Pic & nbr = pic_layout.at({i,j-1});
                for(auto id : ids) {
                    for(auto [b,n] : trans) {
                        Pic pic = orient<Pic>(ip.at(id),b,n);
                        if(pic.left() == nbr.right()) {
                            pic_layout[{i,j}] = pic;
                            populate_subsection(big_pic,pic.p,i,j);
                            ids.erase(id);
                            found = true;
                            cout << i << " " << j << " " << id << endl;
                            break;
                        }
                    }
                    if(found) break;
                }
            } else {
                //all but top row based on above existing neighbour
                const Pic & nbr = pic_layout.at({i-1,j});
                for(auto id : ids) {
                    for(auto [b,n] : trans) {
                        Pic pic = orient<Pic>(ip.at(id),b,n);
                        if(pic.top() == nbr.bttm()) {
                            pic_layout[{i,j}] = pic;
                            populate_subsection(big_pic,pic.p,i,j);
                            ids.erase(id);
                            found = true;
                            cout << i << " " << j << " " << id << endl;
                            break;
                        }
                    }
                    if(found) break;
                }

            }
            if(!found) cout << "ERROR!\n";
        }
    }

    for(auto s : big_pic) cout << s << endl;

    size_t minsea = n*n*8*8;
    for(auto [b,n] : trans) {
        auto oriented_pic = orient(big_pic,b,n);
        size_t nsea = look_for_dragons(oriented_pic);
        minsea = min(minsea,nsea);
    }
    cout << "aoc2020 day 20 part2 for file " << fn << " answer = " << minsea << endl;
    cout << n << endl;


}

int main() {
    day20("test_input");
    day20("input");
    return 0;
}
