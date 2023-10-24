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

using namespace std;

vector<string> read_ip(string s) {
  vector<string> lines;
  string line;
  ifstream ip(s);
  while(getline(ip,line))
    lines.push_back(line);
  return lines;
}

auto parse(string fn) {
    pair<deque<int>,deque<int>> result;
    auto ip = read_ip(fn);
    size_t n = ip.size()/2;
    for(size_t i = 1; i < n; i++) {
        size_t j = i + n + 1;
        string s1 = ip[i];
        string s2 = ip[j];
        //cout << s1 << "," << s2 << endl;
        result.first.push_back(stoi(s1));
        result.second.push_back(stoi(s2));
    }
    return result;
}

auto round(pair<deque<int>,deque<int>> cards)
{
    auto & [deck1,deck2] = cards;
    int card1 = deck1.front();
    int card2 = deck2.front();
    deck1.pop_front(); 
    deck2.pop_front();
    if(card1 > card2) {
        deck1.push_back(card1);
        deck1.push_back(card2);
    } else {
        deck2.push_back(card2);
        deck2.push_back(card1);
    }
    return cards;
}

size_t score (deque<int> winning_deck) {
    reverse(winning_deck.begin(),winning_deck.end());
    //for(int i : winning_deck) cout << i << endl;
    size_t ans = 0;
    for(size_t i = 1; int icard : winning_deck) {
        ans += i * icard;
        //cout << icard << " * " << i << endl;
        i++;
    }
    return ans;
}

void part1(string fn) {
    auto ip = parse(fn);
    while(!ip.first.empty() && !ip.second.empty()) {
        ip = round(ip);
        //cout << ip.first.size() << "," << ip.second.size() << endl;
    }
    auto winning_deck = ip.first.empty() ? ip.second : ip.first;
    size_t ans = score(winning_deck);
    cout << "aoc 2020 day 22 part 1 file " << fn << " ans = " << ans << endl;
}

struct Game {
    pair<deque<int>,deque<int>> cards;
    set<pair<deque<int>,deque<int>>> record;
    Game(const pair<deque<int>,deque<int>> & ip) : cards(ip) {}//print();}
    bool is_end = false;
    int winner = 1;

    int play() {
        while(!is_end) {
            round();
        }
        return winner;
    }

    void round() {
        //print();
        //check for repeat
        if(!record.contains(cards)) {
            record.insert(cards);
        } else {
            is_end = true;
            //cout << "duplicate!\n";
            return;
        }
        auto & [deck1,deck2] = cards;
        int card1 = deck1.front();
        int card2 = deck2.front();
        deck1.pop_front(); 
        deck2.pop_front();

        int winner_of_round;
        if(deck1.size() >= card1 && deck2.size() >= card2) {
            deque<int> new1, new2;
            for(auto it = deck1.begin(); it < deck1.begin() + card1; ++it) new1.push_back(*it);
            for(auto it = deck2.begin(); it < deck2.begin() + card2; ++it) new2.push_back(*it);
            Game game({new1,new2});
            winner_of_round = game.play();
        } else {
            winner_of_round = card1 > card2 ? 1 : 2;
        }

        if(winner_of_round == 1) {
            deck1.push_back(card1);
            deck1.push_back(card2);
        } else {
            deck2.push_back(card2);
            deck2.push_back(card1);
        }

        if(deck1.empty() || deck2.empty()) {
            is_end = true;
            winner = deck1.empty() ? 2 : 1;
        }
        
        auto winning_deck = winner == 1 ? deck1 : deck2;
    }

    void print() {
        for(int i : cards.first) cout << i << ",";
        cout << " : ";
        for(int i : cards.second) cout << i << ",";
        cout << endl;
    }
};

void part2(string fn) {
    Game game(parse(fn));
    game.play();
    auto winning_deck = game.winner == 1 ? game.cards.first : game.cards.second;
    for(int i : winning_deck) cout << i << ","; cout << endl;
    size_t ans = score(winning_deck);
    cout << "aoc 2020 day 22 part 2 file " << fn << " ans = " << ans << endl;
}

int main() {
    part1("test_input");
    part1("input");
    part2("test_input");
    part2("input");
}