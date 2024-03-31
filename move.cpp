#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

#define NUM_STRINGS 50   //大于迷宫最大行数 
#define MAX_LENGTH 50   //大于迷宫每行最大字符数 

/*****************************************************
*读取文件函数，将文件读入二维字符串数组中,           *                       
*输入文件名和字符串指针数组名，返回文件行数          *
******************************************************/ 
int read_file(char file_name[],char *str[]){
 //将文件读入文件指针fp   
    FILE *fp = NULL;
    if ( (fp=fopen(file_name, "r")) == NULL )
     {     printf("file open error.\n"); 
           exit (0);  
     }
     
    //将文件每行的内容读入缓冲区，指针数组为该行内容开辟空间，并复制缓冲区的内容 
    char buffer[MAX_LENGTH];   //元素个数大于迷宫文件一行中最大字符数
    int line = 0;
    while (fgets(buffer, MAX_LENGTH, fp) != NULL) {
     str[line] = new char[strlen(buffer)];
     strcpy(str[line],buffer);
        line++;       //循环结束后，line就记录了迷宫的行数 
    }
    fclose(fp); // 关闭文件
    return line; 
}

void print_map(char *MAZE[], int line){
    cout << "\033[2J\033[1;1H";
    for (int i = 0; i < line; i++)
        {
            cout<<MAZE[i];
        }
}
bool move(int x1, int y1, char *MAZE[]){
    if (MAZE[y1][x1]==' '){
            MAZE[y1][x1]='P';
            return true;
    }
    else
        return false;
        
}

int main() {
    struct termios old_tio, new_tio;
    char c;
    char *MAZE[NUM_STRINGS];
    char file_name[]="map3.txt";    //需要读的迷宫文件名称 
    int line = read_file(file_name,MAZE);
    print_map(MAZE, line);
    

    // 获取当前终端设置
    tcgetattr(STDIN_FILENO, &old_tio);

    // 复制终端设置
    new_tio = old_tio;

    // 禁用标准输入的缓冲区和回显
    new_tio.c_lflag &= (~ICANON & ~ECHO);

    // 将新终端设置应用到标准输入
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    int x = 0;
    int y = 0;
    for(int i = 0; i < line; i++){
        for (int j = 0; j < 50; j++){
            if (MAZE[i][j]=='P'){
                x=j, y=i;
            }
        }
    }
    int x1=x, y1=y;
    while (true) {
        c = getchar();
        if (c == '\033') { // 检查是否是箭头键
            getchar(); // 忽略第二个字符 '['
            c = getchar(); // 获取箭头键的方向
            bool result = false;
            if (c == 'A') { // 检查是否是向上箭头键
                cout<<"1";
                x1 = x, y1 = y-1;
                result = move (x1, y1, MAZE);
            }
            else if(c == 'B'){
                cout<<"2";
                x1 = x, y1 = y+1;
                result = move (x1, y1, MAZE);
            }
            else if(c == 'C'){
                x1 = x+1, y1 = y;
                result = move (x1, y1, MAZE);
                cout<<"3";
                cout<<result;
            }
            else if(c == 'D'){
                cout<<"4";
                x1 = x-1, y1 = y;
                result = move (x1, y1, MAZE);
            }
            if (result==true){
                cout<<"true";
                MAZE[y][x]=' ';
                print_map(MAZE, line);
                x = x1, y = y1;
            }
        }
        else if (c == 'q') {
            cout << "Hello" << endl;
            break;
        }
    }

    // 恢复原始的终端设置
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    return 0;
}