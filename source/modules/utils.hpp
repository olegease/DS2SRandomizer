#ifndef CBOYO_DS2SRANDOMIZER_UTILS_HPP
#define CBOYO_DS2SRANDOMIZER_UTILS_HPP

#include <array>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <unordered_map>

class Stopwatch {
    std::chrono::high_resolution_clock::time_point start;
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
    constexpr uint64_t golden_ratio = 0x9E3779B97F4A7C15ull;
    std::hash<std::string_view> hasher;
    seed ^= hasher(zone) + golden_ratio + (seed << 6) + (seed >> 2);
    return seed;
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

namespace cboyo::parse {

    inline bool getline( std::istream &is, std::string &line ) {
        if ( !std::getline( is, line ) ) return false;
        if ( !line.empty( ) && line.back( ) == '\r' ) line.pop_back( );
        return true;
    }

    inline auto split(std::string_view view, char delimiter) -> std::vector<std::string_view> {
        std::vector<std::string_view> tokens;

        size_t beg = 0;
        size_t end = view.find(delimiter);
        for ( ; end != std::string_view::npos; end = view.find(delimiter, beg)) {
            tokens.emplace_back(view.substr(beg, end - beg));
            beg = end + 1;
        }
        auto last = view.substr(beg);
        if (!last.empty()) tokens.emplace_back(last);

        return tokens;
    }

    inline bool read_var(std::string_view view, auto &out) {
        std::istringstream iss{std::string(view)};

        iss >> out;
        if (iss.fail()) {
            std::cerr << "read_var istringstream fail for: " << view << std::endl;
            return false;
        }
        iss >> std::ws;
        if (!iss.eof()) {
            std::cerr << "read_var istringstream not EOF for: " << view << std::endl;
            return false;
        }
        return true;
    }
}

namespace cboyo::random {
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

    auto vindex(auto &&container, auto &&generator) -> size_t {
        std::uniform_int_distribution<size_t> dist(0, container.size() - 1);
        return dist(generator);
    }

    bool roll(unsigned min, auto &&generator) {
        assert(min <= 100);
        std::uniform_int_distribution<unsigned> dist(1, 100);
        return dist(generator) <= min;
    }

    auto stats(unsigned init, unsigned diff, auto &&generator) -> std::array< uint8_t, 9 > {
        assert(6 < init && init < 51 && init > diff);
        std::array< uint8_t, 9 > stats;
        stats.fill(init);
        if (diff == 0) return stats;
        auto gen_sign = [&generator]() -> int {
            std::uniform_int_distribution<unsigned> dist_sign(0, 1);
            return dist_sign(generator) ? 1 : -1;
        };
        auto gen_diff = [&generator, &diff]() -> uint8_t {
            std::uniform_int_distribution<unsigned> dist_diff(0, diff);
            return dist_diff(generator);
        };
        int curr = 0;
        int sum = 0;
        int sign = gen_sign();
        for (auto &stat : stats) {
            auto gend = gen_diff() * sign;
            stat += gend;
            curr += gend;
            sum += stat;
            if (curr > 0) sign = -1;
            else if (curr < 0) sign = 1;
            else sign = gen_sign();
        }
        int init_sum = init * stats.size();
        assert(sum - diff <= init_sum && init_sum <= sum + diff);
        std::shuffle(stats.begin(), stats.end(), generator);
        return stats;
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
