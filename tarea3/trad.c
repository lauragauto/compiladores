#include "ansin.c"
#include <stdio.h>
/*
 *	Traduccion Dirigida por Sintaxis
 *
 *	Descripcion:
 * Implementar un traductor para el lenguaje Json Simplificado.
 */

void json_trad();
void element_trad();
void array_trad();
void arrayg_trad();
void element_list_trad();
void element_listg_trad();
void object_trad();
void objectg_trad();
void attribute_trad();
void attributes_list_trad();
void attributes_listg_trad();
void attribute_name_trad();
void attribute_value_trad();
void match(int);
void string_sin_comillas(char* cadena);

int cant_esp=0; // cantidad de espacios

void imp_espacios();

// JSON -> element.trad
void json_trad(){
    element_trad();
}

//element.trad -> object.trad | array.trad
void element_trad(){

    if(t.compLex == L_LLAVE){
        object_trad();
    }
    else if(t.compLex == L_CORCHETE){
        array_trad();
    }

}

//array.trad -> arrayX.trad
void array_trad(){
    if(t.compLex == L_CORCHETE){

        match(L_CORCHETE);
        arrayg_trad();

    }
}

//arrayX.trad -> <item> elementespacios-list.trad </item> | ε
void arrayg_trad(){

    if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
        printf("\n");
        imp_espacios();
        printf("<item>\n");
        cant_esp+=1;
        element_list_trad();
        match(R_CORCHETE);
        imp_espacios();
        printf("</item>\n");
    }
    else if(t.compLex == R_CORCHETE){
        match(R_CORCHETE);
    }

}

//element-list.trad -> element.trad element-listX.trad
void element_list_trad(){
    if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
        element_trad();
        element_listg_trad();

    }
}

//element-listX.trad -> </item> <item> element.trad element-listX.trad | ε
void element_listg_trad(){

    if(t.compLex == COMA){
        match(COMA);
	imp_espacios();
        printf("</item>\n");
	imp_espacios();
        printf("<item>\n");
	cant_esp+=1;
        element_trad();
        element_listg_trad();
    }

}

//object.trad -> objectX.trad
void object_trad(){
    if(t.compLex == L_LLAVE){
        match(L_LLAVE);
        objectg_trad();
    }

}

//objectX.trad -> attribute-list.trad | ε
void objectg_trad(){
    if(t.compLex == STRING){
        attributes_list_trad();
        match(R_LLAVE);

    }
    else if(t.compLex == R_LLAVE){
        match(R_LLAVE);
    }
}

//attributes-list.trad -> attribute.trad
void attributes_list_trad(){
    if(t.compLex == STRING){
        attribute_trad();
        attributes_listg_trad();
    }
}

//atributes-listX.trad ->attribute.trad attributes-listX.trad | ε
void attributes_listg_trad(){
    if (t.compLex == R_LLAVE){
        cant_esp-=1;
    }
    if(t.compLex == COMA){
        match(COMA);
        attribute_trad();
        attributes_listg_trad();
    }
}

//attribute.trad -> <attribute-name.trad> attribute-value.trad </atribute-name.trad>
void attribute_trad(){
    if(t.compLex == STRING){
	imp_espacios();
        char lexema[TAMLEX];
        strcpy(lexema,t.lexema);
        printf("<");
        attribute_name_trad();
        printf("> ");
        match(DOS_PUNTOS);
        attribute_value_trad();
        printf("  </");
        string_sin_comillas(lexema);
        printf(">\n");
    }
}

//attribute-name.trad -> string.trad_sin_comilla
void attribute_name_trad(){
    if(t.compLex == STRING){
        string_sin_comillas(t.lexema);
        match(STRING);
    }
}

//attribute-value.trad -> element_trad | string | number | true | false | null

void attribute_value_trad(){

    if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
	cant_esp += 1;
        element_trad();
    }
    else if(t.compLex == STRING){
        printf("%s",t.lexema);
        match(STRING);
    }
    else if(t.compLex == NUMBER){
        printf("%s",t.lexema);
        match(NUMBER);
    }
    else if(t.compLex == PR_TRUE){
        printf("%s",t.lexema);
        match(PR_TRUE);
    }
    else if(t.compLex == PR_FALSE){
        printf("%s",t.lexema);
        match(PR_FALSE);
    }
    else if(t.compLex == PR_NULL){
        printf("%s",t.lexema);
        match(PR_NULL);
    }
}

void string_sin_comillas(char* cadena){
    char string[TAMLEX];
    strcpy(string,cadena);
    int i=1;
    while(string[i] != '"'){
        printf("%c",string[i]);
        i++;
    }
}

void imp_espacios(){
    int i;
    for (i=1;i<=cant_esp;i++){
        printf(" ");
    }
}

int main (int argc,char* args[]){
    if(argc > 1)
    {
        if (!(archivo=fopen(args[1],"rt")))
        {
            printf("Archivo no encontrado.\n");
            exit(1);
        }
        sigLex();
        json();
        if(acepto){
            fclose(archivo);
            archivo=fopen(args[1],"rt");
            sigLex();
            json_trad();
        }
    }else{
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }

    return 0;
}


