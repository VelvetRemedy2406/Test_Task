#include <iostream>
#include <filesystem>
#include "InputParser.h"
#include "ConvertorGmi.h"
void copy_dirs(const std::filesystem::path &from, std::filesystem::path to) {
    int curr_depth = std::filesystem::recursive_directory_iterator(from).depth() - 1;
    GmiConverter convert{};
    for (auto dirEntry = std::filesystem::recursive_directory_iterator(from);
         dirEntry != std::filesystem::recursive_directory_iterator();
         dirEntry++
            ) // итерируемся по всем файлам/дочерним директориям исходной папки
    {
        if (curr_depth < dirEntry.depth()) {
            to /= dirEntry->path().filename();
            curr_depth++;
        } else if (curr_depth == dirEntry.depth()) {
            to.remove_filename();
            to /= dirEntry->path().filename();
        } else {
            to.remove_filename();
            curr_depth--;
        }// Получили путь  в output дериктории(куда нужно копировать файлы/дериктори/файлы .gmi в .html конвертировать
        if (dirEntry->is_directory()) {
            std::filesystem::create_directories(to);//если итератор находится на дериктории, нужно скопировать её
        }
        else if(dirEntry->path().extension() != ".gmi")
        {
            std::filesystem::create_directories(to.parent_path());
            std::filesystem::copy_file(dirEntry->path(),to);//если итератор находится на файле,скопируем файл,
            // при этом если дериктории не существовало(в output) создадим её.
        } else
        {
            convert.ConvertFile(dirEntry->path(),to.replace_extension(".html"));
        }
    }
}

int main(int arg, char **argv) {
    InputParser Iparse(arg, argv);
    if (Iparse.cmdOptionExists("-InputDirectory") && Iparse.cmdOptionExists("-OutputDirectory")) {
        std::filesystem::path input_path = Iparse.getCmdOption("-InputDirectory"); //получули путь исходной дериктории
        std::filesystem::path output_path = Iparse.getCmdOption("-OutputDirectory");//получили путь конечной дериктории
        copy_dirs(input_path, output_path);

    } else {
        std::cerr << "Input directory does not exists or output directory was not set." << //не задали нужные аргументы или папка из которой надо генерировать не найдена
                  std::endl;
    }

    return 0;
}
