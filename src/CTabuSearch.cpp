//
// Created by LINZIJIE on 2022/3/15.
//

#include "../include/CTabuSearch.h"

#include <utility>
#include <cstdlib>
#include <ctime>
#include <iostream>

CTabuSearch::CTabuSearch(int colors_num, int points_num, std::vector<std::vector<int>> adjacency_table, std::ofstream& os)
{
    m_colors_num = colors_num;
    m_points_num = points_num;
    m_os = std::move(os);
    m_adjacency_table = std::move(adjacency_table);
    int seed = (int)time(0);
    std::cout << "Random seed is " << seed << std::endl;
    m_os << "Random seed is " << seed << std::endl;
    srand(seed);
    this->init();
}

CTabuSearch::~CTabuSearch()
{
    m_os.close();
}

/**
 * @brief 初始化着色解、颜色计数表、禁忌表和频率表
 *
 * @version 0.1
 * @author linzijie
 */
void CTabuSearch::init()
{
    m_solution.resize(m_points_num);
    for (int i = 0; i != m_points_num; i++)
        m_solution[i] = rand() % m_colors_num;
    m_current_conflicts = calConflicts();
    m_best_conflicts = m_current_conflicts;
    std::cout << "Init: number of conflicts is " << m_current_conflicts << std::endl;
    m_os << "Init: number of conflicts is " << m_current_conflicts << std::endl;

    m_adjacency_color.resize(m_points_num);
    for (size_t i = 0; i != m_points_num; i++)
    {
        m_adjacency_color[i].resize(m_colors_num);
        for (size_t j = 0; j != m_adjacency_color[i].size(); j++)
            m_adjacency_color[i][j] = 0;
        for (int point : m_adjacency_table[i])
            m_adjacency_color[i][m_solution[point]]++;
    }

    m_tabu_list.resize(m_points_num);
    for (size_t i = 0; i != m_points_num; i++)
    {
        m_tabu_list[i].resize(m_colors_num);
        for (size_t j = 0; j != m_colors_num; j++)
            m_tabu_list[i][j] = 0;
    }

    m_freq.resize(m_points_num);
    for (size_t i = 0; i != m_points_num; i++)
    {
        m_freq[i].resize(m_colors_num + 1);
        for (size_t j = 0; j != m_colors_num; j++)
            m_freq[i][j] = 1;
        m_freq[i][m_colors_num] = m_colors_num;
    }

    m_tabu_tenure = m_current_conflicts + rand() % 10;

}

/**
 * @brief 计算冲突边数
 *
 * @return 冲突边数
 *
 * @version 0.1
 */
int CTabuSearch::calConflicts()
{
    int ans = 0;
    // 遍历所有点
    for (int i = 0; i != m_solution.size(); i++)
    {
        // 遍历当前点的邻接点
        for (int j = 0; j != m_adjacency_table[i].size(); j++)
        {
            // 当前点和它的邻接点是一个颜色时, 冲突+1
            if (m_solution[i] == m_solution[m_adjacency_table[i][j]])
                ans++;
        }
    }
    // 一个冲突的边会计算两次
    return ans / 2;
}

/**
 * @brief 一次迭代: 颜色替换
 *
 * @param iter_count 当前迭代次数
 *
 * @version 0.1
 */
void CTabuSearch::moveIt(const int iter_count)
{
    int delta = 0;
    int tabu_best[3] = {0, 0, 10000000};
    int none_tabu_best[3] = {0, 0, 10000000};
    for (size_t i = 0; i != m_points_num; i++)
    {
        for (size_t j = 0; j != m_colors_num; j++)
        {
            if (m_solution[i] != j) // 将节点i的颜色换成颜色j, 计算delta值
            {
                delta = m_adjacency_color[i][j] - m_adjacency_color[i][m_solution[i]];
                if (m_tabu_list[i][j] > iter_count)
                {
                    if (delta < tabu_best[2])
                    {
                        tabu_best[0] = i;
                        tabu_best[1] = j;
                        tabu_best[2] = delta;
                    }
                }
                else
                {
                    if (delta < none_tabu_best[2])
                    {
                        none_tabu_best[0] = i;
                        none_tabu_best[1] = j;
                        none_tabu_best[2] = delta;
                    }
                }
            }
        }
    }
    if ((m_current_conflicts + tabu_best[2] < m_best_conflicts) && (tabu_best[2] < none_tabu_best[2]))
    {
        none_tabu_best[0] = tabu_best[0];
        none_tabu_best[1] = tabu_best[1];
        none_tabu_best[2] = tabu_best[2];
    }

    // update
    for (int adj_point : m_adjacency_table[none_tabu_best[0]])
    {
        m_adjacency_color[adj_point][m_solution[none_tabu_best[0]]]--;
        m_adjacency_color[adj_point][none_tabu_best[1]]++;
    }
    m_current_conflicts += none_tabu_best[2];
    if (m_current_conflicts < m_best_conflicts)
        m_best_conflicts = m_current_conflicts;
    m_solution[none_tabu_best[0]] = none_tabu_best[1];
    m_freq[none_tabu_best[0]][none_tabu_best[1]]++;
    m_freq[none_tabu_best[0]][m_colors_num]++;
    m_tabu_tenure = (
            m_current_conflicts + rand() % 18 + 20 +
            iter_count + 2 * m_colors_num *
            m_freq[none_tabu_best[0]][none_tabu_best[1]] / m_freq[none_tabu_best[0]][m_colors_num]);
    m_tabu_list[none_tabu_best[0]][none_tabu_best[1]] = m_tabu_tenure;
}

/**
 * @brief 禁忌搜索
 *
 * @param max_iter 最大迭代数
 *
 * @version 0.1
 */
void CTabuSearch::search(const int max_iter)
{
    clock_t start = clock();
    int iter_count = 0;
    while (m_current_conflicts)
    {
        this->moveIt(iter_count);
        if (iter_count % 10000 == 0)
        {
            std::cout << "iteration " << iter_count << ": number of conflicts is " << m_current_conflicts << std::endl;
            m_os << "iteration " << iter_count << ": number of conflicts is " << m_current_conflicts << std::endl;
        }
        if (iter_count > max_iter)
        {
            std::cout << "Exceeded maximum number of iterations!" << std::endl;
            m_os << "Exceeded maximum number of iterations!" << std::endl;
            break;
        }
        iter_count++;
        if (m_current_conflicts == 0)
        {
            std::cout << "iteration " << iter_count << ": number of conflicts is " << m_current_conflicts << std::endl;
            m_os << "iteration " << iter_count << ": number of conflicts is " << m_current_conflicts << std::endl;
            m_os << "solution: ";
            for (int v : m_solution)
                m_os << v << " ";
            m_os << std::endl;
        }
    }
    clock_t end = clock();
    double cost = (double)(end - start)/CLOCKS_PER_SEC;
    m_os << "Time consuming: " << cost << "s" << std::endl;
    std::cout << "Time consuming: " << cost << "s" << std::endl;
}
