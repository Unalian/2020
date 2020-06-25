class lt516:
    def Solution(self,s):
        n = len(s)
        dp = [[0]*n for _ in range(n)]
        for i in range(0,n):
            dp[i][i] = 1  # 基础情况
        for i in range(n-2,-1,-1):
            for j in range(i+1,n):
                if s[i] == s[j]:
                    dp[i][j] = dp[i+1][j-1]+2 # 总体加2 i+1
                else:
                    dp[i][j] = max(dp[i+1][j], dp[i][j-1])
        return dp[0][n-1]

a = lt516()
print(a.Solution("bbbab"))