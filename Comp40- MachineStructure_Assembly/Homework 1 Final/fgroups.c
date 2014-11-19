/*      fgroups.c
 *      by Ian Cross and Alison Tai
 *      Homework 1: Understanding and Using Interfaces
 *
 *      When given a set of names with fingerprints, identifies groups of
 *      names that share a fingerprint. (From assignment)       
 */

#include <stdio.h>
#include <stdlib.h>
#include <table.h>
#include <set.h>
#include <atom.h>
#include <string.h>
#include <ctype.h>

const int FINGER_MAX = 513;
const int NAME_MAX = 500;
const int LINE_MAX = 1012;

/*Function Declarations*/
void input(Table_T*);
void insert_atom(Table_T*, char*, char*);
void print_table(Table_T*); 
void print_set(Set_T*);
char skip_line(char);
char record_fingerprint(char, char*);
char record_name(char, char*);
int num_fgroups(Table_T*);

/*Function Definitions*/
int main(int argc, char** argv)
{
        Table_T ftable = Table_new(1, NULL, NULL);  
        
        if (argc == 1) {
                input(&ftable);
                print_table(&ftable);
        } else {
                fprintf(stderr, "%s: %s\n",
                        argv[0], "Could not open file for reading");
                exit(1);
        }

        Table_free(&ftable);
}

/*
 * Takes the input line and creates 2 strings (fingerprint and name). 
 * This function also accounts for bad input cases.
 */
void input(Table_T* table)
{
        char fingerprint[FINGER_MAX];
        char name[NAME_MAX];
        char letter = getchar();
        
        while (letter != EOF) {
                /*Badly formed line*/
                if (isspace(letter) || letter == '\n') {
                        letter = skip_line(letter);
                        continue;
                }
                /*Create fingerprint*/
                letter = record_fingerprint(letter, fingerprint);       
                if (!isspace(letter)) {
                        letter = skip_line(letter);
                        continue;
                }
                /*Skip whitespace*/
                while (isspace(letter)) {
                        letter = getchar();
                }
                /*Create name*/
                letter = record_name(letter, name);     
                if (letter != '\n') {
                        letter = skip_line(letter);
                }
                letter = getchar();
                /*Make name and fingerprint atoms*/
                insert_atom(table, fingerprint, name);
        }
}
/*
 * creates the char array (string) called fingerprint and returns the next
 * letter in the input.
 */
char record_fingerprint(char letter, char* fingerprint)
{
        int index = 0;
        while (!isspace(letter) && index < FINGER_MAX -1) {
                fingerprint[index] = letter;
                letter = getchar();
                index++;
        }
        fingerprint[index] = '\0';
        return letter;
}

/*
 * creates the char array (string) called name and returns the next letter
 * in the input.
 */
char record_name(char letter, char* name)
{
        int index = 0;
        while (letter != '\n' && index < NAME_MAX-1) {
                name[index] = letter;
                letter = getchar();
                index++;
        }
        name[index] = '\0';
        return letter;
}       

/* 
 * Discards the current line, prints to stderr and then returns the first
 * letter of the next input line
 */
char skip_line(char letter)
{
        fprintf(stderr, "Line skipped or name truncated\n");
        while (letter != '\n') {
                letter = getchar();
        }
        letter = getchar();
        return letter;
}
/*Inserts the value into the table under the corresponding fingerprint*/
void insert_atom(Table_T* table, char* fingerprint, char* name)
{
        const char *key;
        const char *value;
        Set_T old_set;          

        /*Turns fingerprint and name strings into Hanson atoms*/
        key = Atom_string(fingerprint);
        value = Atom_string(name);

        old_set = Table_get(*table, key);
        if (old_set == NULL) {
                Set_T new_set = Set_new(1, NULL, NULL);
                Set_put(new_set, value);
                Table_put(*table, key, new_set);
        } else {
                Set_put(old_set, value);
        }
}
 
 /* prints the sets in the table that have more than 1 value*/
void print_table(Table_T* table)
{
        int table_len = Table_length(*table);
        void **table_array = Table_toArray(*table, NULL);

        int counter = num_fgroups(table);

        for (int i=1; i<(2*table_len); i += 2) {
                if(Set_length((Set_T)table_array[i]) > 1) {
                        print_set((Set_T *)&table_array[i]);
                        counter--;
                        if (counter != 0) {     
                                printf("\n");
                        }
                }
                Set_free((Set_T *)&table_array[i]);
        }
        free(table_array);
}

/*prints all values contained in a set*/
void print_set(Set_T* set)
{
        int set_len = Set_length(*set);
        void **set_array = Set_toArray(*set, NULL);

        for (int i=0; i<set_len; i++) {
                printf("%s\n", (char *)set_array[i]);
        }
        free(set_array);
}

/*finds and returns the number of sets with more than one value*/
int num_fgroups(Table_T* table)
{
        int table_len = Table_length(*table);
        void **table_array = Table_toArray(*table, NULL);
        int counter = 0;

        for (int i=1; i<(2*table_len); i += 2) {
                if (Set_length((Set_T)table_array[i]) > 1) {
                        counter++;
                }
        }
        free(table_array);
        return counter;
}

