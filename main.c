#include "main.h"

//int width,length;
//char **soup;  //matriz que representa la sopa de letra y es compartida por las hebras

float **grid; // float **H;
int N,T;

int main(int argc, char **argv) {

      time_t start, end, duration;
//      srand((unsigned int) time(NULL));

      char *file_name_out = NULL;
      int n_threads, t_iterations = 0;

      int index;
      int c;


      opterr = 0;

      while ((c = getopt (argc, argv, "N:T:H:f:t:")) != -1)
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

      createGrid(N);

      ParamsThread params[n_threads];
      pthread_t threads[n_threads];

      getRowsToWork(params,N,n_threads);

      int i= 0;
      while (i < n_threads){

          pthread_create(&threads[i], NULL, fillGrid,  &params[i]);
          printf("Thread %d me ejecute \n",i);
          i++;
      }



      // int i=0;
      // while(i<n_threads){
      //   printf("Hebra %d , start = %d , end = %d \n",i,params[i].row_start, params[i].row_end );
      //   i++;
      // }

      i=0;
      while(i < n_threads){
          pthread_join(threads[i],NULL); // esperar a que todas las hebras terminen
          i++;
      }

      printGrid();

      return 0;
}

void *fillGrid(void *params){

    ParamsThread *p = (ParamsThread*) params;
    int start, end;
    start = p->row_start;
    end = p->row_end;
    int i,j;




    for (i = start; i <= end; ++i) {
        for (j = 0; j < N; ++j) {

            grid[i][j] = getSchroedingerValue(i,j,T,N);
        }
    }
    // if(start==end){
    //   for (j = 0; j < N; ++j) {
    //
    //       grid[start][j] = getSchroedingerValue(start,j,T,N);
    //   }
    // }




}



//calcula y setea las filas que seran asignadas a cada hebra (fila de inicio y termino para cada hebra)
void getRowsToWork(ParamsThread *p,int N, int n_threads){

    int rows_by_thread = N/n_threads;

    int resto = N%n_threads;

    int i=0, start=0,finish=0;
    while(i<n_threads){

        p[i].row_start = start;
        if(resto > 0){
          finish = start+rows_by_thread;
          resto--;
        }
        else{

          finish = start+rows_by_thread-1;

        }
        p[i].row_end = finish;
        start = finish+1;
        i++;
    }

}



float getSchroedingerValue(int i,int j,int t,int N){
    if(t<=0){
        if(i >= 0.4*N && i <= 0.6*N && j >= 0.4*N && j <= 0.6*N) return 20;
        else return 0;
    }
    else return 2*getSchroedingerValue(i,j,t-1,N)-getSchroedingerValue(i,j,t-2,N)+0.0025*(getSchroedingerValue(i+1,j,t-1,N)+getSchroedingerValue(i-1,j,t-1,N)+getSchroedingerValue(i,j-1,t-1,N)+getSchroedingerValue(i,j+1,t-1,N)+4*getSchroedingerValue(i,j,t-1,N));

}

float getSchroedingerValueIterativo(int i,int j,int t,int N){
    float res = 0;
    int i_aux = 0;
    while(i_aux < t){
        if(i >= 0.4*N && i <= 0.6*N && j >= 0.4*N && j <= 0.6*N) res = res + 20;
        

        i_aux++;
    }
    return res;
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

void createGrid(int N){
    int i,j;
    grid = (float**)malloc(sizeof(float*)*N);
    for(i=0;i<N;i++){
        grid[i] = (float*)malloc(sizeof(float)*N);
        for(j=0;j<N;j++){
            grid[i][j] = 0.0;
        }
    }

    //return soup;
}

void printGrid(){
    int i,j;
    printf("\n");
    for(i=0;i<N;i++){

        printf(" ");
        for(j=0;j<N;j++){
            printf("%f " ,grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
