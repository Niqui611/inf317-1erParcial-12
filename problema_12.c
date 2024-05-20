#include <stdio.h>
#include <mpi.h>

#define N 10 // Número total de términos en la secuencia de Fibonacci

int fibonacci(int n) {
    if (n <= 1)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "Se necesitan al menos 2 procesos para ejecutar este programa.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) { // Proceso maestro (MASTER)
        int i, term;
        printf("Secuencia de Fibonacci:\n");

        // Solicitar cada término a los procesos esclavos (SLAVE)
        for (i = 0; i < N; i++) {
            MPI_Recv(&term, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%d ", term);
        }
        printf("\n");
    } else { // Procesos esclavos (SLAVE)
        int term;
        for (int i = 0; i < N; i++) {
            term = fibonacci(i);
            MPI_Send(&term, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
