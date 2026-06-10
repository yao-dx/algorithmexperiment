import csv
import matplotlib.pyplot as plt

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei']
plt.rcParams['axes.unicode_minus'] = False

# 读取数据
data = {}

with open('knapsack_results.csv', 'r', encoding='utf-8-sig') as f:
    reader = csv.reader(f)
    header = next(reader)  # 跳过表头
    for row in reader:
        n = int(row[0])
        c = int(row[1])
        dp_time = float(row[4])
        dp_value = float(row[5])
        gr_time = float(row[6])
        gr_value = float(row[7])
        
        if c not in data:
            data[c] = {'n': [], 'dp_time': [], 'dp_value': [], 'gr_time': [], 'gr_value': []}
        
        data[c]['n'].append(n)
        data[c]['dp_time'].append(dp_time)
        data[c]['dp_value'].append(dp_value)
        data[c]['gr_time'].append(gr_time)
        data[c]['gr_value'].append(gr_value)

# 为每个容量生成图表
for c in data:
    plt.figure(figsize=(12, 8))
    plt.plot(data[c]['n'], data[c]['dp_time'], marker='o', label='动态规划 O(n*C)', linewidth=2)
    plt.plot(data[c]['n'], data[c]['gr_time'], marker='s', label='贪心法 O(n log n)', linewidth=2)
    plt.xlabel('物品数量 (n)')
    plt.ylabel('执行时间 (ms)')
    plt.title(f'0-1 背包算法性能对比 - C={c}')
    plt.legend()
    plt.grid(True)
    plt.savefig(f'knapsack_C{c}.png', dpi=150, bbox_inches='tight')
    print(f'已生成: knapsack_C{c}.png')
    plt.close()

# 综合对比图（只对比有完整数据的容量）
plt.figure(figsize=(12, 8))
for c in data:
    if len(data[c]['n']) >= 5:  # 只显示数据较多的
        plt.plot(data[c]['n'], data[c]['dp_time'], marker='o', label=f'动态规划 (C={c})', linewidth=2)
        plt.plot(data[c]['n'], data[c]['gr_time'], marker='s', label=f'贪心法 (C={c})', linewidth=2)

plt.xlabel('物品数量 (n)')
plt.ylabel('执行时间 (ms)')
plt.title('0-1 背包算法性能对比')
plt.legend()
plt.grid(True)
plt.savefig('knapsack_all.png', dpi=150, bbox_inches='tight')
print('已生成: knapsack_all.png')
plt.close()

print('所有图表生成完成！')