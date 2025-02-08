#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>

struct User {
    long id;
    std::string name;
    std::vector<std::string> interests;

    User(long id, const std::string& name, const std::vector<std::string>& interests)
        : id(id), name(name), interests(interests) {} // Используем список инициализации
};

class UserService {
    std::unordered_map<long, std::unique_ptr<User>> storage; // Умные указатели
    long id_counter = 0;

public:
    User* getUser(long id) const {
        auto it = storage.find(id);
        return (it != storage.end()) ? it->second.get() : nullptr; // Возвращаем указатель или nullptr
    }

    void addUser(const std::string& name, const std::vector<std::string>& interests) {
        long id = id_counter++;
        storage[id] = std::make_unique<User>(id, name, interests);
    }
};

class FriendsService {
    std::unordered_map<long, std::unordered_set<long>> relations;

public:
    void addRelation(long id1, long id2) {
        relations[id1].insert(id2);
        relations[id2].insert(id1);
    }

    void deleteRelation(long id1, long id2) {
        if (relations.find(id1) != relations.end()) {
            relations[id1].erase(id2);
        }
        if (relations.find(id2) != relations.end()) {
            relations[id2].erase(id1);
        }
    }

    std::vector<std::pair<long, int>> getNewFriends(long id) {
        if (relations.find(id) == relations.end()) {
            return {}; // Если у пользователя нет друзей, вернуть пустой список
        }

        auto& friends = relations[id]; // Друзья текущего пользователя
        std::unordered_map<long, int> new_friends_repeats;

        for (long f : friends) {
            if (relations.find(f) == relations.end()) continue; // Если у друга нет друзей, пропускаем
            for (long u : relations[f]) { // u - потенциальный друг
                if (u == id || friends.find(u) != friends.end()) continue;
                new_friends_repeats[u]++;
            }
        }

        // Преобразуем `unordered_map` в `vector`
        std::vector<std::pair<long, int>> new_friends_sorted(new_friends_repeats.begin(), new_friends_repeats.end());

        // Сортируем по количеству общих друзей (по убыванию)
        std::sort(new_friends_sorted.begin(), new_friends_sorted.end(),
                  [](const std::pair<long, int>& a, const std::pair<long, int>& b) {
                      return a.second > b.second;
                  });

        return new_friends_sorted;
    }
};

class TestService {
    void test1() {
        FriendsService friendsService;
        friendsService.addRelation(1, 2);
        friendsService.addRelation(1, 3);
        friendsService.addRelation(2, 3);
        friendsService.addRelation(1, 10);
        friendsService.addRelation(10, 2);
        friendsService.addRelation(10, 6);
        friendsService.addRelation(6, 7);
        friendsService.addRelation(7, 8);
        friendsService.addRelation(7, 9);
        friendsService.addRelation(8, 9);
        friendsService.addRelation(5, 4);

        auto result = friendsService.getNewFriends(10);
        std::cout << "Possible new friends for user 1:\n";
        for (const auto& f : result) {
            std::cout << "id: " << f.first << " - " << f.second << " common friends\n";
        }
    }

public:
    void run() {
        test1();
    }
};

int main() {
    TestService testService;
    testService.run();
}