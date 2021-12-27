from typing import List

class Solution:
    def longestSubarray(self, nums: List[int], limit: int) -> int:


if __name__ == '__main__':
    s = Solution()
    test_list = [
        (),
    ]
    for test_index, test_case in enumerate(test_list, start=1):
        *test, result = test_case
        test_result = s.longestSubarray(*test)
        if test_result != result:
            raise ValueError("\n testcase %d error:\n expect: %s \n actually %s" % (test_index, result, test_result))
        print("test_case %d succeed." % test_index)
