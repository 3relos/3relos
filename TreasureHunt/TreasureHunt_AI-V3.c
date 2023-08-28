/* Treasure Hunt is evolving.... It's evolving to... an better AI! */
/*
cd /mnt/e/sciebo/Uni/CP/C-Kurs/Tag_9
gcc -Wall -Wpedantic TreasureHunt_AI-V3.c -o TreasureHunt -lm
./TreasureHunt TreasureHunt11.txt
*/

#include <stdlib.h>
#include <stdio.h>

#define GENEROUS 100 // Wie oft, darf das Programm über den selben Punkt laufen

// Strukt für 2D-Position
typedef struct pos_t {
    int x;
    int y;
}pos_t;

// Struct um alle Werte für einen Nachbarn zu bestimmen
typedef struct state_t {
    char value;
    int leaves;
}state_t;

/*
typedef struct neigbors_t {
    state_t right;
    state_t left;
    state_t up;
    state_t down;
}neigbors_t;*/

//==========================================================================================
// Speicher allokieren und freigeben
char **alloc_mem (int l, int c) {
    char **file_mem = calloc(l,sizeof(char*));
    if (file_mem == NULL) {
        printf("Memory could not be allocated!\n");
        exit(0);
    }
    for (int i = 0; i<l; i++) {
        file_mem[i] = calloc(c,sizeof(char));
        if (file_mem[i] == NULL) {
            printf("Memory could not be allocated!\n");
            exit(0);
        }
    }
    return file_mem;
}

int **alloc_mem_int (int l, int c) {
    int **file_mem = calloc(l,sizeof(int*));
    if (file_mem == NULL) {
        printf("Memory could not be allocated!\n");
        exit(0);
    }
    for (int i = 0; i<l; i++) {
        file_mem[i] = calloc(c,sizeof(int));
        if (file_mem[i] == NULL) {
            printf("Memory could not be allocated!\n");
            exit(0);
        }
    }
    return file_mem;
}

void free_mem (char **A, int n) {
    int k;
    for (k = 0; k<n; k++){
        free(A[k]);
    } 
    free(A);
}

void free_mem_int (int **A, int n) {
    int k;
    for (k = 0; k<n; k++){
        free(A[k]);
    } 
    free(A);
}

//==========================================================================================
// Ausgeben des Labyrinths
void print_laby(char **laby, int l, int c) {
    int i,k;
    for (i=0; i<l; i++) {
        for (k=0; k<c; k++){
            if (laby[i][k] == 36) {         // 36 = $
                printf("\033[1m\033[41m%c\033[0m", laby[i][k]);
            }
            else if (laby[i][k] == 42) {    // 42 = *
                printf("\033[1m\033[42m%c\033[0m", laby[i][k]);
            }
            else if (laby[i][k] == 46) {    // 46 = ., 48 = 0
                printf("\033[36m%c\033[0m", laby[i][k]);
            }
            else {
                printf("%c", laby[i][k]);
            }
        }
        printf("\n");
    }
    printf("\n");
    return;
}

// Nachbar initialisieren; Felder außerhalb des Bereichs auf "X"=88 setzen
// Prefere right -- 0 = right, 1 = left, 2 = down, 3 = up
void find_neighbor_pr(pos_t pos, int **temp, char **laby, int cols, int lines, state_t nei_temp[]) {
    nei_temp[0].leaves = temp[pos.x][pos.y+1];
    nei_temp[1].leaves = temp[pos.x][pos.y-1];
    nei_temp[2].leaves = temp[pos.x+1][pos.y];
    nei_temp[3].leaves = temp[pos.x-1][pos.y];
    if (pos.x == 0) {
        if (pos.y == 0) {
            nei_temp[0].value = laby[pos.x][pos.y+1];
            nei_temp[1].value = 88;
            nei_temp[2].value = laby[pos.x+1][pos.y];
            nei_temp[3].value = 88;
        }
        else if(pos.y == cols-1) {
            nei_temp[0].value = 88;
            nei_temp[1].value = laby[pos.x][pos.y-1];
            nei_temp[2].value = laby[pos.x+1][pos.y];
            nei_temp[3].value = 88;
        }
        else {
            nei_temp[0].value = laby[pos.x][pos.y+1];
            nei_temp[1].value = laby[pos.x][pos.y-1];
            nei_temp[2].value = laby[pos.x+1][pos.y];
            nei_temp[3].value = 88;
        }
    }
    else if (pos.x == lines-1) {
        if (pos.y == 0) {
            nei_temp[0].value = laby[pos.x][pos.y+1];
            nei_temp[1].value = 88;
            nei_temp[2].value = 88;
            nei_temp[3].value = laby[pos.x-1][pos.y];
        }
        else if(pos.y == cols-1) {
            nei_temp[0].value = 88;
            nei_temp[1].value = laby[pos.x][pos.y-1];
            nei_temp[2].value = 88;
            nei_temp[3].value = laby[pos.x-1][pos.y];
        }
        else {
            nei_temp[0].value = laby[pos.x][pos.y+1];
            nei_temp[1].value = laby[pos.x][pos.y-1];
            nei_temp[2].value = 88;
            nei_temp[3].value = laby[pos.x-1][pos.y];
        }
    }
    else if (pos.y == 0) {
        nei_temp[0].value = laby[pos.x][pos.y+1];
        nei_temp[1].value = 88;
        nei_temp[2].value = laby[pos.x+1][pos.y];
        nei_temp[3].value = laby[pos.x-1][pos.y];
    }
    else if (pos.y == cols-1) {
        nei_temp[0].value = 88;
        nei_temp[1].value = laby[pos.x][pos.y-1];
        nei_temp[2].value = laby[pos.x+1][pos.y];
        nei_temp[3].value = laby[pos.x-1][pos.y];
    }
    else {
        nei_temp[0].value = laby[pos.x][pos.y+1];
        nei_temp[1].value = laby[pos.x][pos.y-1];
        nei_temp[2].value = laby[pos.x+1][pos.y];
        nei_temp[3].value = laby[pos.x-1][pos.y];
    }
    return;
}

// Prefere left -- 0 = left, 1 = down, 2 = up, 3 = right
void find_neighbor_pl(pos_t pos, int **temp, char **laby, int cols, int lines, state_t nei_temp[]) {
    nei_temp[0].leaves = temp[pos.x][pos.y-1];
    nei_temp[1].leaves = temp[pos.x+1][pos.y];
    nei_temp[2].leaves = temp[pos.x-1][pos.y];
    nei_temp[3].leaves = temp[pos.x][pos.y+1];
    if (pos.x == 0) {
        if (pos.y == 0) {
            nei_temp[0].value = 88;
            nei_temp[1].value = laby[pos.x+1][pos.y];
            nei_temp[2].value = 88;
            nei_temp[3].value = laby[pos.x][pos.y+1];
        }
        else if(pos.y == cols-1) {
            nei_temp[0].value = laby[pos.x][pos.y-1];
            nei_temp[1].value = laby[pos.x+1][pos.y];
            nei_temp[2].value = 88;
            nei_temp[3].value = 88;
        }
        else {
            nei_temp[0].value = laby[pos.x][pos.y-1];
            nei_temp[1].value = laby[pos.x+1][pos.y];
            nei_temp[2].value = 88;
            nei_temp[3].value = laby[pos.x][pos.y+1];
        }
    }
    else if (pos.x == lines-1) {
        if (pos.y == 0) {
            nei_temp[0].value = 88;
            nei_temp[1].value = 88;
            nei_temp[2].value = laby[pos.x-1][pos.y];
            nei_temp[3].value = laby[pos.x][pos.y+1];
        }
        else if(pos.y == cols-1) {
            nei_temp[0].value = laby[pos.x][pos.y-1];
            nei_temp[1].value = 88;
            nei_temp[2].value = laby[pos.x-1][pos.y];
            nei_temp[3].value = 88;
        }
        else {
            nei_temp[0].value = laby[pos.x][pos.y-1];
            nei_temp[1].value = 88;
            nei_temp[2].value = laby[pos.x-1][pos.y];
            nei_temp[3].value = laby[pos.x][pos.y+1];
        }
    }
    else if (pos.y == 0) {
        nei_temp[0].value = 88;
        nei_temp[1].value = laby[pos.x+1][pos.y];
        nei_temp[2].value = laby[pos.x-1][pos.y];
        nei_temp[3].value = laby[pos.x][pos.y+1];
    }
    else if (pos.y == cols-1) {
        nei_temp[0].value = laby[pos.x][pos.y-1];
        nei_temp[1].value = laby[pos.x+1][pos.y];
        nei_temp[2].value = laby[pos.x-1][pos.y];
        nei_temp[3].value = 88;
    }
    else {
        nei_temp[0].value = laby[pos.x][pos.y-1];
        nei_temp[1].value = laby[pos.x+1][pos.y];
        nei_temp[2].value = laby[pos.x-1][pos.y];
        nei_temp[3].value = laby[pos.x][pos.y+1];
    }
    return;
}

// Prefere down -- 0 = down, 1 = up, 2 = right, 3 = left
void find_neighbor_pd(pos_t pos, int **temp, char **laby, int cols, int lines, state_t nei_temp[]) {
    nei_temp[0].leaves = temp[pos.x+1][pos.y];
    nei_temp[1].leaves = temp[pos.x-1][pos.y];
    nei_temp[2].leaves = temp[pos.x][pos.y+1];
    nei_temp[3].leaves = temp[pos.x][pos.y-1];
    if (pos.x == 0) {
        if (pos.y == 0) {
            nei_temp[0].value = laby[pos.x+1][pos.y];
            nei_temp[1].value = 88;
            nei_temp[2].value = laby[pos.x][pos.y+1];
            nei_temp[3].value = 88;
        }
        else if(pos.y == cols-1) {
            nei_temp[0].value = laby[pos.x+1][pos.y];
            nei_temp[1].value = 88;
            nei_temp[2].value = 88;
            nei_temp[3].value = laby[pos.x][pos.y-1];
        }
        else {
            nei_temp[0].value = laby[pos.x+1][pos.y];
            nei_temp[1].value = 88;
            nei_temp[2].value = laby[pos.x][pos.y+1];
            nei_temp[3].value = laby[pos.x][pos.y-1];
        }
    }
    else if (pos.x == lines-1) {
        if (pos.y == 0) {
            nei_temp[0].value = 88;
            nei_temp[1].value = laby[pos.x-1][pos.y];
            nei_temp[2].value = laby[pos.x][pos.y+1];
            nei_temp[3].value = 88;
        }
        else if(pos.y == cols-1) {
            nei_temp[0].value = 88;
            nei_temp[1].value = laby[pos.x-1][pos.y];
            nei_temp[2].value = 88;
            nei_temp[3].value = laby[pos.x][pos.y-1];
        }
        else {
            nei_temp[0].value = 88;
            nei_temp[1].value = laby[pos.x-1][pos.y];
            nei_temp[2].value = laby[pos.x][pos.y+1];
            nei_temp[3].value = laby[pos.x][pos.y-1];
        }
    }
    else if (pos.y == 0) {
        nei_temp[0].value = laby[pos.x+1][pos.y];
        nei_temp[1].value = laby[pos.x-1][pos.y];
        nei_temp[2].value = laby[pos.x][pos.y+1];
        nei_temp[3].value = 88;
    }
    else if (pos.y == cols-1) {
        nei_temp[0].value = laby[pos.x+1][pos.y];
        nei_temp[1].value = laby[pos.x-1][pos.y];
        nei_temp[2].value = 88;
        nei_temp[3].value = laby[pos.x][pos.y-1];
    }
    else {
        nei_temp[0].value = laby[pos.x+1][pos.y];
        nei_temp[1].value = laby[pos.x-1][pos.y];
        nei_temp[2].value = laby[pos.x][pos.y+1];
        nei_temp[3].value = laby[pos.x][pos.y-1];
    }
    return;
}

// Prefere pu -- 0 = up, 1 = right, 2 = left, 3 = down
void find_neighbor_pu(pos_t pos, int **temp, char **laby, int cols, int lines, state_t nei_temp[]) {
    nei_temp[0].leaves = temp[pos.x-1][pos.y];
    nei_temp[1].leaves = temp[pos.x][pos.y+1];
    nei_temp[2].leaves = temp[pos.x][pos.y-1];
    nei_temp[3].leaves = temp[pos.x+1][pos.y];
    if (pos.x == 0) {
        if (pos.y == 0) {
            nei_temp[0].value = 88;
            nei_temp[1].value = laby[pos.x][pos.y+1];
            nei_temp[2].value = 88;
            nei_temp[3].value = laby[pos.x+1][pos.y];
        }
        else if(pos.y == cols-1) {
            nei_temp[0].value = 88;
            nei_temp[1].value = 88;
            nei_temp[2].value = laby[pos.x][pos.y-1];
            nei_temp[3].value = laby[pos.x+1][pos.y];
        }
        else {
            nei_temp[0].value = 88;
            nei_temp[1].value = laby[pos.x][pos.y+1];
            nei_temp[2].value = laby[pos.x][pos.y-1];
            nei_temp[3].value = laby[pos.x+1][pos.y];
        }
    }
    else if (pos.x == lines-1) {
        if (pos.y == 0) {
            nei_temp[0].value = laby[pos.x-1][pos.y];
            nei_temp[1].value = laby[pos.x][pos.y+1];
            nei_temp[2].value = 88;
            nei_temp[3].value = 88;
        }
        else if(pos.y == cols-1) {
            nei_temp[0].value = laby[pos.x-1][pos.y];
            nei_temp[1].value = 88;
            nei_temp[2].value = laby[pos.x][pos.y-1];
            nei_temp[3].value = 88;
        }
        else {
            nei_temp[0].value = laby[pos.x-1][pos.y];
            nei_temp[1].value = laby[pos.x][pos.y+1];
            nei_temp[2].value = laby[pos.x][pos.y-1];
            nei_temp[3].value = 88;
        }
    }
    else if (pos.y == 0) {
        nei_temp[0].value = laby[pos.x-1][pos.y];
        nei_temp[1].value = laby[pos.x][pos.y+1];
        nei_temp[2].value = 88;
        nei_temp[3].value = laby[pos.x+1][pos.y];
    }
    else if (pos.y == cols-1) {
        nei_temp[0].value = laby[pos.x-1][pos.y];
        nei_temp[1].value = 88;
        nei_temp[2].value = laby[pos.x][pos.y-1];
        nei_temp[3].value = laby[pos.x+1][pos.y];
    }
    else {
        nei_temp[0].value = laby[pos.x-1][pos.y];
        nei_temp[1].value = laby[pos.x][pos.y+1];
        nei_temp[2].value = laby[pos.x][pos.y-1];
        nei_temp[3].value = laby[pos.x+1][pos.y];
    }
    return;
}

// Begehbaren Nachbar mit niedrigster Begehung finden
int min_nei (state_t nei[]) {
    int i;
    int pos = -1;
    int res = GENEROUS;
    for (i = 0; i<4; i++) {
        if (nei[i].value != 88) {
            if (nei[i].leaves < res) {
                res = nei[i].leaves;
                pos = i;
            }
        }
    }
    if (pos == -1) {
        printf("No where to go!\n");
        exit(69);
    }
    return pos;
}

//==========================================================================================
// Move to the next field depending on which is best

// Prefere right 
pos_t change_pos_pr (int m, pos_t pos) {
    if (m == 0) {
        pos.y++;
    }
    else if (m == 1) {
        pos.y--;
    }
    else if (m == 2) {
        pos.x++;
    }
    else if (m == 3) {
        pos.x--;
    }
    return pos;
}

// Prefere left 
pos_t change_pos_pl (int m, pos_t pos) {
    if (m == 0) {
        pos.y--;
    }
    else if (m == 1) {
        pos.x++;
    }
    else if (m == 2) {
        pos.x--;
    }
    else if (m == 3) {
        pos.y++;
    }
    return pos;
}

// Prefere down 
pos_t change_pos_pd (int m, pos_t pos) {
    if (m == 0) {
        pos.x++;
    }
    else if (m == 1) {
        pos.x--;
    }
    else if (m == 2) {
        pos.y++;
    }
    else if (m == 3) {
        pos.y--;
    }
    return pos;
}

// Prefere up 
pos_t change_pos_pu (int m, pos_t pos) {
    if (m == 0) {
        pos.x--;
    }
    else if (m == 1) {
        pos.y++;
    }
    else if (m == 2) {
        pos.y--;
    }
    else if (m == 3) {
        pos.x++;
    }
    return pos;
}

int move(char **laby, pos_t pos, int **temp, int cols, int lines, pos_t *way_pos, unsigned long *save_pos, int h) {
    
    //Zustand aller Nachbarn festlegen
    state_t neighbors[4];
    if (h == 0) { //0,4,8,12,16,20,24
        find_neighbor_pr(pos, temp, laby, cols, lines, neighbors);
    }
    else if (h == 1) { //1,5,
        find_neighbor_pl(pos, temp, laby, cols, lines, neighbors);
    }
    else if (h == 2) { //
        find_neighbor_pd(pos, temp, laby, cols, lines, neighbors);
    }
    else if (h == 3) { //
        find_neighbor_pu(pos, temp, laby, cols, lines, neighbors);
    }

    // Abbruchbedingung um unendliche Rekursion zu vermeiden
    int i;
    int score_X = 0;
    for (i = 0; i<4; i++) {
        if (neighbors[i].value == 88) {
            score_X++;
        }
    }
    //printf("Position = %d, %d\n", pos.x, pos.y);
    if (temp[pos.x][pos.y] + score_X == GENEROUS) {
        printf("Labyrinth not solvable!\n");
        print_laby(laby, lines, cols);
        exit(42);
    }

    // Move zum nächsten Feld
    int m,n;
    m = min_nei(neighbors);

    for (n = 0; n<4; n++) {             // Schauen, ob ein Nachbar des Felds das Ziel ist
        if (neighbors[n].value == 36) {
            return 5;
        }
    }

    temp[pos.x][pos.y]++;

    if (h == 0) {
        pos = change_pos_pr(m, pos);
    }
    else if (h == 1) {
        pos = change_pos_pl(m, pos);
    }
    else if (h == 2) {
        pos = change_pos_pd(m, pos);
    }
    else if (h == 3){
        pos = change_pos_pu(m, pos);
    }

    // Weg speichern und dabei sinnvoll (doppelte Punkte -> Weg von pos zu pos löschen)
    int check_sum = 0;
    int check_pos = 0;

    int q;
    for (q=0; q<*save_pos; q++) {
        if (way_pos[q].x == pos.x && way_pos[q].y == pos.y) {
            check_sum++;
            check_pos = q;
			break;			// Bricht beim ersten Eintrag ab (falls mehrere existieren, was nicht so sein sollte)
        }
    }

    if (check_sum != 0) {
        int r;
        for (r = check_pos+1; r<lines*cols; r++) {
            way_pos[r].x = lines+1;
            way_pos[r].y = cols+1;
        }
        *save_pos = check_pos+1;
    }
    else {
        way_pos[*save_pos].x = pos.x;
        way_pos[*save_pos].y = pos.y;
        *save_pos = *save_pos + 1;
    }

    // Rekursiver Aufruf
    int move_to;
    for (int f = h; f < 4; f++) {
        move_to = move(laby, pos, temp, cols, lines, way_pos, save_pos, f);

        if (move_to == 5) {
            //laby[pos.x][pos.y] = 46; // 46 = .
            return 5;
        }
    } 
    return move_to;
}

// Recursive function for moving through the labyrinth
void prep_move(char **laby, pos_t pos, int **temp, int cols, int lines) {

    pos_t way_pos[lines*cols];          // Array zum speichern des gegangenen Weges 
    pos_t *ptr_way_pos = &way_pos[0];
    for (int p=0; p<lines*cols; p++) {
        way_pos[p].x = lines+1;
        way_pos[p].y = cols+1;
    }
    unsigned long save_pos;
    unsigned long *ptr_save_pos = &save_pos;

    // Variablen zum dauerhaften speichern des aktuell kürzesten Weges
    unsigned long curr_score = lines*cols+1;             // Score des aktuell kürzesten Weges
    pos_t curr_way[lines*cols];          // Array zum speichern des gegangenen Weges 

    int h = 0;
	int counter = 0;
    while (h < 4) {
		// Alle relevanten Variablen auf 0 setzen.
		// temp dabei nicht zurück setzen!
		save_pos = 0;
	    for (int p = 0; p<lines*cols; p++) {
	        way_pos[p].x = lines+1;
	        way_pos[p].y = cols+1;
	    }
		
		// Bewegung in Gang setzen
        move(laby, pos, temp, cols, lines, ptr_way_pos, ptr_save_pos, h);

        // Kürzesten Weg suchen
		printf("Score for h = %d: %lu\n", h, save_pos);
        if (save_pos < curr_score) {
            for (int p = 0; p<lines*cols; p++) {
                curr_way[p].x = way_pos[p].x;
                curr_way[p].y = way_pos[p].y;
            }
            curr_score = save_pos;
        }
        h++;
		counter++;
		if (h == 4) {
			h = 0;
		}
		if (counter == GENEROUS/5) {
			break;
		}
    }

    // Ausgeben des Weges im Labyrinth
    int t;
    for (t=0; t<curr_score; t++) {
        laby[curr_way[t].x][curr_way[t].y] = 46; // 46 = .   
    }
    printf("Min score = %lu\n", curr_score);
    return;
}

//==========================================================================================
void find_goal(char **laby, pos_t start_pos, pos_t end_pos, int cols, int lines) {
    //unsigned long score = lines*cols+1;
    //unsigned long *ptr_score = &score;

    pos_t pos;    //Speichert die aktuelle Position
    pos.x = start_pos.x;
    pos.y = start_pos.y;

    int **temp;
    temp = alloc_mem_int(lines, cols);
    int i,k;
    for (i=0; i<lines; i++) {
        for (k=0; k<cols; k++){
            temp[i][k] = 0;
        }
    }

    prep_move(laby, pos, temp, cols, lines);

    print_laby(laby, lines, cols);
    printf("Endposition x = %d, y = %d\n", pos.x, pos.y);

    free_mem_int(temp,lines);
    return;
}

//==========================================================================================
int main (int argc, char *file_name[]) {
    FILE* doc = fopen(file_name[1], "r");
    if ( doc == NULL ) {
		printf("Error while reading the document!\n");
		exit(-1);
	}

    int lines, columns;

    fscanf(doc, "lines = %d, columns = %d\n", &lines, &columns);

    printf("lines = %d, columns = %d\n", lines, columns);

    char **laby;
    laby = alloc_mem(lines, columns);

    //printf("\n");
    int i,k,temp;
    pos_t start_pos, end_pos;
    fseek(doc, -1, SEEK_CUR);
    //long cur_pos = ftell(doc);
    //printf("Cursor Pos = %ld\n", cur_pos);
    for (i=0; i<lines; i++) {
        if ((temp=fgetc(doc)) == EOF) {
            break;
        }
        for (k=0; k<columns; k++){
            temp = fgetc(doc);
            laby[i][k] = temp;
            if(laby[i][k] == 42) {  /* Sucht den Startpunkt (* = 42)*/
                start_pos.x = i;
                start_pos.y = k;
            }
            if(laby[i][k] == 36) {  /* Sucht den Startpunkt ($ = 36)*/
                end_pos.x = i;
                end_pos.y = k;
            }
        }
        fseek(doc, 1, SEEK_CUR);
    }

    printf("Start Position = %d, %d, End Position = %d, %d\n", start_pos.x, start_pos.y, end_pos.x, end_pos.y);

    print_laby(laby, lines, columns);
    
    find_goal(laby, start_pos, end_pos, columns, lines);

    free_mem(laby,lines);
}