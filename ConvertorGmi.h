//
// Created by daniil on 12.06.22.
//

#ifndef KASPE_CONVERTORGMI_H
#define KASPE_CONVERTORGMI_H

#include <string>
#include <filesystem>
#include <fstream>
#include <string>

class GmiConverter {

    inline std::string list(const std::string &line) {
        last_state = curr_state;
        curr_state = State::list;
        return "<li>" + ignore_leading_spaces(line) + "</li>";
    }//вставляем элемент unordered_list

    inline std::string blockquote(const std::string &line) {
        last_state = curr_state;
        curr_state = State::none;
        return "<blockquote>" + ignore_leading_spaces(line) + "</blockquote>";
    }//вставляем цитату

    inline std::string pre() {
        last_state = curr_state;
        curr_state = State::none;
        if (!is_preformatted_text) {
            is_preformatted_text = true;
            return "<pre>";
        } else {
            is_preformatted_text = false;
            return "</pre>";
        }
    }//вставляем тэг для преформатированного текста

    inline std::string href(const std::string &line) {
        last_state = curr_state;
        curr_state = State::none;
        return "<a href=\"" + line.substr(0, line.find(' ')) + "\">" +
               ignore_leading_spaces(line.substr(line.find(' ') + 1)) + "</a>";
    }//вставляем ссылку

    inline std::string h(const std::string &line, int level) {
        last_state = curr_state;
        curr_state = State::none;
        return "<h" + std::to_string(level) + ">" + ignore_leading_spaces(line) + "</h" + std::to_string(level) + ">";
    }

    inline std::string ignore_leading_spaces(const std::string &line) {
        return line.substr(line.find_first_not_of(' '));
    }//игнорируем пробелы в начале строки

public:
    void ConvertFile(const std::filesystem::path &from, const std::filesystem::path &to) {
        std::ifstream fin(from); //входной файл .gmi
        std::ofstream fout(to);  //выходной файл .html
        curr_state = State::none;
        last_state = State::none;
        std::string curr_str;
        while (!fin.eof()) {
            std::getline(fin, curr_str);
            if (curr_str.find('*') == 0) {
                if (curr_state != State::list) {
                    fout << "<ul>" << std::endl; //если первый элемент unordered_list, то необходимо вставить тэг
                    fout << list(curr_str.substr(1)) << std::endl;
                } else {
                    fout << list(curr_str.substr(1)) << std::endl; //если не перый то просто вставляем элемент
                }
            } else if (curr_str.find('>') == 0) { //цитата
                fout << blockquote(curr_str.substr(1)) << std::endl;

            } else if (curr_str.find("=>") == 0) {

                curr_str = curr_str.substr(2);
                fout << href(ignore_leading_spaces(curr_str)) << std::endl;//удаляем из ссылки => и игнорируем пробелы(чтобы аргумет href начинался с адреса ссылки)
            } else if (curr_str.find("###") == 0) {
                fout << h(curr_str.substr(3), 3) << std::endl;//вставляем заголовки соотвествующего уровня

            } else if (curr_str.find("##") == 0) {
                fout << h(curr_str.substr(2), 2) << std::endl;
            } else if (curr_str.find('#') == 0) {
                fout << h(curr_str.substr(1), 1) << std::endl;
            } else if (curr_str.find("```") == 0) {// преформатированный текст
                fout << pre();
                if (curr_str.size() > 3) {// если в этой строке есть что-то ещё, то это уже преформатированный текст
                    fout << curr_str.substr(3);
                }
                fout << std::endl;
            } else {
                last_state = curr_state;
                curr_state = State::none;
                fout << curr_str << std::endl; //если в начале строки .gmi нет тэга, то просто копируем эту строку целиком
            }
            if (last_state != curr_state) {
                if (last_state == State::list) { //если состояние изменилось(наткнулись на другой тэг или просто строку), а до этого это был unordered_list то надо закрыть list
                    fout << "</ul>" << std::endl;
                }
            }

        }
        fin.close();
        fout.close();

    }

private:
    enum class State {
        none,  //всё кроме unordered_list
        list   // unordered_list
    };
    bool is_preformatted_text = false; //флаг отвечающий за преформатированный текст
    State curr_state = State::none; // текущее состояние
    State last_state = State::none; // состояние на прошлой строке
};

#endif //KASPE_CONVERTORGMI_H
