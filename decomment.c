#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/* Write to stdout a filtered input from stdin that removes lines that are
commented out, and preserves string literals and char literals */

enum Statetype {Regular_Text, Begin_Comment, In_Comment, Exit_Comment, Begin_Char, Ord_Char, Begin_Str, Ord_Str};

enum Statetype handleRegularTextState(int c) {
    enum Statetype state; 

    if (c == '/') 
        state = Begin_Comment;

    else if (c == '\'') {
        putchar(c); 
        state = Begin_Char;
    }
    else if (c == '"') {
        putchar(c); 
        state = Begin_Str;
    }
    else {
        putchar(c); 
        state = Regular_Text; 
    } 
    return state; 
}
enum Statetype handleBeginCommentState(int c) { 
    enum Statetype state; 
    if (c == '/') { 
        putchar('/'); 
        putchar(c); 
        state = Begin_Comment;
    }
    else if (c == '*') { 
        putchar(' '); 
        putchar(c); 
        state = In_Comment;
    }
    else if (c == '\'') { 
        putchar('/');
        putchar(c); 
        state = Begin_Char;
    }
    else if (c == '"') { 
        putchar('/'); 
        putchar(c); 
        state = Begin_Str;
    } 
    else {
        putchar('/');
        putchar(c); 
        state = Regular_Text; 
    }
    return state; 
} 
enum Statetype handleBeginCharState(int c) { 
    enum Statetype state; 
    putchar(c); 
    if (c == '\'') { 
        state = Regular_Text; 
    }
    else if (c == '\\') { 
        state = Ord_Char; 
    }
    else { 
        state = Begin_Char; 
    }
    return state; 
}
enum Statetype handleBeginStringState(int c) { 
    enum Statetype state; 
    putchar(c); 
    if (c == '"') { 
        state = Regular_Text; 
    }
    else if (c == '\\') { 
        state = Ord_Str; 
    }
    else {
        state = Begin_Str; 
    }
    return state; 
}
enum Statetype handleInCommentState(int c) {
    enum Statetype state;
    if (c == '*') {
        putchar('N');
        state = Exit_Comment; 
    }
    else if (c == '\n') {
        putchar(c); 
    }
    else {
        state = In_Comment; 
    }
    return state; 
}
enum Statetype handleExitCommentState(int c) { 

    enum Statetype state;
    if (c == '*') {
        state = Exit_Comment;
    }
    else if (c == '/') {
        putchar(' '); 
        state = Regular_Text; 
    }
    else { 
        state = In_Comment; 
    }

    return state; 
}
enum Statetype handleOrdinaryCharState(int c) { 
    enum Statetype state; 
    putchar(c); 
    if (c == '\\') { 
        state = Ord_Char; 
    }
    else {
        state = Begin_Char; 
    }
    return state; 
}
enum Statetype handleOrdinaryStrState(int c) { 
    enum Statetype state; 
    putchar(c); 
    if (c == '\\') {
        state = Ord_Str; 
    }
    else {
        state = Begin_Str; 
    }
    return state; 
}

int main(void) { 
    int c; 
    int totalLines = 1; 
    int lineError = 1; 
    int inCommentTimes = 0; 

    enum Statetype state = Regular_Text; 
    while ((c = getchar()) != EOF) { 
        if (c == '\n'){
            totalLines++; 
        } 
        switch (state) { 
            case Regular_Text:
                state = handleRegularTextState(c); 
                break; 
            case Begin_Char:
                state = handleBeginCharState(c); 
                break; 
            case Begin_Comment:
                state = handleBeginCommentState(c);
                break; 
            case Begin_Str:
                state = handleBeginStringState(c); 
                break; 
            case In_Comment:
                if (inCommentTimes == 0) { 
                    lineError = totalLines; 
                } 
                state = handleInCommentState(c); 
                inCommentTimes++; 
                break; 
            case Exit_Comment:
                state = handleExitCommentState(c);
                break; 
            case Ord_Char: 
                state = handleOrdinaryCharState(c); 
                break; 
            case Ord_Str:
                state = handleOrdinaryStrState(c); 
                break; 
        }
    }

    if (state == Regular_Text || state == Begin_Char || state == Ord_Char || state == Begin_Str || state == Ord_Str) {
        return EXIT_SUCCESS; 
    }

    else { 
        fprintf(stderr, "Error: line %d: unterminated comment\n", lineError); 
        return EXIT_FAILURE; 
    } 
}
