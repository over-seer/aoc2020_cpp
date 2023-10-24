#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <stdexcept>


using namespace std;

//    Set the value to itself multiplied by the subject number.
//    Set the value to the remainder after dividing the value by 20201227.
uint64_t operation(uint64_t subject_no, uint64_t val) {
    const uint64_t denom = 20201227;
    val *= subject_no;
    return val % denom;
}

uint64_t get_loop_size(uint64_t subject_no, uint64_t public_key) {
    uint64_t val = 1;
    uint64_t loop_size = 0;
    while(val != public_key) {
        val = operation(subject_no, val);
        loop_size++;
        //cout << loop_size << ":" << val << endl;
    }
    return loop_size;
}

uint64_t get_key(uint64_t subject_no, uint64_t loop_no) {
    uint64_t val = 1;
    for(size_t i = 0; i < loop_no; i++) {
        val = operation(subject_no,val);
    }
    return val;
}

void day25(uint64_t card_key, uint64_t door_key) {
    uint64_t card_loop_size = get_loop_size(7, card_key);
    uint64_t door_loop_size = get_loop_size(7, door_key);
    cout << card_loop_size << " " << door_loop_size << endl;
    cout << get_key(card_key,door_loop_size) << " " << get_key(door_key,card_loop_size) << endl;
}

void test() {
    uint64_t card_val = 1;
    uint64_t room_val = 1;

    for(size_t i = 0; i < 7; i++) {
        card_val = operation(7,card_val);
        cout << i + 1 << ":" << card_val << endl;
    }
    for(size_t i = 0; i < 11; i++) {
        room_val = operation(7,room_val);
        cout << i + 1 << ":" << room_val << endl;
    }
    // /cout << card_val << " " << room_val << endl;

}

int main() {
    //test();
    array<uint64_t,2> input = {14205034,18047856};
    array<uint64_t,2> test_input = {5764801,17807724};

    day25(test_input[0],test_input[1]);
    day25(input[0],input[1]);
    return 0;
}
