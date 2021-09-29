*--------------------------*
* ESTRUTURA DAS DIRETORIAS *
*--------------------------*

Mutex_for_point
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


Diretorias: 5
Ficheiros: 82

*----------------------------------*
* INSTRUCOES COMPILACAO E EXECUCAO *
*----------------------------------*

Para compilar do projeto, abra, na diretoria root,
Mutex_for_point, o terminal	e execute o comando 'make'.

Para executar o projeto atraves:

LINHA DE COMANDOS:

	Corra no terminal a seguinte linha de comandos,

			./CircuitRouter-ParSolver INPUT -t NUMTAREFAS

	O CircuitRouter-ParSolver devera receber dois argumentos obrigatorios:

	NUMTAREFAS -> O numero de tarefas trabalhadoras, 
					e um inteiro maior que zero
	INPUT -> o caminho do ficheiro com o problema a analisar.


SHELL SCRIPT:

	Corra, no terminal e execute a seguinte linha de comandos,

			./doTest.sh N INPUT

	O shell script devera receber dois argumentos posicionais obrigatorios:

	N -> o numero maximo de tarefas
	INPUT -> o caminho do ficheiro com o problema a analisar.



*---------------------------*
* CARATERISTICAS DA MAQUINA *
*---------------------------*

N de processadores: 4
Modelo Processador: Intel(R) Core(TM) i5-3340M CPU @ 2.70GHz
Clock min: 1200 MHz
Clock Max: 3400 MHz

*-------------------------------------*
* CARATERISTICAS DO SISTEMA OPERATIVO *
*-------------------------------------*

Nome: Ubuntu
Versao: 17.10
Kernel:	Linux Ace 4.13.0-46-generic