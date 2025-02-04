#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 查找结点i的父结点,通过递归得到结点到根的长度
int findParent(int i, int** huffman, int n);
// 根据初始权重构建哈夫曼树
void huffmanTree(int w[], int** huffman, int n);
// 寻找权重最小的两个节点
void findMin(int ii[], int** huffman, int n);
// 对每个叶节点进行哈夫曼编码
void HuffmanCode(int i, int** huffman, int n);
// 交换两个char型数据
void inplace_swap(char* x, char* y);

int main()
{
    char str[100];
    printf("Please Enter the string\n");
    if (fgets(str, sizeof(str), stdin) == NULL) {
        printf("Error reading input\n");
        return -1;
    }
    str[strcspn(str, "\n")] = '\0';

    int arr_big[27] = { 0 };
    int count = 0;
    for (int i = 0; i < strlen(str); i++) {
        for (int j = 0; j < 27; j++) {
            if (str[i] == (char)(0x41 + j)) {
                arr_big[j] += 1;
                if (arr_big[j] == 1) count++;
                break;
            }
        }
    }
    printf("the letter count is %d\n", count);

    // 动态分配arr_small
    int* arr_small = (int*)malloc(count * sizeof(int));
    if (arr_small == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }

    int count2 = 0;
    for (int i = 0; i < 27; i++) {
        if (arr_big[i] > 0) arr_small[count2++] = arr_big[i];
    }

    // 动态分配huffman数组
    int** huffman = (int**)malloc((2 * count - 1) * sizeof(int*));
    if (huffman == NULL) {
        printf("Memory allocation failed\n");
        free(arr_small);
        return -1;
    }
    for (int i = 0; i < 2 * count - 1; i++) {
        huffman[i] = (int*)malloc(4 * sizeof(int));
        if (huffman[i] == NULL) {
            printf("Memory allocation failed\n");
            for (int j = 0; j < i; j++) free(huffman[j]);
            free(huffman);
            free(arr_small);
            return -1;
        }
    }

    // 构建哈夫曼树
    huffmanTree(arr_small, huffman, count);

    // 计算WPL
    int sum = 0;
    for (int i = 0; i < count; i++) {
        if (huffman[i][1] == -1 && huffman[i][2] == -1) {
            int length = findParent(i, huffman, count);
            sum += length * huffman[i][3];
        }
    }
    printf("the tree's WPL is %d\n", sum);

    // 生成编码
    for (int i = 0; i < count; i++) {
        HuffmanCode(i, huffman, count);
    }

    // 释放内存
    free(arr_small);
    for (int i = 0; i < 2 * count - 1; i++) free(huffman[i]);
    free(huffman);

    return 0;
}

// 哈夫曼树构建函数
void huffmanTree(int w[], int** huffman, int n) {
    // 初始化结点
    for (int i = 0; i < 2 * n - 1; i++) {
        for (int j = 0; j < 4; j++) huffman[i][j] = -1;
    }
    for (int i = 0; i < n; i++) huffman[i][3] = w[i];

    // 合并结点
    for (int i = n; i < 2 * n - 1; i++) {
        int ii[2];
        findMin(ii, huffman, i);
        int i1 = ii[0], i2 = ii[1];
        huffman[i][1] = i1;
        huffman[i][2] = i2;
        huffman[i][3] = huffman[i1][3] + huffman[i2][3];
        huffman[i1][0] = i;
        huffman[i2][0] = i;
    }
}

// 寻找最小权重的两个结点
void findMin(int ii[], int** huffman, int n) {
    for (int k = 0; k < 2; k++) {
        int min_val = 9999999;
        for (int i = 0; i < n; i++) { // 注意这里范围应为n而非2n-1
            if (huffman[i][0] == -1 && huffman[i][3] != -1) {
                if (huffman[i][3] < min_val && (k == 0 || i != ii[0])) {
                    min_val = huffman[i][3];
                    ii[k] = i;
                }
            }
        }
    }
}

// 其他函数参数类型修正
int findParent(int i, int** huffman, int n) {
    if (huffman[i][0] == -1) return 0;
    return findParent(huffman[i][0], huffman, n) + 1;
}

void HuffmanCode(int i, int** huffman, int n) {
    char code[30];
    int current = i, father = huffman[i][0], start = 0;
    while (father != -1) {
        code[start++] = (huffman[father][1] == current) ? '0' : '1';
        current = father;
        father = huffman[father][0];
    }
    code[start] = '\0';
    for (int first = 0, last = start - 1; first < last; first++, last--) {
        inplace_swap(&code[first], &code[last]);
    }
    printf("%c Huffman code: %s\n", 'A' + i, code);
}

void inplace_swap(char* x, char* y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}
