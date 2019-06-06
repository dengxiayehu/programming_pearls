package com.dxyh.lc53;

public class Solution {
  public int maxSubArray(int[] nums) {
    int maxSum = nums[0];
    int res = maxSum;
    for (int i = 1; i < nums.length; i++) {
      maxSum = Math.max(maxSum + nums[i], nums[i]);
      if (maxSum > res) {
        res = maxSum;
      }
    }
    return res;
  }
}
