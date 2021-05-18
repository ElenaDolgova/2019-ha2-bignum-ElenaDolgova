#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <cmath>

namespace mp {
    const uint8_t MAX_COUNT_DIGIT_AT_ONE_NUMBER = 9;

    class bignum {
    private:
        std::vector<uint32_t> big_number = std::vector<uint32_t>();

        void swap(bignum &b2) {
            using std::swap;
            swap(this->big_number, b2.big_number);
        }

    public:
        bignum(uint32_t number = 0) {
            big_number.push_back(number);
        }

        explicit bignum(std::string const big_number_str) {

            for (size_t j = big_number_str.length() - 1; j != size_t(-1); --j) {
                if (big_number_str[j] != '0') {
                    break;
                }
                if (j == 0) {
                    big_number.push_back(0);
                    return;
                }
            }


            std::string part_number;
            uint8_t i = 0;

            for (size_t l = big_number_str.length() - 1; l != size_t(-1); l--) {
                if (i == MAX_COUNT_DIGIT_AT_ONE_NUMBER) {

                    big_number.push_back(static_cast<uint32_t>(std::stoul(part_number, nullptr, 10)));
                    part_number = "";
                    i = 0;
                }
                part_number = big_number_str[l] + part_number;
                ++i;
            }

            if (i != 0) {
                big_number.push_back(static_cast<uint32_t>(std::stoul(part_number, nullptr, 10)));
            }
        }

        const std::vector<uint32_t> &get_big_number() const {
            return big_number;
        }

        size_t get_number_of_rank() const {
            return big_number.size();
        }

        bignum &operator=(bignum other) {
            swap(other);
            return *this;
        }

        bool operator==(const bignum &other) const {
            return this->to_string() == other.to_string();
        }

        std::string to_string() const {
            std::string big_number_str;
            for (size_t k = 0; k < big_number.size(); ++k) {
                std::string curr = std::to_string(big_number[k]);
                if (k == big_number.size() - 1) {
                    big_number_str = curr.append(big_number_str);
                } else {
                    size_t zeros = MAX_COUNT_DIGIT_AT_ONE_NUMBER - curr.length();
                    big_number_str = std::string(zeros, '0').append(curr).append(big_number_str);
                }
            }
            return big_number_str;
        }

        explicit operator std::uint32_t() const {

            u_int64_t num64 = big_number[0];
            if (big_number.size() > 1) {
                size_t zeros1 = MAX_COUNT_DIGIT_AT_ONE_NUMBER - std::to_string(big_number[1]).length();
                u_int64_t num2 = big_number[1];
                num2 = num2 * pow(10, 9 - zeros1);
                num64 = num64 + num2;
            }
            //пробую добавить еще 2 разряда
            if (big_number.size() > 2) {
                u_int64_t num2_1 =
                        (static_cast<u_int64_t>(big_number[2]) % static_cast<u_int64_t>(100)) /
                        static_cast<u_int64_t>(10);
                u_int64_t num2_0 = static_cast<u_int64_t>(big_number[2]) % static_cast<u_int64_t>(10);
                u_int64_t num64_0 = num64;


                num2_0 = num2_0 * pow(10, 18);
                num64_0 = num64_0 + num2_0;
                if (num64_0 > num64) {
                    num64 = num64_0;

                    num2_1 = num2_1 * pow(10, 19);
                    num64_0 = num64_0 + num2_1;
                    if (num64_0 > num64) {
                        num64 = num64_0;
                    }
                }
            }

            return static_cast<u_int32_t>(num64);
        }

        explicit operator bool() const {
            return big_number[0];
        }

        bignum operator-(bignum bg) = delete;

    };

    inline std::ostringstream &operator<<(std::ostringstream &os, bignum const &rhs) {
        os << rhs.to_string();
        return os;
    }


    inline std::istringstream &operator>>(std::istringstream &is, bignum &rhs) {
        std::string str;
        is >> str;
        rhs = bignum(str);
        return is;
    }

    inline bignum do_operation(std::vector<u_int32_t> const &array_with_max_rank,
                               std::vector<u_int32_t> const &array_with_min_rank,
                               char op, std::string new_number = "") {
        size_t max_rank = array_with_max_rank.size();
        size_t min_rank = array_with_min_rank.size();
        u_int64_t sum = 0;
        u_int64_t from_min = 0;

        for (size_t i = 0; i < max_rank; ++i) {
            u_int64_t from_max = array_with_max_rank[i];
            if (min_rank > i) {
                from_min = array_with_min_rank[i];
            } else {
                from_min = 0;
            }
            if (op == '+') {
                sum = sum + from_max + from_min;
            } else if (op == '*') {
                sum = sum + from_max * from_min;
            }
            u_int64_t count_zeros = 1000000000;
            u_int32_t new_single_number = sum % count_zeros;
            std::string cur_number = std::to_string(new_single_number);

            size_t zeros = MAX_COUNT_DIGIT_AT_ONE_NUMBER - cur_number.length();
            new_number = std::string(zeros, '0').append(cur_number).append(new_number);

            sum /= count_zeros;
        }
        if (sum != 0)
            new_number = std::to_string(sum).append(new_number);

        return bignum(new_number);

    }

    inline void
    trim(size_t rank, std::vector<uint32_t> const &bn, std::string &new_number, std::vector<u_int32_t> &trim_bn) {
        for (size_t i = 0; i < rank; i++) {
            u_int64_t single_bn = bn[i];
            if (single_bn == 0) {
                new_number = std::string(MAX_COUNT_DIGIT_AT_ONE_NUMBER, '0').append(new_number);
            } else {
                trim_bn = std::vector<u_int32_t>(&bn[i], &bn[rank]);
                break;
            }
        }
    }

    inline bignum multiply_with_zeroes_end(bignum const &lhs, bignum const &rhs) {
        size_t rank_lhs = lhs.get_number_of_rank();
        size_t rank_rhs = rhs.get_number_of_rank();
        std::string new_number = "";
        std::vector<u_int32_t> lhs2;
        std::vector<u_int32_t> rhs2;


        trim(rank_lhs, lhs.get_big_number(), new_number, lhs2);
        trim(rank_rhs, rhs.get_big_number(), new_number, rhs2);

        size_t rank_lhs2 = lhs2.size();
        size_t rank_rhs2 = rhs2.size();

        return rank_lhs2 > rank_rhs2 ?
               do_operation(lhs2, rhs2, '*', new_number) :
               do_operation(rhs2, lhs2, '*', new_number);
    }

    inline bignum operation(bignum const &lhs, bignum const &rhs, char op) {
        size_t rank_lhs = lhs.get_number_of_rank();
        size_t rank_rhs = rhs.get_number_of_rank();

        if (op == '*' && (lhs.get_big_number()[0] == 0 || rhs.get_big_number()[0] == 0)) {
            return multiply_with_zeroes_end(lhs, rhs);
        }

        return rank_lhs > rank_rhs ?
               do_operation(lhs.get_big_number(), rhs.get_big_number(), op) :
               do_operation(rhs.get_big_number(), lhs.get_big_number(), op);
    }

    inline bignum operator+(bignum const &lhs, bignum const &rhs) {
        return operation(lhs, rhs, '+');
    }

    inline bignum operator+=(bignum &lhs, bignum const &rhs) {
        lhs = lhs + rhs;
        return lhs;
    }

    inline bignum operator*(bignum const &lhs, bignum const &rhs) {
        return operation(lhs, rhs, '*');
    }

    inline bignum operator*=(bignum &lhs, bignum const &rhs) {
        lhs = lhs * rhs;
        return lhs;
    }

    class polynomial {
    private:
        std::vector<u_int32_t> coef;

        void check_size_polym(size_t index) {
            if (index > get_rank_polym()) {
                size_t diff = index - get_rank_polym();
                while (diff != 0) {
                    coef.push_back(0);
                    --diff;
                }
            }
        }

        void check_on_space(size_t &curr_rank, size_t const new_rank) {
            if (curr_rank != new_rank) {
                while (curr_rank != new_rank) {
                    coef.push_back(0);
                    ++curr_rank;
                }
            }
        }

        void push_new_coef(std::string &sub_str, size_t &curr_rank) {
            size_t new_rank = static_cast<uint32_t>(
                    std::stoul(sub_str.substr(sub_str.find_first_of('^') + 1), nullptr, 10)
            );
            check_on_space(curr_rank, new_rank);

            sub_str = sub_str.substr(0, sub_str.find_first_of('*'));
            coef.push_back(static_cast<uint32_t>(std::stoul(sub_str, nullptr, 10)));
            ++curr_rank;
        }

    public:
        explicit polynomial(const std::string &polym_str) {
            coef = std::vector<u_int32_t>();
            size_t end_sub = polym_str.find_last_of('+');
            std::string new_str = polym_str;
            size_t curr_rank = 0;

            while (end_sub != size_t(-1)) {
                std::string sub_str = new_str.substr(end_sub + 1);
                new_str = new_str.substr(0, end_sub);
                end_sub = new_str.find_last_of('+');

                push_new_coef(sub_str, curr_rank);

            }
            push_new_coef(new_str, curr_rank);
        }

        u_int32_t get_rank_polym() const {
            return coef.size() - 1;
        }

        u_int32_t at(size_t index) const {
            if (index > get_rank_polym()) {
                return 0;
            }
            return coef[index];
        }

        u_int32_t &at(size_t index) {
            check_size_polym(index);
            return coef[index];
        }

        template<typename T>
        T operator()(T const &num) const {
            T result = coef[get_rank_polym()];

            if (num == 0) {
                return coef[0];
            }

            for (size_t i = 1; i <= get_rank_polym(); i++) {
                result *= num;
                result += coef[get_rank_polym() - i];
            }

            return result;
        }
    };

}