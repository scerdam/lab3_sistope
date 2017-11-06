#include "main.h"

//int width,length;
//char **soup;  //matriz que representa la sopa de letra y es compartida por las hebras

float **grid; // float **H;

int main(int argc, char **argv) {

      time_t start, end, duration;
//      srand((unsigned int) time(NULL));

      char *file_name_out = NULL;
      int T, N, n_threads, t_iterations = 0;

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


      ParamsThread params[n_threads];
      pthread_t threads[n_threads];

      getRowsToWork(params,N,n_threads);





      float foo = getSchroedingerValue(4,7,5,10);
      int i=0;
      while(i<n_threads){
        printf("Hebra %d , start = %d , end = %d \n",i,params[i].row_start, params[i].row_end );
        i++;
      }



      return 0;
}


void getRowsToWork(ParamsThread *p,int N, int n_threads){

    int rows_by_thread = N/n_threads;
    int rows_by_thread_aux = rows_by_thread;

    int resto = N%n_threads;

    if(resto > 0){
      rows_by_thread_aux++;
    }
printf("___ %d ___ %d _____ %d ____\n",rows_by_thread,rows_by_thread_aux,resto );
    int i=0, counter=0 ,start=0,finish=0;
    while(i<n_threads){

      if(resto > 0){
        p[i].row_start = i*rows_by_thread_aux ;
        p[i].row_end = p[i].row_start + rows_by_thread_aux -1;

      }
      else{
        if(resto<0) p[i].row_start = i*rows_by_thread;
        else p[i].row_start = i*rows_by_thread_aux;

        if(i==n_threads-1) p[i].row_end = N-1;
        else p[i].row_end = p[i].row_start + rows_by_thread-1;

      }
      resto--;
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
    while(t>=0){
        if(i >= 0.4*N && i <= 0.6*N && j >= 0.4*N && j <= 0.6*N) res = res +20;

    }

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
