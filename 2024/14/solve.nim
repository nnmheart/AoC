import std/os
import std/strutils
import std/strformat
import std/tables

type Robot = ref object
    pos: tuple[x, y: int]
    vel: tuple[x, y: int]

var width = 101
var height = 103

try:
    width = paramStr(2).parseInt
    height = paramStr(3).parseInt
except:
    discard
var robots: seq[Robot]

for line in paramStr(1).readFile.splitlines:
    var splt = line[len("p=") .. ^1].split(" v=")
    var pos_splt = splt[0].split(",")
    var vel_splt = splt[1].split(",")
    robots.add Robot(
        pos: (
            x: pos_splt[0].parseInt,
            y: pos_splt[1].parseInt
        ),
        vel: (
            x: vel_splt[0].parseInt,
            y: vel_splt[1].parseInt
        )
    )

echo fmt"Map width: {width}, height: {height}, got {robots.len} robots"

proc tick() =
    for robot in robots:
        robot.pos.x += robot.vel.x
        robot.pos.y += robot.vel.y

        if robot.pos.x < 0:
            robot.pos.x += width
        if robot.pos.y < 0:
            robot.pos.y += height
        if robot.pos.x >= width:
            robot.pos.x = robot.pos.x mod width
        if robot.pos.y >= height:
            robot.pos.y = robot.pos.y mod height

proc get_safety_factor(): int =
    var midX = (width - 1) div 2
    var midY = (height - 1) div 2

    var quad_scores = [0, 0, 0, 0]

    for robot in robots:
        var pos = robot.pos
        if pos.x == midX: continue
        if pos.y == midY: continue

        if (pos.x < midX) and (pos.y < midY):
            quad_scores[0] += 1
        elif (pos.x > midX) and (pos.y < midY):
            quad_scores[1] += 1
        elif (pos.x < midX) and (pos.y > midY):
            quad_scores[2] += 1
        elif (pos.x > midX) and (pos.y > midY):
            quad_scores[3] += 1
        else:
            echo fmt"Invalid position: {pos}"
            quit 0
    result = 1
    for score in quad_scores:
        result *= score

for i in 0..<100:
    tick()
echo fmt"(PART 1) Safety factor: {get_safety_factor()}"