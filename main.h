#include <stdio.h>

#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// #define MAX_WORD_LENGTH 50           // largo maximo de una palabra
// #define MAX_WORDS_BY_THREAD 100     // cantidad maxima de palabras por hebra
//
// typedef struct threads{
//
//     int id, x, y;
//     int n_words_by_thread;
//     char **assigned_words;
//     pthread_mutex_t * mutexs;
//
// } ParamsThread;


void writeGrid(int N, char *file_name_out);
int getSchroedingerValue(int i,int j,int t,int N);

void writeGrid(int, char *);
int getSchroedingerValue(int ,int ,int ,int);


char **createSoup(int width, int length);
void fillSoupRandom(char ** soup, int length, int width); // Rellena los espacios vacios(en "0") con letras aleatorias
char **getWords(int quantity_words, char * file_name_in); //Lee las palabras del archivo de entrada
int checkPositions(int x, int y, int word_length); // Checkea que las posiciones donde se insertara la palabra este disponible
void *ubicar(void *params); //funcion que ejecutara cada hebra
int checkArgs(int width, int length, int quantity_words, int n_threads,char * file_name_in); // checkea las parametros de cada hebra
void writeSoup(char *file_name_out); //escribe la sopa final en el archivo(file_name_out)
void printSoup(); //printea por consola la sopa
