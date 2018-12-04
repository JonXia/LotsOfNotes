package com.lon.leetcode.array;

/**
 * @program: jweb
 * @description: 给定一个数组 nums，编写一个函数将所有 0 移动到数组的末尾，同时保持非零元素的相对顺序。
 *
 * 示例:
 *
 * 输入: [0,1,0,3,12]
 * 输出: [1,3,12,0,0]
 * 说明:
 *
 * 必须在原数组上操作，不能拷贝额外的数组。
 * 尽量减少操作次数。
 * @author: John_Xia
 * @create: 2018-12-04 22:27
 */
public class MoveZeros {

    public static void main(String[] args) {
        int [] x=new int[]{0,1,0,3,12};
        moveZeroes(x);
        for (int i = 0; i < x.length; i++) {
            System.out.print(x[i]+" ");
        }

    }
    public static void moveZeroes(int[] nums) {
        if(nums == null || nums.length == 0){
            return;
        }
        //记录非o元素开始位置
        int k = 0;
        for(int i=0;i<nums.length;i++){
            if(nums[i] != 0) {
                nums[k++] = nums[i];
            }
        }
        while(k < nums.length) {
            nums[k] = 0;
            k++;
        }
    }
}
