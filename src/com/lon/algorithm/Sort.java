package com.lon.algorithm;

/**
 * @program: jweb
 * @description:
 * @author: John_Xia
 * @create: 2018-12-02 10:57
 */
public class Sort {
    public static void main(String[] args) {
        int x[]=new int[]{2,1,4,3,5};
        selectionSort(x);
        for (int j = 0; j <x.length ; j++) {
            System.out.println(x[j]);
        }


    }

    /**
     * bubbleSort
     * @param arr
     */
    public static void bubbleSort(int [] arr){
        // 每次需要排序的数组的长度
        int temp;
        for (int i = arr.length-1; i >0 ; i--) {
            // 当数组已经有序的时候，直接退出，不进行接下来的循环。
            boolean breakloop = true;
            for (int j = 0; j <i; j++) {
                if (arr[j]>arr[j+1]){
                    temp=arr[j+1];
                    arr[j+1]=arr[j];
                    arr[j]=temp;
                    breakloop=false;
                }
            }
            if (breakloop == true) {
                break;
            }

        }

    }

    /**
     * selectionSort
     * @param arr
     */
    public static void selectionSort(int [] arr){
        int temp, min = 0;
        // 比较的次数
        for (int i = 0; i < arr.length-1; i++) {

            //找到最小数的下标
            for (int j = i+1; j < arr.length; j++) {
                if (arr[min] > arr[j]) {
                    min=j;

                }
            }// loop j
            //与最前面的做交换


        }// loop i
    }
}

