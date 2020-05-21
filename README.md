# IRCIS (I Run Chars I See)

IRCIS is an esoteric programming language where the program exists in a two-dimensional grid of cells, where each cell contains a single instruction, and execution can proceed in any cardinal direction across this grid -- not just left-to-right, but also right-to-left, top-to-bottom, and bottom-to-top.

It has a vizualizer which creates an animated representation of the program execution in HTML and CSS.

Inspired by [Befunge](https://en.wikipedia.org/wiki/Befunge).

<p align="center">
	<img src="./screencaps/hello_world_banner.gif" alt="Hello World Vizualization">
</p>

The program executes by reading the source file character by character and commands are executed based on the character read.
This reading is done by a *Runner* object which starts from the top-left of the file and moves to the right.
Based on the character read, the *Runner* object can change the direction of parsing, create more *Runners*, store/print/calculate values and more.
Check out the command list to see all the commands implemented.
The Visualizer helps in understanding the movement of these different *Runner* objects throughout the execution of the program.

**Click on the HTML Link below the GIFs to see the actual generated html in its full glory**

## Character commands table

| Character   | Commands                                                   |
|:-----------:|:----------------------------------------------------------:|
| < > ^ v     | Movement control                                           |
| + - * / % ^ | Arithmetic Ops in stack (in *Integer* mode)                |
| & \| V < >   | Binary Ops in stack (in *Integer* mode) V is XOR; <> shifts|
| #           | Print stack top to output                                  |
| $           | Print Newline                                              |
| !           | End Processing                                             |
| \<space\> . | Blanks, Ignored                                            |
| "           | Toggles *Stack* mode                                       |
| '           | Goes to *Integer* mode                                     |
| @\<n\>      | Push n'th index element of stack to top. @0 duplicates top |
| &\<n\>      | Pop n elements from stack                                  |
| ?           | Check stack top. If true then continue, else go right/left |
| *           | Split into multiple Runners                                |
| @\<str\>    | Push value in variable \<str\> to stack                    |
| &\<str\>    | Set value in stack top to variable \<str\>                 |
| r           | Generate a random 0 or 1 and push to stack                 |
| R           | Push random number between 0 and *limit* to stack          |
| p           | Pause the Runner stack top number of ticks                 |
|             |                                                            |


## Language Rules

### Basic and Movement
- Default/First Runner starts at first character of the source file(0, 0) and moves right from there.
- The source file is transformed into a rectangular grid by padding blanks and processed as such.
- '.' (dot) and ' ' (space) characters are considered blanks and are ignored.
- Every Runner has its own stack.
- Runner parses and executes commands based on the character read in the position of the Runner.
- Movement of the Runner can be controlled using the characters '>', '^', 'v' and '>'.
- A Runner dies if it reaches the end of Grid, sees an End Processing(!) character or in the event of an error.

### Printing
- Hash(#) is used to write values to output. It pops and prints the stack top value to output.
- Dollar(\$) character starts a new line in the output.

Here you can see the basic movement and printing in the program

<p align="center">
	<img src="https://github.com/batman-nair/IRCIS/blob/master/screencaps/hello_world.gif" alt="Basic movement gif">
	<a href="https://batman-nair.github.io/IRCIS/samples/hello_world.html" target="_blank"> HTML LINK </a>
</p>

### Stack operations
- Stack push mode can be toggled with the double-quote(") character. In stack push mode, any character that comes in is pushed into the Runner stack as-is.
- *Integer* mode can be used to push in integers and perform arithmetic operations. *Integer* mode processing starts on seeing a quote(') character.
- *Integer* mode has precedence over stack mode.
  i.e `"e'100.f` In this piece of code, 'e' is pushed to stack followed by integer 100 then f. The '.' (blank) after 100 is not pushed to stack but acts as a separator to signify the end of *Integer* mode.
- *Integer* mode ends on seeing a blank character.
- Arithmetic operations are done when the operator is called in *Integer* mode.
- Arithmetic operations on the stack are done as:
  If A and B are the values on the stack, B is stack top and addition(+) is the operation called, then A and B are popped from the stack and B+A is pushed back into the stack with operands in that order.
- The \<n\>'th element from the stack top can be pushed to the top of the stack by specifying \<n\> after the stack push '@' operator.
- Specifying \<n\> after the stack pop '&' operator, pops \<n\> elements from the stack.

Here you can see different arithmetic operations performed on numbers
<p align="center">
	<img src="https://github.com/batman-nair/IRCIS/blob/master/screencaps/calc.gif" alt="Basic arithmetic example">
	<a href="https://batman-nair.github.io/IRCIS/samples/calc.html" target="_blank"> HTML LINK </a>
</p>

### Conditional
- Question mark(?) is used for the conditional operation. It checks if the stack top value is non-zero.
- If the top value is non-zero, execution continues in the current direction.
- If the top value is zero, execution can go left or right depending on which side of the '?' there is a non-blank character. (Left and right here are with respect to the Runner object.)
- Left has higher priority if both sides have a non-blank character.

Here the condition is checked to see if the loop number is divisible and to check if the end is reached
<p align="center">
	<img src="https://github.com/batman-nair/IRCIS/blob/master/screencaps/factors.gif" alt="Basic condition example">
	<a href="https://batman-nair.github.io/IRCIS/samples/factors.html" target="_blank"> HTML LINK </a>
</p>

### Splitting/Creating more Runners
- Star(\*) character is used to create more Runner objects.
- On seeing a star, Runners are created in every direction to the star that has a non-blank character.
- The current Runner object continues in one of the split directions and new Runners are created if any, in other directions.
- The new Runners have a copy of the original Runner's stack.

A split is done where one flow divides the number by 2 whereas the other gets the modulo by 2
<p align="center">
	<img src="https://github.com/batman-nair/IRCIS/blob/master/screencaps/num_to_binary_rev.gif" alt="Basic splitting example">
	<a href="https://batman-nair.github.io/IRCIS/samples/num_to_binary_rev.html" target="_blank"> HTML LINK </a>
</p>

### Variables
- Values can be saved to a variable by specifying a name \<str\> for the variable after the stack pop '&' operator. Saving to a variable doesn't pop the value from the stack.
- Values can be pushed to stack from a variable by specifying the variable name \<str\> after the stack push '@' operator.
- Variables names starting with an uppercase character are global variables and lowercase are local to the Runner instance. Global variables are accessible from any Runner no matter where it was set.
- When a split happens the local variables are copied to both Runner instances.
- When 2 Runner instances try to write the same global variable in the same step, the value in the global variable depends on the Runner update order.

A local 'limit' variable is used to store the loop end value, Global 'Flag' is used to check if Fizz or Buzz has been printed
<p align="center">
	<img src="https://github.com/batman-nair/IRCIS/blob/master/screencaps/fizzbuzz.gif" alt="Variables used to keep limit">
	<a href="https://batman-nair.github.io/IRCIS/samples/fizzbuzz.html" target="_blank"> HTML LINK </a>
</p>

### Random values and Visual Pause
- 'r' operator generates a random 0 or 1 value and pushes to stack.
- 'R' operator pops a *limit* value from stack and generates a random value between 0 and *limit* and pushes it to stack.
- 'p' operator pauses Runner execution stack top number of ticks. The stack top is popped.

Random values and pauses are used to create a racing track for Runner's with random paths and random pauses.
<p align="center">
	<img src="https://github.com/batman-nair/IRCIS/blob/master/screencaps/racetrack.gif" alt="Racetrack for runners with randoms and pauses">
	<a href="https://batman-nair.github.io/IRCIS/samples/racetrack.html" target="_blank"> HTML LINK </a>
</p>


## How to compile and run

When cloning the repo, use `git clone --recursive` so that googletest module is also cloned in.

Run `./build.sh` to compile the program with the tests and create the final executable with a single command. Manual building can be done using cmake.
Run `./build.sh Debug` to build with additional printing of DEBUG messages of the run. It also dumps a debug.log file with the run status.

You can run the program with
``` sh
./ircis examples/hello_world.txt
```

You can run the program on multiple input grids sequentially by just supplying additional parameters, like
``` sh
./ircis examples/hello_world.txt /examples/fizbuzz.txt
```

This will give the output in the console and generate the html vizualization of the final input file as `output.html`


## Options

Normally, the runner starts at (0, 0) and headed East. You can specify the starting position of the initial runner and its direction with the options
```
-x,--startx num         Starting x position (Default: 0)
-y,--starty num         Starting y position (Default: 0)
-d,--direction [NSEW]   Specify the starting direction (Default: E)
-s,--speed num          Set the speed for generated animation (Default: 15)
```

You can see how this works with these examples:
``` sh
./ircis examples/options.txt
./ircis examples/options.txt -x 49 -y 21 -d N
./ircis examples/options.txt -x 2 -y 2 -d S
./ircis examples/options.txt --startx 59 --starty 12 --direction W
```
