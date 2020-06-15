class It322:
    def solution(self, amount, coins):
        dp = [amount + 1]*(amount+1)
        dp[0] = 0
        for i in range(1,amount+1):
            for coin in coins:
                if i  < coin:
                    continue
                dp[i] = min(dp[i - coin]+1, dp[i])
        if dp[amount] == amount + 1: return -1
        return dp[amount]

a = It322()
print(a.solution(2, [3]))
