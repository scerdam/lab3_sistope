#include "main.h"

//int width,length;
//char **soup;  //matriz que representa la sopa de letra y es compartida por las hebras

float **grid; // float **H;

int main(int argc, char **argv) {

      srand((unsigned int) time(NULL));

      char *file_name_out = NULL;
      int T, N, n_threads, t_iterations = 0;

      int index;
      int c;


      opterr = 0;

      while ((c = getopt (argc, argv, "N:h:c:n:m:s:d")) != -1)
        switch (c)
          {
          case 'N':
            N =  atoi(optarg);
            break;
          case 'T':
            T = atoi(optarg);
            break;
          case 'H':
            n_threads = atoi(optarg);
            break;
          case 'f':
            file_name_out  = optarg;
            break;
          case 't':
            t_iterations = atoi(optarg);
            break;
          case '?':
            if (optopt == 'c')
              fprintf (stderr, "Opcion -%c requie re un argumento.\n", optopt);
            else if (isprint (optopt))
              fprintf (stderr, "Opcion desconocida `-%c'.\n", optopt);
            else
              fprintf (stderr,
                       "Opcion con caracter desconocido `\\x%x'.\n",optopt);
            return 1;
          default:
            abort ();
          }


      for (index = optind; index < argc; index++) printf ("No existe opcion para agumento: %s\n", argv[index]);




    soup = createSoup(width, length); //inicializa la sopa con ceros
    char **words;      // arreglo de strings que almacenara las palabras leidas desd el archivo(file_name_in)
    words = getWords(quantity_words, file_name_in);  //obtiene las palabras


    int n_words_by_thread = quantity_words /n_threads;

    int rest_n_words_by_thread = quantity_words%n_threads; // se usara para repartir las palabras sobrantes de la division anterior

    if (!checkArgs(width, length, quantity_words, n_threads, file_name_in)) exit(0);   //Se validan los paremtros, si encuentra un error termian el programa


    ParamsThread params[n_threads];  //se declaran las estructuras que seran pasadas como parametros a cada hebra en la funcion ubicar
    pthread_t threads[n_threads];

    int i = 0, j = 0, k = 0, counter_rest_words = 0;

    while(i<n_threads){
        params[i].assigned_words= malloc(sizeof(char*)*MAX_WORDS_BY_THREAD);

        if(rest_n_words_by_thread>0){
            counter_rest_words= 1;
        }
        while (j< n_words_by_thread + counter_rest_words){
            params[i].assigned_words[j]= malloc(sizeof(char*)*MAX_WORD_LENGTH );
            j++;
        }

        counter_rest_words=0;
        rest_n_words_by_thread--;

        i++;
        j=0;
    }


    pthread_mutex_t mutexs[length];  // un mutex para clada fila

    i=0;
    j=0;

    rest_n_words_by_thread = quantity_words%n_threads;

    while (i < n_threads){
       params[i].id = i;
       if(rest_n_words_by_thread>0){
           counter_rest_words= 1;
       }
       while (j<n_words_by_thread + counter_rest_words) {
           strcpy( params[i].assigned_words[j], words[k]);
            j++;
            k++;
        }

        j=0;

        params[i].mutexs = mutexs;
        params[i].n_words_by_thread = n_words_by_thread+counter_rest_words;

        counter_rest_words=0;
        rest_n_words_by_thread--;

        pthread_mutex_init(mutexs+i, NULL);  // inicializamo cada mutex
        pthread_create(&threads[i], NULL, ubicar,  &params[i]); // se crea cada hebra con sus respectivos parámetros
        i++;
    }



    i=0;
    while(i < n_threads){
        pthread_join(threads[i],NULL); // esperar a que todas las hebras terminen
        i++;
    }



    return 0;
}





int getSchroedingerValue(int i,int j,int t,int N){
    if(t<=0){
        if(i >= 0.4*N && i <= 0.6*N && j >= 0.4*N && j <= 0.6*N) return 20;
        else return 0;
    }
    else return 2*getSchroedingerValue(i,j,t-1,N)-getSchroedingerValue(i,j,t-2,N)+0.0025*(getSchroedingerValue(i+1,j,t-1,N)+getSchroedingerValue(i-1,j,t-1,N)+getSchroedingerValue(i,j-1,t-1,N)+getSchroedingerValue(i,j+1,t-1,N)+4*getSchroedingerValue(i,j,t-1,N));

}

// escribe la matriz(grid) en el archivo de salida
void writeGrid(int N, char *file_name_out){
    FILE *f = fopen(file_name_out , "wb");
    int i;
    for(i=0;i<N;i++){

        fwrite(grid[i],N,sizeof(float),f);

    }
    fclose(f);
}


// llena los espacios vacios con caracteres aleatorios
void fillSoupRandom(char **soup, int length, int width){
    int i,j;

    for (i = 0; i < length; ++i) {
        for (j = 0; j < width; ++j) {

            if(soup[i][j] == '0')  soup[i][j] = rand()%26 + 97; //caracter del alfabeto ingles latino
        }
    }
}


void *ubicar(void *params){

    ParamsThread *p = (ParamsThread*) params; // casteo de los parametros
    int x_random, y_random, word_length;

    int j=0,i=0;
    for (j = 0; j < p->n_words_by_thread; ++j) {

        word_length = strlen(p->assigned_words[j]);

        pthread_mutex_lock(p->mutexs+j);

        do{
            y_random = rand() % length;
            x_random = rand() % (width-word_length); // para asegurar que la palabra quepa
        } while (!checkPositions(x_random, y_random, word_length)); // se itera hasta encontrar un posicion valida donde quepa la palabra a insertar


        for (int i = 0; i < word_length; ++i){
            soup[y_random][x_random + i] = p->assigned_words[j][i];
        }

        pthread_mutex_unlock(p->mutexs+j);

    }

    return NULL;
}
//Obtiene las palabras desde el archivo de entrada
char **getWords(int quantity_words, char *file_name_in){
    int i=0,j=0;
    char** words = (char**)malloc(sizeof(char*)*quantity_words);
    char line [MAX_WORD_LENGTH];
    FILE *f_in = fopen (file_name_in, "r");


    while ( fgets(line, MAX_WORD_LENGTH, f_in )  ){

        words[i] = (char*)malloc(sizeof(char)*MAX_WORD_LENGTH);

        sscanf(line,"%s%*c", words[i]);
        j=0;

        while(j<strlen(words[i])) {
              words[i][j] = toupper(words[i][j]);  // palabra = PALABRA
              j++;
           }
        i++;
    }


    return words;
}

int checkArgs(int width, int length, int quantity_words, int n_threads, char *file_name_in){
    FILE *f_in = fopen(file_name_in, "r");
    char** words = (char**)malloc(sizeof(char*)*quantity_words);
    char line[MAX_WORD_LENGTH];
    int i=0;

    int longer_word=0;

    while ( fgets(line, sizeof line, f_in )){

        words[i] = (char*)malloc(sizeof(char)*MAX_WORD_LENGTH);
        sscanf(line,"%s%*c", words[i]);
        if (longer_word < strlen(words[i]))   longer_word = strlen(words[i]);
    }

    if(n_threads > quantity_words) printf("El numero de hebras debe ser menor a la cantidad de palabras\n");
    else if(n_threads < 1) printf("El numero de hrbras debe ser mayor a 1\n");
    else if(longer_word > width) printf("El ancho de la sopas es menor que el tamño de la palabra más larga\n");
    else return 1;
    return 0;
}


// Checkea que las posiciones donde se insertara la palabra este disponible
int checkPositions( int x, int y, int word_length){
    for (int i = 0; i < word_length; ++i) {
        if(soup[y][x+i] != '0')
            return 0;
    }
    return 1;
}

// Recorre la matriz sopa e imprime cada caracter
void printSoup(){
    int i,j;
    printf("\n");
    for(i=0;i<length;i++){

        printf("  ");
        for(j=0;j<width;j++){
            printf("%c" ,soup[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Revsrva memoria y llena con ceros la matriz sopa
char **createSoup(int width, int length){
    int i,j;
    char** soup = (char**)malloc(sizeof(char*)*length);
    for(i=0;i<length;i++){
        soup[i] = (char*)malloc(sizeof(char)*width);
        for(j=0;j<width;j++){
            soup[i][j]='0';
        }
    }

    return soup;
}
