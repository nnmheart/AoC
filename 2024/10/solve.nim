import std/os
import std/strutils
import std/strformat
import std/tables

type Point = tuple[x, y: int]
type Line = seq[Point]

var map: seq[seq[char]]
var zeros: seq[Point]
var trails: Table[Point, seq[Line]]

var lines = paramStr(1).readFile.splitLines
for y, line in lines:
    var row: seq[char]
    var x = 0
    for c in line:
        if c == '0':
            zeros.add((x: x, y: y))
        row.add(c)
        inc x    
    map.add(row)

var height = map.len
var width = map[0].len

proc iterate(l: Line)

proc proceed_if_allowed(l: Line, p: Point, n: char) =
    if not((0 <= p.x) and (0 <= p.y) and (p.x < width) and (p.y < height)):
        return
    if map[p.y][p.x] != n: return
    var l2 = l
    l2.add(p)
    iterate(l2)

proc iterate(l: Line) =
    var p = l[l.len - 1]
    if map[p.y][p.x] != '9':
        var c = char(l.len.byte + '0'.byte)
        
        proceed_if_allowed(l, (x: p.x + 1, y: p.y), c)
        proceed_if_allowed(l, (x: p.x - 1, y: p.y), c)
        proceed_if_allowed(l, (x: p.x, y: p.y + 1), c)
        proceed_if_allowed(l, (x: p.x, y: p.y - 1), c)
    else:
        if l[0] notin trails:
            trails[l[0]] = @[]
        trails[l[0]].add(l)

for zero in zeros:
    iterate(@[zero])

var score = 0
var rating = 0
for trailhead, lines in trails.pairs:
    var unique_nines: seq[Point]
    for trail in lines:
        if trail[9] notin unique_nines:
            unique_nines.add(trail[9])
    score += unique_nines.len
    rating += lines.len

echo fmt"(PART 1) Score: {score}"
echo fmt"(PART 2) Rating: {rating}"