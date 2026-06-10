import csv
import matplotlib.pyplot as plt

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei']
plt.rcParams['axes.unicode_minus'] = False

# 读取数据
n_values = []
capacities = [10000, 100000, 1000000]
data = {c: {'dp_time': [], 'dp_value': [], 'gr_time': [], 'gr_value': []} for c in capacities}

with open('knapsack_results.csv', 'r', encoding='utf-8') as f:
    reader = csv.reader(f)
    header = next(reader)  # 跳过表头
    for row in reader:
        n = int(row[0])
        c = int(row[1])
        dp_time = float(row[4])
        dp_value = float(row[5])
        gr_time = float(row[6])
        gr_value = float(row[7])
        
        if n not in n_values:
            n_values.append(n)
        data[c]['dp_time'].append(dp_time)
        data[c]['dp_value'].append(dp_value)
        data[c]['gr_time'].append(gr_time)
        data[c]['gr_value'].append(gr_value)

# 生成 C=10000 的图表
plt.figure(figsize=(12, 8))
plt.plot(n_values, data[10000]['dp_time'], marker='o', label='动态规划 O(n*C)', linewidth=2)
plt.plot(n_values, data[10000]['gr_time'], marker='s', label='贪心法 O(n log n)', linewidth=2)
plt.xlabel('物品数量 (n)')
plt.ylabel('执行时间 (ms)')
plt.title('0-1 背包算法性能对比 - C=10000')
plt.legend()
plt.grid(True)
plt.savefig('knapsack_C10000.png', dpi=150, bbox_inches='tight')
print('已生成: knapsack_C10000.png')
plt.close()

# 生成 C=100000 的图表
plt.figure(figsize=(12, 8))
plt.plot(n_values, data[100000]['dp_time'], marker='o', label='动态规划 O(n*C)', linewidth=2)
plt.plot(n_values, data[100000]['gr_time'], marker='s', label='贪心法 O(n log n)', linewidth=2)
plt.xlabel('物品数量 (n)')
plt.ylabel('执行时间 (ms)')
plt.title('0-1 背包算法性能对比 - C=100000')
plt.legend()
plt.grid(True)
plt.savefig('knapsack_C100000.png', dpi=150, bbox_inches='tight')
print('已生成: knapsack_C100000.png')
plt.close()

# 生成 C=1000000 的图表
plt.figure(figsize=(12, 8))
plt.plot(n_values, data[1000000]['dp_time'], marker='o', label='动态规划 O(n*C)', linewidth=2)
plt.plot(n_values, data[1000000]['gr_time'], marker='s', label='贪心法 O(n log n)', linewidth=2)
plt.xlabel('物品数量 (n)')
plt.ylabel('执行时间 (ms)')
plt.title('0-1 背包算法性能对比 - C=1000000')
plt.legend()
plt.grid(True)
plt.savefig('knapsack_C1000000.png', dpi=150, bbox_inches='tight')
print('已生成: knapsack_C1000000.png')
plt.close()

print('所有图表生成完成！')