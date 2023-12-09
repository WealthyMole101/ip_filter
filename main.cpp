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

std::pair<uint32_t, std::string>&& pack_ip(std::vector<std::string>& ip)
{
    uint32_t ip_v4_bin = 0;
    std::string ip_str = "";
    int ip_v4_size = 4;

    for (int i = 0; i < ip_v4_size; i++) {
        ip_v4_bin |= static_cast<uint8_t>(std::stoi(ip.at(3 - i))) << (i * 8);
        ip_str += ip.at(i);

        // Не очень решение
        if (i < (ip_v4_size - 1)) {
             ip_str += ".";
        }
    }

    return std::move(std::make_pair(ip_v4_bin, ip_str));
}

std::multimap<uint32_t, std::string> filter(const std::vector<std::vector<std::string>> ip_pool, uint8_t octet_key)
{
    std::multimap<uint32_t, std::string> result;
    std::string key = std::to_string(octet_key);

    for (auto ip = ip_pool.begin(); ip != ip_pool.end(); ++ip) {
        if (ip->at(0) == key) {
           auto val = *ip;
           auto pair = pack_ip(val);
           result.insert({pair.first, pair.second});
        }
    }

    return result;
}

std::multimap<uint32_t, std::string> filter(const std::vector<std::vector<std::string>> ip_pool, uint8_t octet_key0, uint8_t octet_key1)
{
    std::multimap<uint32_t, std::string> result;
    std::string key0 = std::to_string(octet_key0);
    std::string key1 = std::to_string(octet_key1);

    for (auto ip = ip_pool.begin(); ip != ip_pool.end(); ++ip) {
        if (ip->at(0) == key0 && ip->at(1) == key1) {
           auto val = *ip;
           auto pair = pack_ip(val);
           result.insert({pair.first, pair.second});
        }
    }

    return result;
}

std::multimap<uint32_t, std::string> filter_any(const std::vector<std::vector<std::string>> ip_pool, uint8_t octet_key)
{
    std::multimap<uint32_t, std::string> result;
    std::string key = std::to_string(octet_key);

    for (auto ip = ip_pool.begin(); ip != ip_pool.end(); ++ip) {
        auto it = std::find(ip->begin(), ip->end(), key);
        if (it != ip->end()) {
           auto val = *ip;
           auto pair = pack_ip(val);
           result.insert({pair.first, pair.second});
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
        std::vector<std::vector<std::string> > ip_pool;
        std::multimap<uint32_t, std::string> ip_pool_machine;


        for(std::string line; std::getline(std::cin, line);) {
            std::vector<std::string> v = split(line, '\t');

            auto ip = split(v.at(0), '.');
            ip_pool.push_back(ip);

            auto pair = pack_ip(ip);
            ip_pool_machine.insert({pair.first, pair.second});
        }

        // TODO reverse lexicographically sort
        for (auto ip = ip_pool_machine.rbegin(); ip != ip_pool_machine.rend(); ++ip) {
            std::cout << ip->second << std::endl;
        }

        //for(std::vector<std::vector<std::string> >::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip) {
        //    for(std::vector<std::string>::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part) {
        //        if (ip_part != ip->cbegin()) {
                    //std::cout << ".";

        //        }
                //std::cout << *ip_part;
        //    }
            //std::cout << std::endl;
        //}

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        auto ip_filtred0 = filter(ip_pool, 1);
        for(auto ip = ip_filtred0.rbegin(); ip != ip_filtred0.rend(); ++ip) {
            std::cout << ip->second << std::endl;
        }


        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
        auto ip_filtred1 = filter(ip_pool, 46, 70);
        for(auto ip = ip_filtred1.rbegin(); ip != ip_filtred1.rend(); ++ip) {
            std::cout << ip->second << std::endl;
        }

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)
        auto ip_filtred2 = filter_any(ip_pool, 46);
        for(auto ip = ip_filtred2.rbegin(); ip != ip_filtred2.rend(); ++ip) {
            std::cout << ip->second << std::endl;
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
