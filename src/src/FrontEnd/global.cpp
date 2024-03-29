#include "global.h"

string infile_name ;          // input file's name
string outfile_name = "stdout";    // output file's name
string temp_name = "stdin";
FILE *infp = NULL;   // input file's pointer, default is stdin
FILE *outfp = NULL;  // output file's pointer, default is stdout

static void show_line(int line, const char *file_name)
{
    std::string line_str;
    line_str = "";
    line_str += "./bin/sed.sh '";
    char temp[10];
    sprintf(temp, "%d", line);
    line_str += temp;
    line_str += "p' ";
    line_str += file_name;
    system(line_str.c_str());
}
/**
 * @brief 输出一段波浪线来强调一行源码中的一段代码片段
 * @param column 要强调位置的中心点
 */
static void printWaveLine(int column)
{
    int i = 0;
    for (i = 1; i < column-2; i++)
        fprintf(stdout," ");
    fprintf(stdout, "\033[32m%s\033[0m", "^^^");
    for (i = 0; i < 5; i++)
        fprintf(stdout, "\033[32m%s\033[0m", "~");
    fprintf(stdout, "\n");
};
/**
 * @brief 发现输入的.cos文件有错误时输出报错信息
 * @param msg       报错信息
 * @param line      出错行号
 * @param column    需要强调的标识符的列号
 */
void Error(string msg, int line, int column)
{
    assert(temp_name.c_str());
    error("[%s:%d]  error: %s", infile_name.c_str(), line, msg.c_str());
    if(temp_name != "stdin"){
        show_line(line, temp_name.c_str());
        printWaveLine(column);
    }else{
        printf("\n");
    }
}
/** 和上面函数类似,处理 Warning **/
void Warning(string msg, int line, int column)
{
    warning("[%s:%d]  warning: %s", infile_name.c_str(), line, msg.c_str());
    if (string(temp_name.c_str()) != "stdin" || Level >= WarningLevel)
    {
        show_line(line, temp_name.c_str());
        printWaveLine(column);
    }else
    {
        printf("\n");
    }
}
