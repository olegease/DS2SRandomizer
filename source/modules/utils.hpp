#ifndef CBOYO_DS2SRANDOMIZER_UTILS_HPP
#define CBOYO_DS2SRANDOMIZER_UTILS_HPP

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <random>
#include <unordered_map>

class Stopwatch {
    std::chrono::steady_clock::time_point start;
public:
    Stopwatch() : start(std::chrono::high_resolution_clock::now()) {}
    auto passed() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    }
};

inline bool open_file( std::ifstream &file, const std::filesystem::path &path ) {
    file.open(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return false;
    }
    return true;
}

inline auto combine_seed_with_zone(uint64_t seed, std::string_view zone) -> uint64_t {
    std::hash<std::string_view> hasher;
    return hasher(zone) ^ (seed * 0x9e3779b97f4a7c15);
}

inline auto vector_find_swap_pop(auto &&container, auto &&value) -> decltype(auto) {
    auto it = std::find(container.begin(), container.end(), value);
    if (it != container.end()) {
        std::swap(*it, container.back());
        container.pop_back();
        return true;
    }
    return false;
}

inline bool vector_contains(auto &&container, auto &&value) {
    return std::find(container.begin(), container.end(), value) != container.end();
}

inline void vector_remove_duplicates(auto &&container) {
    std::stable_sort(container.begin(), container.end());
    container.erase(std::unique(container.begin(), container.end()), container.end());
}

inline auto get_file_contents_binary(const std::filesystem::path &path) -> std::string {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return {};
    }
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

inline void write_to_file_binary(const std::filesystem::path &path, const std::string &data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }
    file.write(data.data(), data.size());
}

inline auto time_string_now() -> std::string {
    return std::to_string(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

namespace parse {
    inline auto split(std::string_view view, char delimiter) -> std::vector<std::string_view> {
        std::vector<std::string_view> rvo;

        size_t beg = 0;
        size_t end = view.find(delimiter);
        for ( ; end != std::string_view::npos; end = view.find(delimiter, beg)) {
            rvo.emplace_back(view.substr(beg, end - beg));
            beg = end + 1;
        }
        auto last = view.substr(beg);
        if (!last.empty()) rvo.emplace_back(last);

        return rvo;
    }

    inline bool read_var(std::string_view view, auto &out) {
        std::istringstream iss{std::string(view)};
        iss >> out;

        return !iss.fail() && iss.eof();
    }
}

namespace random {
    inline auto m_gen = std::mt19937_64(std::random_device{}());

    auto element(auto &&container, auto &&generator) -> decltype(auto) {
        std::uniform_int_distribution<size_t> dist(0, container.size() - 1);
        return container[dist(generator)];
    }

    auto choose_n_elements(auto &&container, size_t n, bool b, auto &&generator) -> decltype(auto) {
        typename std::remove_cvref<decltype(container)>::type indices;
        indices.reserve(n);
        std::uniform_int_distribution<size_t> dist(0, container.size() - 1);
        while (n--) indices.emplace_back(dist(generator));

        return indices;
    }

    auto vindex(auto &&container, auto &&generator) -> decltype(auto) {
        std::uniform_int_distribution<size_t> dist(0, container.size() - 1);
        return dist(generator);
    }

    bool roll(unsigned min, auto &&generator) {
        assert(min <= 100);
        std::uniform_int_distribution<unsigned> dist(1, 100);
        return dist(generator) <= min;
    }

    template<typename T, typename Gen>
    T integer(T min, T max, Gen &&generator) {
        assert(min <= max);
        if (min == max) return min;
        std::uniform_int_distribution<T> dist(min, max);
        return dist(generator);
    }

    template<typename T, typename Gen>
    T real(T min, T max, Gen &&generator) {
        assert(min < max);
        std::uniform_real_distribution<T> dist(min, max);
        return dist(generator);
    }
}

#endif//CBOYO_DS2SRANDOMIZER_UTILS_HPP
