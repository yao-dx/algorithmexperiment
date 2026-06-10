#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// 物品结构体
typedef struct {
    int id;          // 物品编号
    int weight;      // 重量 (1-100 随机整数)
    double value;    // 价值 (100-1000 随机浮点数，保留2位小数)
    double ratio;    // 价值/重量比 (用于贪心算法)
} Item;

// 计时函数
#ifdef _WIN32
#include <windows.h>
double get_time_ms() {
    LARGE_INTEGER freq, cnt;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&cnt);
    return (double)cnt.QuadPart / freq.QuadPart * 1000;
}
#else
#include <sys/time.h>
double get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}
#endif

// 生成随机测试数据
void generate_random_items(Item* items, int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        items[i].id = i + 1;
        items[i].weight = rand() % 100 + 1;  // 1-100
        items[i].value = floor((rand() % 90000 + 10000) / 100.0 * 100) / 100.0;  // 100.00-1000.00
        items[i].ratio = items[i].value / items[i].weight;
    }
}

// 生成固定测试样例（背包容量10，重量[2,2,6,5,4]，价值[6,3,5,4,6]）
void generate_sample_items(Item* items) {
    int weights[] = {2, 2, 6, 5, 4};
    double values[] = {6.0, 3.0, 5.0, 4.0, 6.0};
    for (int i = 0; i < 5; i++) {
        items[i].id = i + 1;
        items[i].weight = weights[i];
        items[i].value = values[i];
        items[i].ratio = items[i].value / items[i].weight;
    }
}

// 蛮力法（穷举）- 仅适用于 n <= 20
void brute_force_knapsack(Item items[], int n, int capacity, int* selected, double* max_value) {
    *max_value = 0;
    memset(selected, 0, n * sizeof(int));
    
    if (n > 20) {
        printf("  [蛮力法] n=%d 太大，跳过（指数级复杂度）\n", n);
        return;
    }
    
    int* best_selected = (int*)malloc(n * sizeof(int));
    memset(best_selected, 0, n * sizeof(int));
    
    // 枚举所有 2^n 种组合
    for (int mask = 1; mask < (1 << n); mask++) {
        int total_weight = 0;
        double total_value = 0;
        
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                total_weight += items[i].weight;
                total_value += items[i].value;
            }
        }
        
        if (total_weight <= capacity && total_value > *max_value) {
            *max_value = total_value;
            memcpy(best_selected, selected, n * sizeof(int));
            for (int i = 0; i < n; i++) {
                best_selected[i] = (mask & (1 << i)) ? 1 : 0;
            }
        }
    }
    
    memcpy(selected, best_selected, n * sizeof(int));
    free(best_selected);
}

// 动态规划（一维滚动数组优化）
void dynamic_programming_knapsack(Item items[], int n, int capacity, int* selected, double* max_value) {
    // 创建二维 DP 数组用于准确回溯
    int** dp = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)malloc((capacity + 1) * sizeof(int));
        memset(dp[i], 0, (capacity + 1) * sizeof(int));
    }
    
    // 填充 DP 表
    for (int i = 1; i <= n; i++) {
        int w = items[i-1].weight;
        int v = (int)(items[i-1].value * 100 + 0.5);
        
        for (int j = 1; j <= capacity; j++) {
            dp[i][j] = dp[i-1][j];
            if (j >= w && dp[i-1][j - w] + v > dp[i][j]) {
                dp[i][j] = dp[i-1][j - w] + v;
            }
        }
    }
    
    *max_value = dp[n][capacity] / 100.0;
    
    // 回溯找到选中的物品
    memset(selected, 0, n * sizeof(int));
    int j = capacity;
    for (int i = n; i >= 1; i--) {
        if (dp[i][j] != dp[i-1][j]) {
            selected[i-1] = 1;
            j -= items[i-1].weight;
        }
    }
    
    // 释放内存
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);
}

// 贪心算法比较函数（单位价值降序）
int cmp_greedy(const void* a, const void* b) {
    Item* itemA = (Item*)a;
    Item* itemB = (Item*)b;
    if (itemB->ratio > itemA->ratio) return 1;
    if (itemB->ratio < itemA->ratio) return -1;
    return 0;
}

// 贪心法（单位价值优先）- 近似算法
void greedy_knapsack(Item items[], int n, int capacity, int* selected, double* max_value) {
    // 创建物品副本并按单位价值排序
    Item* sorted_items = (Item*)malloc(n * sizeof(Item));
    memcpy(sorted_items, items, n * sizeof(Item));
    qsort(sorted_items, n, sizeof(Item), cmp_greedy);
    
    // 创建映射表记录原始索引
    int* original_index = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (items[j].id == sorted_items[i].id) {
                original_index[i] = j;
                break;
            }
        }
    }
    
    memset(selected, 0, n * sizeof(int));
    *max_value = 0;
    int remaining_capacity = capacity;
    
    // 按单位价值降序选取
    for (int i = 0; i < n; i++) {
        if (sorted_items[i].weight <= remaining_capacity) {
            selected[original_index[i]] = 1;
            *max_value += sorted_items[i].value;
            remaining_capacity -= sorted_items[i].weight;
        }
    }
    
    free(sorted_items);
    free(original_index);
}

// 回溯法（带剪枝）
void backtrack_knapsack(Item items[], int n, int capacity, int* selected, double* max_value) {
    *max_value = 0;
    memset(selected, 0, n * sizeof(int));
    
    if (n > 25) {
        printf("  [回溯法] n=%d 太大，跳过（指数级复杂度）\n", n);
        return;
    }
    
    int* current_selected = (int*)malloc(n * sizeof(int));
    memset(current_selected, 0, n * sizeof(int));
    
    // 按价值/重量比排序，便于剪枝
    Item* sorted_items = (Item*)malloc(n * sizeof(Item));
    memcpy(sorted_items, items, n * sizeof(Item));
    qsort(sorted_items, n, sizeof(Item), cmp_greedy);
    
    // 计算剩余价值上界（贪心估算）
    double* upper_bound = (double*)malloc(n * sizeof(double));
    upper_bound[n-1] = sorted_items[n-1].value;
    for (int i = n-2; i >= 0; i--) {
        upper_bound[i] = upper_bound[i+1] + sorted_items[i].value;
    }
    
    double current_value = 0;
    int current_weight = 0;
    
    // 创建索引映射
    int* original_index = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (items[j].id == sorted_items[i].id) {
                original_index[i] = j;
                break;
            }
        }
    }
    
    // 回溯函数
    void backtrack(int depth) {
        if (depth == n) {
            if (current_value > *max_value) {
                *max_value = current_value;
                memcpy(selected, current_selected, n * sizeof(int));
            }
            return;
        }
        
        // 剪枝：如果加上剩余物品的最大价值仍小于当前最优，则跳过
        if (current_value + upper_bound[depth] <= *max_value) {
            return;
        }
        
        // 选当前物品
        if (current_weight + sorted_items[depth].weight <= capacity) {
            current_selected[original_index[depth]] = 1;
            current_value += sorted_items[depth].value;
            current_weight += sorted_items[depth].weight;
            backtrack(depth + 1);
            current_weight -= sorted_items[depth].weight;
            current_value -= sorted_items[depth].value;
            current_selected[original_index[depth]] = 0;
        }
        
        // 不选当前物品
        backtrack(depth + 1);
    }
    
    backtrack(0);
    
    free(current_selected);
    free(sorted_items);
    free(upper_bound);
    free(original_index);
}

// 输出结果
void print_results(Item items[], int n, int capacity, int* selected, double max_value, double time_ms, const char* algorithm) {
    printf("  算法: %s\n", algorithm);
    printf("  容量: %d\n", capacity);
    printf("  总价值: %.2f\n", max_value);
    printf("  耗时: %.0f ms\n", time_ms);
    
    printf("  选中物品:\n");
    printf("  +-----+--------+--------+\n");
    printf("  | 编号 |  重量  |  价值  |\n");
    printf("  +-----+--------+--------+\n");
    
    int total_weight = 0;
    for (int i = 0; i < n; i++) {
        if (selected[i]) {
            printf("  | %3d | %6d | %6.2f |\n", items[i].id, items[i].weight, items[i].value);
            total_weight += items[i].weight;
        }
    }
    
    printf("  +-----+--------+--------+\n");
    printf("  | 合计 | %6d | %6.2f |\n", total_weight, max_value);
    printf("  +-----+--------+--------+\n\n");
}

// 输出到文件
void save_results_to_file(FILE* fp, int n, int capacity, double bf_time, double bf_value, 
                          double dp_time, double dp_value, 
                          double gr_time, double gr_value, 
                          double bt_time, double bt_value) {
    fprintf(fp, "%d,%d,%.0f,%.2f,%.0f,%.2f,%.0f,%.2f,%.0f,%.2f\n",
            n, capacity,
            bf_time, bf_value,
            dp_time, dp_value,
            gr_time, gr_value,
            bt_time, bt_value);
}

int main() {
    // 记录程序开始时间
    double program_start = get_time_ms();
    
    printf("=====================================================\n");
    printf("              0-1 背包算法实验\n");
    printf("=====================================================\n\n");
    
    // 任务 2: 样例测试（背包容量10，重量[2,2,6,5,4]，价值[6,3,5,4,6]）
    printf("【任务 2】样例测试\n");
    printf("-----------------------------------------------------\n");
    printf("测试用例：背包容量=10，物品数=5\n");
    printf("重量: [2, 2, 6, 5, 4]\n");
    printf("价值: [6, 3, 5, 4, 6]\n");
    printf("预期最优解: 15 (选中物品1,2,5: 重量2+2+4=8, 价值6+3+6=15)\n\n");
    
    Item sample_items[5];
    generate_sample_items(sample_items);
    int sample_selected[5];
    double sample_value;
    double start, end;
    
    // 蛮力法测试
    start = get_time_ms();
    brute_force_knapsack(sample_items, 5, 10, sample_selected, &sample_value);
    end = get_time_ms();
    printf("蛮力法结果:\n");
    print_results(sample_items, 5, 10, sample_selected, sample_value, end - start, "蛮力法");
    
    // 动态规划测试
    start = get_time_ms();
    dynamic_programming_knapsack(sample_items, 5, 10, sample_selected, &sample_value);
    end = get_time_ms();
    printf("动态规划结果:\n");
    print_results(sample_items, 5, 10, sample_selected, sample_value, end - start, "动态规划");
    
    // 贪心法测试
    start = get_time_ms();
    greedy_knapsack(sample_items, 5, 10, sample_selected, &sample_value);
    end = get_time_ms();
    printf("贪心法结果:\n");
    print_results(sample_items, 5, 10, sample_selected, sample_value, end - start, "贪心法");
    
    // 回溯法测试
    start = get_time_ms();
    backtrack_knapsack(sample_items, 5, 10, sample_selected, &sample_value);
    end = get_time_ms();
    printf("回溯法结果:\n");
    print_results(sample_items, 5, 10, sample_selected, sample_value, end - start, "回溯法");
    printf("\n");
    
    // 任务 1 & 任务 3: 大规模测试
    printf("【任务 1 & 3】大规模性能测试\n");
    printf("-----------------------------------------------------\n");
    
    // 测试规模和容量（按照用户要求）
    int n_values[] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 20000, 40000, 80000, 160000, 320000};
    int n_sizes = 15;
    int capacities[] = {10000, 100000, 1000000};
    int c_sizes = 3;
    
    // 打开结果文件
    FILE* result_fp = fopen("../0-1data/knapsack_results.csv", "w");
    if (!result_fp) {
        printf("错误：无法创建结果文件\n");
        return 1;
    }
    
    fprintf(result_fp, "物品数n,容量C,蛮力法时间(ms),蛮力法价值,动态规划时间(ms),动态规划价值,贪心法时间(ms),贪心法价值,回溯法时间(ms),回溯法价值\n");
    
    // 生成 1000 个物品的数据保存到文件
    printf("生成 1000 个物品的测试数据...\n");
    Item* items_1000 = (Item*)malloc(1000 * sizeof(Item));
    generate_random_items(items_1000, 1000);
    
    FILE* data_fp = fopen("../0-1data/knapsack_1000_items.csv", "w");
    if (data_fp) {
        fprintf(data_fp, "编号,重量,价值,价值/重量比\n");
        for (int i = 0; i < 1000; i++) {
            fprintf(data_fp, "%d,%d,%.2f,%.4f\n", 
                    items_1000[i].id, items_1000[i].weight, 
                    items_1000[i].value, items_1000[i].ratio);
        }
        fclose(data_fp);
        printf("已保存 1000 个物品数据到 knapsack_1000_items.csv\n");
    }
    free(items_1000);
    
    // 遍历所有测试用例
    for (int c_idx = 0; c_idx < c_sizes; c_idx++) {
        int capacity = capacities[c_idx];
        printf("\n>>> 测试背包容量 C = %d\n", capacity);
        
        // 先测试小规模数据（用于蛮力法和回溯法）
        int small_n_values[] = {10, 15, 20, 25};
        int small_n_sizes = 4;
        
        printf("\n--- 小规模测试（蛮力法和回溯法）---\n");
        for (int s_idx = 0; s_idx < small_n_sizes; s_idx++) {
            int n = small_n_values[s_idx];
            printf("\n--- n = %d ---\n", n);
            
            Item* items = (Item*)malloc(n * sizeof(Item));
            generate_random_items(items, n);
            
            int* selected = (int*)malloc(n * sizeof(int));
            double value;
            double times[4];
            double values[4];
            
            // 蛮力法
            start = get_time_ms();
            brute_force_knapsack(items, n, capacity, selected, &value);
            end = get_time_ms();
            times[0] = end - start;
            values[0] = value;
            
            // 动态规划
            start = get_time_ms();
            dynamic_programming_knapsack(items, n, capacity, selected, &value);
            end = get_time_ms();
            times[1] = end - start;
            values[1] = value;
            
            // 贪心法
            start = get_time_ms();
            greedy_knapsack(items, n, capacity, selected, &value);
            end = get_time_ms();
            times[2] = end - start;
            values[2] = value;
            
            // 回溯法
            start = get_time_ms();
            backtrack_knapsack(items, n, capacity, selected, &value);
            end = get_time_ms();
            times[3] = end - start;
            values[3] = value;
            
            // 输出结果
            printf("  蛮力法:   价值=%.2f, 时间=%.0f ms\n", values[0], times[0]);
            printf("  动态规划: 价值=%.2f, 时间=%.0f ms\n", values[1], times[1]);
            printf("  贪心法:   价值=%.2f, 时间=%.0f ms (近似)\n", values[2], times[2]);
            printf("  回溯法:   价值=%.2f, 时间=%.0f ms\n", values[3], times[3]);
            
            // 保存到文件
            save_results_to_file(result_fp, n, capacity, 
                                times[0], values[0], 
                                times[1], values[1], 
                                times[2], values[2], 
                                times[3], values[3]);
            
            free(items);
            free(selected);
        }
        
        // 大规模测试（动态规划和贪心法）
        printf("\n--- 大规模测试（动态规划和贪心法）---\n");
        for (int n_idx = 0; n_idx < n_sizes; n_idx++) {
            int n = n_values[n_idx];
            printf("\n--- n = %d ---\n", n);
            
            // 对于超大n，只测试贪心法（内存限制）
            int skip_dp = (n > 100000 || (n > 40000 && capacity >= 100000));
            
            // 生成随机物品
            Item* items = (Item*)malloc(n * sizeof(Item));
            generate_random_items(items, n);
            
            int* selected = (int*)malloc(n * sizeof(int));
            double value;
            double times[4];
            double values[4];
            
            // 蛮力法（跳过）
            times[0] = 0;
            values[0] = 0;
            printf("  蛮力法:   n=%d 太大，跳过\n", n);
            
            // 动态规划（根据规模决定是否跳过）
            if (skip_dp) {
                times[1] = 0;
                values[1] = 0;
                printf("  动态规划: n=%d, C=%d 太大，跳过（内存限制）\n", n, capacity);
            } else {
                start = get_time_ms();
                dynamic_programming_knapsack(items, n, capacity, selected, &value);
                end = get_time_ms();
                times[1] = end - start;
                values[1] = value;
                printf("  动态规划: 价值=%.2f, 时间=%.0f ms\n", values[1], times[1]);
            }
            
            // 贪心法
            start = get_time_ms();
            greedy_knapsack(items, n, capacity, selected, &value);
            end = get_time_ms();
            times[2] = end - start;
            values[2] = value;
            printf("  贪心法:   价值=%.2f, 时间=%.0f ms (近似)\n", values[2], times[2]);
            
            // 回溯法（跳过）
            times[3] = 0;
            values[3] = 0;
            printf("  回溯法:   n=%d 太大，跳过\n", n);
            
            // 保存到文件
            save_results_to_file(result_fp, n, capacity, 
                                times[0], values[0], 
                                times[1], values[1], 
                                times[2], values[2], 
                                times[3], values[3]);
            
            free(items);
            free(selected);
        }
    }
    
    fclose(result_fp);
    
    // 记录程序结束时间并计算总耗时
    double program_end = get_time_ms();
    double total_time = program_end - program_start;
    
    printf("\n=====================================================\n");
    printf("                     实验完成!\n");
    printf("=====================================================\n");
    printf("结果文件: 0-1data/knapsack_results.csv\n");
    printf("物品数据: 0-1data/knapsack_1000_items.csv\n");
    printf("请运行 Python 脚本生成图表\n");
    printf("-----------------------------------------------------\n");
    printf("程序总执行时间: %.2f 秒 (%.0f ms)\n", total_time / 1000, total_time);
    printf("=====================================================\n");
    
    return 0;
}