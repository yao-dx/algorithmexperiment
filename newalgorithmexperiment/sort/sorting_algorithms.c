#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// 全局计数器
long long compare_cnt = 0;

// 子问题规模记录 - 增大数组以支持更大规模
int merge_sub_sizes[200000];
int merge_sub_count = 0;
int quick_sub_sizes[200000];
int quick_sub_count = 0;

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// 冒泡排序
void bubble_sort(int arr[], int n) {
    compare_cnt = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            compare_cnt++;
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// 合并
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        compare_cnt++;
        if (L[i] <= R[j]) { arr[k] = L[i]; i++; }
        else { arr[k] = R[j]; j++; }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
    free(L); free(R);
}

// 合并排序
void merge_sort_internal(int arr[], int left, int right) {
    if (left < right) {
        merge_sub_sizes[merge_sub_count++] = right - left + 1;
        int mid = left + (right - left) / 2;
        merge_sort_internal(arr, left, mid);
        merge_sort_internal(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void merge_sort(int arr[], int left, int right) {
    compare_cnt = 0;
    merge_sub_count = 0;
    merge_sort_internal(arr, left, right);
}

// 快速排序分区
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        compare_cnt++;
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quick_sort_internal(int arr[], int low, int high) {
    if (low < high) {
        quick_sub_sizes[quick_sub_count++] = high - low + 1;
        int pi = partition(arr, low, high);
        quick_sort_internal(arr, low, pi - 1);
        quick_sort_internal(arr, pi + 1, high);
    }
}

void quick_sort(int arr[], int low, int high) {
    compare_cnt = 0;
    quick_sub_count = 0;
    quick_sort_internal(arr, low, high);
}

void generate_random_array(int arr[], int n, unsigned int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000000;
    }
}

void copy_array(int src[], int dest[], int n) {
    memcpy(dest, src, n * sizeof(int));
}

int main() {
    printf("=====================================================\n");
    printf("           排序算法性能分析实验\n");
    printf("=====================================================\n\n");

    // 测试规模
    int sizes[] = {10, 100, 1000, 2000, 5000, 10000, 100000};
    int n_sizes = 7;
    long long bubble_cnt[20], merge_cnt[20], quick_cnt[20];

    printf("【任务1】不同规模数据的比较次数统计\n");
    printf("%-10s %-15s %-15s %-15s\n", "n", "冒泡排序", "合并排序", "快速排序");
    printf("%-10s %-15s %-15s %-15s\n", "", "O(n^2)", "O(n log n)", "O(n log n)");
    printf("-----------------------------------------------------\n");

    for (int i = 0; i < n_sizes; i++) {
        int n = sizes[i];
        int* arr1 = (int*)malloc(n * sizeof(int));
        int* arr2 = (int*)malloc(n * sizeof(int));
        int* arr3 = (int*)malloc(n * sizeof(int));

        generate_random_array(arr1, n, 20241120168 + i);
        copy_array(arr1, arr2, n);
        copy_array(arr1, arr3, n);

        bubble_sort(arr1, n);
        bubble_cnt[i] = compare_cnt;

        merge_sort(arr2, 0, n - 1);
        merge_cnt[i] = compare_cnt;

        quick_sort(arr3, 0, n - 1);
        quick_cnt[i] = compare_cnt;

        printf("%-10d %-15lld %-15lld %-15lld\n", n, bubble_cnt[i], merge_cnt[i], quick_cnt[i]);

        free(arr1); free(arr2); free(arr3);
    }

    // 保存数据
    FILE* fp = fopen("data\\sorting_comparison_counts.csv", "w");
    fprintf(fp, "n,bubble,merge,quick\n");
    for (int i = 0; i < n_sizes; i++) {
        fprintf(fp, "%d,%lld,%lld,%lld\n", sizes[i], bubble_cnt[i], merge_cnt[i], quick_cnt[i]);
    }
    fclose(fp);
    printf("\n数据已保存到 data\\sorting_comparison_counts.csv\n");

    // 任务2: 输入等价类
    printf("\n【任务2】输入等价类演示 (n=100)\n");
    printf("-----------------------------------------------------\n");
    int n = 100;
    int arr1[100], arr2[100], arr3[100];
    int arr4[100], arr5[100], arr6[100];
    int original1[100], original4[100];  // 保存原始数据

    printf("第一次生成随机数据 (seed=12345):\n");
    generate_random_array(arr1, n, 12345);
    copy_array(arr1, original1, n);  // 保存原始数据
    printf("  前10个数据: ");
    for (int i = 0; i < 10; i++) printf("%d ", arr1[i]);
    printf("...\n");
    copy_array(arr1, arr2, n);
    copy_array(arr1, arr3, n);
    bubble_sort(arr1, n);
    long long b1 = compare_cnt;
    merge_sort(arr2, 0, n - 1);
    long long m1 = compare_cnt;
    quick_sort(arr3, 0, n - 1);
    long long q1 = compare_cnt;
    printf("  冒泡=%lld, 合并=%lld, 快速=%lld\n", b1, m1, q1);

    printf("\n第二次生成随机数据 (seed=67890):\n");
    generate_random_array(arr4, n, 67890);
    copy_array(arr4, original4, n);  // 保存原始数据
    printf("  前10个数据: ");
    for (int i = 0; i < 10; i++) printf("%d ", arr4[i]);
    printf("...\n");
    copy_array(arr4, arr5, n);
    copy_array(arr4, arr6, n);
    bubble_sort(arr4, n);
    long long b2 = compare_cnt;
    merge_sort(arr5, 0, n - 1);
    long long m2 = compare_cnt;
    quick_sort(arr6, 0, n - 1);
    long long q2 = compare_cnt;
    printf("  冒泡=%lld, 合并=%lld, 快速=%lld\n", b2, m2, q2);

    printf("\n分析: n*(n-1)/2 = %d\n", n*(n-1)/2);
    printf("  冒泡: 两次相同=%s (属于同一等价类)\n", b1==b2?"是":"否");
    printf("  合并: 两次接近=%s (属于同一等价类)\n", abs(m1-m2)<50?"是":"否");
    printf("  快速: 两次相同=%s (不属于同一等价类)\n", q1==q2?"是":"否");

    // 保存两次生成的随机数据和比较次数
    fp = fopen("data\\sorting_equivalence_class.csv", "w");
    fprintf(fp, "序号,第一次随机数,第二次随机数\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d,%d,%d\n", i+1, original1[i], original4[i]);
    }
    fprintf(fp, "\n比较次数统计\n");
    fprintf(fp, "算法,第一次,第二次,差值,是否同一等价类\n");
    fprintf(fp, "冒泡排序,%lld,%lld,%lld,%s\n", b1, b2, b1-b2, b1==b2?"是":"否");
    fprintf(fp, "合并排序,%lld,%lld,%lld,%s\n", m1, m2, m1-m2, abs(m1-m2)<50?"是":"否");
    fprintf(fp, "快速排序,%lld,%lld,%lld,%s\n", q1, q2, q1-q2, q1==q2?"是":"否");
    fclose(fp);
    printf("\n随机数据和比较次数已保存到 data\\sorting_equivalence_class.csv\n");

    // 任务3: 子问题规模 - 记录每个规模的详细子问题规模
    printf("\n【任务3】递归子问题规模分析\n");
    printf("-----------------------------------------------------\n");

    // 使用与任务1相同的规模
    int test_sizes[] = {10, 100, 1000, 2000, 5000, 10000, 100000};
    int n_test = 7;
    int merge_counts[20], quick_counts[20];

    printf("\n合并排序子问题规模:\n");
    for (int i = 0; i < n_test; i++) {
        int sz = test_sizes[i];
        int* arr = (int*)malloc(sz * sizeof(int));
        generate_random_array(arr, sz, 1000 + i);
        merge_sort(arr, 0, sz - 1);
        merge_counts[i] = merge_sub_count;
        printf("n=%d: 子问题数=%d, 前 10 个：", sz, merge_sub_count);
        for (int j = 0; j < 10 && j < merge_sub_count; j++) {
            printf("%d ", merge_sub_sizes[j]);
        }
        printf("\n");
        free(arr);
    }

    printf("\n快速排序子问题规模:\n");
    for (int i = 0; i < n_test; i++) {
        int sz = test_sizes[i];
        int* arr = (int*)malloc(sz * sizeof(int));
        generate_random_array(arr, sz, 2000 + i);
        quick_sort(arr, 0, sz - 1);
        quick_counts[i] = quick_sub_count;
        printf("n=%d: 子问题数=%d, 前 10 个：", sz, quick_sub_count);
        for (int j = 0; j < 10 && j < quick_sub_count; j++) {
            printf("%d ", quick_sub_sizes[j]);
        }
        printf("\n");
        free(arr);
    }

    // 单独运行每个规模以保存详细的子问题规模
    // 为6个规模保存详细子问题规模（去掉最大的100000以避免文件过大）
    int detail_sizes[] = {10, 100, 1000, 2000, 5000, 10000};
    int n_detail = 6;
    
    // 保存子问题数据 - 完整记录所有规模的统计
    fp = fopen("sorting_subproblem_scales.csv", "w");
    if (fp == NULL) {
        printf("错误：无法创建文件 sorting_subproblem_scales.csv\n");
    } else {
        fprintf(fp, "规模 n,合并排序子问题数,快速排序子问题数\n");
        for (int i = 0; i < n_test; i++) {
            fprintf(fp, "%d,%d,%d\n", test_sizes[i], merge_counts[i], quick_counts[i]);
        }
        
        // 为每个规模保存详细的子问题规模
        for (int d = 0; d < n_detail; d++) {
            int sz = detail_sizes[d];
            int* arr = (int*)malloc(sz * sizeof(int));
            
            // 合并排序
            generate_random_array(arr, sz, 3000 + d);
            merge_sort(arr, 0, sz - 1);
            fprintf(fp, "\n=== 合并排序子问题规模详情 (n=%d) ===\n", sz);
            for (int i = 0; i < merge_sub_count; i++) {
                fprintf(fp, "%d\n", merge_sub_sizes[i]);
            }
            
            // 快速排序
            generate_random_array(arr, sz, 4000 + d);
            quick_sort(arr, 0, sz - 1);
            fprintf(fp, "\n=== 快速排序子问题规模详情 (n=%d) ===\n", sz);
            for (int i = 0; i < quick_sub_count; i++) {
                fprintf(fp, "%d\n", quick_sub_sizes[i]);
            }
            
            free(arr);
        }
        
        fclose(fp);
        printf("已保存子问题数据到 sorting_subproblem_scales.csv\n");
    }

    printf("\n=====================================================\n");
    printf("                     实验完成!\n");
    printf("=====================================================\n");
    printf("请运行: python data\\plot_sorting_charts.py\n");
    printf("=====================================================\n");

    return 0;
}