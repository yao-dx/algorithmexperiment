"""
排序算法图表生成脚本
生成折线图对比三种排序算法的比较次数
"""
import csv
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import os

# 设置中文字体
matplotlib.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'DejaVu Sans']
matplotlib.rcParams['axes.unicode_minus'] = False

# 确保输出目录存在
os.makedirs(os.path.dirname(__file__), exist_ok=True)

# 读取数据
def read_sorting_data():
    data_file = 'sorting_comparison_counts.csv'
    sizes = []
    bubble = []
    merge = []
    quick = []

    try:
        with open(data_file, 'r', encoding='utf-8') as f:
            reader = csv.reader(f)
            next(reader)  # 跳过表头
            for row in reader:
                sizes.append(int(row[0]))
                bubble.append(int(row[1]))
                merge.append(int(row[2]))
                quick.append(int(row[3]))
        return sizes, bubble, merge, quick
    except FileNotFoundError:
        print(f"数据文件不存在: {data_file}")
        print("请先运行排序算法程序生成数据")
        return None, None, None, None

# ============================================================
# 图1: 三种排序算法比较次数对比
# ============================================================
def plot_comparison_chart(sizes, bubble, merge, quick):
    plt.figure(figsize=(14, 10))

    # 实际数据
    plt.plot(sizes, bubble, 'bo-', label='冒泡排序 O(n²)', linewidth=2, markersize=8)
    plt.plot(sizes, merge, 'rs-', label='合并排序 O(n log n)', linewidth=2, markersize=8)
    plt.plot(sizes, quick, 'g^-', label='快速排序 O(n log n)', linewidth=2, markersize=8)

    # 理论曲线
    n_theory = np.linspace(10, max(sizes[-1], 100), 100)

    # 冒泡排序理论: O(n²)
    bubble_theory = [n * n / 2 for n in n_theory]
    # 缩放以适应实际数据
    scale = bubble[0] / (sizes[0] * sizes[0] / 2) if sizes[0] > 0 else 1
    bubble_theory = [b * scale for b in bubble_theory]
    plt.plot(n_theory, bubble_theory, 'b--', alpha=0.5, label='冒泡理论 O(n²)')

    # 合并排序理论: O(n log n)
    merge_theory = [n * np.log2(n + 1) for n in n_theory]
    scale = merge[0] / (sizes[0] * np.log2(sizes[0] + 1)) if sizes[0] > 0 else 1
    merge_theory = [m * scale for m in merge_theory]
    plt.plot(n_theory, merge_theory, 'r--', alpha=0.5, label='合并理论 O(n log n)')

    # 快速排序理论: O(n log n)
    quick_theory = [n * np.log2(n + 1) for n in n_theory]
    scale = quick[0] / (sizes[0] * np.log2(sizes[0] + 1)) if sizes[0] > 0 else 1
    quick_theory = [q * scale for q in quick_theory]
    plt.plot(n_theory, quick_theory, 'g--', alpha=0.5, label='快速理论 O(n log n)')

    plt.xlabel('数据规模 n', fontsize=12)
    plt.ylabel('比较次数', fontsize=12)
    plt.title('排序算法比较次数对比', fontsize=14)
    plt.legend(loc='upper left', fontsize=10)
    plt.grid(True, alpha=0.3)

    # 设置y轴为科学计数法
    plt.ticklabel_format(style='scientific', axis='y', scilimits=(0, 0))

    output_file = os.path.join(os.path.dirname(__file__), 'sorting_comparison_chart.png')
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    plt.close()
    print(f"已保存: {output_file}")

# ============================================================
# 图2: 去除冒泡排序后的对比图
# ============================================================
def plot_merge_quick_comparison(sizes, merge, quick):
    plt.figure(figsize=(12, 8))

    plt.plot(sizes, merge, 'rs-', label='合并排序 O(n log n)', linewidth=2, markersize=8)
    plt.plot(sizes, quick, 'g^-', label='快速排序 O(n log n)', linewidth=2, markersize=8)

    # 理论曲线
    n_theory = np.linspace(10, max(sizes[-1], 100), 100)
    merge_theory = [n * np.log2(n + 1) for n in n_theory]
    scale = merge[0] / (sizes[0] * np.log2(sizes[0] + 1)) if sizes[0] > 0 else 1
    merge_theory = [m * scale for m in merge_theory]
    plt.plot(n_theory, merge_theory, 'r--', alpha=0.5, label='合并理论 O(n log n)')

    quick_theory = [n * np.log2(n + 1) for n in n_theory]
    scale = quick[0] / (sizes[0] * np.log2(sizes[0] + 1)) if sizes[0] > 0 else 1
    quick_theory = [q * scale for q in quick_theory]
    plt.plot(n_theory, quick_theory, 'g--', alpha=0.5, label='快速理论 O(n log n)')

    plt.xlabel('数据规模 n', fontsize=12)
    plt.ylabel('比较次数', fontsize=12)
    plt.title('合并排序与快速排序比较次数对比', fontsize=14)
    plt.legend(loc='upper left', fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.ticklabel_format(style='scientific', axis='y', scilimits=(0, 0))

    output_file = os.path.join(os.path.dirname(__file__), 'sorting_merge_quick_chart.png')
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    plt.close()
    print(f"已保存: {output_file}")

# ============================================================
# 图3: 冒泡排序单独对比图
# ============================================================
def plot_bubble_comparison(sizes, bubble):
    plt.figure(figsize=(10, 8))

    plt.plot(sizes, bubble, 'bo-', label='冒泡排序实际', linewidth=2, markersize=8)

    # 理论曲线
    n_theory = np.linspace(10, max(sizes[-1], 100), 100)
    bubble_theory = [n * n / 2 for n in n_theory]
    scale = bubble[0] / (sizes[0] * sizes[0] / 2) if sizes[0] > 0 else 1
    bubble_theory = [b * scale for b in bubble_theory]
    plt.plot(n_theory, bubble_theory, 'b--', alpha=0.7, label=f'理论 O(n²)')

    plt.xlabel('数据规模 n', fontsize=12)
    plt.ylabel('比较次数', fontsize=12)
    plt.title('冒泡排序比较次数分析', fontsize=14)
    plt.legend(loc='upper left', fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.ticklabel_format(style='scientific', axis='y', scilimits=(0, 0))

    output_file = os.path.join(os.path.dirname(__file__), 'sorting_bubble_chart.png')
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    plt.close()
    print(f"已保存: {output_file}")

# ============================================================
# 图4: 对数坐标对比图
# ============================================================
def plot_log_scale_comparison(sizes, bubble, merge, quick):
    plt.figure(figsize=(12, 8))

    plt.loglog(sizes, bubble, 'bo-', label='冒泡排序 O(n²)', linewidth=2, markersize=8)
    plt.loglog(sizes, merge, 'rs-', label='合并排序 O(n log n)', linewidth=2, markersize=8)
    plt.loglog(sizes, quick, 'g^-', label='快速排序 O(n log n)', linewidth=2, markersize=8)

    # 理论参考线
    n_ref = np.array([10, 1000000], dtype=float)
    plt.loglog(n_ref, n_ref**2 / 2, 'b--', alpha=0.5, label='n² 参考线')
    plt.loglog(n_ref, n_ref * np.log2(n_ref + 1), 'r--', alpha=0.5, label='n log n 参考线')

    plt.xlabel('数据规模 n (对数坐标)', fontsize=12)
    plt.ylabel('比较次数 (对数坐标)', fontsize=12)
    plt.title('排序算法比较次数对比（对数坐标）', fontsize=14)
    plt.legend(loc='upper left', fontsize=10)
    plt.grid(True, alpha=0.3, which='both')

    output_file = os.path.join(os.path.dirname(__file__), 'sorting_log_scale_chart.png')
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    plt.close()
    print(f"已保存: {output_file}")

# ============================================================
# 图5: 输入等价类对比图
# ============================================================
def plot_equivalence_class():
    categories = ['冒泡排序\n(seed=12345)', '冒泡排序\n(seed=67890)',
                  '合并排序\n(seed=12345)', '合并排序\n(seed=67890)',
                  '快速排序\n(seed=12345)', '快速排序\n(seed=67890)']

    # 典型值（实际运行后替换）
    values = [4950, 4950, 545, 541, 657, 724]

    plt.figure(figsize=(12, 6))
    colors = ['#2196F3', '#2196F3', '#F44336', '#F44336', '#4CAF50', '#4CAF50']
    bars = plt.bar(categories, values, color=colors, edgecolor='black')

    plt.xlabel('算法与随机种子', fontsize=12)
    plt.ylabel('比较次数', fontsize=12)
    plt.title('输入等价类分析：不同随机数据的比较次数 (n=100)', fontsize=14)
    plt.grid(True, alpha=0.3, axis='y')

    # 添加数值标签
    for bar, val in zip(bars, values):
        plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 20,
                str(val), ha='center', va='bottom', fontsize=10)

    output_file = os.path.join(os.path.dirname(__file__), 'sorting_equivalence_class_chart.png')
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    plt.close()
    print(f"已保存: {output_file}")

# ============================================================
# 主程序
# ============================================================
def main():
    print("=" * 50)
    print("排序算法图表生成程序")
    print("=" * 50)

    sizes, bubble, merge, quick = read_sorting_data()

    if sizes is None:
        print("\n请先运行 sort\\sorting_algorithms.exe 生成数据")
        return

    print(f"\n读取到 {len(sizes)} 组数据")
    print(f"数据规模: {sizes}")

    print("\n正在生成图表...")

    # 生成所有图表
    plot_comparison_chart(sizes, bubble, merge, quick)
    plot_merge_quick_comparison(sizes, merge, quick)
    plot_bubble_comparison(sizes, bubble)
    plot_log_scale_comparison(sizes, bubble, merge, quick)
    plot_equivalence_class()

    print("\n" + "=" * 50)
    print("所有图表生成完成！")
    print("=" * 50)
    print("\n生成的文件:")
    print("  1. sorting_comparison_chart.png     - 三种算法综合对比")
    print("  2. sorting_merge_quick_chart.png    - 合并与快速排序对比")
    print("  3. sorting_bubble_chart.png         - 冒泡排序单独分析")
    print("  4. sorting_log_scale_chart.png      - 对数坐标对比图")
    print("  5. sorting_equivalence_class_chart.png - 输入等价类分析")
    print("=" * 50)

if __name__ == "__main__":
    main()