package com.dxyh.lc53;

public class LC53MaximumSubarray {
  public static void main(String[] args) {
    System.out.println(new Solution().maxSubArray(new int[] {-2,1,-3,4,-1,2,1,-5,4}));
    System.out.println(new Solution1().maxSubArray(new int[] {-2,1,-3,4,-1,2,1,-5,4}));
    System.out.println(new Solution1().maxSubArray(new int[] {-1}));
    System.out.println(new Solution1().maxSubArray(new int[] {-1, -2}));
  }
}
