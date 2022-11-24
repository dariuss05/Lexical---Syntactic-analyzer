#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum { REAL, INT, STR, COMMA, COLON, SEMICOLON, LPAR, RPAR, FINISH, ADD, SUB, MUL, DIV, AND, OR, NOTEQ, NOT, EQUAL, ASSIGN, LESS, ID, VAR, FUNCTION, IF, ELSE, WHILE, END, RETURN, TYPE_INT, TYPE_REAL, TYPE_STR };

char* atomsSTR[] = { "REAL", "INT", "STR", "COMMA", "COLON", "SEMICOLON", "LPAR", "RPAR", "FINISH", "ADD", "SUB", "MUL", "DIV", "AND", "OR", "NOTEQ", "NOT", "EQUAL", "ASSIGN","LESS", "ID", "VAR", "FUNCTION", "IF", "ELSE", "WHILE", "END", "RETURN", "TYPE_INT", "TYPE_REAL", "TYPE_STR" };

char buffer_in[30001];
char* cursor;
int linie = 1;

typedef struct {
    int cod, linie;
    union {
        char text[100];
        int INTEGER;
        double REAL;
    };
}Atom;

Atom atoms[10000];
int atomCount;

void addToken(int code) {
    atoms[atomCount].cod = code;
    atoms[atomCount].linie = linie;
    atomCount++;
}

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
        //
    }
    displayAtoms();

}



