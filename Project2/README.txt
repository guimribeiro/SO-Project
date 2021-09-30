*---------------------*
* DIRECTORY STRUCTURE *
*---------------------*

Project2
├── CircuitRouter-ParSolver
│   ├── CircuitRouter-ParSolver
│   ├── CircuitRouter-ParSolver.c
│   ├── CircuitRouter-ParSolver.o
│   ├── coordinate.c
│   ├── coordinate.h
│   ├── coordinate.o
│   ├── grid.c
│   ├── grid.h
│   ├── gridMutex.c
│   ├── gridMutex.h
│   ├── gridMutex.o
│   ├── grid.o
│   ├── Makefile
│   ├── maze.c
│   ├── maze.h
│   ├── maze.o
│   ├── router.c
│   ├── router.h
│   └── router.o
├── CircuitRouter-SeqSolver
│   ├── CircuitRouter-SeqSolver
│   ├── CircuitRouter-SeqSolver.c
│   ├── CircuitRouter-SeqSolver.o
│   ├── coordinate.c
│   ├── coordinate.h
│   ├── coordinate.o
│   ├── grid.c
│   ├── grid.h
│   ├── grid.o
│   ├── Makefile
│   ├── maze.c
│   ├── maze.h
│   ├── maze.o
│   ├── router.c
│   ├── router.h
│   └── router.o
├── doTest.sh
├── inputs
│   ├── generate.py
│   ├── random-x128-y128-z3-n128.txt
│   ├── random-x128-y128-z3-n64.txt
│   ├── random-x128-y128-z5-n128.txt
│   ├── random-x256-y256-z3-n256.txt
│   ├── random-x256-y256-z5-n256.txt
│   ├── random-x32-y32-z3-n64.txt
│   ├── random-x32-y32-z3-n64.txt.res
│   ├── random-x32-y32-z3-n64.txt.res.old
│   ├── random-x32-y32-z3-n96.txt
│   ├── random-x48-y48-z3-n48.txt
│   ├── random-x48-y48-z3-n64.txt
│   ├── random-x512-y512-z7-n512.txt
│   ├── random-x64-y64-z3-n48.txt
│   └── random-x64-y64-z3-n64.txt
├── lib
│   ├── commandlinereader.c
│   ├── commandlinereader.h
│   ├── commandlinereader.o
│   ├── list.c
│   ├── list.h
│   ├── list.o
│   ├── pair.c
│   ├── pair.h
│   ├── pair.o
│   ├── queue.c
│   ├── queue.h
│   ├── queue.o
│   ├── timer.h
│   ├── types.h
│   ├── utility.h
│   ├── vector.c
│   ├── vector.h
│   └── vector.o
├── Makefile
├── README.txt
└── results
    ├── random-x128-y128-z3-n128.txt.speedups.csv
    ├── random-x128-y128-z3-n64.txt.speedups.csv
    ├── random-x128-y128-z5-n128.txt.speedups.csv
    ├── random-x256-y256-z3-n256.txt.speedups.csv
    ├── random-x256-y256-z5-n256.txt.speedups.csv
    ├── random-x32-y32-z3-n64.txt.speedups.csv
    ├── random-x32-y32-z3-n96.txt.speedups.csv
    ├── random-x48-y48-z3-n48.txt.speedups.csv
    ├── random-x48-y48-z3-n64.txt.speedups.csv
    ├── random-x64-y64-z3-n48.txt.speedups.csv
    └── random-x64-y64-z3-n64.txt.speedups.csv


Directories: 5
Files: 82

*----------------------------------------*
* Compilation and execution instructions *
*----------------------------------------*

To compile the project, open the terminal in the root directory (Project2),
and execute the command 'make'.

To execute the project from:

Command line:

	Run in the terminal the following line of commands,

			./CircuitRouter-ParSolver INPUT -t NUMTAREFAS

	The CircuitRouter-ParSolver must receive 2 positional arguments:

	NUMTAREFAS -> The number of working tasks, 
					and an integer larger than zero
	INPUT -> The path to the file that contains the problem to analyse.


SHELL SCRIPT:

	Run in the terminal the following line of commands,

			./doTest.sh N INPUT

	O shell script must receive 2 positional arguments:

	N -> the maximum number of tasks
	INPUT -> The path to the file that contains the problem to analyse.



*---------------------------*
* Machine features *
*---------------------------*

N of processors: 4
Processor model: Intel(R) Core(TM) i5-3340M CPU @ 2.70GHz
Clock min: 1200 MHz
Clock Max: 3400 MHz

*-------------------------------------*
* Operating System Features *
*-------------------------------------*

Name: Ubuntu
Version: 17.10
Kernel:	Linux Ace 4.13.0-46-generic
