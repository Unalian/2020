class dp_long_increasing_sub(object):
    @staticmethod
    def solution(num):
        bp = [1]* len(num)
        res = 0
        for i in range(0,len(num)):
            for j in range(0,i):
                if num[i] > num[j]:
                    bp[i] = max(bp[j]+1, bp[i])
        for item in bp:
            res = max(item, res)
        return res


a = dp_long_increasing_sub()
print(a.solution(
[10,9,2,5,3,7,101,18]))







