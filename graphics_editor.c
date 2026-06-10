/*
 * 2D Graphics Editor in C
 * Canvas : 40 rows x 80 cols
 * Fill   : '_'   Draw: '*'
 *
 * Compile: gcc graphics_editor.c -o graphics_editor -lm
 * Run    : ./graphics_editor
 * Works on: OnlineGDB, Windows (MinGW/GCC), Linux, Mac
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROWS        40
#define COLS        80
#define MAX_OBJECTS 50
#define DRAW_CH     '*'
#define FILL_CH     '_'

#define OBJ_CIRCLE   1
#define OBJ_RECT     2
#define OBJ_LINE     3
#define OBJ_TRIANGLE 4

/* ── Object struct ── */
typedef struct {
    int type;
    int id;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
    int active;
} Object;

/* ── Globals ── */
static char   canvas[ROWS][COLS + 1];
static Object objects[MAX_OBJECTS];
static int    obj_count = 0;
static int    next_id   = 1;

/* ════════════════════════════════════════
   CANVAS HELPERS
   ════════════════════════════════════════ */

void canvas_clear(void) {
    for (int r = 0; r < ROWS; r++) {
        memset(canvas[r], FILL_CH, COLS);
        canvas[r][COLS] = '\0';
    }
}

void plot(int r, int c) {
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
        canvas[r][c] = DRAW_CH;
}

/* ════════════════════════════════════════
   DRAWING FUNCTIONS
   ════════════════════════════════════════ */

/* Bresenham's Line Algorithm */
void draw_line(int r1, int c1, int r2, int c2) {
    int dr = abs(r2 - r1), dc = abs(c2 - c1);
    int sr = (r1 < r2) ? 1 : -1;
    int sc = (c1 < c2) ? 1 : -1;
    int err = dr - dc;
    while (1) {
        plot(r1, c1);
        if (r1 == r2 && c1 == c2) break;
        int e2 = 2 * err;
        if (e2 > -dc) { err -= dc; r1 += sr; }
        if (e2 <  dr) { err += dr; c1 += sc; }
    }
}

/* Midpoint Circle Algorithm */
void draw_circle(int cr, int cc, int rad) {
    int x = 0, y = rad, d = 1 - rad;
    while (x <= y) {
        plot(cr+y, cc+x); plot(cr-y, cc+x);
        plot(cr+y, cc-x); plot(cr-y, cc-x);
        plot(cr+x, cc+y); plot(cr-x, cc+y);
        plot(cr+x, cc-y); plot(cr-x, cc-y);
        if (d < 0) d += 2*x + 3;
        else { d += 2*(x-y) + 5; y--; }
        x++;
    }
}

/* Rectangle using 4 lines */
void draw_rect(int r1, int c1, int r2, int c2) {
    draw_line(r1, c1, r1, c2);
    draw_line(r2, c1, r2, c2);
    draw_line(r1, c1, r2, c1);
    draw_line(r1, c2, r2, c2);
}

/* Triangle using 3 lines */
void draw_triangle(int r1,int c1,int r2,int c2,int r3,int c3) {
    draw_line(r1,c1,r2,c2);
    draw_line(r2,c2,r3,c3);
    draw_line(r3,c3,r1,c1);
}

void render_object(const Object *o) {
    switch (o->type) {
        case OBJ_CIRCLE:   draw_circle(o->y1, o->x1, o->radius); break;
        case OBJ_RECT:     draw_rect(o->y1, o->x1, o->y2, o->x2); break;
        case OBJ_LINE:     draw_line(o->y1, o->x1, o->y2, o->x2); break;
        case OBJ_TRIANGLE: draw_triangle(o->y1,o->x1,o->y2,o->x2,o->y3,o->x3); break;
    }
}

void rebuild_canvas(void) {
    canvas_clear();
    for (int i = 0; i < obj_count; i++)
        if (objects[i].active) render_object(&objects[i]);
}

/* ════════════════════════════════════════
   DISPLAY FUNCTION
   ════════════════════════════════════════ */

void display_canvas(void) {
    printf("\n");
    /* Top border */
    printf("+");
    for (int c = 0; c < COLS; c++) printf("-");
    printf("+\n");

    for (int r = 0; r < ROWS; r++) {
        printf("|%s|\n", canvas[r]);
    }

    /* Bottom border */
    printf("+");
    for (int c = 0; c < COLS; c++) printf("-");
    printf("+\n");
}

/* ════════════════════════════════════════
   INPUT HELPERS
   ════════════════════════════════════════ */

int get_int(const char *prompt) {
    int val;
    printf("%s: ", prompt);
    scanf("%d", &val);
    return val;
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ════════════════════════════════════════
   OPERATIONS
   ════════════════════════════════════════ */

void op_add(void) {
    if (obj_count >= MAX_OBJECTS) {
        printf("Canvas full! Max %d objects.\n", MAX_OBJECTS);
        return;
    }

    printf("\n--- Add Object ---\n");
    printf("1. Circle\n2. Rectangle\n3. Line\n4. Triangle\n");
    printf("Choose shape: ");
    int type; scanf("%d", &type);

    if (type < 1 || type > 4) {
        printf("Invalid choice.\n");
        return;
    }

    Object o; memset(&o, 0, sizeof(o));
    o.type = type; o.active = 1; o.id = next_id++;

    switch (type) {
        case OBJ_CIRCLE:
            printf("\n-- Circle --\n");
            o.x1     = get_int("Centre col (0-79)");
            o.y1     = get_int("Centre row (0-39)");
            o.radius = get_int("Radius");
            break;
        case OBJ_RECT:
            printf("\n-- Rectangle --\n");
            o.x1 = get_int("Top-left col  (0-79)");
            o.y1 = get_int("Top-left row  (0-39)");
            o.x2 = get_int("Bot-right col (0-79)");
            o.y2 = get_int("Bot-right row (0-39)");
            break;
        case OBJ_LINE:
            printf("\n-- Line --\n");
            o.x1 = get_int("Start col (0-79)");
            o.y1 = get_int("Start row (0-39)");
            o.x2 = get_int("End col   (0-79)");
            o.y2 = get_int("End row   (0-39)");
            break;
        case OBJ_TRIANGLE:
            printf("\n-- Triangle --\n");
            o.x1 = get_int("Vertex-1 col (0-79)");
            o.y1 = get_int("Vertex-1 row (0-39)");
            o.x2 = get_int("Vertex-2 col (0-79)");
            o.y2 = get_int("Vertex-2 row (0-39)");
            o.x3 = get_int("Vertex-3 col (0-79)");
            o.y3 = get_int("Vertex-3 row (0-39)");
            break;
    }

    objects[obj_count++] = o;
    render_object(&o);

    const char *names[] = {"","Circle","Rectangle","Line","Triangle"};
    printf("\nAdded %s with ID=%d.\n", names[type], o.id);
    display_canvas();
}

int find_by_id(int id) {
    for (int i = 0; i < obj_count; i++)
        if (objects[i].active && objects[i].id == id) return i;
    return -1;
}

void op_delete(void) {
    printf("\n--- Delete Object ---\n");
    int id = get_int("Enter object ID to delete");
    int idx = find_by_id(id);
    if (idx < 0) { printf("ID %d not found.\n", id); return; }
    objects[idx].active = 0;
    rebuild_canvas();
    printf("Deleted object ID=%d.\n", id);
    display_canvas();
}

void op_modify(void) {
    printf("\n--- Modify Object ---\n");
    int id = get_int("Enter object ID to modify");
    int idx = find_by_id(id);
    if (idx < 0) { printf("ID %d not found.\n", id); return; }

    Object *o = &objects[idx];
    const char *names[] = {"","Circle","Rectangle","Line","Triangle"};
    printf("Modifying %s (ID=%d)\n", names[o->type], o->id);

    switch (o->type) {
        case OBJ_CIRCLE:
            printf("Current: centre(%d,%d) radius=%d\n", o->x1, o->y1, o->radius);
            o->x1     = get_int("New centre col");
            o->y1     = get_int("New centre row");
            o->radius = get_int("New radius");
            break;
        case OBJ_RECT:
            printf("Current: top-left(%d,%d) bot-right(%d,%d)\n", o->x1,o->y1,o->x2,o->y2);
            o->x1 = get_int("New top-left col");
            o->y1 = get_int("New top-left row");
            o->x2 = get_int("New bot-right col");
            o->y2 = get_int("New bot-right row");
            break;
        case OBJ_LINE:
            printf("Current: start(%d,%d) end(%d,%d)\n", o->x1,o->y1,o->x2,o->y2);
            o->x1 = get_int("New start col");
            o->y1 = get_int("New start row");
            o->x2 = get_int("New end col");
            o->y2 = get_int("New end row");
            break;
        case OBJ_TRIANGLE:
            printf("Current: v1(%d,%d) v2(%d,%d) v3(%d,%d)\n",o->x1,o->y1,o->x2,o->y2,o->x3,o->y3);
            o->x1 = get_int("New V1 col"); o->y1 = get_int("New V1 row");
            o->x2 = get_int("New V2 col"); o->y2 = get_int("New V2 row");
            o->x3 = get_int("New V3 col"); o->y3 = get_int("New V3 row");
            break;
    }

    rebuild_canvas();
    printf("Modified ID=%d.\n", id);
    display_canvas();
}

void op_list(void) {
    printf("\n--- Object List ---\n");
    int found = 0;
    const char *names[] = {"","Circle","Rectangle","Line","Triangle"};
    for (int i = 0; i < obj_count; i++) {
        if (!objects[i].active) continue;
        found = 1;
        Object *o = &objects[i];
        printf("[ID=%d] %s  ", o->id, names[o->type]);
        switch (o->type) {
            case OBJ_CIRCLE:
                printf("centre(%d,%d) radius=%d", o->x1, o->y1, o->radius); break;
            case OBJ_RECT:
                printf("top-left(%d,%d) bot-right(%d,%d)", o->x1,o->y1,o->x2,o->y2); break;
            case OBJ_LINE:
                printf("start(%d,%d) end(%d,%d)", o->x1,o->y1,o->x2,o->y2); break;
            case OBJ_TRIANGLE:
                printf("v1(%d,%d) v2(%d,%d) v3(%d,%d)",o->x1,o->y1,o->x2,o->y2,o->x3,o->y3); break;
        }
        printf("\n");
    }
    if (!found) printf("No objects on canvas.\n");
}

void op_clear(void) {
    for (int i = 0; i < obj_count; i++) objects[i].active = 0;
    obj_count = 0;
    canvas_clear();
    printf("Canvas cleared.\n");
    display_canvas();
}

void print_menu(void) {
    printf("\n=============================\n");
    printf("    2D GRAPHICS EDITOR\n");
    printf("=============================\n");
    printf(" 1. Add Object\n");
    printf(" 2. Delete Object\n");
    printf(" 3. Modify Object\n");
    printf(" 4. Display Canvas\n");
    printf(" 5. Clear Canvas\n");
    printf(" 6. List Objects\n");
    printf(" 0. Quit\n");
    printf("=============================\n");
    printf("Objects on canvas: %d/%d\n", obj_count, MAX_OBJECTS);
    printf("Choice: ");
}

/* ════════════════════════════════════════
   MAIN
   ════════════════════════════════════════ */

int main(void) {
    canvas_clear();

    printf("Welcome to 2D Graphics Editor!\n");
    printf("Canvas: %d rows x %d cols\n", ROWS, COLS);
    printf("Background: '%c'  |  Draw char: '%c'\n", FILL_CH, DRAW_CH);

    int choice;
    do {
        print_menu();
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1: op_add();         break;
            case 2: op_delete();      break;
            case 3: op_modify();      break;
            case 4: display_canvas(); break;
            case 5: op_clear();       break;
            case 6: op_list();        break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}
