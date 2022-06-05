#include <iostream>
#include <omp.h>
//структура игры
int difficulty = 4;
struct Game {
    int *board;			// доска для игры
    int currentMove;	// номер столбца текущего хода игрока
    int validMove;		// принимает значения 0 или 1, можно ли ходить на эту клетку
    int currentP;		// принимает значения 1 или 2, определяет чья очередь ходить
    int winningP;		// принимает значения 1 или 2, отмечает выигрывшего игрока
    int winner;			// отмечает была ли победа, принимает значения 0 и 1
    int bot;		    // проверка играет ли бот
};

// выделяем память для игры
struct Game *makeGame(){
    int *currentBoard;
    struct Game *currentGame = NULL;
    int move, currentPlayer, winner, winningPlayer,valid,computer;
    winningPlayer = 0;
    winner = 0;
    move = 0;
    currentPlayer = 1;
    valid = 1;
    computer = 0;

    currentBoard = (int *)malloc(sizeof(int) * 6 * 7);
    if(!currentBoard) {
        std::cout<<"не удалось выделить память для текущей доски\n";
        exit(1);
    }

    currentGame = (struct Game*)malloc(sizeof(int) * 6 * 7 + sizeof(int) * 3);
    if(!currentGame) {
        std::cout<<"не удалось выделить память для текущей доски\n";
        exit(1);
    }

    int i,j;
    for(i=0; i<6; i++){
        for(j=0; j<7; j++){
            currentBoard[i * 7 + j] = 0;
        }
    }

    currentGame->board=currentBoard;
    currentGame->currentMove = move;
    currentGame->currentP = currentPlayer;
    currentGame->winner = winner;
    currentGame->winningP = winningPlayer;
    currentGame->validMove = valid;
    currentGame->bot = computer;
    return currentGame;
}
//проверяем заполнена ли доска
int fullBoard(struct Game * game) {
    int *board = game->board;
    if (board[0] != 0 && board[1] != 0 && board[2] != 0 && board[3] != 0 &&
        board[4] != 0 && board[5] != 0 && board[6] != 0) {
        return 1;
    }
    return 0;
}
//проверяем победил ли кто-то
void checkWin(struct Game * game) {
    int *board = game->board;
    int i,j;
    //проверяем есть ли победы по вертикали
    for(j=0; j<7; j++){
        if((board[0*7+j] != 0) && (board[0*7+j] == board[1*7+j]) &&
           (board[1*7+j] == board[2*7+j]) && (board[2*7+j] == board[3*7+j])) {
            game->winner = 1;
            game->winningP = board[0 * 7 + j];
        }
        if((board[1*7+j] != 0) && (board[1*7+j] == board[2*7+j]) &&
           (board[2*7+j] == board[3*7+j]) && (board[3*7+j] == board[4*7+j])) {
            game->winner = 1;
            game->winningP = board[1 * 7 + j];
        }
        if((board[2*7+j] != 0) && (board[2*7+j] == board[3*7+j]) &&
           (board[3*7+j] == board[4*7+j]) && (board[4*7+j] == board[5*7+j])) {
            game->winner = 1;
            game->winningP = board[2 * 7 + j];
        }
    }

    //проверяем есть ли победы по горизонтали
    for(i=0; i<6; i++){
        if((board[i*7+0] != 0) && (board[i*7+0] == board[i*7+1]) &&
           (board[i*7+1] == board[i*7+2]) && (board[i*7+2] == board[i*7+3])) {
            game->winner = 1;
            game->winningP = board[i * 7 + 0];
        }
        if((board[i*7+1] != 0) && (board[i*7+1] == board[i*7+2]) &&
           (board[i*7+2] == board[i*7+3]) && (board[i*7+3] == board[i*7+4])) {
            game->winner = 1;
            game->winningP = board[i * 7 + 1];
        }
        if((board[i*7+2] != 0) && (board[i*7+2] == board[i*7+3]) &&
           (board[i*7+3] == board[i*7+4]) && (board[i*7+4] == board[i*7+5])) {
            game->winner = 1;
            game->winningP = board[i * 7 + 2];
        }
        if((board[i*7+3] != 0) && (board[i*7+3] == board[i*7+4]) &&
           (board[i*7+4] == board[i*7+5]) && (board[i*7+5] == board[i*7+6])) {
            game->winner = 1;
            game->winningP = board[i * 7 + 3];
        }
    }
    //проверяем диагональные "победы"
    for(j=0; j<4; j++){
        if((board[0*7+j] != 0) && (board[0*7+j] == board[1*7+j+1]) &&
           (board[1*7+j+1] == board[2*7+j+2]) && (board[2*7+j+2] == board[3*7+j+3])) {
            game->winner = 1;
            game->winningP = board[0 * 7 + j];
        }
        if((board[1*7+j] != 0) && (board[1*7+j] == board[2*7+j+1]) &&
           (board[2*7+j+1] == board[3*7+j+2]) && (board[3*7+j+2] == board[4*7+j+3])) {
            game->winner = 1;
            game->winningP = board[1 * 7 + j];
        }
        if((board[2*7+j] != 0) && (board[2*7+j] == board[3*7+j+1]) &&
           (board[3*7+j+1] == board[4*7+j+2]) && (board[4*7+j+2] == board[5*7+j+3])) {
            game->winner = 1;
            game->winningP = board[2 * 7 + j];
        }
        if((board[3*7+j] != 0) && (board[3*7+j] == board[2*7+j+1]) &&
           (board[2*7+j+1] == board[1*7+j+2]) && (board[1*7+j+2] == board[0*7+j+3])) {
            game->winner = 1;
            game->winningP = board[3 * 7 + j];
        }
        if((board[4*7+j] != 0) && (board[4*7+j] == board[3*7+j+1]) &&
           (board[3*7+j+1] == board[2*7+j+2]) && (board[2*7+j+2] == board[1*7+j+3])) {
            game->winner = 1;
            game->winningP = board[4 * 7 + j];
        }
        if((board[5*7+j] != 0) && (board[5*7+j] == board[4*7+j+1]) &&
           (board[4*7+j+1] == board[3*7+j+2]) && (board[3*7+j+2] == board[2*7+j+3])) {
            game->winner = 1;
            game->winningP = board[5 * 7 + j];
        }
    }
}

// оценивает игру выгодна ли она для бота
int scoreBoard(struct Game * game) {
    int *w;
    int *board = game->board;
    int i,j,k,score;
    checkWin(game);
    if (game->winner == 1) {
        if (game->winningP == 1) {
            return 99999;
        } else if(game->winningP == 2) {
            return -99999;
        }
    }

    score = 0;
    w = (int *)malloc(sizeof(int)*6*7);

    if(!w) {
        std::cout<<"не удалось выделить память для весов\n";
        exit(1);
    }
    for(i=0; i<6; i++){
        if (i==0||i==5) {w[i*7+0]=3;w[i*7+1]=4;w[i*7+2]=5;w[i*7+3]=7;
            w[i*7+4]=5;w[i*7+5]=4;w[i*7+6]=3;}
        if (i==1||i==4) {w[i*7+0]=4;w[i*7+1]=6;w[i*7+2]=8;w[i*7+3]=10;
            w[i*7+4]=8;w[i*7+5]=6;w[i*7+6]=4;}
        if (i==2||i==3) {w[i*7+0]=5;w[i*7+1]=8;w[i*7+2]=11;w[i*7+3]=13;
            w[i*7+4]=11;w[i*7+5]=8;w[i*7+6]=5;}}

    //проверяем по горизонтали ходы
    for(i=0; i<6; i++){
        for(j=0; j<4; j++){
            for(k=0; k<4; k++){
                if(board[i*7+j+k] == 1) {
                    score += w[i*7+j+k];
                } else if(board[i*7+j+k] == 0) {
                    score += 0;
                } else {
                    score += w[i*7+j+k]*-1;
                }
            }
        }
    }

    //проверяем вертикальные ходы
    for(j=0; j<6; j++){
        for(i=0; i<4; i++){
            for(k=0; k<4; k++){
                if(board[(i+k)*7+j] == 1) {
                    score += w[(i+k)*7+j];
                } else if(board[(i+k)*7+j] == 0) {
                    score += 0;
                } else {
                    score += w[(i+k)*7+j]*-1;
                }
            }
        }
    }

    //проверяем диагонали
    for(i=3; i<6; i++){
        for(j=0; j<4; j++){
            for(k=0; k<4; k++){
                if(board[(i-k)*7+j+k] == 1) {
                    score += w[(i-k)*7+j+k];
                } else if(board[(i-k)*7+j+k] == 0) {
                    score += 0;
                } else {
                    score += w[(i-k)*7+j+k]*-1;
                }
            }
        }
    }

    //проверяем диагонали
    for(i=0; i<3; i++){
        for(j=0; j<4; j++){
            for(k=0; k<4; k++){
                if(board[(i+k)*7+j+k] == 1) {
                    score += w[(i+k)*7+j+k];
                } else if(board[(i+k)*7+j+k] == 0) {
                    score += 0;
                } else {
                    score += w[(i+k)*7+j+k]*-1;
                }
            }
        }
    }
    return score;
}

// копируем информацию о текущей игре
struct Game *cloneGame(struct Game * oldGame){
    struct Game *newGame;
    int *newBoard;
    int *oldBoard;
    int i,j;

    newGame = makeGame();
    newGame -> currentMove = oldGame -> currentMove;
    newGame -> validMove = oldGame -> validMove;
    newGame -> currentP = oldGame -> currentP;
    newGame -> winningP = oldGame -> winningP;
    newGame -> winner = oldGame -> winner;
    newGame -> bot = oldGame -> bot;

    newBoard = newGame -> board;
    oldBoard = oldGame -> board;

    for(i=0; i<6; i++){
        for(j=0; j<7; j++){
            newBoard[i * 7 + j] = oldBoard[i * 7 + j];
        }
    }
    return newGame;
}

//ставим ход
void enterMove(struct Game * game,int player) {
    int move = game->currentMove;
    int *board = game->board;
    int i;

    for(i=0; i<6; i++){
        if (board[i*7+move] != 0) {
            board[(i-1)*7+move] = player;
            return;
        }
        else if (i==5) {
            board[35+move] = player;
        }
    }
}
// проверяем можно ли так ходить
void checkMove(struct Game * game){
    int *board = game->board;
    int move = game->currentMove;
    if (move>=0 && move <=6) {
        if (board[move] == 0) {
            game->validMove = 1;
        }
        else {
            game->validMove = 0;
        }
    }
    else {
        game->validMove = 0;
    }
}
//просчитывает все возможные ходы игрока и выбирает наилучший(максимизирующий) для бота и наихудший(минимизирующий) для игрока
int miniMax(int depth, int max_depth, int player, struct Game * game) {
    int mminScore;
    int maxScore;
    int column;
    struct Game * game_clone;
    int next_level;
    int move_column;

    checkWin(game);
    if (game->winner == 1) {
        if (game->winningP == 1) {
            return 99999;
        } else if(game->winningP == 2) {
            return -99999;
        }
    }

    else if (fullBoard(game) == 1) {
        return 9000;
    } else if (depth == 0) {
        return scoreBoard(game);
    } else {
        mminScore = 100000;
        maxScore = -100000;
        column = 0;
        for (move_column = 0; move_column < 7; move_column++) {
            game_clone = cloneGame(game);
            game_clone->currentMove = move_column;
            checkMove(game_clone);
            if (game_clone->validMove == 0) {
                continue;
            }
            enterMove(game_clone, player);
            if (player == 2) {
                next_level = miniMax(depth - 1, max_depth, 1, game_clone);
                if (next_level < mminScore) {
                    column = move_column;
                    mminScore = next_level;
                }
            } else {
                next_level = miniMax(depth - 1, max_depth, 2, game_clone);
                if (next_level > maxScore) {
                    column = move_column;
                    maxScore = next_level;
                }
            }
        }
        if (player == 2) {{ return mminScore; }}
            else { return maxScore; }
    }
}
//поиск наилучшего хода - вызов минимакса
int bestMove(int depth, int max_depth, int player, struct Game * game){
    int maxScore = -100000;
    int column = 0;
    struct Game * game_clone;
    int next_level;
    int move_column;
    int num_threads = 21, rank=0;
    omp_set_num_threads(num_threads);
#pragma omp parallel private(rank)
    {
        int localMaxScore = -100000;
        int localColumn;
        rank = omp_get_thread_num();
#pragma omp for schedule(dynamic)
        for (move_column = 0; move_column < 7; move_column++) {
            game_clone = cloneGame(game);
            game_clone->currentMove = move_column;
            checkMove(game_clone);
            if (game_clone->validMove == 0) {
                continue;
            }
            enterMove(game_clone, player);
            next_level = miniMax(depth - 1, max_depth, 2, game_clone);
            if (next_level > localMaxScore) {
                localMaxScore = next_level;
                localColumn = move_column;
            }
        }
#pragma omp critical
        {
            if(localMaxScore>maxScore && max_depth == depth)
            {
                maxScore=localMaxScore;
                column=localColumn;
//                std::cout<<"THRAD: "<<rank<<std::endl;
            }
        }
    }
    return column;
}
//запрашиваем ход пользователя или ищем лучший шаг для бота
void getMove(struct Game * game) {
    int move,diff;
    if (game -> bot == 1 && game -> currentP == 1) {
        std::cout<<"\nБот думает над ходом...\n";
        diff = difficulty;
        double start = omp_get_wtime();
        game->currentMove = bestMove(diff, diff, 1, game);
        std::cout<<"\nБот думал над ходом "<<omp_get_wtime()-start<<" секунд \n";
    } else {
        std::cout<<"\nВведите ход Игрока "<<game->currentP<<" :\n";
        std::cin>>move;
        game->currentMove=move; //ставим шаг игрока текущим
    }
}
//отрисовываем доску
void printBoard(struct Game * game) {
    int i;
    int j;
    system("clear");
    std::cout<<"|_0___1___2___3___4___5___6_|\n";
    for(i=0;i<5;i++) {
        std::cout<<"|";
        for(j=0;j<7;j++) {
            if (game->board[i*7+j]==1) {
                std::cout<<" x |";
            } else if (game->board[i*7+j]==2) {
                std::cout<<" o |";
            } else {
                std::cout<<"   |";
            }
        }
        std::cout<<std::endl;
    }
    std::cout<<"|";
    for(j=0;j<7;j++) {
        if (game->board[5*7+j]==1) {
            std::cout<<"_x_|";
        } else if (game->board[5*7+j]==2) {
            std::cout<<"_o_|";
        } else {
            std::cout<<"___|";
        }
    }
    std::cout<<std::endl;
}

//функция запускает игру пока кто-то не победит
int play(struct Game *currentGame) {
    while (1) {
        printBoard(currentGame);			//рисуем доску
        getMove(currentGame);				//получаем первый шаг
        checkMove(currentGame);			//проверяем ход
        while(currentGame->validMove == 0) {	//если ход верный
            printBoard(currentGame);
            std::cout<<currentGame->currentMove<<" это некорректный ход\n";
            getMove(currentGame);				//получаем новый шаг и проверяем его
            checkMove(currentGame);
        }
        //корректный ход отмечаем на доске
        enterMove(currentGame, currentGame->currentP);
        checkWin(currentGame);				//проверяем нет ли победы
        if (currentGame->winner == 1) {	//если кто-то победил завершаем игру и объявляем победителя
            printBoard(currentGame);
            std::cout<<"\nИгрок "<<currentGame->winningP<<" победил!\n\n";
            return 1;
        }
        if (currentGame->currentP == 1) {	//меняем игрока для следующего хода
            currentGame->currentP = 2;
        }
        else {
            currentGame->currentP = 1;
        }
    }
}

int main() {
    std::cout<<"\n-------------------------------------------------------------------\n\n";
    std::cout<<"\t\t\t Четыре в ряд\n";
    std::cout<<"\n-------------------------------------------------------------------\n\n";
    std::cout<<"Вы хотите начать игру? (+ :играть с ботом)/(-: завершить) : ";
    char choice;
    std::cin>>choice;
    while (choice != '+' && choice != '-') {
        printf("Некорректный ответ");
    }
    if (choice == '-') {
        return 1;
    }

    else if (choice == '+') { //игрок играет против бота, бот начинает
        struct Game *currentGame = NULL;		//инициализация игры
        currentGame = makeGame();//создаем игру
        currentGame -> bot = 1;
        play(currentGame);
    }
}
