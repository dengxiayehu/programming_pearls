### 程序性能分析

若计算机快1000倍，对于时间复杂度是$O(n^2)$的程序，相同的时间下可处理$\sqrt{1000}n$倍的数据；而对于时间复杂度是$n\log_2{n}$的程序，相同时间下可处理约590倍的数据。  
参考链接<https://blog.csdn.net/yellowstar5/article/details/52352188> （未实际验证）

关于单精度与双精度数在运算时的效率问题，结论是：若是float表示的数范围够的话，优先使用float类型。

"在提高效率之前先保证正确性"通常是一个好建议。但是若运行该系统非常慢，例如要好几个小时，然而它今天有10个已知的错误，下个月又将出现10个错误。可能这时候，会优先选择优化时间（也就是效率问题），其他出现的错误再慢慢解决掉。
