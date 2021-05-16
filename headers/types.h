
#ifndef H_TYPES
#define H_TYPES

typedef enum {
    NORD = 1,
    OUEST = 2,
    EST = 3,
    SUD = 4
} direction;

typedef struct {
    int nord, ouest, est, sud;
} distances;

typedef struct {
	int x, y;
    direction d;
} t_position;

#endif