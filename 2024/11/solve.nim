import std/os
import std/strutils
import std/strformat
import std/tables

var stones = initTable[int, int]()
var new_stones = initTable[int, int]()

for stone in paramStr(1).readFile.split(" "):
    var stn = stone.parseInt
    if stn notin stones:
        stones[stn] = 0
    stones[stn] += 1

proc apply(stone: int): seq[int] =
    var l = ($stone).len
    if stone == 0:
        return @[1]
    elif ($stone).len mod 2 == 0:
        return @[
            parseInt(($stone)[0 ..< (l div 2)]),
            parseInt(($stone)[(l div 2) ..< l])
        ]
    else:
        return @[stone * 2024]

proc gen() =
    for (stone, count) in stones.pairs:
        for num in apply(stone):
            if num notin new_stones:
                new_stones[num] = 0
            new_stones[num] += count

    stones = new_stones
    new_stones = initTable[int, int]()

for i in 0..<25:
    gen()
var count = 0
for _, count2 in stones.pairs:
    count += count2
echo fmt"(PART 1) Total stones: {count}"

for i in 0..<50:
    gen()
count = 0
for _, count2 in stones.pairs:
    count += count2
echo fmt"(PART 2) Total stones: {count}"