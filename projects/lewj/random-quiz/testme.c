#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    int r;
    r = rand() % 9;         // values: 0 - 8
    
    if (r == 0)
        return '[';
    if (r == 1)
        return '(';
    if (r == 2)
        return '{';
    if (r == 3)
        return ' ';
    if (r == 4)
        return 'a';
    if (r == 5)
        return 'x';
    if (r == 6)
        return '}';
    if (r == 7)
        return ')';
    
    return ']';
}

char *inputString()
{
    char* s = NULL;
    s = malloc(sizeof(char) * 6);       // 5 characters + '\0'
    
    int i, r;
    
    for (i = 0; i < 5; i++){
        r = rand() % 4;         // values: 0 - 3
        if (r == 0)
            s[i] = 'r';
        if (r == 1)
            s[i] = 'e';
        if (r == 2)
            s[i] = 's';
        if (r == 3)
            s[i] = 't';
    }

    s[5] = '\0';
    return s;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}

