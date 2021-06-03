#include <mpi.h>
#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



char *stro="aa5UYq6trT5u.";


int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int size,rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    char cmp[5]={0};

    char salt[3]={0};
    salt[0]=stro[0];
    salt[1]=stro[1];

    /* Ten fragment kodu nalezy _jakos_ zrównoleglić */
    /* pewnie będzie potrzebna jakaś pętla... */
    int i,j,k,l;
    for (i='a';i<='z';i++){
	    cmp[0]=i;
	    for (j='a';j<='z';j++){
		    cmp[1]=j;
		    for (k='a';k<='z';k++){
			    cmp[2]=k;
			    for (l='a';l<='z';l++){
				    cmp[3]=l;

    				    char * x=crypt(cmp, salt);

    					if ((strcmp(x,stro))==0)
    					{
            					/* w docelowej wersji przeslac odgadnięte hasło masterowi */
	    					printf("Udalo sie: %s %s %s\n", cmp, x, stro);
           					MPI_Abort(MPI_COMM_WORLD, 0);
	    					exit(0);
    					}
			    }
		    }
	    }
    }

    MPI_Finalize();
}
