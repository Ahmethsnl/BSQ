#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct s_mapinfo{
    int rows;
    int cols;
    char empty;
    char obs;
    char full;
} t_mapinfo;
int ft_strlen(char *str) { int i = 0; while (str[i]) i++; return i; }
void fill_square(char **map_lines, int row, int col, int size, t_mapinfo *info) {
    for (int i = row - size + 1; i <= row; i++)
        for (int j = col - size + 1; j <= col; j++)
            map_lines[i][j] = info->full;}
int first_line_ctrl(FILE *file, t_mapinfo *info) {
    return fscanf(file, "%d %c %c %c", &info->rows, &info->empty, &info->obs, &info->full) != 4 || info->rows < 1 ||
           info->empty == info->obs || info->empty == info->full || info->full == info->obs ||
           info->empty < 32 || info->obs < 32 || info->full < 32 || info->empty > 126 || info->obs > 126 || info->full > 126;}
char **read_lines(FILE *file, int *line_cont) {
    char *line = NULL, **lines = NULL; size_t len = 0, count = 0;
    while (getline(&line, &len, file) != -1) {
        char **tmp = realloc(lines, sizeof(char *) * (count + 1));
        if (!tmp) return NULL;
        lines = tmp; lines[count++] = line; line = NULL;
    }
    *line_cont = count; return lines;}
int map_ctrl(char **map_lines, int line_count, t_mapinfo *info) {
    if (line_count - 1 != info->rows) return 1;
    info->cols = ft_strlen(map_lines[1]);
    if (map_lines[1][info->cols - 1] == '\n') info->cols--;
    for (int i = 1; i < line_count; i++) {
        int len = ft_strlen(map_lines[i]);
        if (map_lines[i][len - 1] == '\n') len--;
        if (len != info->cols) return 1;
        for (int j = 0; j < len; j++)
            if (map_lines[i][j] != info->empty && map_lines[i][j] != info->obs) return 1;
    }
    return 0;}
int solve_map(char **map_lines, int line_count, t_mapinfo *info) {
    if (map_ctrl(map_lines, line_count, info)) return 1;
    int **dp = calloc(info->rows, sizeof(int *)), max_size = 0, max_row = 0, max_col = 0;
    for (int i = 0; i < info->rows; i++) {
        dp[i] = calloc(info->cols, sizeof(int));
        for (int j = 0; j < info->cols; j++) {
            if (map_lines[i + 1][j] == info->obs) dp[i][j] = 0;
            else {
                dp[i][j] = (i == 0 || j == 0) ? 1 : 1 + (dp[i - 1][j] < dp[i][j - 1] ? (dp[i - 1][j] < dp[i - 1][j - 1] ? dp[i - 1][j] : dp[i - 1][j - 1]) : (dp[i][j - 1] < dp[i - 1][j - 1] ? dp[i][j - 1] : dp[i - 1][j - 1]));
                if (dp[i][j] > max_size) max_size = dp[i][j], max_row = i, max_col = j;}}}
    if (max_size > 0) fill_square(map_lines, max_row + 1, max_col, max_size, info);
    for (int i = 1; i < line_count; i++) fputs(map_lines[i], stdout);
    return 0;}
int process_stdin() {
    t_mapinfo info; int line_count = 0;
    char **map_lines = first_line_ctrl(stdin, &info) ? NULL : read_lines(stdin, &line_count);
    return (!map_lines || line_count == 0) ? 1 : solve_map(map_lines, line_count, &info);}
int process_file(char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (!file) return 1;
    t_mapinfo info; int line_count = 0;
    char **map_lines = first_line_ctrl(file, &info) ? NULL : read_lines(file, &line_count);
    fclose(file);
    return (!map_lines || line_count == 0) ? 1 : solve_map(map_lines, line_count, &info);}
int main(int ac, char **av) {
    if (ac == 1) process_stdin();
    else for (int i = 1; i < ac; i++) {
        if (process_file(av[i])) fprintf(stderr, "Error: map error.");
        if (i < ac - 1) fputs("\n", stdout);
    }
    return 0;}