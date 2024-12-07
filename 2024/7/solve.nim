import std/os
import std/strutils
import std/strformat

template concat(a: int, b: int): int =
    parseInt($a & $b)

proc canBeValid(resultant: int, values: seq[int], includeConcat: static bool = false): bool =
    var prev: seq[int] = @[values[0]]
    var next: seq[int] = @[]

    for i in 1..<values.len:
        var val = values[i]
        for p in prev:
            if (p + val) <= resultant:
                next.add(p + val)
            if (p * val) <= resultant:
                next.add(p * val)
            when (includeConcat):
                if concat(p, val) <= resultant:
                    next.add(concat(p, val))
        prev = @[]
        prev.add(next)
        next = @[]
    
    return (resultant in prev)

var part1_result = 0
var part2_result = 0

var fileContent = os.paramStr(1).readFile
for line in fileContent.split('\n'):
    var splt = line.split(": ")
    var resultant = splt[0].parseInt

    var values: seq[int]
    for i in splt[1].split(' '):
        values.add(i.parseInt)
    
    if canBeValid(resultant, values):
        part1_result += resultant
        part2_result += resultant
    elif canBeValid(resultant, values, true):
        part2_result += resultant

echo fmt"(PART 1) Sum: {part1_result}"
echo fmt"(PART 2) Sum. {part2_result}"