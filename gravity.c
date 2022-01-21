// Author: Jeremy Lee z5316983
// Date: Yes
// UNSW Freefall assignment.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 15
#define EMPTY 0
#define STONE 1
#define MOVEMENT 1  // lower case is my #defines
#define LASER 2
#define SHIFT 3
#define FLIP 4
#define GRAVITY 5
#define NEG_SIZE 150

// TODO: Add any extra #defines here.

void printMap(int map[SIZE][SIZE], int playerX);
void printBlocks(int map[SIZE][SIZE], int playerX, int negative_map[NEG_SIZE][SIZE]);
void move(int map[SIZE][SIZE], int *playerXY);
void shoot(int map[SIZE][SIZE], int playerX, int *gameCon);
void shift_down(int map[SIZE][SIZE], int playerX, int *gameCon, int *marching, int negative_map[NEG_SIZE][SIZE], int *neg_row);
void flip(int map[SIZE][SIZE], int *flip_count, int playerX);
void detonate(int map[SIZE][SIZE], int tnt_size, int lowest_row, int playerX);
void gravity_storm(int map[SIZE][SIZE], int playerX, int *moved);

// TODO: Add any extra function prototypes here.

int main (void) {
    // This line creates our 2D array called "map" and sets all
    // of the blocks in the map to EMPTY.
    int map[SIZE][SIZE] = {EMPTY};  
    int negative_map[NEG_SIZE][SIZE] = {EMPTY};
    int playerX = SIZE / 2;
    printBlocks(map, playerX, negative_map);
     
    int command;    
    int winCondition = 0;
    int flipped = 0;
    int neg_row = 1;
    int marching = 1;
    int moved = 0;
    while (winCondition == 0 && scanf("%d", &command) == 1) {
        if (command == MOVEMENT) {
            move(map, &playerX); 
            printMap(map, playerX);
        } else if (command == LASER) {
            shoot(map, playerX, &winCondition);
        } else if (command == SHIFT) {
            shift_down(map, playerX, &winCondition, &marching, negative_map, &neg_row);            
        } else if (command == FLIP) {
            flip(map, &flipped, playerX);
        } else if (command == GRAVITY) {
            gravity_storm(map, playerX, &moved);
        }
    }   
    return 0;
}

// Print out the contents of the map array. Then print out the player line
// which will depends on the playerX variable.
void printMap(int map[SIZE][SIZE], int playerX) {    
    // Print values from the map array.
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", map[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }        
    // Print the player line.
    i = 0;
    while (i < playerX) {
        printf("  ");
        i++;
    }
    printf("P\n");
}

// This function counts, scans and prints lines of stone on the map
void printBlocks(int map[SIZE][SIZE], int playerX, int negative_map[NEG_SIZE][SIZE]) { 
    // This line creates out playerX variable. The player starts in the
    // middle of the map, at position 7.
    int lines;    
    printf("How many lines of stone? ");
    scanf("%d", &lines);
    
    printf("Enter lines of stone:\n");   
    int row;
    int column;
    int length;
    int value;    
    int i = 0;
    
    while (i < lines) { // as long as there are lines to be displayed      
        scanf("%d %d %d %d", &row, &column, &length, &value);        
        if (column + length > SIZE || 
            row > SIZE - 1 ||  
            column < 0) { // if it exceeds max size, skip it
            i++;
        } else if (row < 0) {
            int j = 0;
            while (j < length) { 
                negative_map[row * (- 1)][column++] = value;               
                j++; 
            }
            i++;
        
        } else { //if not, display it
            int j = 0;
            while (j < length) { 
                map[row][column++] = value;               
                j++; 
            }
            i++;
        }
    }
    printMap(map, playerX);
  
}

// yhis function allows us to move
void move(int map[SIZE][SIZE], int *playerXY) {    
    int direction;
    scanf("%d", &direction);           
    if ((direction == 1 || direction == -1) && 
        *playerXY < SIZE - 1 && 
        *playerXY > 0) { // If we are still within the endpoints
        *playerXY += direction;         
    } else if (*playerXY == SIZE - 1) { // if we hit right wall
        if (direction == -1) { //move left, print
            *playerXY += direction;
        }            
    } else if (*playerXY == 0) { // if we hit left wall            
        if (direction == 1) { //move right, print
            *playerXY += direction;
        }            
    }      
}

// This function allows us to shoot 
void shoot(int map[SIZE][SIZE], int playerX, int *gameCon) {
    // shooting     
    int lowest_row = 14;
    int blocks_destroyed = 0;
    while (lowest_row >= 0 && blocks_destroyed < 3) {
        if (map[lowest_row][playerX] == 1 || map[lowest_row][playerX] == 2) {
            map[lowest_row][playerX] = 0;
            lowest_row--;
            blocks_destroyed++;
        } else if (map[lowest_row][playerX] > 2) { // if it is a tnt block
            int tnt_size = 0;
            tnt_size = map[lowest_row][playerX];
            detonate(map, tnt_size, lowest_row, playerX);        
            blocks_destroyed = 3; // stop shooting
        
        } else if (map[lowest_row][playerX] == 0) {
            lowest_row--;
        }
    }   
    
    // scan every line
    int row = 14;
    while (row >= 0) {
        int col = 0;
        while (col < SIZE) {
            if (map[row][col] > 0) { // haven't won yet
                col = SIZE; // end 2nd loop
                row = -1; // end 1st loop                
            } else if (row == 0 && 
                       col == SIZE - 1 && 
                       map[0][SIZE - 1] == 0) { // if the top right box = 0
                *gameCon = 1; 
            }                        
            col++;
        }                
        row--;
    }        
    if (*gameCon == 1) { // empty map
        printMap(map, playerX);
        printf("Game Won!\n");
    } else { // still has blocks
        printMap(map, playerX);
    }   
}

// tnt detonator
void detonate(int map[SIZE][SIZE], int tnt_size, int lowest_row, int playerX) {    
    int row = lowest_row - tnt_size + 1;
    while (row <= lowest_row + tnt_size - 1) {
        if (row < SIZE && row >= 0) {
            int col = 0;
            while (col < SIZE) {
                double distance = sqrt(pow(col - playerX, 2) + 
                                       pow(row - lowest_row, 2));
                if (distance < tnt_size && map[row][col] > 0) { // check range                  
                    map[row][col] = 0;                    
                }                
                col++;
            }    
            row++;
        } else {
            row++;
        }
    }  
}    

// This function shifts the entire map down 1 
void shift_down(int map[SIZE][SIZE], int playerX, 
                int *gameCon, int *marching, 
                int negative_map[NEG_SIZE][SIZE], int *neg_row) {
    int row = 13; // starting at row 14 (array[13][?]) since row 15 clears    
    int j = 0;
    if (*marching == 5) {    
        *marching = 1;
    }
    while (j < SIZE && *gameCon == 0) { // checking the last line for loss
        if (map[14][j] > 0) {
            printMap(map, playerX);
            printf("Game Lost!\n");
            *gameCon = 1;
        }
        j++;
    }
    if (*gameCon == 0) {    
        while (row >= 0) { // until we reach the first row
            int col = 0; // scanning across the row
            int block = 0;
            while (col < SIZE) { // until we reach the end of the row                                
                // scan left to right, move everything else down
                int l = 0;
                while (l < SIZE) {
                    if (map[row][l] == 2) {
                        block = 1;
                        l = SIZE;
                    } else {
                        l++;
                    }                    
                }  
                  
                if (block == 0) {
                    int m = 0;
                    while (m < SIZE) {
                        if (map[row + 1][m] == 2) {
                            m++;
                        } else {
                            map[row + 1][m] = map[row][m];
                            map[row][m] = EMPTY;
                            m++;
                        }
                    }
                    col = SIZE;
                } 
                  
                if (block == 1) {        
                    // down                     
                    if (*marching % 2 == 1) { 
                        int col2 = 0;
                        // scan left to right, move marching blocks down
                        while (col2 < SIZE) { 
                            map[row + 1][col2] = map[row][col2];
                            map[row][col2] = EMPTY;
                            col2++;
                        }                        
                    // scan right to left, move marching blocks right
                    } else if (*marching == 2) { 
                        int col_right = SIZE - 1;                        
                        while (col_right >= 1) {
                            int counter = 0;
                            if (col_right > 1) {
                                // regular move down
                                if (map[row][col_right - 1] != 2 && 
                                    map[row][col_right - 2] != 2) { 
                                    map[row + 1][col_right - 1] 
                                    = map[row][col_right - 1];
                                }
                                counter = 1;
                            }
                            // 0 2| - do nothing
                            if (map[row][SIZE - 1] == 2 &&                                 
                                map[row][col_right - 1] != 2) { 
                            // 2 2| or 0 2 0| - replace the -> element   
                            } else if (map[row][col_right - 1] == 2) {                              
                                map[row][col_right] = map[row][col_right - 1];                               
                                map[row][col_right - 1] = EMPTY;                   
                            // regular move down            
                            } else if (map[row][col_right - 1] 
                                       != 2 || counter != 1) { 
                                map[row + 1][col_right - 1] 
                                = map[row][col_right - 1];
                            }                            
                            col_right--;  
                        }
                    // scan left to right, move marching blocks left
                    } else if (*marching == 4) { 
                        int col3 = 0;
                        while (col3 < SIZE - 1) {                
                            // |2 0 - do nothing        
                            if (map[row][0] == 2 && map[row][1] != 2) { 
                            // 2 2| or 0 2 0| - destroy <- element    
                            } else if (map[row][col3 + 1] == 2) { 
                                map[row][col3] = map[row][col3 + 1];
                                map[row + 1][col3 + 1] = EMPTY;
                                map[row][col3 + 1] = EMPTY;
                            // regular move down
                            } else if (map[row][col3 + 1] != 2 && 
                                       map[row][col3 + 2] != 2) { 
                                map[row + 1][col3 + 1] = map[row][col3 + 1];
                            }                            
                            col3++;  
                        }
                    }                       
                    col = SIZE;              
                } else {
                    col++;                    
                }                
            }
            row--;
        }                    
        int i = 0;
        while (i < SIZE) {
            map[0][i] = 0;
            i++;
        }            
        int k = 0;
        while (k < SIZE) { // printing from hidden array
            map[0][k] = negative_map[*neg_row][k];
            k++;
        }       
        *neg_row = *neg_row + 1;
        printMap(map, playerX);    
    }
    *marching = *marching + 1;
}

// Flips the map upside down, only once
void flip(int map[SIZE][SIZE], int *flip_count, int playerX) {
    if (*flip_count == 0) {
        int tempMap[SIZE][SIZE] = {};
        int row = 0;
        while (row < SIZE) {
            int col = 0;
            while (col < SIZE) {
                tempMap[row][col] = map[row][col];
                col++;
            }
            row++;
        }
        int row2 = 14;
        int row3 = 0;
        while (row2 >= 0) {
            int col = 0;
            while (col < SIZE) {
                map[row3][col] = tempMap[row2][col];
                col++;
            }
            row2--;
            row3++;
        }
        printMap(map, playerX);
        *flip_count = 1;
        
    } else if (*flip_count == 1) {
        printMap(map, playerX);
    }

}

void gravity_storm(int map[SIZE][SIZE], int playerX, int *moved) {
    
    int direction;
    scanf("%d", &direction);
    int LEFT = 1;
    int TOP_LEFT = 2;
    int RIGHT = 3;
    int TOP_RIGHT = 4;
    int INVALID = 5;
    
    if (direction == LEFT) {
        int row = 0;
        while (row < SIZE) {
            int col = 0;
            int j = 0;
            while (col < SIZE) {
                if (map[row][col] != 0 && j != col) {
                    map[row][j] = map[row][col];
                    map[row][col] = EMPTY;                    
                    j++;
                } else if (j == col && map[row][col] != 0) {
                    map[row][j] = map[row][col];
                    j++;
                }       
                col++;
            }            
            row++;            
        }
    } else if (direction == TOP_LEFT) {
    
    } else if (direction == RIGHT) {
        int row = 0;
        while (row < SIZE) {
            int col = SIZE - 1;
            int j = SIZE - 1;
            while (col >= 0) {
                if (map[row][col] != 0 && j != col) {
                    map[row][j] = map[row][col];
                    map[row][col] = EMPTY;                    
                    j--;
                } else if (j == col && map[row][col] != 0) {
                    map[row][j] = map[row][col];
                    j--;
                }       
                col--;
            }            
            row++;            
        }
    } else if (direction == TOP_RIGHT) {
    
    } else if (direction == INVALID)  {
        // nothing
    }
    printMap(map, playerX);
}






