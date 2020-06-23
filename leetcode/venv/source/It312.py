class It312:
    def Solution(self,nums):
        n = len(nums)
        sol = [1] * (n + 2)
        i = 1
        for num in nums:
            sol[i] = num
            i = i+1
        dp = [[0] * (n+2) for _ in range(n+2)]
        for i in range(n, -1, -1):
            for j in range(i+1, n+2):
                for k in range(i+1, j):
                    dp[i][j] = max(dp[i][k] + dp[k][j] + sol[i]*sol[j]*sol[k], dp[i][j])
        return dp[0][n+1]

a = It312()
print(a.Solution([3,1,5,8]))


