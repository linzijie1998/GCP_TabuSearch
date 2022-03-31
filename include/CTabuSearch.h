/**
 * @brief 图染色问题的禁忌搜索求解
 *
 * @file CUtils.h
 * @author linzijie
 * @version 0.1
 * @date 2022/03/15
 */

#ifndef GCP_CTABUSEARCH_H
#define GCP_CTABUSEARCH_H

#include <vector>
#include <fstream>

class CTabuSearch {
private:
    int m_colors_num;                  // 颜色数
    int m_points_num;                  // 节点数
    int m_current_conflicts;           // 当前冲突边数
    int m_best_conflicts;              // 最小冲突边数
    int m_tabu_tenure;                 // 禁忌长度

    std::ofstream m_os;  // 文件输出流

    std::vector<int> m_solution;                      // 着色解
    std::vector<std::vector<int>> m_adjacency_table;  // 邻接表
    std::vector<std::vector<int>> m_adjacency_color;  // 颜色计数表
    std::vector<std::vector<int>> m_tabu_list;        // 禁忌表
    std::vector<std::vector<int>> m_freq;             // 频率表

    void init();                        // 初始化各项参数
    int calConflicts();                 // 计算冲突边数
    void moveIt(int iter_count);        // 一次迭代: 颜色交换

public:
    // 构造&析构函数
    CTabuSearch(int colors_num, int points_num, std::vector<std::vector<int>> adjacency_table, std::ofstream& os);
    ~CTabuSearch();

    // 禁忌搜索
    void search(int max_iter);
};


#endif //GCP_CTABUSEARCH_H
