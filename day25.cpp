
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

using Key = std::array<unsigned int, 5>;
using Lock = std::array<unsigned int, 5>;

std::pair<std::vector<Key>, std::vector<Lock>>
read_input(std::string file_location) {
  std::ifstream input_file;
  input_file.open(file_location);

  std::string line;
  // State: 0 = unknown, 1 Lock(down), -1 = Key(up)
  int count = 0, state = 0;

  Key key{};
  std::vector<Key> keys{};
  Lock lock{};
  std::vector<Lock> locks{};
  while (std::getline(input_file, line)) {
    if (line == "") {
      if (state == 1) {
        locks.push_back(lock);
      } else if (state == -1) {
        keys.push_back(key);
      }
      state = 0;

      lock = Lock{0, 0, 0, 0, 0};
      key = Key{0, 0, 0, 0, 0};
      continue;
    }

    if (state == 0) {
      switch (line.at(0)) {
      case '#':
        state = 1;
        count = 0;
        break;
      case '.':
        state = -1;
        count = 6;
        break;
      }
      // Lock
    } else if (state == 1) {
      ++count;
      for (int i = 0; i < 5; ++i) {
        if (line.at(i) == '#') {
          lock[i] = count;
        }
      }
    } else if (state == -1) {
      --count;
      for (int i = 0; i < 5; ++i) {
        if (line.at(i) == '#' && key[i] == 0) {
          key[i] = count;
        }
      }
    }
  }
  if (state == 1) {
    locks.push_back(lock);
  } else if (state == -1) {
    keys.push_back(key);
  }

  return {keys, locks};
}

std::ostream &operator<<(std::ostream &os, std::array<unsigned int, 5> arr) {
  os << "[";
  for (int i = 0; i < 4; ++i) {
    os << arr[i] << ",";
  }
  os << arr[4] << "]";
  return os;
}

void print_arr(const std::vector<std::array<unsigned int, 5>> arr, std::string prefix){
  std::cout << prefix << "\n";
  for (const auto &elem : arr) {
    std::cout << elem << "\n";
  }
}

bool combination_fits(const Key& key, const Lock& lock) {
    for(int i = 0; i < 5; ++i){
        if(key[i] + lock[i] > 5){
            return false;
        }
    }
    return true;
}

int get_unique_lock_key_pairs(const std::vector<Key>& keys, const std::vector<Lock>& locks) {
    int key_lock_pairs = 0;
    std::ranges::for_each(keys, [&](const auto& key){
        std::ranges::for_each(locks, [&](const auto& lock) {
            if(combination_fits(key, lock)) {
                key_lock_pairs++;
            }
        });
    });
    return key_lock_pairs;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  std::pair<std::vector<Lock>, std::vector<Key>> pair = read_input(argv[1]);
  std::vector<Key> keys = pair.first;
  std::vector<Lock> locks = pair.second;

  //print_arr(keys, "Keys:");
  //print_arr(locks, "Locks:");
  std::cout << locks.size() << " Locks!" << "\n";
  std::cout << keys.size() << " Keys!" << "\n";

  // ===== Task 1 =====
  int unique_key_lock_pairs = get_unique_lock_key_pairs(keys, locks);
  std::cout << "Unique key/lock pairs: " << unique_key_lock_pairs << "\n";
}