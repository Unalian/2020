
class It877(object):
    def stoneGame(self, piles):
        n = len(piles)
        dp = [[[0 for _ in range(2)]for _ in range(n)]for _ in range(n)]

        for i in range(n):
            dp[i][i] = [piles[i],0]
            print("%d %d first: %d" % (i, i, dp[i][i][0]))
            print("%d %d second: %d" % (i, i, dp[i][i][1]))
        for i in range(n-2,-1,-1):
            for j in range(i+1, n):
                print("%d,%d" % (i,j))
                dp[i][j][0] = max(dp[i+1][j][1]+piles[i], dp[i][j-1][1]+piles[j])
                if dp[i][j][0] == dp[i+1][j][1]+piles[i]:
                    print("left")
                    dp[i][j][1] = dp[i+1][j][0]
                else:
                    dp[i][j][1] = dp[i][j-1][0]
                    print("right")
                print("%d %d first: %d" % (i,j,dp[i][j][0]))
                print("%d %d second: %d" % (i, j, dp[i][j][1]))
        return dp[0][n-1][0]-dp[0][n-1][1]>0

a = It877()
print(a.stoneGame([3,7,2,3]))
