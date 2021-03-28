# Sokoban
# Work still in progress 

Program implementing algorithm for solving sokoban game
Sokoban: https://en.wikipedia.org/wiki/Sokoban

## Input:

In first two lines there should be 2 numbers. First is number n whcih denotes height of the map, second is m which denotes width of map.
Next there should be n lines each containing m sybols.
Meaning of each symbol:
- "#" wall
- "O" box
- "*" storage location
- "-" worker starting location
- " " empty space
Example:
<pre>
9 8
########
#####  #
#  *   #
# ###  #
#  OO -#
# ###  #
#  *   #
#####  #
########
</pre>

## Output:

Output will contain a line of symbols.
Meaning of each symbol:
- "l" move left
- "r" move right
- "u" move up
- "d" move down
- "L" move left dragging a box
- "R" move right dragging a box
- "U" move up dragging a box
- "D" move down dragging a box
Example:
lRulUUrdddddllllluuuurrrLLlddddrrrruulRRdlDDruuulllllddrrrLL
