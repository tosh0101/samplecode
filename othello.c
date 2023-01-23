#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4

#define P1 'o'
#define P2 'x'
#define CHANGE_TURN() current = ((current == P1)? P2 : P1); opp = ((opp == P2)? P1 : P2);

char current = 'o';
char opp = 'x';
char message[256];

char board[N][N];
int C1 = ( N / 2 ) - 1;
int C2 = ( N / 2 );

int ocnt=2;
int xcnt=2;

void board_init(){
        int x = 0;
        int y = 0;
        for(y = 0;y < N; y++ ){
                for ( x= 0; x< N; x++ ){
                        if (y == C1 && x == C1){
                                board[y][x] = P1;
                        }else
                        if (y == C1 && x == C2){
                                board[y][x] = P2;
                        }else
                        if (y == C2 && x == C1){
                                board[y][x] = P2;
                        }else
                        if (y == C2 && x == C2){
                                board[y][x] = P1;
                        }else{
                                board[y][x] = ' ';
                        }
                }
        }
}
void print_line(){
        int x = 0;
        printf("-+");
        for (x = 0; x< N; x++){
                printf("-+");
        }
        printf("\n");
}
void print_head(){
        int x = 1;
        printf(" ");
        for(x = 1;x < N+1; x++ ){
                printf("|%d",x);
        }
        printf("\n");
}
void print_board(){
        print_head();
        int x = 0;
        int y = 0;
        int row = 0;
        for(y = 0;y < N; y++){
                print_line();
                row++;
                printf("%d|",row);
                for (x= 0; x< N; x++){
                        printf("%c|",board[y][x]);
                }
                printf("\n");
        }
}
int tryput(int x, int y, int dx ,int dy){
        int vy = y;
        int vx = x;
        int oppsize= 0;
        int cnt = 0;
        int cend = 0;
        do {
                vx = vx + dx;
                vy = vy + dy;
                if ( board[vy][vx] == opp ){
                        oppsize++;
                }else
                if ( board[vy][vx] == ' ' ){
                        return 0;
                }else
                if ( board[vy][vx] == current ){
                        cend = 1;
                        break;
                }
        } while ((vy >= 0 && vy < N) && (vx >= 0 && vx < N));
        vy = y;
        vx = x;
        // 自身のカラーで終わり、相手カラーの数がある時
        while (cend == 1 && oppsize > 0){
                vx = vx + dx;
                vy = vy + dy;
                // reverse
                board[vy][vx] = current;
                oppsize--;
                cnt++;
        }
        if (cnt > 0){
                // put new piece
                board[y][x] = current;
        }
        return cnt;
}
/**
 * 置けなかったら0
 * 置けたら裏返した枚数
 * を返す
 */
int putCell(int ix, int iy){
        // 実INDEXへ変換
        int x = ix -1;
        int y = iy -1;

        // 盤の範囲外への配置
        if (x < 0 || x >=N || y < 0 || y >= N){
                char tmp[] = "out of board address.\0";
                memcpy(message,tmp,strlen(tmp)+1);
                return 0;
        }
        // 配置済み
        if (board[y][x] != ' '){
                char tmp[] = "piece already exists.\0";
                memcpy(message,tmp,strlen(tmp)+1);
                return 0;
        }
        int changed = 0;
        // 全方向へ検査と裏返し
        changed +=tryput(x,y,1,0); // east
        changed +=tryput(x,y,1,1); // east-south
        changed +=tryput(x,y,0,1); // south
        changed +=tryput(x,y,-1,1); // west-south
        changed +=tryput(x,y,-1,0); // west
        changed +=tryput(x,y,-1,-1); // west-north
        changed +=tryput(x,y,0,-1); // north
        changed +=tryput(x,y,1,-1); // east-north
        if ( changed == 0 ){
                char tmp[] = "no reversible piaces. when skip your turn, input 'skip'\0";
                memcpy(message,tmp,strlen(tmp)+1);
                return 0;
        }
        return changed;
}

int main()
{
        int ix =0;
        int iy =0;
        int ret = 0;
        char str[256];
        int changed =0;
        message[0] = '\0';
        board_init();
        do {
                system("clear");
                print_board();
                printf("o:%d,x:%d\n",ocnt,xcnt);
                if (message[0] != '\0' ){
                        printf("%s\n",message);
                        message[0] = '\0';
                }
                printf("input x,y turn:%c >",current);
                ret = scanf("%d,%d", &ix,&iy);
                if ( ret != 2 ){
                        // 文字列入力などで無限ループを防ぐ
                        // 配置不可となった時skip
                        scanf("%s",str);
                        if (strncmp(str,"skip",4)==0){
                                CHANGE_TURN();
                        }
                }else{
                        changed = putCell(ix,iy);
                        if ( changed ){
                                if ( current == P1 ){
                                        ocnt++; // 新規で配置したコマの数
                                        ocnt += changed; // 相手のコマを裏返した数
                                        xcnt -= changed;
                                }else{
                                        xcnt++;
                                        xcnt += changed;
                                        ocnt -= changed;
                                }
                                CHANGE_TURN();
                        }
                }
        } while((xcnt + ocnt) < N*N) ;

        system("clear");
        print_board();
        printf("o:%d,x:%d\n",ocnt,xcnt);
        if ( xcnt == ocnt ){
                printf("even score\n");
        }else{
                printf("winner is %c\n", ocnt > xcnt ? P1 : P2);
        }
}