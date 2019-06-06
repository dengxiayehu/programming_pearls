/* Copyright (C) 1999 Lucent Technologies */
/* From 'Programming Pearls' by Jon Bentley */

/* search.c -- test and time binary and sequential search
   Select one of three modes by editing main() below.
   1.) Probe one function
   2.) Test one function extensively
   3.) Time all functions
		Input lines:  algnum n numtests
		Output lines: algnum n numtests clicks nanosecs_per_elem
		See timedriver for algnum codes
 */

// 瞻仰一下大师的代码。

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXN 1000000

typedef int DataType;

DataType x[MAXN];
int n;

/* Scaffolding */

int i = -999999;
// 自定义assert宏定义，输出出错的情况。
#define assert(v) { if ((v) == 0) printf("  binarysearch bug %d %d\n", i, n); }

/* Alg 1: From Programming Pearls, Column 4: raw transliteration */
// 二分查找算法。
int binarysearch1(DataType t)
{	int l, u, m;
	l = 0;
	u = n-1;
	for (;;) {
		if (l > u)
			return -1;
		m = (l + u) / 2;
		if (x[m] < t)
			l = m+1;
		else if (x[m] == t)
			return m;
		else /* x[m] > t */
			u = m-1;
	}
}

/* Alg 2: Make binarysearch1 more c-ish */
// 二分查找算法，更像C语言的风格。
int binarysearch2(DataType t)
{	int l, u, m;
	l = 0;
	u = n-1;
	while (l <= u) {
		m = (l + u) / 2;
		if (x[m] < t)
			l = m+1;
		else if (x[m] == t)
			return m;
		else /* x[m] > t */
			u = m-1;
	}
	return -1;
}

/* Alg 3: From PP, Col 8 */
// PP是Programming pearls的缩写。
// Col 8表示第八章中会涉及相关知识。
// 这个算法比一般的二分搜索效率更高，至少有2倍+。
int binarysearch3(DataType t)
{	int l, u, m;
	l = -1; // 待查找数据范围的下界-1
	u = n;  // 待查找数据范围的上界+1，当l+1==u时表示范围为空
	while (l+1 != u) {
		m = (l + u) / 2; // 定位到中间元素
		if (x[m] < t)
			l = m; // 更新下界-1
		else
			u = m; // 更新上界+1
	}
	if (u >= n || x[u] != t)
		return -1;
	return u;
}

/* Alg 4: From PP, Col 9 */
// 元素个数为1000时的二分搜索的展开，效率提升得不是一点半点。
int binarysearch4(DataType t)
{	int l, p;
	if (n != 1000)
		return binarysearch3(t);
	l = -1;
	if (x[511]   < t) l = 1000 - 512;
	if (x[l+256] < t) l += 256;
	if (x[l+128] < t) l += 128;
	if (x[l+64 ] < t) l += 64;
	if (x[l+32 ] < t) l += 32;
	if (x[l+16 ] < t) l += 16;
	if (x[l+8  ] < t) l += 8;
	if (x[l+4  ] < t) l += 4;
	if (x[l+2  ] < t) l += 2;
	if (x[l+1  ] < t) l += 1;
	p = l+1;
	if (p >= n || x[p] != t)
		return -1;
	return p;
}

/* Alg 9: Buggy, from Programming Pearls, Column 5 */
// 判断数组是否事先有序，这是二分搜索算法的前提。
int sorted()
{   int i;
    for (i = 0; i < n-1; i++)
        if (x[i] > x[i+1])
            return 0;
    return 1;
}

// 最简单的二分查找思想，但这个算法 ！！有bug！！，若一次搜寻没找到t时，l和u的更新错误。

// 注意看注释掉的部分，主要是关于断言：
// 1 每次循环一次，待搜查的数字范围缩小；
// 2 特定关键位置的assert()，确保在该位置逻辑正确；
int binarysearch9(DataType t)
{	int l, u, m;
/* int oldsize, size = n+1; */
	l = 0;
	u = n-1;
	while (l <= u) {
/* oldsize = size;
size = u - l +1;
assert(size < oldsize); */
		m = (l + u) / 2;
/* printf("  %d %d %d\n", l, m, u); */
		if (x[m] < t)
			l = m; // 错误，应该是l = m+1;
		else if (x[m] > t)
			u = m; // 错误，应该是u = m-1;
		else {
			/* assert(x[m] == t); */
			return m;
		}
	}
	/* assert(x[l] > t && x[u] < t); */
	return -1;
}

/* Alg 21: Simple sequential search */
// 简单的顺序查找算法。
int seqsearch1(DataType t)
{	int i;
	for (i = 0; i < n; i++)
		if (x[i] == t)
			return i;
	return -1;
}

/* Alg 22: Faster sequential search: Sentinel */
// 这个之所以快，是因为少了for循环中的判断判断。
int seqsearch2(DataType t)
{	int i;
	DataType hold = x[n]; 	// 取得哨兵元素原先的值
	x[n] = t; 				// 将t放到x[n]中，这样顺序查找的话，肯定能找到元素t
	for (i = 0; ; i++)
		if (x[i] == t) 		// 找到元素t就返回
			break;
	x[n] = hold; 			// 恢复哨兵元素位置上原先的值
	if (i == n) 			// 找到哨兵元素，说明没有找到t，返回-1
		return -1;
	else
		return i; 			// 成功找到元素t
}

/* Alg 23: Faster sequential search: loop unrolling */
// 充分利用缓存的顺序查找版本。
int seqsearch3(DataType t)
{	int i;
	DataType hold = x[n];
	x[n] = t;
	for (i = 0; ; i+=8) { // 以8个元素为单位进行查找，指令并行等优化
						  // 之所以不用怕越界是因为肯定能“找到元素t”（至少在哨兵元素中找到）
		if (x[i] == t)   {         break; }
		if (x[i+1] == t) { i += 1; break; }
		if (x[i+2] == t) { i += 2; break; }
		if (x[i+3] == t) { i += 3; break; }
		if (x[i+4] == t) { i += 4; break; }
		if (x[i+5] == t) { i += 5; break; }
		if (x[i+6] == t) { i += 6; break; }
		if (x[i+7] == t) { i += 7; break; }
	}
	x[n] = hold;
	if (i == n)
		return -1;
	else
		return i;
}


/* Scaffolding to probe one algorithm */
// 自动化构造一个数组，然后使用特定的搜索算法去查找。
void probe1()
{	int i;
	DataType t;
	while (scanf("%d %d", &n, &t) != EOF) {
		for (i = 0; i < n; i++)
			x[i] = 10*i;
		printf(" %d\n", binarysearch9(t));
	}
}


/* Torture test one algorithm */
// 通过宏定义来测试不同的搜索算法
#define s seqsearch3
void test(int maxn)
{	int i;
	for (n = 0; n <= maxn; n++) {
		printf("n=%d\n", n);
		/* distinct elements (plus one at top) */
		for (i = 0; i <= n; i++)
			x[i] = 10*i;
		for (i = 0; i < n; i++) {
			assert(s(10*i)     ==  i); // 查找成功的情况
			assert(s(10*i - 5) == -1); // 查找失败的情况
		}
		assert(s(10*n - 5) == -1); // 超出最后边界的查找情况
		assert(s(10*n)     == -1); // 确保没有查找到哨兵元素
		/* equal elements */
		for (i = 0; i < n; i++)
			x[i] = 10;
		if (n == 0) {
			assert(s(10) == -1); // 空数组，确保找不到元素
		} else {
			assert(0 <= s(10) && s(10) < n); // 确保找到的元素位置合法
		}
		assert(s(5) == -1); // 测试找不到的情况
		assert(s(15) == -1);
	}
}

/* Timing */

int p[MAXN];

void scramble(int n) // 讲数组p打乱，我们之前学习了一个算法，这里又是一个
{	int i, j;
	DataType t;
	for (i = n-1; i > 0; i--) {
		j = (RAND_MAX*rand() + rand()) % (i + 1);
		t = p[i]; p[i] = p[j]; p[j] = t;
	}
}

void timedriver()
{	int i, algnum, numtests, test, start, clicks;
	while (scanf("%d %d %d", &algnum, &n, &numtests) != EOF) {
		for (i = 0; i < n; i++)
			x[i] = i;
		for (i = 0; i < n; i++)
			p[i] = i;
		scramble(n); // 讲数组p打乱
		start = clock();
		for (test = 0; test < numtests; test++) {
			for (i = 0; i < n; i++) {
				switch (algnum) {
				case 1:  assert(binarysearch1(p[i]) == p[i]); break;
				case 2:  assert(binarysearch2(p[i]) == p[i]); break;
				case 3:  assert(binarysearch3(p[i]) == p[i]); break;
				case 4:  assert(binarysearch4(p[i]) == p[i]); break;
				case 9:  assert(binarysearch9(p[i]) == p[i]); break;
				case 21: assert(seqsearch1(p[i]) == p[i]); break;
				case 22: assert(seqsearch2(p[i]) == p[i]); break;
				case 23: assert(seqsearch3(p[i]) == p[i]); break;
				}
			}
		}
		clicks = clock() - start;
		printf("%d\t%d\t%d\t%d\t%g\n", // 大师的打印方式，均使用制表符
			algnum, n, numtests, clicks,
			1e9*clicks/((float) CLOCKS_PER_SEC*n*numtests)); // 这里代码是对的，书上相关部分的代码示例是错的
	}
}

/* Main */

int main()
{	//probe1();
	//test(25);
	timedriver();
	return 0;
}