#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


/* Enums pentru asignarea atomilor cat si pentru afisarea acestora */
enum { REAL, INT, STR, COMMA, COLON, SEMICOLON, LPAR, RPAR, FINISH, ADD, SUB, MUL, DIV, AND, OR, NOTEQ, NOT, EQUAL, ASSIGN, LESS, ID, VAR, FUNCTION, IF, ELSE, WHILE, END, RETURN, TYPE_INT, TYPE_REAL, TYPE_STR };

char* atomsSTR[] = { "REAL", "INT", "STR", "COMMA", "COLON", "SEMICOLON", "LPAR", "RPAR", "FINISH", "ADD", "SUB", "MUL", "DIV", "AND", "OR", "NOTEQ", "NOT", "EQUAL", "ASSIGN","LESS", "ID", "VAR", "FUNCTION", "IF", "ELSE", "WHILE", "END", "RETURN", "TYPE_INT", "TYPE_REAL", "TYPE_STR" };


char buffer_in[30001]; /* Buffer pentru adaugarea atomilor*/
char* cursor; /* Cursor pentru parcurgerea atomilor */
int linie = 1; /* Variabila pentru linia curenta */

typedef struct {
    int cod, linie;
    union {
        char text[100];
        int INTEGER;
        double REAL;
    };
}Atom;

Atom atoms[10000]; /* Vector de atomi */
int atomCount; /* Numarul de atomi */
int idxCrtAtom = 0; /* Atomul curent => pentru analiza sintactica */

/* Functie de adaugare a atomilor in vector in analiza lexicala */
void addToken(int code) {
    atoms[atomCount].cod = code;
    atoms[atomCount].linie = linie;
    atomCount++;
}

/* Functie de consumare a atomului curent in analiza sintactica */
int consume(int code) {
    if (atoms[idxCrtAtom].cod == code) {
        idxCrtAtom++;
        return 1;
    }
    return 0;
}

/* Declaratii functii pentru analiza sintactica */
int program();
int defVar();
int baseType();
int defFunc();
int block();
int funcParams();
int funcParam();
int instr();
int expr();
int exprLogic();
int exprAssign();
int exprComp();
int exprAdd();
int exprMul();
int exprPrefix();
int factor();


/* Functie de identificarea atomilor in analiza lexicala */
int getNextToken() {
    int state = 0, n = 0;
    char buffer_text[100];
    for (;;) {
        char ch = *cursor;
        switch (state) {
        case 0:
            if (isdigit(ch)) {
                state = 1;
                cursor++;
                buffer_text[n++] = ch;
            }
            else if (ch == '"') {
                state = 6;
                cursor++;
            }
            else if (ch == ',') {
                state = 8;
                cursor++;
            }
            else if (ch == ':') {
                state = 9;
                cursor++;
            }
            else if (ch == ';') {
                state = 10;
                cursor++;
            }
            else if (ch == '(') {
                state = 11;
                cursor++;
            }
            else if (ch == ')') {
                state = 12;
                cursor++;
            }
            else if (ch == '\0' || ch == EOF) {
                state = 13;
                cursor++;
            }
            else if (ch == '+') {
                state = 14;
                cursor++;
            }
            else if (ch == '-') {
                state = 15;
                cursor++;
            }
            else if (ch == '*') {
                state = 16;
                cursor++;
            }
            else if (ch == '/') {
                state = 17;
                cursor++;
            }
            else if (ch == '&') {
                state = 18;
                cursor++;
            }
            else if (ch == '|') {
                state = 20;
                cursor++;
            }
            else if (ch == '!') {
                state = 22;
                cursor++;
            }
            else if (ch == '=') {
                state = 25;
                cursor++;
            }
            else if (ch == '<') {
                state = 28;
                cursor++;
            }
            else if (ch == ' ' || ch == '\r' || ch == '\t') {
                cursor++;
                state = 0;
            }
            else if (ch == '\n') {
                linie++;
                cursor++;
                state = 0;
            }
            else if (ch == '#') {
                state = 30;
                cursor++;
            }
            else if (isalpha(ch) || ch == '_') {
                state = 31;
                cursor++;
                buffer_text[n++] = ch;
            }
            break;
        case 1:
            if (isdigit(ch)) {
                cursor++;
                buffer_text[n++] = ch;
            }
            else if (ch == '.') {
                cursor++;
                state = 2;
                buffer_text[n++] = ch;
            }
            else {
                state = 5;
            }
            break;
        case 2:
            if (isdigit(ch)) {
                state = 3;
                cursor++;
                buffer_text[n++] = ch;
            }
            break;
        case 3:
            if (isdigit(ch)) {
                cursor++;
                buffer_text[n++] = ch;
            }
            else {
                state = 4;
            }
            break;
        case 4:
            buffer_text[n] = '\0';
            atoms[atomCount].REAL = atof(buffer_text);
            addToken(REAL);
            return REAL;
        case 5:
            buffer_text[n] = '\0';
            atoms[atomCount].INTEGER = atoi(buffer_text);
            addToken(INT);
            return INT;   
        case 6:
            if (ch != '"') {
                cursor++;
                buffer_text[n++] = ch;
            }
            else if (ch == '"') {
                state = 7;
                cursor++;
            }
            break;
        case 7:
            buffer_text[n] = '\0';
            strcpy(atoms[atomCount].text, buffer_text);
            n = 0;
            addToken(STR);
            return STR;
        case 8:
            addToken(COMMA);
            return COMMA;
        case 9:
            addToken(COLON);
            return COLON;
        case 10:
            addToken(SEMICOLON);
            return SEMICOLON;
        case 11:
            addToken(LPAR);
            return LPAR;
        case 12:
            addToken(RPAR);
            return RPAR;
        case 13:
            addToken(FINISH);
            return FINISH;
        case 14:
            addToken(ADD);
            return ADD;
        case 15:
            addToken(SUB);
            return SUB;
        case 16:
            addToken(MUL);
            return MUL;
        case 17:
            addToken(DIV);
            return DIV;
        case 18:
            if (ch == '&') {
                state = 19;
                cursor++;
            }
            break;
        case 19:
            addToken(AND);
            return AND;
        case 20:
            if (ch == '|') {
                state = 21;
                cursor++;
            }
            break;
        case 21:
            addToken(OR);
            return OR;
        case 22:
            if (ch == '=') {
                state = 24;
                cursor++;
            }
            else {
                state = 23;
            }
            break;
        case 23:
            addToken(NOT);
            return NOT;
        case 24:
            addToken(NOTEQ);
            return NOTEQ;
        case 25:
            if (ch == '=') {
                state = 27;
                cursor++;
            }
            else {
                state = 26;
            }
            break;
        case 26:
            addToken(ASSIGN);
            return ASSIGN;
        case 27:
            addToken(EQUAL);
            return EQUAL;
        case 28:
            addToken(LESS);
            return LESS;
        case 30:
            if (ch != '\n' && ch != '\r' && ch != '\0') {
                cursor++;
            }
            else {
                state = 0;
            }
            break;
        case 31:
            if (isalnum(ch) || ch == '_') {
                cursor++;
                buffer_text[n++] = ch;
            }
            else {
                state = 32;
            }
            break;
        case 32:
            buffer_text[n] = '\0';
            if (strcmp(buffer_text, "var") == 0) {
                addToken(VAR);
                return VAR;
            }
            else if (strcmp(buffer_text, "function") == 0) {
                addToken(FUNCTION);
                return FUNCTION;
            }
            else if (strcmp(buffer_text, "if") == 0) {
                addToken(IF);
                return IF;
            }
            else if (strcmp(buffer_text, "else") == 0) {
                addToken(ELSE);
                return ELSE;
            }
            else if (strcmp(buffer_text, "while") == 0) {
                addToken(WHILE);
                return WHILE;
            }
            else if (strcmp(buffer_text, "end") == 0) {
                addToken(END);
                return END;
            }
            else if (strcmp(buffer_text, "return") == 0) {
                addToken(RETURN);
                return RETURN;
            }
            else if (strcmp(buffer_text, "int") == 0) {
                addToken(TYPE_INT);
                return TYPE_INT;
            }
            else if (strcmp(buffer_text, "real") == 0) {
                addToken(TYPE_REAL);
                return TYPE_REAL;
            }
            else if (strcmp(buffer_text, "str") == 0) {
                addToken(TYPE_STR);
                return TYPE_STR;
            }
            else {
                addToken(ID);
                strcpy(atoms[atomCount - 1].text, buffer_text);
            }
            return ID;
        default:
            printf("stare invalida: %d\n", state);
        }
    }
}

/* Functie de afisare a erori in analiza sintactica */
void atomError(char* message, int linie) {
    printf("\n%s, linia %d\n", message, atoms[idxCrtAtom].linie);
    exit(1);
}


/* program ::= ( defVar | defFunc | block )* FINISH */
int program() {
    /* printf("%d program: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    for (;;) {
        if (defVar()) {
            /* ... */
        }
        else if (defFunc()) {
            /* ... */
        }
        else if (block()) {
            /* ... */
        }
        else {
            break;
        }
    }
    if (consume(FINISH)) {
        return 1;
    }
    return 0;
}


/* defVar ::= VAR ID COLON baseType SEMICOLON */
int defVar() {
    /* printf("%d defVar: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (consume(VAR)) {
        if (consume(ID)) {
            if (consume(COLON)) {
                if (baseType()) {
                    if (consume(SEMICOLON)) {
                        return 1;
                    }
                    else atomError("EROARE: Lipseste \";\" in declararea variabilei", atoms[idxCrtAtom].linie);
                }
                else atomError("EROARE: Lipseste tipul variabilei in declararea acesteia", atoms[idxCrtAtom].linie);
            }
            else atomError("EROARE: Lipseste \":\" in declararea variabilei", atoms[idxCrtAtom].linie);
        }
        else atomError("EROARE: Lipseste numele variabilei in declararea acesteia", atoms[idxCrtAtom].linie);
    }
    return 0;
}


/* baseType ::= TYPE_INT | TYPE_REAL | TYPE_STR */
int baseType() {
    /* printf("%d baseType: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (consume(TYPE_INT)) {
        return 1;
    }
    else if (consume(TYPE_REAL)) {
        return 1;
    }
    else if (consume(TYPE_STR)) {
        return 1;
    }
    return 0;
}


/* defFunc ::= FUNCTION ID LPAR funcParams RPAR COLON baseType defVar* block END */
int defFunc() {
    /* printf("%d defFunc: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (consume(FUNCTION)) {
        if (consume(ID)) {
            if (consume(LPAR)) {
                if (funcParams()) {
                    if (consume(RPAR)) {
                        if (consume(COLON)) {
                            if (baseType()) {
                                for (;;) {
                                    if (defVar()) {
                                        /* ... */
                                    }
                                    else {
                                        break;
                                    }
                                }
                                if (block()) {
                                    if (consume(END)) {
                                        return 1;
                                    }
                                    else atomError("EROARE: Lipseste keyword-ul \"end\" in definirea functiei", atoms[idxCrtAtom].linie);
                                }
                            }
                            else atomError("EROARE: Lipseste tipul variabilei in declararea acesteia, in definirea functiei", atoms[idxCrtAtom].linie);
                        }
                        else atomError("EROARE: Lipseste \":\" in declararea variabilei, in definirea functiei", atoms[idxCrtAtom].linie);
                    }
                    else atomError("EROARE: Lipseste \")\", in definirea functiei", atoms[idxCrtAtom].linie);
                }
                else atomError("EROARE: Lipsesc parametrii functiei in definirea acesteia", atoms[idxCrtAtom].linie);
            }
            else atomError("EROAE: Lipseste \"(\", in definirea functiei", atoms[idxCrtAtom].linie);
        }
        else atomError("EROARE: Lipseste numele functiei in definirea acesteia", atoms[idxCrtAtom].linie);
    }
    return 0;
}

/* block ::= instr+ */
int block() {
    /* printf("%d block: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (instr()) {
        for (;;) {
            if (instr()) {
                /* ... */
            }
            else break;
        }
        return 1;
    }
    return 0;
}

/* funcParams ::= ( funcParam ( COMMA funcParam )* )? */
int funcParams() {
    /* printf("%d funcParams: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (funcParam()) {
        for (;;) {
            if (consume(COMMA)) {
                if (funcParam()) {
                    return 1;
                }
            }
            else {
                break;
            }
        }
        return 1;
    }
    return 1;
}

/* funcParam ::= ID COLON baseType */
int funcParam() {
    /* printf("%d funcParam: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (consume(ID)) {
        if (consume(COLON)) {
            if (baseType()) {
                return 1;
            }
            else atomError("EROARE: Lipseste tipul variabilei in declararea acesteia, in parametrii functiei", atoms[idxCrtAtom].linie);
        }
        else atomError("EROARE: Lipseste \":\" in declararea variabilei, in parametrii functiei", atoms[idxCrtAtom].linie);
    }
    return 0;
}

/*
instr ::= expr? SEMICOLON
                        | IF LPAR expr RPAR block ( ELSE block )? END
                        | RETURN expr SEMICOLON
                        | WHILE LPAR expr RPAR block END
*/
int instr() {
    /* printf("%d instr: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (expr()) {
        /* ... */
    }
    if (consume(SEMICOLON)) {
        return 1;
    } 
    else if (consume(IF)) {
        if (consume(LPAR)) {
            if (expr()) {
                if (consume(RPAR)) {
                    if (block()) {
                        if (consume(ELSE)) {
                            if (block()) { /* ... */ }
                            else atomError("EROARE: Lipsesc instructiunile dupa keyword-ul ELSE", atoms[idxCrtAtom].linie);
                        }
                        if (consume(END)) {
                            return 1;
                        }
                        else atomError("EROARE: Lipseste keyword-ul \"end\" din blocul IF", atoms[idxCrtAtom].linie);
                    }
                    else atomError("EROARE: Lipsesc instructiunile dupa keyword-ul ELSE", atoms[idxCrtAtom].linie);
                }
                else atomError("EROARE: Lipseste \")\" dupa keyword-ul IF", atoms[idxCrtAtom].linie);;
            }
            else atomError("EROARE: Lipseste conditia din IF", atoms[idxCrtAtom].linie);
        }
        else atomError("EROAE: Lipseste \"(\" inainte de keyword-ul IF", atoms[idxCrtAtom].linie);
    }
    else if (consume(RETURN)) {
        if (expr()) {
            if (consume(SEMICOLON)) {
                return 1;
            }
            else atomError("EROARE: Lipseste \";\" dupa keyword-ul RETURN", atoms[idxCrtAtom].linie);
        }
        else atomError("EROARE: Lipseste instructiunea dupa keyword-ul RETURN", atoms[idxCrtAtom].linie);
    }
    else if (consume(WHILE)) {
        if (consume(LPAR)) {
            if (expr()) {
                if (consume(RPAR)) {
                    if (block()) {
                        if (consume(END)) {
                            return 1;
                        }
                        else atomError("EROARE: Lipseste keyword-ul \"end\" din blocul WHILE", atoms[idxCrtAtom].linie);
                    }
                    else atomError("EROARE: Eroare in blocul while", atoms[idxCrtAtom].linie);
                }
                else atomError("EROARE: Lipseste \")\" dupa keyword-ul WHILE", atoms[idxCrtAtom].linie);
            }
            else atomError("EROARE: Lipseste expresia din blocul WHILE", atoms[idxCrtAtom].linie);
        }
        else atomError("EROARE: Lipseste \")\" inainte de keyword-ul WHILE", atoms[idxCrtAtom].linie);
    }
    return 0;
}


/* expr ::= exprLogic */
int expr() {
    /* printf("%d exprLogic: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    return exprLogic();
}


/* exprLogic ::= exprAssign ( ( AND | OR ) exprAssign )* */
int exprLogic() {
    /* printf("%d exprLogic: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (exprAssign()) {
        for (;;) {
            if (consume(AND)) {
                if (exprAssign()) {
                    return 1;
                }
                else atomError("EROARE: Lipseste asignarea variabilei", atoms[idxCrtAtom].linie);
            }
            else if (consume(OR)) {
                if (exprAssign()) {
                    return 1;
                }
                else atomError("EROARE: Lipseste asignarea variabilei", atoms[idxCrtAtom].linie);
            }
            else {
                break;
            }
        }
        return 1;
    }
    return 0;
}


/* exprAssign ::= ( ID ASSIGN )? exprComp */
int exprAssign() {
    /* printf("%d exprAssign: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    int startIDx = idxCrtAtom;
    if (consume(ID)) {
        if (consume(ASSIGN)) {
            if (exprComp()) {
                return 1;
            }
            else atomError("EROARE: Lipseste expresie dupa EGAL", atoms[idxCrtAtom].linie);
        } 
        idxCrtAtom = startIDx;
    }
    if (exprComp()) {
        return 1;
    }
    return 0;
}


/* exprComp ::= exprAdd ( ( LESS | EQUAL ) exprAdd )? */
int exprComp() {
    /* printf("%d exprComp: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (exprAdd()) {
        if (consume(LESS)) {
            if (exprAdd()) {
                return 1;
            }
            else atomError("EROARE: Expresie invalida dupa <", atoms[idxCrtAtom].linie);
        }
        else if (consume(EQUAL)) {
            if (exprAdd()) {
                return 1;
            }
            else atomError("EROARE: Expresie din conditie nerecunoscuta", atoms[idxCrtAtom].linie);
        }
        return 1;
    }
    return 0;
}

/* exprAdd ::= exprMul ( ( ADD | SUB ) exprMul )* */
int exprAdd() {
    /* printf("%d exprAdd: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (exprMul()) {
        if (consume(ADD)) {
            if (exprMul()) {
                return 1;
            }
            else atomError("EROARE: Expresie matematica nerecunoscuta (+)", atoms[idxCrtAtom].linie);
        }
        else if (consume(SUB)) {
            if (exprMul()) {
                return 1;
            }
            else atomError("EROARE: Expresie matematica nerecunoscuta (-)", atoms[idxCrtAtom].linie);
        }
        return 1;
    }
    return 0;
}

/* exprMul ::= exprPrefix ( ( MUL | DIV ) exprPrefix )* */
int exprMul() {
    /* printf("%d exprMul: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (exprPrefix()) {
        if (consume(MUL)) {
            if (exprPrefix()) {
                return 1;
            }
            else atomError("EROARE: Expresie matematica nerecunoscuta (*)", atoms[idxCrtAtom].linie);
        }
        else if (consume(DIV)) {
            if (exprPrefix()) {
                return 1;
            }
            else atomError("EROARE: Expresie matematica nerecunoscuta (/)", atoms[idxCrtAtom].linie);
        }
        return 1;
    }
    return 0;
}

/* exprPrefix ::= ( SUB | NOT )? factor */
int exprPrefix() {
    /* printf("%d exprPrefix: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (consume(SUB) || consume(NOT)) {
        /* ... */
    }
    if (factor()) {
        return 1;
    }
    return 0;
}


/* factor ::= INT
                | REAL
                | STR
                | LPAR expr RPAR
                | ID ( LPAR ( expr ( COMMA expr )* )? RPAR )?
*/
int factor() {
    /* printf("%d factor: %s\n", atoms[idxCrtAtom].linie, atomsSTR[atoms[idxCrtAtom].cod]); */
    if (consume(INT)) {
        return 1;
    }
    if (consume(REAL)) {
        return 1;
    }
    if (consume(STR)) {
        return 1;
    }
    if (consume(LPAR)) {
        if (expr()) {
            if (consume(RPAR)) {
                return 1;
            }
            else atomError("EROARE: Lipseste \")\" in expresie", atoms[idxCrtAtom].linie);
        }
        else atomError("EROARE: Lipseste expresia", atoms[idxCrtAtom].linie);
    }
    else if (consume(ID)) {
        if (consume(LPAR)) {
            if (expr()) {
                for (;;) {
                    if (consume(COMMA)) {
                        if (expr()) {
                            /* ... */
                        }
                        else atomError("EROARE: Lipseste \",\" in expresie", atoms[idxCrtAtom].linie);
                    }
                    else {
                        break;
                    }
                }
            }
            if (consume(RPAR)) {
                return 1;
            }
            else atomError("EROARE: Lipseste \")\" in expresie", atoms[idxCrtAtom].linie);
        }
        return 1;
    }
    return 0;
}

/* Functie de afisarea atomilor in analiza lexicala */
void displayAtoms() {
    for (int i = 0; i < atomCount; i++) {
        printf("linie: %d, cod: %s", atoms[i].linie, atomsSTR[atoms[i].cod]);
        if (atoms[i].cod == ID) {
            printf(", %s\n", atoms[i].text);
        }
        else if (atoms[i].cod == INT) {
            printf(", %d\n", atoms[i].INTEGER);
        }
        else if (atoms[i].cod == REAL) {
            printf(", %lf\n", atoms[i].REAL);
        }
        else if (atoms[i].cod == STR) {
            printf(", %s\n", atoms[i].text);
        }
        else {
            printf("\n");
        }
    }
}

int main() {
    FILE* file;
    file = fopen("1.q", "rb");
    if (file == NULL) {
        perror("Nu s-a putut deschide fisierul;\n");
        return -1;
    }
    int n = fread(buffer_in, 1, 30000, file);
    buffer_in[n] = '\0';
    fclose(file);
    cursor = buffer_in;
    while (getNextToken() != FINISH) {
        /* ... */
    }
    displayAtoms();
    if (program()) {
        printf("Sintaxa OK");
    }
    else {
        printf("Eroare de sintaxa");
    }
    return 0;
}