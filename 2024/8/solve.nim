import std/os
import std/strutils
import std/strformat
import std/tables
import std/math

var antennaes = initTable[char, seq[tuple[x, y: int]]]()
var antinodes: seq[tuple[x, y: int]] = @[]
var antinodes2: seq[tuple[x, y: int]] = @[]
var width = 0
var height = 0

block:
    var rawContent = paramStr(1).readFile
    if '\r' in rawContent:
        rawContent = rawContent.replace("\r", "")

    var lines = rawContent.split("\n")
    for y, line in lines:
        width = line.len
        for x, col in line:
            if col == '.': continue
            if col notin antennaes:
                antennaes[col] = @[]
            antennaes[col].add((x: x, y: y))
    height = (rawContent.len + 1) div (width + 1)

proc is_bound(p: tuple[x, y: int]): bool = 
    (0 <= p.x) and (0 <= p.y) and (p.x < width) and (p.y < height)

proc `-`(p1, p2: tuple[x, y: int]): tuple[x, y: int] =
    result.x = p1.x - p2.x
    result.y = p1.y - p2.y

proc `+`(p1, p2: tuple[x, y: int]): tuple[x, y: int] =
    result.x = p1.x + p2.x
    result.y = p1.y + p2.y

proc `*`(scalar: int, p: tuple[x, y: int]): tuple[x, y: int] =
    result.x = scalar * p.x
    result.y = scalar * p.y

proc register_antinode(an: tuple[x, y: int], t: static int) =
    if is_bound(an):
        when t == 0:
            if an notin antinodes:
                antinodes.add(an)
        else:
            if an notin antinodes2:
                antinodes2.add(an)

for frequency, towers in antennaes.pairs:
    for i, tower1 in towers:
        for j in (i+1)..<towers.len:
            var tower2 = towers[j]

            var d = tower2 - tower1

            register_antinode(tower1 - d, 0)
            register_antinode(tower2 + d, 0)

            var n = 0
            while is_bound(tower1 - n * d) or is_bound(tower2 + n * d):
                register_antinode(tower1 - n * d, 1)
                register_antinode(tower2 + n * d, 1)
                inc n

echo fmt"Height: {height}, Width: {width}"
echo fmt"Total unique frequency: {antennaes.len}"
echo fmt"(PART 1) Unique antinodes: {antinodes.len}"
echo fmt"(PART 2) Unique antinodes: {antinodes2.len}"