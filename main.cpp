#include <iostream>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <map>
//#include <multimap>
#include <utility>
#include <arpa/inet.h>
//#include <boost/asio/ip/address_v4.hpp>

#include "lib.h"

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]

uint32_t ip_to_uint(const std::array<uint8_t, 4>& ip_arr)
{
    return ip_arr[0] << 24 | ip_arr[1] << 16 | ip_arr[2] << 8 | ip_arr[3];
}

std::string ip_to_string(const std::array<uint8_t, 4>& ip_arr)
{
    std::string res = std::to_string(ip_arr.at(0));

    for (auto it = ip_arr.cbegin() + 1; it != ip_arr.cend(); ++it) {
        res += "." + std::to_string(*it);
    }

    return res;
}

std::vector<std::array<uint8_t, 4>> filter(const std::vector<std::array<uint8_t, 4>> ip_pool, uint8_t octet_key)
{
    std::vector<std::array<uint8_t, 4>> result;

    for (auto ip = ip_pool.begin(); ip != ip_pool.end(); ++ip) {
        if (ip->at(0) == octet_key) {
           result.push_back(*ip);
        }
    }

    return result;
}

std::vector<std::array<uint8_t, 4>> filter(const std::vector<std::array<uint8_t, 4>> ip_pool, uint8_t octet_key0, uint8_t octet_key1)
{
    std::vector<std::array<uint8_t, 4>> result;

    for (auto ip = ip_pool.begin(); ip != ip_pool.end(); ++ip) {
        if (ip->at(0) == octet_key0  && ip->at(1) == octet_key1) {
           result.push_back(*ip);
        }
    }

    return result;
}

std::vector<std::array<uint8_t, 4>> filter_any(const std::vector<std::array<uint8_t, 4>> ip_pool, uint8_t octet_key)
{
    std::vector<std::array<uint8_t, 4>> result;

    for (auto ip = ip_pool.begin(); ip != ip_pool.end(); ++ip) {
        auto it = std::find(ip->begin(), ip->end(), octet_key);
        if (it != ip->end()) {
            result.push_back(*ip);
        }
    }

    return result;
}

std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);

    while(stop != std::string::npos) {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

int main (int, char **) 
{
    try
    {
        std::vector<std::array<uint8_t, 4>> ip_pool;

        auto converter = [](const std::vector<std::string> ip){
                std::array<uint8_t, 4> arr;
                for (uint32_t i = 0; i < arr.size(); i++) {
                    arr[i] = static_cast<uint8_t>(std::stoi(ip.at(i)));
                }

                return arr;
            };

        for(std::string line; std::getline(std::cin, line);) {
            std::vector<std::string> v = split(line, '\t');

            auto ip = split(v.at(0), '.');
            std::array<uint8_t, 4> arr = converter(ip);
            ip_pool.push_back(arr);
        }

        std::sort(ip_pool.begin(), ip_pool.end(), [](const std::array<uint8_t, 4> a, const std::array<uint8_t, 4> b){return ip_to_uint(a) < ip_to_uint(b);});

        // TODO reverse lexicographically sort
        for (auto ip = ip_pool.rbegin(); ip != ip_pool.rend(); ++ip) {
            std::cout << ip_to_string(*ip) << std::endl;
        }

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        auto ip_filtred0 = filter(ip_pool, 1);
        for (auto ip = ip_filtred0.rbegin(); ip != ip_filtred0.rend(); ++ip) {
            std::cout << ip_to_string(*ip) << std::endl;
        }

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        auto ip_filtred1 = filter(ip_pool, 46, 70);
        for(auto ip = ip_filtred1.rbegin(); ip != ip_filtred1.rend(); ++ip) {
            std::cout << ip_to_string(*ip) << std::endl;
        }

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        auto ip_filtred2 = filter_any(ip_pool, 46);
        for(auto ip = ip_filtred2.rbegin(); ip != ip_filtred2.rend(); ++ip) {
            std::cout << ip_to_string(*ip) << std::endl;
        }

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
