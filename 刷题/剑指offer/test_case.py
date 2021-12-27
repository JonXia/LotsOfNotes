import unittest

from collections import *
class MyTestCase(unittest.TestCase):

    def majorityElement(self, nums) -> int:
        # 摩尔投票
        vote = 0
        res = 0
        for num in nums:
            if vote == 0:
                # 假定 num 为众数
                res = num
            if num == res:
                vote += 1
            else:
                vote -= 1

        return res

    def testList(self):
        a = UserList([1, 2, 3])
        print(a)
    def test_something(self):
        # self.assertEqual(True, False)
        nums = [0,6,2,6,1]
        print(self.majorityElement(nums))

    def test_groupby(self):
        import pandas as pd
        import numpy as np
        import matplotlib.pyplot as plt
        df = pd.DataFrame({"Name": ["Alice", "Bob", "Mallory", "Mallory", "Bob",
                                    "Mallory"],
                           "City": ["Seattle", "Seattle", "Portland", "Seattle", "Seattle", "Portland"],
                           "Val": [4, 3, 3, np.nan, np.nan, 4]})

        print(df)
        df1 = df.groupby(["Name", "City"], as_index=False)['Val'].count()
        print('--------------------------groupby(["Name", "City"], as_index=False)[Val].count()')
        print(df1)

        df1 = df.groupby(["City"], as_index=False).count()  # count把每一列的值都返回出现的次数
        print('--------------------------groupby([City], as_index=False).count()')
        print(df1)

        df1 = df.groupby(["City"], as_index=False).size()  # size只返回分组字段的出现次数
        print('--------------------------.groupby(["City"], as_index=False).size()')
        print(df1)

        df2 = df.groupby(["Name", "City"], as_index=False).count()
        print('--------------------------df.groupby(["Name", "City"], as_index=False).count()')
        print(df2)

        df3 = df.groupby(["Name", "City"])['Val'].size().reset_index(name='Size')
        print('--------------------------groupby(["Name", "City"])[Val].size().reset_index(name=Size)')

        print(df3)

        df4 = df.groupby(["Name", "City"]).size()
        print('--------------------------groupby(["Name", "City"]).size()')

        print(df4)


if __name__ == '__main__':
    import pandas as pd
    import numpy as np
    import matplotlib.pyplot as plt
    unittest.main()
