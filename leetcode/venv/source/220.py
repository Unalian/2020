# This is a sample Python script.

# Press ⇧F10 to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.


def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press ⌘F8 to toggle the breakpoint.


class Solution(object):
    def containsNearbyAlmostDuplicate(self, nums, k, t):
        backet = dict()
        if t < 0 or k < 0:
            return False
        for i in range(len(nums)):
            backetID = nums[i]//(t+1)
            if backetID in backet:
                return True
            if backetID+1 in backet and abs(backet[backetID+1]-nums[i]) <= t:
                return True
            if backetID-1 in backet and abs(backet[backetID-1]-nums[i]) <= t:
                return True
            backet[backetID] = nums[i]
            if i >= k:
                backet.pop(nums[i-k]//(t+1))
        return False



if __name__ == '__main__':
    print_hi('PyCharm')
    print(Solution().containsNearbyAlmostDuplicate([1,5,9,1,5,9], 2, 3))





# See PyCharm help at https://www.jetbrains.com/help/pycharm/
