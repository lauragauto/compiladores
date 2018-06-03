/*
 *	Analizador Léxico
 *
 *	Descripcion:
 *	Implementar un analizador léxico para el lenguaje Json simplificado
 */
/*********** Inclusion de cabecera **************/

#include "anlex.h"

/************* Variables globales **************/
token t;				    // token global para recibir componentes del Analizador Lexico

/***** Variables para el analizador lexico *****/

FILE *archivo;			    // Fuente json
char id[TAMLEX];		    // Utilizado por el analizador lexico
int numLinea=1;			    // Numero de Linea
int cantEspacio=-1;        // Cantidad de espacio que ya se ha cargado en espacio
char espacios[TAMESP];      // Cantidad de espacio


/**************** Funciones **********************/

// Rutinas del analizador lexico
void error(const char* mensaje)
{
	printf(" Lin %d: Error Lexico: %s",numLinea,mensaje);
}

void sigLex()
{
	int i=0;
    char c=0;
    int ban=0;
	int acepto=0;
	int estado=0;
	char msg[41];
	char aux[TAMAUX] = " "; // array auxiliar para leer false, null y true
	cantEspacio = -1;
	token e;

   	while((c=fgetc(archivo))!=EOF)
	{
        if(c=='\n')
	    {
		    //Sumar al numero de linea actual
		    numLinea++;
            printf("\n"); //imprimir los saltos de lineas
		    continue;
	    }
	    else if (c==' ')
	    {
            do
            {
                cantEspacio++; //contar la cantidad de espacios, para imprimir los token en el formato del archivo fuente
                espacios[cantEspacio]=' '; // guardar la cantidad de espacios para imprimirlos
                c=fgetc(archivo);
            }while(c ==' ');
            c=ungetc(c,archivo);
	    }
        else if (c=='\t')
        {
            while(c=='\t')
            {
                //printf("%c", c);
	        	c=fgetc(archivo);
            }
        }
    	else if (c=='\"')
		{
            //un caracter o una cadena de caracteres
			i=0;
			id[i]=c;
			i++;
			do
			{
				c=fgetc(archivo);
				if (c=='\"')
				// Fin del literal
				{
                    id[i]=c;
                    i++;
                    ban=1;
                    break;
				}
                else if(c==EOF || c==',' || c=='\n' || c==':')
				{
                    sprintf(msg,"Se esperaba que finalize el literal");
					error(msg);
					break;
                    while(c!='\n')
                        c=fgetc(archivo);

                    ungetc(c,archivo);
                    cantEspacio=-1;
                    break;
				}
				else
				{
					id[i]=c;
					i++;
				}
			}while(isascii(c) || ban==0);
			    id[i]='\0';
            strcpy(t.lexema,id);
			t.compLex = STRING;
			break;
		}
    	else if (isdigit(c) || c=='+' || c=='-')
	    {
            //es un numero
            i=0;
            estado=0;
            acepto=0;
            id[i]=c;
			while(!acepto)
			{
				switch(estado)
				{
				    case 0: //una secuencia de digitos, puede ocurrir . o e
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=0;
						}
						else if(c=='.')
						{
							id[++i]=c;
							estado=1;
						}
						else if(tolower(c)=='e')
						{
							id[++i]=c;
							estado=3;
						}
						else
							estado=6;
						break;
					case 1://despues de un punto, debe seguir un digito
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else{
							sprintf(msg,"No se esperaba '%c' despues del . ",c);
							estado=-1;
						}
						break;
					case 2://la fraccion decimal, pueden seguir los digitos o e
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else if(tolower(c)=='e')
						{
							id[++i]=c;
							estado=3;
						}
						else
							estado=6;
						break;
					case 3://una e, puede seguir +, - o una secuencia de digitos
						c=fgetc(archivo);
						if (c=='+' || c=='-')
						{
							id[++i]=c;
							estado=4;
						}
						else if(isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else
						{
							sprintf(msg,"Se esperaba signo o digitos despues del exponente");
							estado=-1;
						}
						break;
					case 4://necesariamente debe venir por lo menos un digito
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else
						{
							sprintf(msg,"No se esperaba '%c' despues del signo",c);
							estado=-1;
						}
						break;
					case 5://una secuencia de digitos correspondiente al exponente
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else
							estado=6;
						break;
					case 6://estado de aceptacion, devolver el caracter correspondiente a otro componente lexico
						if (c!=EOF)
							ungetc(c,archivo);
						else
                            c=0;
						id[++i]='\0';
						acepto=1;
                        t.compLex=NUMBER;
						strcpy(t.lexema,id);
						break;
					case -1:
						if (c==EOF)
                            error("No se esperaba el fin de archivo\n");
						else
                            error(msg);
                        acepto=1;
                    t.compLex=VACIO;
                    while(c!='\n')
                        c=fgetc(archivo);
                    ungetc(c,archivo);
					break;
				}
			}
			break;
		}
		else if (c==':')
		{
            //puede ser un :
            t.compLex=DOS_PUNTOS;
            strcpy(t.lexema,":");
            break;
		}
		else if (c==',')
		{
			//es una ,
			t.compLex=COMA;
			strcpy(t.lexema,",");
			break;
		}
		else if (c=='[')
		{
			// es un corchete izquierdo
			t.compLex=L_CORCHETE;
			strcpy(t.lexema,"[");
			break;
		}
		else if (c==']')
		{
			// es un corchete derecho
			t.compLex=R_CORCHETE;
			strcpy(t.lexema,"]");
			break;
		}
		else if (c=='{')
		{
			// es una llave izquierda
			t.compLex=L_LLAVE;
			strcpy(t.lexema,"{");
			break;
        }
        else if (c=='}')
		{
			// es una llave derecha
			t.compLex=R_LLAVE;
			strcpy(t.lexema,"}");
			break;
        }
		else if (c=='n' || c=='N')
        {
            ungetc(c,archivo);
            fgets(aux,5,archivo);//Verificar si es Null
            if (strcmp(aux, "null")==0 || strcmp(aux, "NULL")==0)
            {
                t.compLex = PR_NULL;
                strcpy(t.lexema,aux);
            }
            else
            {
                sprintf(msg,"%c no esperado",c);
			    error(msg);

                while(c!='\n')
                    c=fgetc(archivo);

                t.compLex = VACIO;
                ungetc(c,archivo);
            }
            break;
        }
        else if (c=='f' || c=='F')
        {
            ungetc(c,archivo);
            fgets(aux,6,archivo);//ver si es boolean False
            if (strcmp(aux, "false")==0 || strcmp(aux, "FALSE")==0)
            {
                t.compLex = PR_FALSE;
                strcpy(t.lexema,aux);
            }
            else{
                sprintf(msg,"%c no esperado",c);
			    error(msg);

                while(c!='\n')
                    c=fgetc(archivo);

                t.compLex = VACIO;
                ungetc(c,archivo);
            }
            break;
        }
        else if (c=='t' || c=='T')
        {
            ungetc(c,archivo);
            fgets(aux,5,archivo);//verificar si es Boolean True
            if (strcmp(aux, "true")==0 || strcmp(aux, "TRUE")==0)
            {
                t.compLex = PR_TRUE;
                strcpy(t.lexema,aux);
            }
            else
            {
                sprintf(msg,"%c no esperado",c);
			    error(msg);

                while(c!='\n')
                    c=fgetc(archivo);

                t.compLex = VACIO;
                ungetc(c,archivo);
            }
            break;
        }
        else if (c!=EOF)
		{
			sprintf(msg,"%c no esperado",c);
			error(msg);
            while(c!='\n')
                c=fgetc(archivo);
            strcpy(espacios, " ");
            cantEspacio = -1; //no hay espacio para imprimir
            ungetc(c,archivo);
		}
	}
	if (c==EOF)
	{
		t.compLex=EOF;
		strcpy(t.lexema,"EOF");
		sprintf(t.lexema,"EOF");
	}
}


int main(int argc,char* args[])
{
	if(argc > 1)
	{
		if (!(archivo=fopen(args[1],"rt")))
		{
			printf("Archivo no encontrado.\n");
			exit(1);
		}
		while (t.compLex!=EOF)
		{
			sigLex();

            if(cantEspacio > -1) //si ya no existen espacios en esta linea se imprimen los ya guardados
            {
                int j = 0;
                for(j=0; j<=cantEspacio;j++)
                    printf("%c",espacios[j]);
            }
            switch(t.compLex)
			{
                case L_CORCHETE:
                    printf("%s"," L_CORCHETE");
                    break;
                case R_CORCHETE:
                    printf("%s"," R_CORCHETE");
                    break;
                case L_LLAVE:
                    printf("%s"," L_LLAVE");
                    break;
                case R_LLAVE:
                    printf("%s"," R_LLAVE");
                    break;
                case COMA:
                    printf("%s"," COMA");
                    break;
                case DOS_PUNTOS:
                    printf("%s"," DOS_PUNTOS");
                    break;
                case STRING:
                    printf("%s"," STRING");
                    break;
                case NUMBER:
                    printf("%s"," NUMBER");
                    break;
                case PR_TRUE:
                    printf("%s"," PR_TRUE");
                    break;
                case PR_FALSE:
                    printf("%s"," PR_FALSE");
                    break;
                case PR_NULL:
                    printf("%s"," PR_NULL");
                    break;
                case EOF:
					printf("%s"," EOF\n");
                    break;
            }

		}
		fclose(archivo);
	}
	else
	{
		printf("Debe pasar como parametro el path al archivo fuente.\n");
		exit(1);
	}

	return 0;
}
