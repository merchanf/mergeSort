#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ordenarArchivo(char nombreArchivo[]){
    int estado = 1;
    FILE* ptr_file = fopen(nombreArchivo, "r");
    if(ptr_file != NULL){
	estado = 0;
    	struct Tabla tabla;
    	tabla = importarTabla(ptr_file);
    	tabla = ordenarTabla(tabla);
    	exportartabla(tabla, nombreArchivo);
    }
    return estado;
}

void *ordenarArchivoHilo(void *param){
	char* nombreArchivo = (char*)param;
	struct Tabla tabla;
	tabla = importarTablaHilo(nombreArchivo );
	tabla = ordenarTabla(tabla);
	exportartabla(tabla, nombreArchivo);
}

struct Fila crearFila(char linea[]){
    struct Fila fila;
    char *stringp = linea;
    char *delim = " ";
    char *token;

    int i=0;
    while (stringp != NULL) {
        token = strsep(&stringp, delim);
    	if( strcmp(token, "") != 0  ){
    	    strcpy(fila.columna[i++], token);
    	}
    }
    return fila;
};

void crearFilaMerge(char linea[], char** retorno){
    char *stringp = linea;
    char *delim = " ";
    char *token;
    int i=0;
    while (stringp != NULL) {
        token = strsep(&stringp, delim);
    	if( strcmp(token, "") != 0  ){
    	    strcpy(retorno[i++], token);
    	}
    }
};
struct Tabla importarTabla(FILE *ptr_file){
    char buf[1000];
    struct Tabla tabla;
    int tam = 0;
    if (ptr_file){
        while (fgets(buf,1000, ptr_file)!=NULL)
            tabla.fila[tam++] = crearFila(buf);
	tabla.tam = tam;
        fclose(ptr_file);
    }
    return tabla;
}


struct Tabla importarTablaHilo(char nombreArchivo[]){
	FILE *ptr_file;
	char buf[1000];
	struct Tabla tabla;
	ptr_file = fopen(nombreArchivo, "r");
	int tam = 0;
	if (ptr_file){
	while (fgets(buf,1000, ptr_file)!=NULL)
	tabla.fila[tam++] = crearFila(buf);
	tabla.tam = tam;
	fclose(ptr_file);
	}
	return tabla;
}
struct TablaMerge importarTablaMerge(char* argc[], int argv, int tamTotal)
{
    char buf[1000];
    int tam = 0, i,j;
    struct TablaMerge tabla;
    tabla.filas = (char***)malloc(sizeof(char**)*tamTotal*600);
    for(i = 0; i < tamTotal; i++){
	tabla.filas[i] = (char**)malloc(sizeof(char*)*6);
	for(j = 0; j < 6; j++){
		tabla.filas[i][j] = (char*)malloc(sizeof(char)*100);
    	}
    }

    FILE* ptr_file;
    for(i = 1; i < argv-1; i++){
    	    ptr_file = fopen(argc[i], "r");
	    if (ptr_file){
		while (fgets(buf,1000, ptr_file)!=NULL){
		    crearFilaMerge(buf,tabla.filas[tam++]);
		}
		fclose(ptr_file);
	    }
    }
    tabla.tam = tamTotal;
    return tabla;
}
struct Tabla ordenarTabla( struct Tabla tabla ){
	int i, j, m, n;
	for( i=1; i<tabla.tam; i++){
		for(j=0; j<tabla.tam-i; j++){
			//criterio 1
			if( atoi(tabla.fila[j].columna[3])>atoi(tabla.fila[j+1].columna[3]) ){
				struct Fila temp = tabla.fila[j];
				tabla.fila[j] = tabla.fila[j+1];
				tabla.fila[j+1] = temp;
			}else if( atoi(tabla.fila[j].columna[3])>atoi(tabla.fila[j+1].columna[3]) ){
				//Criterio 2
				if( strcmp(tabla.fila[j].columna[4], tabla.fila[j+1].columna[4]) > 0 ){
					struct Fila temp = tabla.fila[j];
					tabla.fila[j] = tabla.fila[j+1];
					tabla.fila[j+1] = temp;
				}else if( strcmp(tabla.fila[j].columna[4], tabla.fila[j+1].columna[4]) == 0 ){
					//Criterio 3
					if( strcmp(tabla.fila[j].columna[5], tabla.fila[j+1].columna[5]) > 0 ){
					struct Fila temp = tabla.fila[j];
					tabla.fila[j] = tabla.fila[j+1];
					tabla.fila[j+1] = temp;
				}
			}
		}
	}
}
return tabla;
}

void exportartabla( struct Tabla tabla, char *nombreArchivo ){
    printf("\nnombre archivo = %s\n",nombreArchivo);
    FILE *file = fopen(nombreArchivo, "w");
    int i;
    if (file != NULL){
        for( i=0; i<tabla.tam; i++ ){
            fprintf(file, "%s %s %s %s %s %s\n", tabla.fila[i].columna[0],
                tabla.fila[i].columna[1], tabla.fila[i].columna[2],
                tabla.fila[i].columna[3], tabla.fila[i].columna[4],
                tabla.fila[i].columna[5] );
            fflush(file);
        }
        printf("\ncerro\n");
        fclose(file);
    }
}

void exportartablaMerge( struct TablaMerge tabla, char *nombreArchivo ){
    printf("\nnombre archivo = %s\n",nombreArchivo);
    FILE *file = fopen(nombreArchivo, "w");
    int i, j;
    if (file != NULL){
        for( i=0; i<tabla.tam; i++ ){
            fprintf(file, "%8s %4s %8s %6s %8s %8s\n", tabla.filas[i][0],
                tabla.filas[i][1], tabla.filas[i][2],
                tabla.filas[i][3], tabla.filas[i][4],
                tabla.filas[i][5] );
        }
        fclose(file);
    }
}

void imprimirTabla( struct Tabla tabla ){
    int i, j;
    int tam = tabla.tam;
    for(i=0; i<tam; i++){
        for(j=0; j<6; j++ ){
	    printf("%s ",tabla.fila[i].columna[j]);
        }
    }
    printf("\n");
}

void imprimirTablaMerge( struct TablaMerge tabla ){
    int i, j;
    for(i=0; i<tabla.tam; i++){
        for(j=0; j<6; j++ ){
	    printf("%s ",tabla.filas[i][j]);
        }
    }
}
void ssort(struct TablaMerge arr, struct TablaMerge arrAux, int lo, int hi)
{
    if(lo >= hi) return;
    int mid = (lo + hi)/2;
    ssort(arr, arrAux, lo, mid);
    ssort(arr, arrAux, mid+1, hi);
    mmerge(arr, arrAux, lo, mid, hi);
}

void mmerge(struct TablaMerge arr, struct TablaMerge arrAux, int lo, int mid, int hi)
{
    int k,l;

    for(k = lo; k <= hi; k++)
        for(l = 0; l < 6; l++)
        	strcpy(arrAux.filas[k][l],arr.filas[k][l]);
    int i = lo, j = mid+1;


    for(k = lo; k <= hi; k++)
    {
        if(i > mid){
		for(l = 0; l < 6; l++){
			strcpy(arr.filas[k][l], arrAux.filas[j][l]);
		}
		j++;
        }else if(j > hi) {
		for(l = 0; l < 6; l++){
			strcpy(arr.filas[k][l] , arrAux.filas[i][l]);
		}
		i++;
        }else if((atoi(arrAux.filas[i][3]) > atoi(arrAux.filas[j][3])) || (atoi(arrAux.filas[i][3]) == atoi(arrAux.filas[j][3]) && arrAux.filas[i][4] > arrAux.filas[j][4]) || (atoi(arrAux.filas[i][3]) == atoi(arrAux.filas[j][3]) && arrAux.filas[i][4] == arrAux.filas[j][4] && arrAux.filas[i][5] == arrAux.filas[j][5])){
		for(l = 0; l < 6; l++){
	            strcpy(arr.filas[k][l] , arrAux.filas[j][l]);
		}
		j++;
        }else{
		for(l = 0; l < 6; l++){
            		strcpy(arr.filas[k][l] , arrAux.filas[i][l]);
		}
		i++;
        }
    }
}

int fileTam(char *nombreArchivo){
        FILE * fp = fopen(nombreArchivo, "r");
	int lines = 0;
	char buf[1000];
    	if (fp != NULL){
        	while (fgets(buf,1000, fp)!=NULL){
		 lines++;
		}
	}
	return lines;
}
