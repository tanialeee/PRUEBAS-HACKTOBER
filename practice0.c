/*    AUTORES:
*   Ignacio Borregan Naya :ignacio.borregan
*   Carlos Iglesias Gomez :carlos.iglesias1
*   GRUPO: 32
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#define   MAX 500
#define  MAXH 4096 //maximo del array utilizado en hist



int TrocearCadena(char * cadena, char * trozos[]){
  int i=1;

  if ((trozos[0]=strtok(cadena," \n\t"))==NULL) //se trocea la cadena utilizando como delimitador los espacios
  return 0;
  while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)i++;
  return i;
}
/*****************************************************************/
int opcion(char *cadena[]) {
  if(*cadena != NULL){
    if (!strcmp(cadena[0],"autores")) {
      return 1;
    } else if (!strcmp(cadena[0],"pid")) {
      return 2;
    } else if (!strcmp(cadena[0],"cdir")) {
      return 3;
    } else if (!strcmp(cadena[0],"fecha")) {
      return 4;
    } else if (!strcmp(cadena[0],"hora")) {
      return 5;
    } else if (!strcmp(cadena[0],"hist")) {
      return 6;
    } else if (!strcmp(cadena[0],"fin")||!strcmp(cadena[0],"end")||!strcmp(cadena[0],"exit")){ //se pone || ya que la hacen lo ismo
      return 0;
    }else
    return 8;
  }else
  return 7;
}
/*****************************************************************/
void autores(char *trozos[]){
  if ((trozos[0]==NULL)||!strcmp(trozos[0],"-n")) { //la ausencia de opcion imprime
    printf("Ignacio Borregan Naya\n ");             //la combinacion de -n y -l
    printf("Carlos Iglesias Gomez\n");
   }
    if((trozos[0]==NULL)||!strcmp(trozos[0],"-l")){
    printf(" ignacio.borregan\n");
    printf(" carlos.iglesias1\n");
  }
}
/*****************************************************************/
void processID(char *trozos[]){
  if (trozos[0]==NULL){
    printf("Pid del shell : %d\n", getpid());
  } else if (!strcmp(trozos[0],"-p")) {
    printf("Pid del padre del shell: %d\n", getppid());
  }//else
    //printf("Invalid option %s\n",trozos[0]);
}
/*****************************************************************/
void cdir(char *trozos[]){
  char direccion [100];
  if (trozos[0]==NULL){
    printf("Ahora estas en %s\n", getcwd(direccion, 100));
  }else{
    if (chdir(trozos[0])!=0){
      printf("Error: no existe la ruta especificada\n");
    }
  }
}
/******************************************************************/
void fecha(){
	time_t t = time(NULL);
			struct tm *tm = localtime(&t);
			char date[MAX];
			strftime(date,MAX,"%a %b %2d %2H:%2M:%2S %Y",tm);
			printf("%s\n",date);
}
/*****************************************************************/
void hora(){
  time_t t = time(NULL);
			struct tm *tm = localtime(&t);
			char hour[MAX];
			strftime(hour,MAX,"%2H:%2M:%2S",tm);
			printf("%s\n",hour);
}
/******************************************************************/

int hisLength(char *his[]){ //funcion que devuelve longitud del array
  int i=0;
  int cont=0;
  while (his[i]!=NULL) {
      cont++;
      i++;
  }
  return cont;
}
/******************************************************************/
void pointar(char *copia, char *his[]) {
  int i=0;

    while (his[i]!=NULL) { //vamos hasta la posicion siguiente al ultimo dato introducido
      i++;
    }
    his[i]=strdup(copia); // y almacenamos el comando


}

/******************************************************************/
/*void emptyHist(char *his[]){ //funcion para vaciar el array

  for (int i=hisLength(his)-1; i <1 ; i--) {
    his[i]=NULL;
  }

}*/
void emptyHist(char *his[]){ //funcion para vaciar el array
  int i=hisLength(his)-1;
  while (his[0]!=NULL) {
    free(his[i]);
    his[i]=NULL;
    i--;
  }

}
/*****************************************************************/
/*void hist(char *trozos[], char *his[]) {
  int fin=hisLength(his);
  if (trozos[0]==NULL) {

      for (int i = 0; i <= fin -1 ; i++) {
        printf("%s", his[i] );
      }
  } else if (!strcmp(trozos[0],"-c")){
      emptyHist(his);
  }

}*/
void hist(char *trozos[], char *his[]) {
  int i=0;
  if (trozos[0]==NULL) {
      while (his[i]!=NULL) {
        printf("%s", his[i] );
        i++;
      }
  } else if (!strcmp(trozos[0],"-c")){
      emptyHist(his);
      printf("Historico Borrado\n");
  }

}
/******************************************************************/
void inicializarHistorico(char *his[]) {
  for (int i = 0; i < MAXH; i++) {
    his[i]=NULL;
  }
}

/*****************************************************************/
void printPrompt(){
    printf("♠ : ");
}
/*****************************************************************/
void leerEntrada(char *orden){

  fgets(orden,MAX,stdin);

}

/****************************************************************/
void procesarEntrada(char *orden,char *trozos[],char *his[]) {

    char* copia=NULL;
    copia=strdup(orden); //almacenamos una copia de la orden del usuario para poder guardarla en el historico

    int n = TrocearCadena(orden,trozos);
    if (*trozos!=NULL)   //comprobamos si se introdujo algo por parte del usuario
      pointar(copia, his);



  switch (opcion(trozos)) {

    case 0:

      emptyHist(his);   //antes de terminar liberamos el historial
      free(copia);
      exit(0);
        break;
    case 1:
      autores(trozos+1);

      break;

    case 2:
      processID(trozos+1);
      break;

    case 3:
      cdir(trozos+1);
      break;

    case 4:
      fecha();
      break;

    case 5:
      hora();
        break;

    case 6:
        hist(trozos+1,his);
        break;
    case 7:
        printf("\n");
            break;
    default:
      printf("Error: Comando no reconocido, vuelva a intentarlo.\n");

      break;

    }
    while (n!=0) {
      trozos[n]=NULL;
      n--;
    }
    free(copia);


}
/*******************************************************************/
int main() {

  char orden[MAX];
  char *trozos[MAX];
  char *his[MAXH];

  inicializarHistorico(his);
  while (1) {
    printPrompt();
    leerEntrada(orden);
    procesarEntrada(orden,trozos,his); //aqui estaba pasando contador


  }
  return 0;
}
