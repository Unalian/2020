class It509:
    def fib(self,N):
        dp = [0]*(N+1)
        if N == 0:
            return 0
        if N == 1:
            return 1
        dp[0] = 0
        dp[1] = 1
        for i in range(2, N+1):
            dp[i] = dp[i-1]+dp[i-2]
        return dp[N]
    def fib2(self,N):
        if N == 0:
            return 0
        if N == 1:
            return 1
        pre = 0
        cur = 1
        for i in range(2,N+1):
            res = pre + cur
            pre = cur
            cur = res
        return res


a = It509()
print(a.fib2(5))