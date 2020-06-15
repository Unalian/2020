class baggage01:
    def solution(self, wt, val, W, N):
        # 具有实际意义更容易思考，0到W 并且应该初始化为0 或者其他的数字 （还可能初始化为一个很大的数，比如最大上升子序列）
        # 避免浅复制 应该如此声明二维数组
        dp = [[0]*(W+1)for _ in range(N+1)]
        for i in range(1,N+1):
            for j in range(1,W+1):
                if  j < wt[i-1] :# 第i个物体的重量索引是i-1
                    dp[i][j] = dp[i-1][j]
                    continue
                dp[i][j] = max(dp[i-1][j-wt[i-1]] + val[i-1], dp[i-1][j])
        # return的对象也很重要，和dp的定义有关
        return dp[N][W]


a = baggage01()
print(a.solution([2, 1, 3], [4, 2, 3],4,3))


