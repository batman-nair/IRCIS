# PTrain 2D Programming Language

This is a 2D grid based programming language inspired from [Befunge](https://en.wikipedia.org/wiki/Befunge).
It works by having a Runner starts at the top of the source and moves right. Based on the characters it sees it executes commands till it reaches an explict stop command or the end of the Grid.
Each Runner has a stack which is it's main source of memory.

# Rules #
- Defualt/First Runner starts first character of the source file((0, 0) position) and move right from there.
- '.' (dot) and ' ' (space) characters are considered blanks and are ignored.
- Runners process characters in orthongonal directions in a clockwise manner.
    - Current Runner is assigned to the first character caught in the clockwise scan.
    - If more than one character is found, then new Runners are created in those directions.
    - New Runners with same stack are created for parsing the other directions.
    - *Note: Movement control is not necessarily needed to control direction of run. Still it's better to have a directional marker as the first character when signifying a change in direction.*

- Runner dies if it reaches the end of Grid or a End Processing character.
- Every Runner has a stack.
- In stack push mode, any literal that comes in is pushed into stack as is.
- *Integer* mode can be used to push in integers as is and special characters.
- *Integer* mode has precedence over stack mode.
  i.e `"e'100.f` In this piece of code, 'e' is pushed to stack followed by integer 100 then f. The '.' after 100 is not pushed to stack.
- *Integer* mode ends on seeing a blank character.
- Arithmetic operations on the stack is done as:
  If A and B are the values on the stack and B is stack top. On seeing a '+' sign, A and B are popped from stack and B+A is pushed back into the stack with operands in that order.


## Character commands list ##

| Character | Commands                           |
|:---------:|:----------------------------------:|
| < > ^ v   | Movement control                   |
| + - * /   | Arithmetic Ops in stack            |
| $         | Print Newline                      |
| !         | End Processing                     |
| @         | Access Memory and push to stack    |
| &         | Push from stack to Memory          |
| <space> . | Blanks, Ignored                    |
| ,         | Like blank but preserves direction |
| &&        | Pop stack                          |
| "         | Toggles stack push mode            |
| '         | Goes to *Integer* mode             |
| #         | Print stack value to output        |
|           |                                    |

## Classes ##
- PTrain
    - Master Class, holds all the Runner instances, the Logger and Grid.
- Runner
    - Executes commands based on the characters under it.
    - Has a position in Grid, facing one direction.
    - Holds a shared reference to the Grid and Log object.
- Grid
    - Reads source file and saves it as a 2D grid for random access.
- Logger
    - Takes care of printing to console/log.
- Memory
    - Holds variable data.
