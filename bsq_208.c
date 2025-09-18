#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_mapinfo{
    int rows;
    int cols;
    char empty;
    char obs;
    char full;
} t_mapinfo;

int first_line_ctrl(FILE *file, t_mapinfo *info){

    if (fscanf(file, "%d %c %c %c", &info->rows, &info->empty, &info->obs, &info->full) != 4)
        return 1;

    if (info->rows < 1)
        return 1;

    if (info->empty == info->obs || info->empty == info->full || info->full == info->obs)
        return 1;

    if (info->empty < 32 || info->empty > 126)
        return 1;

    if (info->obs < 32 || info->obs > 126)
        return 1;

    if (info->full < 32 || info->full > 126)
        return 1;
    return 0;
}

char **read_lines(FILE *file, int *line_cont){
    char *line = NULL;
    char **lines = NULL;
    size_t len = 0;
    size_t count = 0;
    int read;

    while ((read = getline(&line, &len, file)) != -1)
    {
        char **tmp = realloc(lines, sizeof(char *) * (count + 1));
        if (!tmp){
            free(line);
            for(int i = 0; i < count; i++)
                free(lines[i]);
            free(lines);
            return NULL;
        }
        lines = tmp;
        lines[count] = line;
        line = NULL;
        count++;
    }
    free(line);
    *line_cont = count;
    return lines;
}

int map_ctrl(char **map_lines, int line_count, t_mapinfo *info){
    if (line_count - 1 != info->rows)
        return 1;
    info->cols = strlen(map_lines[1]);
    if (map_lines[1][info->cols - 1] == '\n')
        info->cols--;
    for (int i = 1; i < line_count; i++){
        int len = strlen(map_lines[i]);
        if (map_lines[i][len - 1] == '\n')
            len--;
        if (len != info->cols)
            return 1;
        for(int j = 0; j < len; j++){
            char c = map_lines[i][j];
            if (c != info->empty && c != info->obs)
                return 1;
        }
    }
    return 0;
}

int fill_square(char **map_lines, int row, int col, int size, t_mapinfo *info){
    for (int i = row - size + 1; i <= row; i++){
        for (int j = col - size + 1; j <= col; j++){
            map_lines[i][j] = info->full;
        }
    }
}

int solve_map(char **map_lines, int line_count, t_mapinfo *info){
    if (map_ctrl(map_lines, line_count, info))
        return 1;
    int **dp = calloc(info->rows, sizeof(int *));
    for (int i = 0; i < info->rows; i++){
        dp[i] = calloc(info->cols, sizeof(int));
        if (!dp){
            for (int j = 0; j < i; j++)
                free(dp[j]);
            free(dp);
            return 1;
        }
    }
    int max_size = 0, max_row = 0, max_col = 0;
    for (int i = 0; i < info->rows; i++){
        for (int j = 0; j < info->cols; j++){
            char c = map_lines[i + 1][j];
            if (c == info->obs)
                dp[i][j] = 0;

            else{
                if (i == 0 || j == 0)
                    dp[i][j] = 1;
                else{
                    int min_val = dp[i -1][j];
                    if (dp[i][j - 1] < min_val) min_val = dp[i][j - 1];
                    if (dp[i - 1][j - 1] < min_val) min_val = dp[i - 1][j - 1];
                    dp[i][j] = 1 + min_val;
                }
                if (dp[i][j] > max_size)
                {
                    max_size = dp[i][j];
                    max_row = i;
                    max_col = j;
                }
            }

        }
    }
    if (max_size > 0)
        fill_square(map_lines, max_row + 1, max_col, max_size, info);

    for (int i = 1; i < line_count; i++)
        fputs(map_lines[i], stdout);

    for (int i = 0; i < info->rows; i++)
        free(dp[i]);
    free(dp);

    return 0;
}

int process_stdin(){
    t_mapinfo info;
    int line_count = 0;
    if (first_line_ctrl(stdin, &info))
        return 1;
    char **map_lines = read_lines(stdin, &line_count);
    if (!map_lines || line_count == 0)
    {
        if (map_lines)
        {
            for (int i = 0; i < line_count; i++)
                free(map_lines[i]);
            free(map_lines);
            return 1;
        }
    }
    int res = solve_map(map_lines, line_count, &info);
    for (int i = 0; i < line_count; i++)
        map_lines[i];
    free(map_lines);
    return res;
}

int process_file(char *file_name){
    FILE *file = fopen(file_name, "r");
    if (!file)
        return 1;

    t_mapinfo info;
    int line_count = 0;
    if (first_line_ctrl(file, &info))
        return 1;

    char **map_lines = read_lines(file, &line_count);
    if (!map_lines || line_count == 0){
        if (map_lines)
        {
            for (int i = 0; i < line_count; i++)
                free(map_lines[i]);
            free(map_lines);
            return 0;
        }
    }
    int res = solve_map(map_lines, line_count, &info);
    for (int i = 0; i < line_count; i++)
        map_lines[i];
    free(map_lines);
    return res;
}

int main(int argc, char **argv){
    if (argc == 1){
        if (process_stdin() != 0)
            fprintf(stderr, "Error: map error.");
    }
    else{
        for (int i = 1; i < argc; i++){
            if (process_file(argv[i]))
                fprintf(stderr, "Error: map error.");

            if (i < argc - 1)
                fputs("\n", stdout);
        }
    }
    return 0;
}