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

static int is_inside(double center_x, double center_y, const Tile *tile, long num)
{
    long crossings = 0;
    for (long i = 0; i < num; i++) {
        long next = (i + 1) % num;
        long x1 = tile[i].x;
        long y1 = tile[i].y;
        long x2 = tile[next].x;
        long y2 = tile[next].y;
        if (x1 == x2) {
            double min_y = y1 < y2 ? y1 : y2;
            double max_y = y1 > y2 ? y1 : y2;
            if (x1 > center_x && min_y <= center_y && center_y < max_y) {
                crossings++;
            }
        }
    }
    return (crossings % 2) != 0;
}

static int is_valid(long lhs, long rhs, const Tile *tile, long num)
{
    long min_x = tile[lhs].x < tile[rhs].x ? tile[lhs].x : tile[rhs].x;
    long max_x = tile[lhs].x > tile[rhs].x ? tile[lhs].x : tile[rhs].x;
    long min_y = tile[lhs].y < tile[rhs].y ? tile[lhs].y : tile[rhs].y;
    long max_y = tile[lhs].y > tile[rhs].y ? tile[lhs].y : tile[rhs].y;

    for (long i = 0; i < num; i++) {
        long next = (i + 1) % num;
        long x1 = tile[i].x;
        long y1 = tile[i].y;
        long x2 = tile[next].x;
        long y2 = tile[next].y;
        if (x1 == x2) {
            long edge_min_y = y1 < y2 ? y1 : y2;
            long edge_max_y = y1 > y2 ? y1 : y2;
            if (min_x < x1 && x1 < max_x && edge_max_y > min_y && edge_min_y < max_y) {
                return 0;
            }
            if (min_y == max_y && min_x < x1 && x1 < max_x && edge_min_y < min_y &&
                min_y < edge_max_y) {
                return 0;
            }
        }
        else {
            long edge_min_x = x1 < x2 ? x1 : x2;
            long edge_max_x = x1 > x2 ? x1 : x2;
            if (min_y < y1 && y1 < max_y && edge_max_x > min_x && edge_min_x < max_x) {
                return 0;
            }
            if (min_x == max_x && min_y < y1 && y1 < max_y && edge_min_x < min_x &&
                min_x < edge_max_x) {
                return 0;
            }
        }
    }

    double center_x = (min_x + max_x) / 2.0;
    double center_y = (min_y + max_y) / 2.0;
    return is_inside(center_x, center_y, tile, num);
}

int main(void)
{
    Tile tile[NUM];
    long num = 0;

    FILE *file = fopen("input/09.txt", "r");
    assert(file);

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%ld,%ld", &tile[num].x, &tile[num].y);
        num += 1;
    }
    fclose(file);

    long max_area1 = 0;
    long max_area2 = 0;
    for (long i = 0; i < num; i++) {
        for (long j = i + 1; j < num; j++) {
            long area = compute_area(i, j, tile);
            if (area > max_area1) {
                max_area1 = area;
            }
            if (area > max_area2 && is_valid(i, j, tile, num)) {
                max_area2 = area;
            }
        }
    }
    printf("%ld\n", max_area1);
    printf("%ld\n", max_area2);
}
