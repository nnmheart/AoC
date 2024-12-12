import std/os
import std/strutils
import std/strformat

type Point = tuple[x, y: int]
type Region = ref object
    letter: char
    points: seq[Point]

var map: seq[seq[char]]
var regions: seq[Region]

for line in paramStr(1).readFile.splitlines:
    var row: seq[char] = @[]
    for c in line:
        row.add(c)
    map.add(row)

var height = map.len
var width = map[0].len

proc forward(r: Region, p: Point) =
    if not((0 <= p.x) and (0 <= p.y) and (p.x < width) and (p.y < height)):
        return
    if map[p.y][p.x] != r.letter:
        return
    r.points.add(p)
    map[p.y][p.x] = '#'

    r.forward((x: p.x + 1, y: p.y))
    r.forward((x: p.x - 1, y: p.y))
    r.forward((x: p.x, y: p.y + 1))
    r.forward((x: p.x, y: p.y - 1))

proc find_region_from(p: Point) =
    if map[p.y][p.x] == '#':
        return
    var region = Region(
        letter: map[p.y][p.x], 
        points: @[]
    )
    region.forward(p)
    regions.add(region)

for y in 0..<height:
    for x in 0..<width:
        find_region_from((x: x, y: y))

proc non_member_neighbours(letter: char, p: Point): int =
    result = 4

    if (p.x > 0) and (map[p.y][p.x - 1] == letter):
        dec result
    if (p.y > 0) and (map[p.y - 1][p.x] == letter):
        dec result
    
    if (p.x < (width - 1)) and (map[p.y][p.x + 1] == letter):
        dec result
    if (p.y < (height - 1)) and (map[p.y + 1][p.x] == letter):
        dec result

proc corner_length(r: Region): int =
    result = 0
    for p in r.points:
        result += non_member_neighbours(r.letter, p)

for region in regions:
    for point in region.points:
        map[point.y][point.x] = region.letter

echo fmt"Created {regions.len} regions."
var total_price = 0

for region in regions:
    total_price += (region.points.len * region.corner_length)

echo fmt"(PART 1) Total price: {total_price}"