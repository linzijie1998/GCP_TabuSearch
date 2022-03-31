#include <iostream>

#include "../include/CUtils.h"
#include "../include/CTabuSearch.h"


int main(int argc, char* argv[])
{
    // 参数读取
    std::vector<std::string> params;
    for (int i = 1; i < argc; i++)
        params.emplace_back(argv[i]);
    std::string path = "../dsjc250.5.col";
    int max_iter = 0;
    int colors_num = 0;
    std::string out = "out.txt";
    if (!CUtils::analyzeParam(params, path, colors_num, max_iter, out))
    {
        std::cout << "UNKNOWN PARAMS FOUND!" << std::endl;
        return -1;
    }

    std::ofstream os;
    os.open(out);

    // 文件读取
    int degree = 0;
    int points_num = 0;
    std::vector<std::vector<int>> adjacency_table;
    if(!CUtils::loadDataFromFile(path, adjacency_table, points_num, degree))
    {
        std::cout << "CANNOT LOAD DATA FROM " << path << std::endl;
        os << "CANNOT LOAD DATA FROM " << path << std::endl;
        return -1;
    }
    std::cout << "Information:\n\t" << "number of points is " << points_num <<
              "\n\tsource file path: " << path <<
              "\n\toutput file path: " << out <<
              "\n\tnumber of edges: " << degree / 2 <<
              "\n\tnumber of colors: " << colors_num <<
              "\n\tnumber of max iteration: " << max_iter << std::endl;

    os << "Information:\n\t" << "number of points is " << points_num <<
       "\n\tsource file path: " << path <<
       "\n\toutput file path: " << out <<
       "\n\tnumber of edges: " << degree / 2 <<
       "\n\tnumber of colors: " << colors_num <<
       "\n\tnumber of max iteration: " << max_iter << std::endl;

    // 禁忌搜索
    auto *ts = new CTabuSearch(colors_num, points_num, adjacency_table, os);
    ts->search(max_iter);
    delete(ts);

    return 0;
}