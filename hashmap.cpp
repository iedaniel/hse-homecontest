#include <iostream>
#include <vector>
#include <list>
#include <initializer_list>
#include <functional>
#include <exception>

template<class Key, class Value, class Hash = std::hash<Key> >
class HashMap {
private:
    std::vector<std::pair<typename std::list<std::pair<const Key, Value> >::iterator, size_t> > table;
    std::list<std::pair<const Key, Value> > lst;
    size_t sz, cp;
    Hash hasher;
public:
    HashMap(Hash _hasher = Hash()) : hasher(_hasher) {
        sz = 0, cp = 10;
        table.assign(cp, {lst.end(), 0});
    }

    size_t size() const {
        return sz;
    }

    bool empty() const {
        return (sz == 0);
    }

    using const_iterator = typename std::list<std::pair<const Key, Value> >::const_iterator;
    using iterator = typename std::list<std::pair<const Key, Value> >::iterator;

    iterator begin() {
        return lst.begin();
    }

    iterator end() {
        return lst.end();
    }

    const_iterator begin() const {
        return lst.begin();
    }

    const_iterator end() const {
        return lst.end();
    }

    iterator find(const Key& key) {
        size_t pos = hasher(key) % cp;
        auto tmp = table[pos].first;
        for (size_t i = 0; i < table[pos].second; ++i) {
            if (tmp->first == key)
                return tmp;
            ++tmp;
        }
        return end();
    }

    const_iterator find(const Key& key) const {
        size_t pos = hasher(key) % cp;
        auto tmp = table[pos].first;
        for (size_t i = 0; i < table[pos].second; ++i) {
            if (tmp->first == key)
                return tmp;
            ++tmp;
        }
        return end();
    }

    void inc() {
        std::list<std::pair<Key, Value> > tmp(lst.begin(), lst.end());
        lst.clear();
        sz = 0, cp *= 2;
        table.assign(cp, {lst.end(), 0});
        for (auto it = tmp.begin(); it != tmp.end(); ++it)
            insert(*it);
    }

    void insert(const std::pair<const Key, Value>& val) {
        if (find(val.first) != end())
            return;
        ++sz;
        size_t pos = hasher(val.first) % cp;
        table[pos].first = lst.insert(table[pos].first, val);
        ++table[pos].second;

        if (sz >= cp * 3 / 5)
            inc();
    }

    template <typename Iter>
    HashMap(Iter first, Iter last, Hash _hasher = Hash()) : hasher(_hasher) {
        sz = 0, cp = 10;
        table.assign(cp, {lst.end(), 0});
        while (first != last) {
            insert(*(first++));
        }
    }

    HashMap(const std::initializer_list<std::pair<const Key, Value> >& _lst, Hash _hasher = Hash()) : hasher(_hasher) {
        sz = 0, cp = 10;
        table.assign(cp, {lst.end(), 0});
        for (auto it = _lst.begin(); it != _lst.end(); ++it)
            insert(*it);
    }

    HashMap& operator = (const HashMap& other) {
        cp = other.cp;
        hasher = other.hasher;

        table.assign(cp, {lst.end(), 0});
        for (auto it = other.begin(); it != other.end(); ++it)
            insert(*it);
        return *this;
    }

    HashMap(const HashMap& other) {
        cp = other.cp;
        hasher = other.hasher;

        table.assign(cp, {lst.end(), 0});
        for (auto it = other.begin(); it != other.end(); ++it)
            insert(*it);
    }

    const Hash hash_function() const {
        return hasher;
    }

    void dec() {
        std::list<std::pair<Key, Value> > tmp(lst.begin(), lst.end());
        lst.clear();
        sz = 0, cp /= 2;
        table.assign(cp, {lst.end(), 0});
        for (auto it = tmp.begin(); it != tmp.end(); ++it)
            insert(*it);
    }

    void erase(const Key& key) {
        auto tmp = find(key);
        if (tmp == end())
            return;
        size_t pos = hasher(key) % cp;

        if (table[pos].second == 1) {
            table[pos].first = lst.end();
        } else {
            if (table[pos].first == tmp)
                ++table[pos].first;
        }

        lst.erase(tmp);
        --table[pos].second;
        --sz;

        if (sz <= cp / 4)
            dec();
    }

    Value& operator[](const Key& key) {
        auto tmp = find(key);
        if (tmp == end()) {
            insert({key, Value()});
            tmp = find(key);
        }
        return tmp->second;
    }

    const Value& at(const Key& key) const {
        auto tmp = find(key);
        if (tmp == end())
            throw std::out_of_range("u Racists");
        return tmp->second;
    }

    void clear() {
        while (!empty())
           erase(begin()->first);
    }

    ~HashMap() {
        table.clear();
        lst.clear();
        cp = sz = 0;
    }
};
