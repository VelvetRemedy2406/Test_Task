//
// Created by daniil on 12.06.22.
//

#ifndef KASPE_INPUTPARSER_H
#define KASPE_INPUTPARSER_H
#include <vector>
#include <string>
class InputParser { // простейший парсер аргументов строки
public:
    InputParser(int &argc, char **argv) {
        for (int i = 1; i < argc; ++i)
            this->m_tokens.emplace_back(argv[i]); //положим аргументы в вектор стрингов
    }

    [[nodiscard]] const std::string &getCmdOption(const std::string &option)  { //получить значение после -file например
        std::vector<std::string>::const_iterator itr;
        itr = std::find(this->m_tokens.begin(), this->m_tokens.end(), option);
        if (itr != this->m_tokens.end() && ++itr != this->m_tokens.end()) {
            return *itr;
        }
        static const std::string empty_string; // один раз инициализируем пустую строку и будем её возращать если не нашлось опции
        return empty_string;
    }

    [[nodiscard]] bool cmdOptionExists(const std::string &option) const { // проверка существует ли опция -file например
        return std::find(this->m_tokens.begin(), this->m_tokens.end(), option)
               != this->m_tokens.end();
    }

private:
    std::vector<std::string> m_tokens;
};
#endif //KASPE_INPUTPARSER_H
