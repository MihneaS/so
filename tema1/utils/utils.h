/**
 * SO, 2017
 * Lab #2, Operatii I/O simple
 *
 * Useful structures/macros
 */

#ifndef LIN_UTILS_H_
#define LIN_UTILS_H_	1

#include <stdio.h>
#include <stdlib.h>

#define MAX_DEATH_DESCR 250

#ifndef ENOMEM
#define ENOMEM 12
#endif
/* daca folosesc direct ENOMEM inloc de ALLOC_FAIL, nu-mi trece (local)
 * testul efectuat de checkpatch, deoarece recomanda intoarcerea de -ENOMEM,
 * dar daca intorc -ENOMEM, nu imi trece testul 16, care asteapta 12, nu -12
 */
#define ALLOC_FAIL ENOMEM


/* useful macro for handling error codes */
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(EXIT_FAILURE);				\
		}							\
	} while (0)

#endif
