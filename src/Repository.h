#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>
#include <string>
#include <fstream>
#include <type_traits>
#include "ProjectExceptions.h"

template <typename T, bool IsPointer = std::is_pointer<T>::value>
struct RepositoryIO;

template <typename T>
struct RepositoryIO<T, false> {
    static void save(std::ofstream& file, const T& item) {
        file << item.serialize() << "\n";
    }

    static T load(const std::string& line) {
        return T::deserialize(line);
    }
};

template <typename T>
struct RepositoryIO<T, true> {
    typedef typename std::remove_pointer<T>::type Pointee;

    static void save(std::ofstream& file, const T& item) {
        if (item) {
            file << item->serialize() << "\n";
        }
    }

    static T load(const std::string& line) {
        return convert(Pointee::deserialize(line));
    }

private:
    static T convert(T ptr) {
        return ptr;
    }

    static T convert(const Pointee& value) {
        return new Pointee(value);
    }
};

template <typename T>
class Repository {
private:
    std::vector<T> items;

public:
    void add(const T& item) {
        items.push_back(item);
    }

    int getSize() const {
        return static_cast<int>(items.size());
    }

    T& operator[](int index) {
        return items.at(index);
    }

    const T& operator[](int index) const {
        return items.at(index);
    }

    void removeAt(int index) {
        items.erase(items.begin() + index);
    }

    void clear() {
        items.clear();
    }

    void saveAll(const std::string& filename) const {
        std::ofstream file(filename.c_str());
        if (!file.is_open()) {
            throw FileAccessException("Could not open file for writing: " + filename);
        }

        for (typename std::vector<T>::const_iterator it = items.begin(); it != items.end(); ++it) {
            RepositoryIO<T>::save(file, *it);
        }
    }

    void loadAll(const std::string& filename) {
        std::ifstream file(filename.c_str());
        if (!file.is_open()) {
            throw FileAccessException("Could not open file for reading: " + filename);
        }

        clear();
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                items.push_back(RepositoryIO<T>::load(line));
            }
        }
    }
};

#endif
