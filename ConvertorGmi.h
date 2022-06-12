//
// Created by daniil on 12.06.22.
//

#ifndef KASPE_CONVERTORGMI_H
#define KASPE_CONVERTORGMI_H

#include <string>
#include <filesystem>
#include <fstream>

class GmiConverter {
    void ConvertFile(const std::filesystem::path& from, const std::filesystem::path& to) {
        std::ifstream fin(from);
        std::ofstream fout(to);
        state = header::none;
        last_state = header::none;

        fin.close();
        fout.close();

    }

    inline std::string list(const std::string &line) {
        last_state = state;
        state = header::list;
        return "<li>" + line + "</li>";
    }

private:
    enum class header {
        none, list
    };
    header state;
    header last_state;
};

#endif //KASPE_CONVERTORGMI_H
