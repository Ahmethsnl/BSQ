#include <stdio.h>
#include <stdlib.h>
int ft_strlen(char *str){int i = 0; while(str[i]) {i++;}  return i;}
int min3 (int a, int b, int c){
    int min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}
typedef struct {
    int rows, cols;
    char empty, obs, full;
} map;
int parse_first_line(FILE *file, map *m){
    if (fscanf(file, "%d%c%c%c", &m->rows, &m->empty, &m->obs, &m->full) != 4) return 1;
    if (m->rows < 1) return 1;
    if (m->empty == m->full || m->empty == m->obs || m->full == m->obs) return 1;
    if (m->empty < 32 || m->empty > 126 ||m->full < 32 || m->full > 126 || m->obs < 32 ||m->obs > 126) return 1;
    return 0;
}
char** read_grid(FILE *file, int *count){
    char **grid = NULL;
    char *line = NULL;
    size_t len = 0;
    int read, n = 0;
    while ((read = getline(&line, &len, file)) != -1){
        char **tmp = realloc(grid, sizeof(char *) * (n + 1));
        grid = tmp;
        grid[n] = line;
        line = NULL;
        n++;
    }
    *count = n;
    return grid;
}
int check_grid(char **grid, int count, map *m){
    if (count - 1 != m->rows) return 1;
    int cols = ft_strlen(grid[1]);
    if (grid[1][cols - 1] == '\n') cols --;
    m->cols = cols;
    for (int i = 1; i < count; i++){
        int len = ft_strlen(grid[i]);
        if (grid[i][len - 1] == '\n') len--;
        if (len != m->cols) return 1;
    }
    return 0;
}
void mark_square(char **grid, int row, int col, int size, map *m){
    for (int i = row - size + 1; i <= row; i++)
        for (int j = col - size + 1; j <= col; j++)
            grid[i][j] = m->full;
}
int solve_grid(char **grid, int count, map *m){
    if (check_grid(grid, count, m)) return 1;
    int **dp = calloc(m->rows, sizeof(int *));
    for (int i = 0; i < m->rows; i++)
        dp[i] = calloc (m->cols, sizeof(int));
    int max_size = 0, best_row = 0, best_col = 0;
    for (int i = 0; i < m->rows; i++){
        for (int j = 0; j < m->cols; j++){
            char c = grid[i + 1][j];
            if (c == m->obs) dp[i][j] = 0;
            else {
                if (i == 0 || j == 0) dp[i][j] = 1;
                else 
                    dp[i][j] = 1 + min3(dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]);
                if (dp[i][j] > max_size){
                    max_size = dp[i][j];
                    best_col = j;
                    best_row = i;
                }
            }
        }
    }
    if (max_size > 0) mark_square(grid, best_row + 1, best_col, max_size, m);
    for (int i = 1; i < count; i++)
        fputs(grid[i], stdout);
    return 0;
}
int process_stdin(){
    map m;
    int count = 0;
    if (parse_first_line(stdin, &m)) return 1;
    char **grid = read_grid(stdin, &count);
    if (!grid || count == 0) return 1;
    int res = solve_grid(grid, count, &m);
    return res;
}
int process_file(char *filename){
    FILE *file = fopen(filename, "r");
    map m;
    int count = 0;
    if (parse_first_line(file, &m)){
        fclose(file);
        return 1;
    }
    char **grid = read_grid(file, &count);
    fclose(file);
    if (!grid || count == 0) return 1;
    int res = solve_grid(grid, count, &m);
    return res;
}
int main (int ac, char **av){
    if (ac == 1){
        if (process_stdin() != 0) fprintf(stderr, "map error\n");}
    else {
        for (int i = 1; i < ac; i++){
            if (process_file(av[i])) fprintf(stderr, "map error\n");
            if (i < ac - 1) fputs("\n", stdout);
        }
    }
    return 0;
}
