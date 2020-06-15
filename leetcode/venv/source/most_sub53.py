class most_sub_53:
    def solution(self, num):
        if len(num) == 0:
            return 0
        res = float("-inf")
        dp_0 = 0
        dp_1 = num[0]
        for i in range(1, len(num)+1):
            dp_1 = max(dp_0 + num[i-1], num[i-1])
            dp_0 = dp_1
        res = max(res, dp_1)
        return res

a = most_sub_53()
print(a.solution([1]))