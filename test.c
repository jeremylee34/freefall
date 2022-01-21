void shoot(int command, int map[SIZE][SIZE], int playerX) {
    // shooting 
    int end_shooting = 0;
    while (scanf("%d", &command) == 1 && command == 2 && end_shooting == 0) {
        if (command == 2) { //command to fire laser
            int Lrow = 14; // count from bottom row to top
            int blocks_destroyed = 0; // count up to 3 destroyed
            int finish_shooting = 0;
            // if we reach the top, finish shooting            
            while (Lrow >= 0 && finish_shooting == 0) { 
                if (map[Lrow][playerX - 1] == 0) { //no block
                    Lrow--;
                } else if (map[Lrow][playerX - 1] == 1 && blocks_destroyed < 3) {
                    Lrow--; // destroys block
                    blocks_destroyed++;
                    map[Lrow][playerX - 1] = 0;
                } else if (map[Lrow][playerX - 1] == 1 && blocks_destroyed == 3) { 
                    finish_shooting = 1;
                    // if we have shot 3 blocks, finish shooting
                } else if (map[Lrow][playerX - 1] == 0 && blocks_destroyed > 0) {
                    finish_shooting = 1;
                    /* if we have destroyed a block and 
                    nothing is above it,    finish shooting */
                }
            }
            printMap(map, playerX);
        }
        end_shooting = 1; 
    }      
}
