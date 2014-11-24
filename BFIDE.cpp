#include <iostream>
#include <string>
#include <locale>
#include "stdio.h"

using namespace std;

void duffCopy(char* to, int size, char* from)
{
    int n = (size + 7) / 8;
    switch (size % 8)
    {
        case 0: do {
                        *to++ = *from++;
        case 7:         *to++ = *from++;
        case 6:         *to++ = *from++;
        case 5:         *to++ = *from++;
        case 4:         *to++ = *from++;
        case 3:         *to++ = *from++;
        case 2:         *to++ = *from++;
        case 1:         *to++ = *from++;
        } while (--n > 0);
    }
}

void testDuffCopy()
{
    int size = 100;
    char* from = new char[size];
    for (int i = 0; i < size; i++)
    {
        int c = 97 + i % 26;
        from[i] = c;
    }

    char* to = new char[size];
    duffCopy(to, size, from);
    for (int i = 0; i < size; i++)
    {
        printf("%c\r\n", to[i]);
    }
}

char *ptr = new char[1000];
int index = 0;
int maxSize = 0;
int running = 1;
int startLoopCount = 0;

void parseSingleInput(char in)
{
    switch (in)
    {
    case 62:
        ++index;
        break;
    case 60:
        if (index < 1)
        {
            printf("Runtime error - index moved out of bounds\n");
        }
        else
            --index;
        break;
    case 43:
        ++ptr[index];
        break;
    case 45:
        --ptr[index];
        break;
    case 46:
        putchar(ptr[index]);
        break;
    case 44:
        //input is handled separately
        //ptr[index]=getchar();
        break;
    case 91:
        //loop endpoints are handled separately
        break;
    case 93:
        //loop endpoints are handled separately
        break;
    default:
        printf("Syntax error - invalid BF symbol: '%c'\n", in);
        break;
    }
    if (index >= maxSize)
        maxSize = index + 1;
}

void printState()
{
    printf("\n\n");
    printf("=============PROGRAM STATE=============\n");
    printf("VALUES:\t");
    for (int i = 0; i < maxSize; i++)
        printf("%d\t", (int)(ptr[i]));
    printf("\nindex:\t");
    for (int i = 0; i < maxSize; i++)
        printf("%d\t", i);
    printf("\nPTR:\t");
    for (int i = 0; i < index; i++)
        printf("\t");
    printf("^\n");
}

void printHelp()
{
    printf("Valid BF syntax: \n");
    printf("\t>\tIncrement data pointer\n");
    printf("\t<\tDecrement data pointer\n");
    printf("\t+\tIncrement data pointee\n");
    printf("\t-\tDecrement data pointee\n");
    printf("\t.\tOutput data pointee byte\n");
    printf("\t,\tStore input as data pointee byte\n");
    printf("\t[\tLoop opener\n");
    printf("\t]\tIf data pointee == 0, jump to '['\n\n");
    printf("\"clear\" to clear program state\n");
    printf("\"exit\" to exit\n\n");
    printState();
}

void init()
{
    for (int i = 0; i < 1000; i++)
        ptr[i] = 0;
    index = 0;
    maxSize = 1;
    running = 1;
    printf("\n=============STATE CLEARED=============\n");
    printState();
}

int findLoopEnd(string line, int start)
{
    int openCount = 0;
    for (int i = start + 1; i < line.length(); i++)
    {
        if (line[i] == '[')
            openCount++;
        if (line[i] == ']')
        {
            if (openCount)
                openCount--;
            else
                return i;
        }
    }
    return -1;
}

void queryInput()
{
    printf("input: ");
    cin >> ptr[index];
}

int parseLine(string line)
{
    if (line.compare("help") == 0)
        printHelp();
    else if (line.compare("exit") == 0)
        running = 0;
    else if (line.compare("clear") == 0)
        init();
    else
    {
        //CAN'T DO IT THIS WAY
        //DOESN'T SUPPORT MULTIPLE LOOPS
        for (int i = 0; i < (int)(line.length()); i++)
        {
            if (line[i] == '[')
            {
                int loopEnd = findLoopEnd(line, i);
                if (i > loopEnd)
                    printf("Syntax error - loop end before loop start: ']' at position %d; '[' at position %d\n", loopEnd, i);
                string insideLoop = line.substr(i + 1, loopEnd - i - 1);
                parseLine(insideLoop);
                while (ptr[index])
                    parseLine(insideLoop);
                i = loopEnd;
            }
            else
            {
                if (line[i] == ',')
                    queryInput();
                else
                    parseSingleInput(line[i]);
                //printState();
            }
        }
        return 1;
    }
    return 0;
}

string getLine()
{
    string input;
    getline(cin, input);
    return input;
}

void BFIDEMain()
{
    init();
    printf("Enter BF code or type \"help\" for help.\n");
    while (running)
    {
        string input = getLine();
        if (parseLine(input))
            printState();
    }
    printf("Goodbye :)\n");
}

int main(void)
{
    BFIDEMain();
    // don't let the terminal window go away until we want it to
    /*
    char in;
    std::cin >> in;
    */
    return 0;
}
