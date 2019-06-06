package com.dxyh.lc53;

public class Solution1 {
  public int maxSubArray(int[] nums) {
    if (nums.length == 0) {
      return 0;
    }
    return maxSubArray(nums, 0, nums.length - 1);
  }
  
  private static int maxSubArray(int[] nums, int left, int right) {
    if (left >= right) {
      return nums[left];
    }
    final int mid = (left + right) / 2;
    int maxLeftSubArray = maxSubArray(nums, left, mid - 1);
    int maxRightSubArray = maxSubArray(nums, mid + 1, right);
    int maxBorderSum = nums[mid];
    for (int i = mid - 1, tmp = maxBorderSum; i >= left; i--) {
      tmp += nums[i];
      maxBorderSum = Math.max(maxBorderSum, tmp);
    }
    for (int i = mid + 1, tmp = maxBorderSum; i <= right; i++) {
      tmp += nums[i];
      maxBorderSum = Math.max(maxBorderSum, tmp);
    }
    return max3sum(maxBorderSum, maxLeftSubArray, maxRightSubArray);
  }
  
  private static int max3sum(int a, int b, int c) {
    return Math.max(a, Math.max(b, c));
  }
}
