class It416:
    def solution(self,nums):
        sums = 0.0
        for i in nums:
            sums = sums + i
        if sums % 2 == 1:
            return False
        sums = sums / 2
        sums = int(sums) # 注意这里对sums int和float的转换
        dp = [[False] * (sums + 1) for _ in range(len(nums)+1)]
        for i in range(len(nums)):
            dp[i][0] = True # False and True

        for i in range(1, len(nums)+1):
            for j in range(1, sums + 1):
                if j < nums[i-1]:
                    dp[i][j] = dp[i-1][j]
                else:
                    dp[i][j] = dp[i-1][j]|dp[i-1][j-nums[i-1]]
        return dp[len(nums)][sums]


a = It416()
print(a.solution2([1,2,5]))