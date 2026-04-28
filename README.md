# Systme dinformations temps rel (temps rel)  Mini-projet / rapport

Ce dpt regroupe des lments de travail autour des systmes temps rel :
- une petite base de code en **C** (mesures et simulation),
- un solveur dordonnancement (branch & bound),
- un script **Python** pour estimer un WCET,
- un **rapport LaTeX** (PDF) et lnonc du devoir.

> Langages (approx.) : TeX  71.8%  C  24.1%  Python  4.1%.

## Contenu du rpo

- `ordonancing.c` : algorithme dordonnancement (branch & bound) sur un ensemble de tches priodiques. Calcule lhperpriode (PPCM des priodes), gnre les jobs sur lintervalle, explore les squences dexcution et minimise le **temps d attente total**.
- `final.c` : petit programme C de mesure/simulation (excute une charge `tau1()` et affiche le temps dexcution).
- `MakeFile` : compilation de `final.c` et excution de lexcutable `final`.
- `stats.py` : script danalyse WCET *ct compilation* (mesure le temps de `gcc -c final.c` sur 1000 itrations) et affiche WCET/SET/quartiles.
- `report/` : rapport LaTeX et PDF gnr.
  - `report/report.tex` : source LaTeX
  - `report/report.pdf` : rapport compil
- `Final_Assignment.pdf` : nonc du devoir.

## Prrequis

### C
- GCC (ou compatible)
- Make

### Python
- Python 3
- `tqdm` (pour la barre de progression)

```bash
pip install tqdm
```

## Compilation / excution

### Compiler et excuter `final.c`

Le `MakeFile` compile `final.c` en `final` puis lexcute.

```bash
make
```

Nettoyer :

```bash
make clean
```

### Lancer le solveur dordonnancement

`ordonancing.c` nest pas printgr au Makefile du dpt. Pour compiler  la main :

```bash
gcc -O2 -Wall -Wextra -o ordonancing ordonancing.c
./ordonancing
```

Le programme excute deux scnarios :
1. **Strict** : aucune tche na le droit de rater sa deadline
2. **Relaxed** : la tche **T5** est autorise  rater sa deadline

Il affiche  la fin le meilleur planning trouv et des statistiques (busy/idle/utilization).

### Estimer un WCET (script Python)

`stats.py` mesure le temps de compilation de `final.c` (ce nest pas le temps dexcution du binaire, mais bien le temps de `gcc -c`).

```bash
python3 stats.py
```

Paramtres importants dans le script :
- `MAX_ITERATIONS = 1000`
- `SECURITY_FACTOR = 1.3` (pour un SET)

## Rapport

Le rapport est dans `report/`.
- PDF : `report/report.pdf`
- Source : `report/report.tex`

Si vous souhaitez recompiler le rapport (optionnel), il vous faut une distribution LaTeX (TeX Live / MikTeX). Selon votre configuration :

```bash
cd report
latexmk -pdf report.tex
```

## Notes

- Le dpt contient des fichiers gnrs (ex: `report/*.aux`, `report/*.log`, `final.o`, `a.out`). Pour un dpt  "propre", on peut ajouter un `.gitignore` et retirer les artefacts compils.

## Licence

Non spcifie.
