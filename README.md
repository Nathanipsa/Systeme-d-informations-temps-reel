# Systèmes d'informations temps réel - Mini-projet / rapport

Ce dépôt regroupe des travaux autour des systèmes temps réel :
- une petite base de code en C pour les mesures et la simulation,
- un solveur d'ordonnancement en branch and bound,
- un script Python pour estimer un WCET,
- un rapport LaTeX (PDF) et l'énoncé du devoir.

> Langages (approx.) : TeX 71.8 % · C 24.1 % · Python 4.1 %.

## Contenu du dépôt

- `ordonancing.c` : algorithme d'ordonnancement en branch and bound sur un ensemble de tâches périodiques. Il calcule l'hyperpériode, génère les jobs sur l'intervalle, explore les séquences d'exécution et minimise le temps d'attente total.
- `final.c` : petit programme C de mesure/simulation qui exécute une charge `tau1()` et affiche le temps d'exécution.
- `MakeFile` : compilation de `final.c` et exécution du binaire `final`.
- `stats.py` : script d'analyse WCET côté compilation, qui mesure le temps de `gcc -c final.c` sur 1000 itérations et affiche WCET, SET et quartiles.
- `report/` : rapport LaTeX et PDF générés.
  - `report/report.tex` : source LaTeX
  - `report/report.pdf` : rapport compilé
- `Final_Assignment.pdf` : énoncé du devoir.

## Prérequis

- GCC ou un compilateur compatible
- Make
- Python 3
- `tqdm` pour la barre de progression

```bash
pip install tqdm
```

## Compilation / exécution

### Compiler et exécuter `final.c`

Le `MakeFile` compile `final.c` en `final` puis exécute le programme.

```bash
make
```

Nettoyer :

```bash
make clean
```

### Lancer le solveur d'ordonnancement

`ordonancing.c` n'est pas intégré au `MakeFile` du dépôt. Pour le compiler manuellement :

```bash
gcc -O2 -Wall -Wextra -o ordonancing ordonancing.c
./ordonancing
```

Le programme exécute deux scénarios :
1. **Strict** : aucune tâche n'a le droit de rater sa deadline.
2. **Relaxed** : la tâche **T5** est autorisée à rater sa deadline.

À la fin, il affiche le meilleur planning trouvé et des statistiques (busy/idle/utilization).

### Estimer un WCET

`stats.py` mesure le temps de compilation de `final.c`. Ce n'est pas le temps d'exécution du binaire, mais le temps de `gcc -c`.

```bash
python3 stats.py
```

Paramètres importants du script :
- `MAX_ITERATIONS = 1000`
- `SECURITY_FACTOR = 1.3` pour le calcul du SET

## Rapport

Le rapport se trouve dans `report/`.
- PDF : `report/report.pdf`
- Source : `report/report.tex`

Pour recompiler le rapport, il faut une distribution LaTeX comme TeX Live ou MiKTeX :

```bash
cd report
latexmk -pdf report.tex
```

## Notes

- Le dépôt contient des fichiers générés, par exemple `report/*.aux`, `report/*.log`, `final.o` ou `a.out`. Pour un dépôt plus propre, vous pouvez ajouter un `.gitignore` et supprimer les artefacts compilés.

## Licence

Non spécifiée.
# Real-Time Information Systems (Real-Time) — Mini Project / Report

This repository contains work related to real-time systems:
- a small **C** codebase for measurements and simulation,
- a scheduling solver (branch and bound),
- a **Python** script to estimate a WCET,
- a **LaTeX report** (PDF) and the assignment statement.

> Languages (approx.): TeX 71.8 % · C 24.1 % · Python 4.1 %.

## Repository Contents

- `ordonancing.c`: scheduling algorithm (branch and bound) for a set of periodic tasks. It computes the hyperperiod (LCM of the periods), generates the jobs over the interval, explores execution sequences, and minimizes the **total waiting time**.
- `final.c`: small C measurement/simulation program (runs a `tau1()` workload and prints the execution time).
- `Makefile`: builds `final.c` and runs the `final` executable.
- `stats.py`: WCET analysis script on the compilation side (measures the time taken by `gcc -c final.c` over 1000 iterations) and prints WCET/SET/quartiles.
- `report/`: generated LaTeX report and PDF.
  - `report/report.tex`: LaTeX source
  - `report/report.pdf`: compiled report
- `Final_Assignment.pdf`: assignment statement.

## Requirements

### C
- GCC (or compatible)
- Make

### Python
- Python 3
- `tqdm` (for the progress bar)

```bash
pip install tqdm
```

## Build / Run

### Build and run `final.c`

The `Makefile` compiles `final.c` into `final`, then runs it.

```bash
make
```

Clean:

```bash
make clean
```

### Run the scheduling solver

`ordonancing.c` is not integrated into the repository `Makefile`. To compile it manually:

```bash
gcc -O2 -Wall -Wextra -o ordonancing ordonancing.c
./ordonancing
```

The program runs two scenarios:
1. **Strict**: no task is allowed to miss its deadline.
2. **Relaxed**: task **T5** is allowed to miss its deadline.

At the end, it prints the best schedule found and some statistics (busy/idle/utilization).

### Estimate a WCET (Python script)

`stats.py` measures the compilation time of `final.c` (this is not the binary execution time, but the time taken by `gcc -c`).

```bash
python3 stats.py
```

Important script parameters:
- `MAX_ITERATIONS = 1000`
- `SECURITY_FACTOR = 1.3` (for a SET)

## Report

The report is in `report/`.
- PDF: `report/report.pdf`
- Source: `report/report.tex`

If you want to recompile the report (optional), you need a LaTeX distribution (TeX Live / MiKTeX). Depending on your setup:

```bash
cd report
latexmk -pdf report.tex
```

## Notes

- The repository contains generated files, for example `report/*.aux`, `report/*.log`, `final.o`, `a.out`. For a cleaner repository, you can add a `.gitignore` and remove compiled artifacts.

## License

Not specified.