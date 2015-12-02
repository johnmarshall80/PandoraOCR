/* Header file for the findblob routine. */

/* Manifest constant values of some input flags. */

/* Values for erase_flag */
#define ERASE      0
#define NO_ERASE   1

/* Values for alloc_flag */
#define ALLOC      0
#define NO_ALLOC   1

/* Values for out_flag */
#define ORIG_BLOB  0
#define W_H_BLOB   1
#define BOUND_BLOB 2

/* Starting, growth-increment, and maximum number of elts for the
   internal list used by findblob.  (Each list elt occupies 12 bytes.) */
#define LIST_STARTSIZE  5000
#define LIST_INCR       1000
#define LIST_MAXSIZE    10000

typedef struct { /* info about one run of pixels */
  unsigned short y;
  unsigned char *w_on, *e_off;
} RUN;
