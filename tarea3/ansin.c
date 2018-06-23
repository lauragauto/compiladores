#include "anlex.c"
#include <stdio.h>


/*
 *	Analizador Sintáctico
 *
 *	Descripcion:
 * Implementar un Analizador Sintáctico que reconozca códigos fuentes escritos en el lenguaje
   Json simplificado propuesto.
 */



/*BNF JSON*/
void json();
void element(int []);
void array(int []);
void arrayg(int []);
void elementList(int []);
void elementListg(int []);
void object(int []);
void objectg(int []);
void attribute(int []);
void attributesList(int []);
void attributesListg(int []);
void attributeName(int []);
void attributeValue(int []);


//Funciones de control
void match(int);

void contEntrada(int [], int []);

void sto(int []);

void error_();

//Aceptación para la entrada
int acepto=1;


//Gramatica

void json(){
	int get_token[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};
	//synchset de sincronizacion
	element(get_token);
}

//puede ser un object o un array
void element(int synchset[]){
    int c_primero[] = {L_CORCHETE,L_LLAVE,'\0'};
    int get_token[] = {COMA,R_CORCHETE,R_LLAVE, '\0'};

    contEntrada(c_primero, synchset);

    if(t.compLex == L_LLAVE)
        object(get_token);
    else if(t.compLex == L_CORCHETE)
        array(get_token);

    contEntrada(get_token, c_primero);

}


void array(int synchset[]){
	int c_primero[] = {L_CORCHETE, '\0'};
	int get_token[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	contEntrada(c_primero, synchset);

	if (t.compLex == L_CORCHETE){
		match(L_CORCHETE);
		arrayg(get_token);
	}

	contEntrada(get_token, c_primero);

}


void arrayg(int synchset[]){
	int c_primero[] = {L_LLAVE, L_CORCHETE, R_CORCHETE, '\0'};

	//PROBLEMA CON L_CORCHETE
	int get_token[] = {COMA, R_CORCHETE, R_LLAVE, L_CORCHETE, '\0'};

	contEntrada(c_primero, synchset);

	if ((t.compLex == L_LLAVE) || (t.compLex == L_CORCHETE)){

		elementList(get_token);
		match(R_CORCHETE);
	}
	else if(t.compLex == R_CORCHETE){
		match(R_CORCHETE);

	}

	contEntrada(get_token, c_primero);
}


void elementList(int synchset[]){
	int c_primero[] = {L_LLAVE, L_CORCHETE, '\0'};
	int get_token[] = {R_CORCHETE, '\0'};

	contEntrada(c_primero, synchset);

	if((t.compLex == L_LLAVE) || (t.compLex == L_CORCHETE)){
		element(get_token);
		elementListg(get_token);
	}

	contEntrada(get_token, c_primero);
}


void elementListg(int synchset[]){

	if(t.compLex == R_CORCHETE)
       return;

	int c_primero[] = {COMA,'\0'};
	int get_token[] = {R_CORCHETE, '\0'};

	contEntrada(c_primero, synchset);



	if (t.compLex == COMA){
		match(COMA);
		element(get_token);
		elementListg(get_token);
	}

	contEntrada(get_token, c_primero);
}


void object(int synchset[]){
	int c_primero[] = {L_LLAVE, '\0'};
	int get_token[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	contEntrada(c_primero, synchset);

	if (t.compLex == L_LLAVE) {
		match(L_LLAVE);
		objectg(get_token);
	}

	contEntrada(get_token, c_primero);
}


void objectg(int synchset[]){
	int c_primero[] = {STRING, R_LLAVE, '\0'};

	//problema con STRING
	int get_token[] = {COMA, R_CORCHETE, R_LLAVE, STRING, '\0'};

	contEntrada(c_primero, synchset);

	if (t.compLex == STRING){
		attributesList(synchset);
		match(R_LLAVE);
	} else if (t.compLex == R_LLAVE){
		match(R_LLAVE);
	}

	contEntrada(get_token, c_primero);
}


void attributesList(int synchset[]){
	int c_primero[] = {STRING, '\0'};
	int get_token[] = {R_LLAVE, '\0'};

	contEntrada(c_primero, synchset);

	if (t.compLex == STRING){
		attribute(get_token);
		attributesListg(get_token);
	}

	contEntrada(get_token, c_primero);

}


void attributesListg(int synchset[]){

	if (t.compLex == R_LLAVE)
        return;

	int c_primero[] = {COMA, '\0'};
	int get_token[] = {R_LLAVE, '\0'};

	contEntrada(c_primero, synchset);


	if (t.compLex == COMA){
		match(COMA);
		attribute(get_token);
		attributesListg(get_token);
	}

	contEntrada(get_token, c_primero);
}


void attribute(int synchset[]){
	int c_primero[] = {STRING, '\0'};
	int get_token[] = {COMA, R_LLAVE, '\0'};

	contEntrada(c_primero, synchset);

	if (t.compLex == STRING){
		attributeName(get_token);
		match(DOS_PUNTOS);
		attributeValue(get_token);
	}

	contEntrada(get_token, c_primero);
}


void attributeName(int synchset[]){
	int c_primero[] = {STRING, '\0'};
	int get_token[] = {DOS_PUNTOS, '\0'};

	contEntrada(c_primero, synchset);

	if (t.compLex == STRING)
		match(STRING);

	contEntrada(get_token, c_primero);
}


void attributeValue(int synchset[]){
	int c_primero[] = {L_LLAVE, L_CORCHETE, STRING,
		NUMBER, PR_TRUE, PR_FALSE, PR_NULL, '\0'};
	int get_token[] = {COMA, R_LLAVE, '\0'};

	contEntrada(c_primero, synchset);

	if (t.compLex == L_LLAVE){
		element(get_token);
	}
	else if (t.compLex == L_CORCHETE) {
		element(get_token);
	}
	else if (t.compLex == STRING){
		match(STRING);
	}
	else if (t.compLex == NUMBER){
		match(NUMBER);
	}
	else if (t.compLex == PR_TRUE){
		match(PR_TRUE);
	}
	else if (t.compLex == PR_FALSE){
		match(PR_FALSE);
	}
	else if (t.compLex == PR_NULL){
		match(PR_NULL);
	}

	contEntrada(get_token, c_primero);

}


//llama al siguiente elemento de la pila
void match(int expectedToken){
    if(t.compLex == expectedToken)
        sigLex();
}



void contEntrada(int c_primero[], int get_token[]){

    int i=0;
    if(t.compLex == EOF)
    	return;

    //entra cuando no hay errores
    while(c_primero[i] != '\0')
    {
        if(t.compLex == c_primero[i])
            return;
        i++;
    }

 	i=0;

    int synchset[]={1}; //acumulador

    while(c_primero[i] != '\0'){
        synchset[i] = c_primero[i];
        i++;
    }
    int j=0;

    while(get_token[j] != '\0'){
        synchset[i] = get_token[j];
        i++; //aumenta i
        j++; //aumenta j
    }

    sto(synchset);

}



//si no esta en el synchset llama a sigLex() e imprimer errores de la entrada
void sto(int synch[]){
    error_();
	int i = 0;
    while(t.compLex != synch[i] && t.compLex != EOF){
        if (synch[i] == '\0'){
            sigLex();
            i=0;
        }
        i++;
    }
    //el sigLex que falta luego de salir
    sigLex();
    return;
}


/*errores en pantalla*/
void error_(){
	acepto=0;
   	printf("\nError: Linea %d. Se encontro %s.\n", numLinea, t.lexema);
}



//int main(int argc,char* args[]){
//	//como imprimir un \n
//	if(argc > 1)
//	{
//		if (!(archivo=fopen(args[1],"rt")))
//		{
//			printf("Archivo no encontrado.\n");
//			exit(1);
//		}
//		/*
//		while (t.compLex!=EOF)
//		{
//			sigLex();
//
//            if(cantEspacio > -1) //si ya no existen espacios en esta linea se imprimen los ya guardados
//            {
//                int j = 0;
//                for(j=0; j<=cantEspacio;j++)
//                    printf("%c",espacios[j]);
//            }
//            switch(t.compLex)
//			{
//                case L_CORCHETE:
//                    printf("%s"," L_CORCHETE");
//                    break;
//                case R_CORCHETE:
//                    printf("%s"," R_CORCHETE");
//                    break;
//                case L_LLAVE:
//                    printf("%s"," L_LLAVE");
//                    break;
//                case R_LLAVE:
//                    printf("%s"," R_LLAVE");
//                    break;
//                case COMA:
//                    printf("%s"," COMA");
//                    break;
//                case DOS_PUNTOS:
//                    printf("%s"," DOS_PUNTOS");
//                    break;
//                case STRING:
//                    printf("%s"," STRING");
//                    break;
//                case NUMBER:
//                    printf("%s"," NUMBER");
//                    break;
//                case PR_TRUE:
//                    printf("%s"," PR_TRUE");
//                    break;
//                case PR_FALSE:
//                    printf("%s"," PR_FALSE");
//                    break;
//                case PR_NULL:
//                    printf("%s"," PR_NULL");
//                    break;
//                case EOF:
//					printf("%s"," EOF\n");
//                    break;
//            }
//
//		}*/
//
//		sigLex();
//        json();
//        if(acepto){
//        	printf("Sin errores sintacticos.\n");
//		}
//		else {
//			printf("Proceso de parser finalizado con errores.\n");
//		}
//		fclose(archivo);
//	}
//	else
//	{
//		printf("Debe pasar como parametro el path al archivo fuente.\n");
//		exit(1);
//	}
//
//	return 0;
//}
