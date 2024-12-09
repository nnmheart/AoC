import std/os
import std/strutils
import std/strformat
import std/tables

var blocks: seq[tuple[num, length: int]] # num = -1 means "empty"
var is_file = true

for c in paramStr(1).readFile:
    blocks.add((
        num: if is_file: (blocks.len div 2) else: -1,
        length: int(c.byte - '0'.byte)
    ))
    is_file = not is_file

proc part1() =
    var seq_blocks: seq[int] = @[]
    for bl in blocks:
        for i in 0..<bl.length:
            seq_blocks.add(bl.num)

    proc part1_is_defragged(): bool =
        var seen_empty = false
        for i in seq_blocks:
            if i == -1: seen_empty = true
            if (i != -1) and seen_empty:
                return false
        return true

    for i in countdown(seq_blocks.len - 1, 0):
        if seq_blocks[i] == -1: continue
        for i2 in 0..<seq_blocks.len:
            if seq_blocks[i2] != -1: continue
            seq_blocks[i2] = seq_blocks[i]
            seq_blocks[i] = -1
            break
        if part1_is_defragged():
            break

    var part1_checksum = 0
    for i, num in seq_blocks:
        if num == -1: continue
        part1_checksum += (i * num)
    echo fmt"(PART 1) Checksum: {part1_checksum}"

proc part2() =
    var deactivated_nums: seq[int]
    var marked_empties = initOrderedTable[int, seq[tuple[by: int, space: int]]]()
    for i, bl in blocks:
        if bl.num != -1: continue
        marked_empties[i] = @[]
    
    for rbi in countdown(blocks.len - 1, 0):
        var rbl = blocks[rbi]
        if rbl.num == -1: continue # block to move to left

        for lbi in 0..<rbi:
            var lbl = blocks[lbi]
            if lbl.num != -1: continue # empty block to move into

            var used_space = 0
            for used in marked_empties[lbi]:
                used_space += used.space
            if rbl.length <= (lbl.length - used_space):
                marked_empties[lbi].add((by: rbl.num, space: rbl.length))
                deactivated_nums.add(rbl.num)
                break

    var seq_blocks: seq[int]
    #echo marked_empties
    #echo deactivated_nums

    for index, bl in blocks:
        if bl.num == -1:
            var total_used = 0
            for user in marked_empties[index]:
                for i in 0..<user.space:
                    seq_blocks.add(user.by)
                total_used += user.space
            for i in 0..<(bl.length - total_used):
                seq_blocks.add(-1)
        elif bl.num in deactivated_nums:
            for i in 0..<bl.length:
                seq_blocks.add(-1)
        else:
            for i in 0..<bl.length:
                seq_blocks.add(bl.num)

    var checksum = 0
    for i, num in seq_blocks:
        if num == -1: continue
        checksum += (i * num)
    echo fmt"(PART 2) Checksum: {checksum}"


part1()
part2()