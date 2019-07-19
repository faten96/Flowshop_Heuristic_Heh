#include <stdio.h>
#include <stdlib.h>

float **problem; //processing time of each job in each machine
int machines=0,jobs=0;
char *nomfichier;

struct sub {
    float machine1;
    float machine2;};

struct sub1{
    float valeur;
    int index;};
/*void fill_matrix(int rows, int cols){

      FILE *myfile;
      double myvariable;
      int i,j;

      myfile=fopen("test.txt","r");
      for(i = 0; i < rows; i++)
      {
        for (j = 0 ; j < cols; j++)
        {
          fscanf(myfile,"%lf",&myvariable);
          problem[i][j]=myvariable;
          //printf("%.4f ",tab[i][j]);
        }
        //printf("\n");
      }
      fclose(myfile);
}*/

void fill_matrix(){

      FILE *myfile;
      double myvariable;
      int i,j;

      myfile=fopen(nomfichier,"r");

      //read number of rows and columns (jobs and machines)
      int rows,cols;
      fscanf(myfile,"%d",&rows); //machines
      fscanf(myfile,"%d",&cols); //jobs
      machines=rows;
      jobs=cols;

      problem = malloc(machines * sizeof *problem);
      for (i=0; i<machines; i++){
        problem[i] = malloc(jobs * sizeof *problem[i]);
      }

      for(i = 0; i < rows; i++)
      {
        for (j = 0 ; j < cols; j++)
        {
          fscanf(myfile,"%lf",&myvariable);
          problem[i][j]=myvariable;
          //printf("%.4f ",tab[i][j]);
        }
        //printf("\n");
      }
      fclose(myfile);
}

void read(float** array, int rows, int cols){
  int i, j;
  for (i=0; i<rows; i++)
  {
    for (j=0; j<cols; j++)
    {
      printf("Enter value for Array[%d][%d]: ",i,j);
      scanf("%f",&array[i][j]);
    }
  }
}

void display(float** array, int rows, int cols){
  int i, j;
  for (i=0; i<rows; i++)
  {
    for (j=0; j<cols; j++)
    {
      printf("%f    ",array[i][j]);
    }
    printf("\n");
  }
}

float minimum(float a, float b){

    if(a<=b) return a;
    else return b;
}

float maximun(float a, float b){

    if(a>=b) return a;
    else return b;
}

int main(int argc, char *argv[]){

  int i,j,k,p,tmp=0;
  int k0=0;
  nomfichier=argv[1];// set the file name

  //*************************************** Data initialization *****************************************//

      //read(problem,machines,jobs);
      fill_matrix();
      //display(problem,machines,jobs);

  //*************************************** Treatment *****************************************//

     ///Step1: For  each  k  and   i,  using  relation  (3)  and  (4),  calculate  Rk (i,  1) and Rk (i,  2).
      k0=machines-1; /* number of sub-problems */
      struct sub array[jobs][k0];
      float sum1=0,sum2=0,tmp_time=0,tmp_index=0;

      for(k=1;k<=k0;k++){
        for(i=0;i<jobs;i++){

                /* calculate Pk(i,1) */
                for(j=0;j<k;j++){
                    sum1=sum1+problem[j][i];
                }
                array[i][k-1].machine1=sum1;
                sum1=0;
                /* calculate Pk(i,2) */
                for(j=machines-k;j<machines;j++){
                    sum2=sum2+problem[j][i];
                }
                array[i][k-1].machine2=sum2;
                sum2=0;
        }
      }

    ///Step2: For  each  k  and   i,  using  relation  (3)  and  (4),  calculate  R(k, i).
    struct sub1 array1[jobs][k0];

    for(i=0;i<jobs;i++){
        for(j=0;j<k0;j++){

            if(array[i][j].machine1>=array[i][j].machine2){
                array1[i][j].valeur=1/minimum(array[i][j].machine1,array[i][j].machine2);
                array1[i][j].index=i;
            }
            else{
                array1[i][j].valeur=-1/minimum(array[i][j].machine1,array[i][j].machine2);
                array1[i][j].index=i;
            }
        }
      }


    ///Step3: For  each  value  of  k9  the  jobs  are to be arranged in ascending order  of R(k,  i), breaking the ties.

    for(p=0;p<k0;p++){  /* each column of array1 contains a sequence of jobs */
        for(i=0;i<jobs;i++){

                for(j=i+1; j<jobs; j++){

                    if(array1[i][p].valeur > array1[j][p].valeur){
                        tmp_time= array1[i][p].valeur;
                        tmp_index=array1[i][p].index;
                        array1[i][p].valeur = array1[j][p].valeur;
                        array1[i][p].index=array1[j][p].index;
                        array1[j][p].valeur = tmp_time;
                        array1[j][p].index=tmp_index;

                    }
                    if(array1[i][p].valeur==array1[j][p].valeur){ /* break the tie using the rule in the article */
                        for(k=p+1;k<k0;k++){
                            if(array1[i][k].valeur > array1[j][k].valeur){
                                tmp_time= array1[i][p].valeur;
                                tmp_index=array1[i][p].index;
                                array1[i][p].valeur = array1[j][p].valeur;
                                array1[i][p].index=array1[j][p].index;
                                array1[j][p].valeur = tmp_time;
                                array1[j][p].index=tmp_index;


                            }
                        }
                    }
                }
    }
  }

   ///Step4: Calculate the make-span for each sequence and choose the minimum make-span
   float schedule[machines][jobs];
   int sequence=0;
   int makespan=100000000;

    struct sub1 array2[k0][jobs];
    for(i=0;i<jobs;i++){ /* fill the schedule array in order to calculate the make-span */
        for(j=0;j<k0;j++){

                array2[j][i].valeur=array1[i][j].valeur;
                array2[j][i].index=array1[i][j].index;
        }
    }

   for(k=0;k<k0;k++){ /* repeat for each sequence */


           for(i=0;i<machines;i++){ /* fill the schedule array in order to calculate the make-span */
                for(j=0;j<jobs;j++){

                    if(i==0 && j==0) schedule[i][j]=problem[i][array2[k][j].index];
                    if(i==0 && j!=0) schedule[i][j]=schedule[i][j-1]+problem[i][array2[k][j].index];
                    if(i!=0 && j==0) schedule[i][j]=schedule[i-1][j]+problem[i][array2[k][j].index];
                    if(i!=0 && j!=0) schedule[i][j]=maximun(schedule[i][j-1],schedule[i-1][j])+problem[i][array2[k][j].index];

                }
            }
            /* calculate the make-span */
            if(schedule[machines-1][jobs-1]<makespan){
               makespan=schedule[machines-1][jobs-1];
               sequence=k; /* store the sequence of the minimum make-span */
            }

   }

   /***      Print the sequence and the make-span   ***/
   //printf("The sequence:  ");
   for(i=0;i<jobs;i++) printf("%d,",(array2[sequence][i].index)+1);
   printf("%d",makespan);







    return 0;
}
