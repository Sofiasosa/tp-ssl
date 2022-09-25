#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pila.c"

int perteneceDecimal(const char *);
int charInt(char c);
bool esOperador(char c);
int perteneceEq(const char *cadena);
int precedencia(char x, char y);
int esDigito(char ch);
char intToChar(int i);


int main()
{
    char fileName[] = "test3.txt";
    FILE *f;
    int lArchivo;
    char *inf;

    f = fopen(fileName, "r");

    // Error
    if (f == NULL)
        perror("ERROR"), exit(1);

    // Buffer size
    fseek(f, 0L, SEEK_END);
    lArchivo = ftell(f);
    rewind(f);

    inf = (char *)malloc((lArchivo + 1) * sizeof(char));
    
    if (!inf)
        fclose(f), fputs("Memory alloc fails", stderr), exit(1);

    if (1 != fread(inf, lArchivo, 1, f))
        fclose(f), free(inf), exit(1);

    fclose(f);

    printf("EXPRECION INFIJA -> %s\n", inf);
    if (perteneceEq(inf) == 1)
        printf("-- ES ECUACION VALIDA --\n");
    else
    {
        printf("-- ES ECUACION INVALIDA --\n");
        return 1;
    }

    // Pila
    struct stack *p = newStack(lArchivo);
    char *post = (char *)malloc((lArchivo + 1) * sizeof(char));

    if (!p)
    {
        perror("ERROR");
        return -1;
    }

    int i = 0;
    int j = 0;

    while (inf[i] != '\0')
    {
        if (esDigito(inf[i]))
        {
            post[j] = inf[i];
            j++;
        }
        else if (esOperador(inf[i]))
        {
            if (!(isEmpty(p)) && (precedencia(inf[i], peek(p)) < 0))
            {
                post[j] = pop(p);
                j++;
            }
            push(p, inf[i]);
        }
        i++;
    }
    
    // Vaciar pila
    while (!isEmpty(p))
    {
        post[j] = pop(p);
        j++;
    }

    post[j] = '\0';

    printf("\n EXPRECION POSFIJA -> %s\n", post);

    //////////////// RESULTADO //////////////////////
    struct stack *p2 = newStack(lArchivo);

    for (int i = 0; post[i] != '\0'; i++)
    {

        if (!(esOperador(post[i])))
        {
            push(p2, post[i]);
        }
        else if (esOperador(post[i]))
        {
            char pop1 = charInt(pop(p2));
            char pop2 = charInt(pop(p2));

            switch (post[i])
            {
            case '+':
                push(p2,intToChar(pop2+pop1));
                break;
            case '-':
                push(p2,intToChar(pop2-pop1));
                break;
            case '*':
                push(p2,intToChar(pop2*pop1));
                break;
            }
        }
    }
    int resultado = charInt(pop(p2));

    printf("RESULTADO -> %d\n", resultado);

    free(inf);
    free(post);
    free(p);
    free(p2);
}

int columnaDecimal(int c)
{
    if (c == '0')
        return 0;
    else if (c == '-')
        return 2;
    else
        return 1;
}

int perteneceDecimal(const char *cadena)
{
    static int tt[5][3] = {{2, 3, 1},
                           {4, 3, 4},
                           {4, 4, 4},
                           {3, 3, 4},
                           {4, 4, 4}};
    int e = 0, i = 0;

    // ver si los caracteres pertenecen al alfabeto
    for (int i = 0; cadena[i] != '\0'; i++)
    {
        int numero = charInt(cadena[i]);
        if (!((numero >= 0 && numero < 10) || cadena[i] == '-'))
        {
            return 0;
        }
    }
    int c = cadena[i];

    while (c != '\0' && e != 4)
    {
        e = tt[e][columnaDecimal(c)];
        i++;
        c = cadena[i];
    }
    if (e == 2 || e == 3)
        return 1;
    else
        return 0;
}

int charInt(char c)
{
    return c - '0';
}

char intToChar(int i)
{
   return '0'+i;
}

bool esOperador(char c)
{
    if (c == '+' || c == '-' || c == '*')
        return true;
    return false;
}

int perteneceEq(const char *cadena)
{
    static int tt[4][2] = {{1, 3},
                           {1, 2},
                           {1, 3},
                           {3, 3}};
    int e = 0, i = 0;

    for (int i = 0; cadena[i] != '\0'; i++)
    { // ver si los caracteres pertenecen al alfabeto
        int numero = charInt(cadena[i]);
        if (!((numero >= 0 && numero <= 9) || cadena[i] == '+' || cadena[i] == '-' || cadena[i] == '*'))
        {
            return 0;
        }
    }
    int c = cadena[i];

    while (c != '\0' && e != 3)
    {
        e = tt[e][esOperador(c)];
        i++;
        c = cadena[i];
    }

    if (e == 1)
        return 1;
    else
        return 0;
}

int precedencia(char x, char y)
{
    int prec1, prec2;

    switch (x)
    {
    case '+':
        prec1 = 1;
        break;
    case '-':
        prec1 = 2;
        break;
    case '*':
        prec1 = 3;
        break;
    }

    switch (y)
    {
    case '+':
        prec2 = 1;
        break;
    case '-':
        prec2 = 2;
        break;
    case '*':
        prec2 = 3;
        break;
    }
    return prec1 - prec2;
}

int esDigito(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
