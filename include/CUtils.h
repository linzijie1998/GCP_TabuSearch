//
// Created by LINZIJIE on 2022/3/15.
//

#ifndef GCP_CUTILS_H
#define GCP_CUTILS_H

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

class CUtils {
public:
    /**
     * @brief读取文件信息, 创建邻接表
     *
     * @param path 文件路径
     * @param adjacency_table 邻接表
     * @param points_num 节点数
     * @param g_degree 图的度
     *
     * @return 读取成功时返回邻接表信息、节点数以及图的度
     *
     * @version 0.1
     * @author linzijie
     */
    static bool loadDataFromFile(
            const std::string& path, std::vector<std::vector<int>>& adjacency_table, int& points_num, int& g_degree)
    {
        std::ifstream is;
        bool is_openfile = false;
        is.open(path);
        if (is)
        {
            is_openfile = true;
            std::string line;
            while (std::getline(is, line))
            {
                if (line.size() < 2)
                    continue;
                std::stringstream ss;
                ss << line;
                char flag;
                ss >> flag;
                if (flag == 'p')
                {
                    // 读取图的点和度的信息
                    std::string tmp;
                    ss >> tmp >> points_num >> g_degree;
                    adjacency_table.resize(points_num); // 初始化邻接表的长度
                }
                else if (flag == 'e')
                {
                    int a, b;
                    ss >> a >> b;
                    // 向邻接表中添加节点
                    adjacency_table[a - 1].push_back(b - 1);
                    adjacency_table[b - 1].push_back(a - 1);
                }
                else
                    continue;
            }
        }
        is.close();
        return is_openfile;
    }

    /**
     * @brief 分析传入的参数, 获取颜色数和最大迭代数信息
     *
     * @param params 参数
     * @param color_num 颜色数
     * @param max_iter 最大迭代数
     *
     * @return 分析成功返回颜色数和最大迭代数信息
     *
     * @version 0.1
     * @author linzijie
     */
    static bool analyzeParam(const std::vector<std::string>& params, std::string& path, int& color_num, int& max_iter, std::string& out)
    {
        if (params.size() != 8)
            return false;
        int flag_color = 0;
        int flag_iter = 0;
        int flag_file = 0;
        int flag_out = 0;
        for (const std::string& param : params)
        {
            if (param == "-c")
                flag_color = 1;
            else if (param == "-i")
                flag_iter = 1;
            else if (param == "-p")
                flag_file = 1;
            else if (param == "-o")
                flag_out = 1;
            else
            {
                if (flag_color == 1)
                {
                    color_num = atoi(param.c_str());
                    flag_color = 2;
                    continue;
                }
                if (flag_iter == 1)
                {
                    max_iter = atoi(param.c_str());
                    flag_iter = 2;
                    continue;
                }
                if (flag_file == 1)
                {
                    path = param;
                    flag_file = 2;
                    continue;
                }
                if (flag_out == 1)
                {
                    out= param;
                    flag_out = 2;
                    continue;
                }
            }
        }
        return flag_color == 2 && flag_iter == 2 && flag_file == 2 && flag_out == 2;
    }
};


#endif //GCP_CUTILS_H
