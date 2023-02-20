#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
/**
 * @brief $ echo "1\n2" | xargs -n 1 echo line
            line 1
            line 2
          $
 * 上面只是举例Unix中的 xargs 用法, 在我们实现的xars中, 可以认为是我们需要实现 Unix版本 xargs -n 1 xx xx的 , -n 1 是缺省值.
 * 
 */


/**
 * @brief  read lines from th standard input and run a command for each line, supplying the line as arguments to the command.
 *  从标准输入中读取多行数据, 然后传入到cmd中, 作为运行cmd的参数, 有多少行 cmd 就运行多少次
 * @param cmd 
 * @param argv[]
 */

void xargs(char *cmd, char *argv[]);

/**
 * @brief 
 * @params buf is standard input, where we read
 * @params line is the return address that we return our line to ourside.
 * @return  1 for have another line , ,-1 for nothing leave
 */
// int readLine(char *buf, char *line);

char* readLine() {
    char c;
    char *line = malloc(sizeof(char) * (MAXARG+1));
    memset(line, '\0', MAXARG+1);
    int pos = 0;
    while(read(0,&c, sizeof(char)) != 0) {
        if (c != '\n') {
            line[pos++] = c;
        } else {
            break;
        }
    }
    return line;
}

void spilt_line(int pos,char* line, char* argv[]) {
    char buf[MAXARG];
    memset(buf,'\0',MAXARG);
    int j=0;
    for (int i=0; i< strlen(line) ; i++) {
        if (line[i] != ' ') {
            buf[j++] = line[i];
        } else {
            if (strlen(buf) != 0) {
                char *mem = malloc(sizeof(char) * j);
                memcpy(mem,buf,j);
                memset(buf,'\0',MAXARG);
                j = 0;
                argv[pos++] = mem;
            }
        }
    }
    // add last one
    if (strlen(buf) != 0) {
        char *mem = malloc(sizeof(char) * j);
        memcpy(mem,buf,j);
        memset(buf,'\0',MAXARG);
        j = 0;
        argv[pos] = mem;
    }
}

/**
 * @brief using wait to wait child process exit.
 *        using make clean to make file system clean again if you mess something up
 */
int main(int argc, char* argv[]) {
    
    if(argc != 3) {
        printf("Error");
        exit(-1);
    }
    
    int i = 0,pos = 0;
    char c;
    char* cmd_argv[MAXARG];

    // copy argv  to cmd_argv
    for (; i< argc - 1;i++) {
        cmd_argv[i] = argv[i+1];
    }
    
    char *line = malloc(sizeof(char) * (MAXARG+1));
    while(read(0,&c, sizeof(char)) != 0) {
        if (c != '\n') {
            line[pos++] = c;
        } else {
            spilt_line(i,line,cmd_argv);
            xargs(argv[1],cmd_argv);
            memset(line, '\0', MAXARG+1);
            pos = 0;
        }
    }
    // wait(0);
    // 等待所有子进程退出
    while(wait(0) != -1) {};
    exit(0);
}

/**
 * @brief  Using fork & exec to run cmd
 *         MAX_PARAMS from kernel/params.h
 *          
 */
void xargs(char *cmd, char *argv[]) {
    if (fork() == 0) {
        // 惨痛教训
        exec(cmd, argv);
        exit(0);
    }
}


