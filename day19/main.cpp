#ifndef _GLIBCXX_REGEX_STATE_LIMIT
#define _GLIBCXX_REGEX_STATE_LIMIT 10000000
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <regex>
#include <utility>
#include <tuple>
#include <map>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

using namespace std;

vector<string> read_ip(string s)
{
  vector<string> lines;
  string line;
  ifstream ip(s);
  while(getline(ip,line))
    lines.push_back(line);
  return lines;
}

// trim
string &trim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(),
            not1(ptr_fun<int, int>(isspace))));
    s.erase(find_if(s.rbegin(), s.rend(),
            not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}


pair<map<size_t,string>,vector<string>> parse_ip(string s) {
    const auto ip = read_ip(s);
    bool is_message = false;

    pair<map<size_t,string>,vector<string>> result;

    for(auto line : ip) {
        if(line == "") {
            is_message = true;
        } else if(is_message) {
            result.second.push_back(line);
        } else {
            stringstream ss(line);
            string key,rule;
            getline(ss,key,':');
            getline(ss,rule,':');
            result.first[stoul(trim(key))] = trim(rule);
        }
    }
    return result;
}


struct GetRegex {
    map<size_t,string> cache;
    map<size_t,string> rules;
    GetRegex(map<size_t,string> r) : rules(r) {}
    string get(size_t i) {
        string regex_rule;
        auto iter = cache.find(i);
        if(iter != cache.end()) {
            return iter->second;
        }
        else {
            string rule = rules[i];
            if(rule[0] == '\"' ) {
                regex_rule = rule.substr(1,1);
                //cout << " quote! " << regex_rule << endl;
            } else {
                stringstream ss(rule);
                regex_rule = "(";
                string word;
                while(getline(ss,word,' ')) {
                    if(word == "|") {
                        regex_rule.push_back('|');
                    } else {
                        regex_rule += get(stoul(word));
                    }
                }
                regex_rule.push_back(')');
            }

        }
        //cout << "new rule! " << i << " : " << regex_rule << endl;
        cache[i] = regex_rule;
        return regex_rule;
    } 
};

void part1(string s) {
    auto [rules,messages] = parse_ip(s);
    for(auto [i,rule] : rules) {
        //cout << rule << endl;
    }

    GetRegex get_regex(rules); 

    cout << get_regex.get(0) << endl;
    //cout << get_regex.get(0) << endl;

    regex rx(get_regex.get(0));
    //regex rx("a((aa|bb)(ab|ba)|(ab|ba)(aa|bb))b");

    size_t count = 0;
    for(string m : messages) {
        if(regex_match(m,rx)) count ++;
        //cout << m << "   " << regex_match(m,rx) << endl;
    }
    cout << "aoc 2020 part 1 file " << s << " answer = " << count << endl;


}


void part2(string s) {
    auto [rules,messages] = parse_ip(s);
    size_t n = rules.rbegin()->first + 1;
    rules[8] = "42 | 42 8";
    rules[11] = "42 31 | 42 11 31";
    rules[n] = "\"X\"";
    rules[n+1] = "\"Y\"";
    rules[8] = regex_replace(rules[8],regex("8"),to_string(n));
    rules[11] = regex_replace(rules[11],regex("11"),to_string(n+1));


    for(auto [i,r] : rules) {
        //cout << i << " : " << r << endl;
    }

    GetRegex get_regex(rules);

    get_regex.get(0);
    for(auto [i,rule] : rules) {
        //string r = get_regex.get(i);
        //if(r.find('X') != string::npos || r.find('Y') != string::npos) {
          //  cout << i << " : " << r << endl;
        //}
    }


    for(int i=0;i<5;i++) {
        string & s8 = get_regex.cache[8];
        string & s11 = get_regex.cache[11];
        s8 = regex_replace(s8, regex("X"), s8); 
        s11 = regex_replace(s11, regex("Y"), s11); 
    }

    get_regex.cache.erase(0);
    get_regex.get(0);



    //cout << get_regex.get(0) << endl;
    
    //cout << get_regex.get(0) << endl;

    regex rx(get_regex.get(0));
    size_t count = 0;
    for(string m : messages) {
        if(regex_match(m,rx)) {
            count ++;
            cout << m << "   " << regex_match(m,rx) << endl;
        }
    }
    cout << "aoc 2020 part 2 file " << s << " answer = " << count << endl;


}

int main() {
    //part1("test_input");
    //part1("input");
    part2("test_input2");
    part2("input");
    return 0;
}
