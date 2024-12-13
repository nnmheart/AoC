import std/os
import std/strutils
import std/strformat
import std/math

type Task = ref object
    but_a: tuple[x, y: int]
    but_b: tuple[x, y: int]
    prize: tuple[x, y: int]
    nums: tuple[a, b: int]
    nums2: tuple[a, b: int]
    solved: bool
    solved2: bool


var line_type = 0
var tasks: seq[Task]

proc solve(task: Task, part2: static bool) =
    # Assume numbers are "a" and "b" and increments are X, Y for A; Z, W for B
    # Xa + Zb = R_x
    # Ya + Wb = R_y
    # =>
    # XYa + YZb = YR_x
    # XYa + XWb = XR_y
    # =>
    # b(YZ - XW) = YR_x - XR_y
    # =>
    # b = (YR_x - XR_y)/(YZ - XW)
    var X = task.but_a.x
    var Y = task.but_a.y
    var Z = task.but_b.x
    var W = task.but_b.y

    var R_x = task.prize.x
    var R_y = task.prize.y

    when part2:
        R_x += 10000000000000
        R_y += 10000000000000

    var b_e = (Y * R_x - X * R_y) / (Y * Z - X * W)
    var b = b_e.toInt
    if floor(b_e) != b_e:
        return
    var a_e = (R_x - Z * b) / X
    var a = a_e.toInt
    if floor(a_e) != a_e:
        return
    when part2:
        task.nums2 = (a: a, b: b)
        task.solved2 = true
    else:
        task.nums = (a: a, b: b)
        task.solved = true

for line in paramStr(1).readFile.splitlines:
    if line.len == 0:
        continue
    if line_type == 0:
        var nums = line[len("Button A: X+") .. ^1].split(", Y+")
        tasks.add(Task(
            but_a: (x: parseInt(nums[0]), y: parseInt(nums[1])),
            but_b: (x: 0, y: 0),
            prize: (x: 0, y: 0),
            nums: (a: 0, b: 0),
            nums2: (a: 0, b: 0),
            solved: false,
            solved2: false
        ))
    elif line_type == 1:
        var nums = line[len("Button B: X+") .. ^1].split(", Y+")
        tasks[tasks.len - 1].but_b = (x: parseInt(nums[0]), y: parseInt(nums[1]))
    else:
        var nums = line[len("Prize: X=") .. ^1].split(", Y=")
        tasks[tasks.len - 1].prize = (x: parseInt(nums[0]), y: parseInt(nums[1]))
        tasks[tasks.len - 1].solve(false)
        tasks[tasks.len - 1].solve(true)
    line_type = (line_type + 1) mod 3

var total_token = 0
var total_tokens2 = 0
for task in tasks:
    if task.solved:
        total_token += task.nums.a * 3
        total_token += task.nums.b * 1
    if task.solved2:
        total_tokens2 += task.nums2.a * 3 
        total_tokens2 += task.nums2.b * 1 

echo fmt"(PART 1) Total token: {total_token}"
echo fmt"(PART 2) Total token: {total_tokens2}"