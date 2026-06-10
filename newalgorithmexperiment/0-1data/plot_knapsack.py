import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'DejaVu Sans']
plt.rcParams['axes.unicode_minus'] = False

# 读取数据
df = pd.read_csv('knapsack_results.csv')

# 三种容量
capacities = [10000, 100000, 1000000]
capacity_names = ['C=10000', 'C=100000', 'C=1000000']

# 创建图表目录
os.makedirs('charts', exist_ok=True)

# 为每种容量生成单独的图表（带理论复杂度对比）
for c, c_name in zip(capacities, capacity_names):
    # 筛选当前容量的数据
    df_c = df[df['容量C'] == c]
    
    plt.figure(figsize=(14, 10))
    
    # 获取数据点
    n_values = df_c['物品数n'].values
    dp_times = df_c['动态规划时间(ms)'].values
    greedy_times = df_c['贪心法时间(ms)'].values
    
    # 绘制实际算法曲线
    plt.plot(n_values, dp_times, marker='o', label='动态规划 (实际)', linewidth=2, color='blue')
    plt.plot(n_values, greedy_times, marker='s', label='贪心法 (实际)', linewidth=2, color='green')
    
    # 绘制蛮力法和回溯法小数据点
    df_small = df_c[df_c['物品数n'] <= 20]
    if not df_small.empty:
        plt.plot(df_small['物品数n'], df_small['蛮力法时间(ms)'], marker='^', label='蛮力法 (实际)', linewidth=2, color='red')
        plt.plot(df_small['物品数n'], df_small['回溯法时间(ms)'], marker='d', label='回溯法 (实际)', linewidth=2, color='purple')
    
    # 计算理论复杂度参考线（归一化）
    if len(n_values) > 0:
        # 动态规划 O(n*C) 理论参考
        dp_theory = n_values * c / 100000
        dp_theory = dp_theory * (dp_times[-1] / dp_theory[-1]) if dp_times[-1] > 0 else dp_theory
        plt.plot(n_values, dp_theory, linestyle='--', label=f'理论 O(n*C)', color='blue', alpha=0.6)
        
        # 贪心法 O(n log n) 理论参考
        greedy_theory = n_values * np.log2(n_values)
        greedy_theory = greedy_theory * (greedy_times[-1] / greedy_theory[-1]) if greedy_times[-1] > 0 else greedy_theory
        plt.plot(n_values, greedy_theory, linestyle='--', label='理论 O(n log n)', color='green', alpha=0.6)
    
    plt.xlabel('物品数量 (n)', fontsize=12)
    plt.ylabel('执行时间 (ms)', fontsize=12)
    plt.title(f'0-1 背包算法性能对比 - {c_name}（含理论复杂度）', fontsize=14)
    plt.legend(fontsize=10)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.ticklabel_format(style='plain', axis='x')
    
    # 保存图表
    plt.savefig(f'charts/knapsack_performance_{c}.png', dpi=150, bbox_inches='tight')
    plt.savefig(f'knapsack_C{c}.png', dpi=150, bbox_inches='tight')
    plt.close()
    
    print(f'已生成图表: charts/knapsack_performance_{c}.png 和 knapsack_C{c}.png')

# 生成时间复杂度分析图（对数坐标）
plt.figure(figsize=(14, 10))

df_c = df[df['容量C'] == 10000]
n_values = df_c['物品数n'].values
dp_times = df_c['动态规划时间(ms)'].values
greedy_times = df_c['贪心法时间(ms)'].values

# 实际曲线
plt.semilogy(n_values, dp_times, marker='o', label='动态规划 (实际)', linewidth=2, color='blue')
plt.semilogy(n_values, greedy_times, marker='s', label='贪心法 (实际)', linewidth=2, color='green')

# 理论复杂度参考线
if len(n_values) > 0:
    # O(n) 参考线
    o_n = n_values * (dp_times[-1] / n_values[-1])
    plt.semilogy(n_values, o_n, linestyle='-.', label='理论 O(n)', color='gray', alpha=0.5)
    
    # O(n log n) 参考线
    o_nlogn = n_values * np.log2(n_values)
    o_nlogn = o_nlogn * (greedy_times[-1] / o_nlogn[-1]) if greedy_times[-1] > 0 else o_nlogn
    plt.semilogy(n_values, o_nlogn, linestyle='--', label='理论 O(n log n)', color='green', alpha=0.6)
    
    # O(n*C) 参考线
    o_nc = n_values * 10000 / 10000
    o_nc = o_nc * (dp_times[-1] / o_nc[-1])
    plt.semilogy(n_values, o_nc, linestyle='--', label='理论 O(n*C)', color='blue', alpha=0.6)

plt.xlabel('物品数量 (n)', fontsize=12)
plt.ylabel('执行时间 (ms) - 对数刻度', fontsize=12)
plt.title('0-1 背包算法时间复杂度分析（对数坐标）', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True, linestyle='--', alpha=0.7)
plt.savefig('charts/knapsack_complexity_analysis.png', dpi=150, bbox_inches='tight')
plt.close()

print('已生成图表: charts/knapsack_complexity_analysis.png')

# 生成算法对比总结图（小规模）
plt.figure(figsize=(12, 8))

# 只使用C=10000的数据，避免不同容量数据混合
df_c = df[df['容量C'] == 10000]
df_small = df_c[df_c['物品数n'] <= 20]

if not df_small.empty:
    # 所有算法都用折线图（显示增长趋势）
    plt.plot(df_small['物品数n'], df_small['蛮力法时间(ms)'], marker='^', label='蛮力法', linewidth=2, color='red')
    plt.plot(df_small['物品数n'], df_small['回溯法时间(ms)'], marker='d', label='回溯法', linewidth=2, color='purple')
    plt.plot(df_small['物品数n'], df_small['动态规划时间(ms)'], marker='o', label='动态规划', linewidth=2, color='blue')
    plt.plot(df_small['物品数n'], df_small['贪心法时间(ms)'], marker='s', label='贪心法', linewidth=2, color='green')

plt.xlabel('物品数量 (n)', fontsize=12)
plt.ylabel('执行时间 (ms)', fontsize=12)
plt.title('0-1 背包算法小规模数据对比 (n <= 20)', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True, linestyle='--', alpha=0.7)
plt.savefig('charts/knapsack_small_scale.png', dpi=150, bbox_inches='tight')
plt.savefig('knapsack_small_scale.png', dpi=150, bbox_inches='tight')
plt.close()

print('已生成图表: charts/knapsack_small_scale.png')

# 生成所有容量对比图
plt.figure(figsize=(14, 10))

for c, c_name in zip(capacities, capacity_names):
    df_c = df[df['容量C'] == c]
    plt.plot(df_c['物品数n'], df_c['动态规划时间(ms)'], marker='o', label=f'动态规划 ({c_name})', linewidth=2)
    plt.plot(df_c['物品数n'], df_c['贪心法时间(ms)'], marker='s', label=f'贪心法 ({c_name})', linewidth=2)

plt.xlabel('物品数量 (n)', fontsize=12)
plt.ylabel('执行时间 (ms)', fontsize=12)
plt.title('0-1 背包算法性能对比 - 不同容量', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True, linestyle='--', alpha=0.7)
plt.savefig('charts/knapsack_all_capacities.png', dpi=150, bbox_inches='tight')
plt.savefig('knapsack_all.png', dpi=150, bbox_inches='tight')
plt.close()

print('已生成图表: charts/knapsack_all_capacities.png 和 knapsack_all.png')

# 生成价值对比图
plt.figure(figsize=(12, 8))

for c, c_name in zip(capacities, capacity_names):
    df_c = df[df['容量C'] == c]
    plt.plot(df_c['物品数n'], df_c['动态规划价值'], marker='o', label=f'动态规划 ({c_name})', linewidth=2)
    plt.plot(df_c['物品数n'], df_c['贪心法价值'], marker='s', label=f'贪心法 ({c_name})', linewidth=2)

plt.xlabel('物品数量 (n)', fontsize=12)
plt.ylabel('总价值', fontsize=12)
plt.title('0-1 背包算法价值对比', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True, linestyle='--', alpha=0.7)
plt.savefig('charts/knapsack_value_comparison.png', dpi=150, bbox_inches='tight')
plt.close()

print('已生成图表: charts/knapsack_value_comparison.png')

print('\n所有图表生成完成！')
