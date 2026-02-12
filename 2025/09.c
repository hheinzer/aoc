#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum { NUM = 500 };

typedef struct {
    long x, y;
} Tile;

static long compute_area(long lhs, long rhs, const Tile *tile)
{
    long delta_x = labs(tile[lhs].x - tile[rhs].x) + 1;
    long delta_y = labs(tile[lhs].y - tile[rhs].y) + 1;
    return delta_x * delta_y;
}

typedef struct {
    long num;
    long beg[NUM];
    long end[NUM];
} Edges;

static long count_hits(long tile, const Edges *edges)
{
    long count = 0;
    for (long i = 0; i < edges->num; i++) {
        if (edges->beg[i] <= tile && tile < edges->end[i]) {
            count += 1;
        }
    }
    return count;
}

static bool is_inside(Tile tile, const Edges *horizontal, const Edges *vertical)
{
    return (count_hits(tile.x, horizontal) % 2 == 0) && (count_hits(tile.y, vertical) % 2 == 0);
}

int main(void)
{
    Tile tile[NUM];
    long num = 0;

    FILE *file = fopen("test/09.txt", "r");
    assert(file);

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%ld,%ld", &tile[num].x, &tile[num].y);
        num += 1;
    }
    fclose(file);

    long max_area = 0;
    for (long i = 0; i < num; i++) {
        for (long j = 0; j < num; j++) {
            long area = compute_area(i, j, tile);
            if (area > max_area) {
                max_area = area;
            }
        }
    }
    printf("%ld\n", max_area);

    Edges horizontal;
    Edges vertical;
    for (long i = 0; i < num; i++) {
        long beg = i;
        long end = (i + 1) % num;
        if (tile[beg].y == tile[end].y) {
            horizontal.beg[horizontal.num] = beg;
            horizontal.end[horizontal.num] = end;
            horizontal.num += 1;
        }
        else {
            vertical.beg[vertical.num] = beg;
            vertical.end[vertical.num] = end;
            vertical.num += 1;
        }
    }
}
