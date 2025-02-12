#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/* Write to stdout a filtered input from stdin that removes
 lines that are commented out, and preserves string literals
and char literals */
/* defines constants representing each state in the DFA */

enum Statetype {Regular_Text, Begin_Comment,
In_Comment, Exit_Comment, Begin_Char, Ord_Char,
Begin_Str, Ord_Str};
/* Implement the Regular Text state of DFA. c is the current DFA 
character. If c is a regular character, prints and stays 
in Regular Text state. If c is a /, moves to 
Begin_Comment state. If ', moves state to Begin Char state
If ", moves to Begin String state. Returns current 
state at the end. "*/
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
/* Implement the Begin_Comment state of the DFA. 
c is the current DFA character
If c is a /, remains in this state.
 If c is *, it is a comment, prints 
space, and goes into the In_Comment state. 
If it is a ', put / and 
goes to Begin_Char state. Everything else, 
prints / and c, and goes back
to Regular_Text state. Returns current state at end. */
enum Statetype handleBeginCommentState(int c) { 
    enum Statetype state; 

    if (c == '/') { 
        putchar('/'); 
        state = Begin_Comment;
    }
    else if (c == '*') { 
        putchar(' '); 
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
/* Implement the Begin_Char state of the DFA. 
c is the current DFA character. 
Put c to stdout everytime. If c is ', 
goes back to regular text. If c is a \, goes to 
the Ordinary Character state, and everything else remains 
in the Begin_Char state. Returns current state*/ 
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
/*Implement the Begin_String state of the DFA. 
c is the current DFA character. Puts c to stdout. If c is a ", 
goes to Regular_Text state. If c is a \, 
goes to Ordinary_Str state.  Everything else goes 
to Begin_Str state. Returns current state*/
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
/*Implement the In_Comment state of the DFA. 
c is the current DFA character. If c is a *, 
moves to Exit_Comment State. 
If c is a newline character, put c to stdout and move to 
In_Comment state. Everything else, remain in In_Comment state
Return current state. */
enum Statetype handleInCommentState(int c) {
    enum Statetype state;
    if (c == '*') {
        state = Exit_Comment; 
    }
    else if (c == '\n') {
        putchar(c); 
        state = In_Comment; 
    }
    else {
        state = In_Comment; 
    }
    return state; 
}
/*Implement the Exit_State state of the DFA. 
c is the current DFA character. If c is a *, 
we stay in this state.
if c is a /, we move to Regular_Text state.
If c is a newline character, 
we put c to stdout and move to the In_Comment
state. Else, go to 
In_Comment state. Return current state. */
enum Statetype handleExitCommentState(int c) { 

    enum Statetype state;
    if (c == '*') {
        state = Exit_Comment;
    }
    else if (c == '/') {
        state = Regular_Text; 
    }
    else if (c == '\n') { 
        putchar(c); 
        state = In_Comment; 
    }
    else { 
        state = In_Comment; 
    }

    return state; 
}
/*Implement the Ordinary Char state of the DFA. 
c is the current DFA character. Puts c to stdout, and moves
to Begin_Char State*/
enum Statetype handleOrdinaryCharState(int c) { 
    enum Statetype state; 
    putchar(c); 
    state = Begin_Char; 
    return state; 
}
/*Implement the Ordinary Str state of the DFA. 
c is the current DFA character. Puts c to stdout, and moves
to Begin_Str State*/
enum Statetype handleOrdinaryStrState(int c) { 
    enum Statetype state; 
    putchar(c); 
    state = Begin_Str; 
    return state; 
}
/* Read text from stdin. If a comment is detected, remove that 
comment from the text and write the result to stdout. If comment
is in a string or character literal, don't remove comment. If 
there is a unterminated comment, throw EXIT_FAILURE and return
line of the the unterminated comment. Else, return EXIT_SUCCESS*/
int main(void) { 
    /* Use a DFA approach. State is the current dfa state,
    totallines and lineError start at beginning, and determine 
    the line in which an error occurs. */
    int c; 
    int totalLines = 1; 
    int lineError = 1; 

    enum Statetype state = Regular_Text; 
    while ((c = getchar()) != EOF) { 
        if (c == '\n'){
            totalLines++; 
        } 
        switch (state) { 
            case Regular_Text:
                lineError  = totalLines; 
                state = handleRegularTextState(c); 
                break; 
            case Begin_Char:
                lineError = totalLines; 
                state = handleBeginCharState(c); 
                break; 
            case Begin_Comment:
                lineError = totalLines; 
                state = handleBeginCommentState(c);
                break; 
            case Begin_Str:
                lineError = totalLines; 
                state = handleBeginStringState(c); 
                break; 
            case In_Comment:
                state = handleInCommentState(c); 
                break; 
            case Exit_Comment:
                state = handleExitCommentState(c);
                break; 
            case Ord_Char:
                lineError = totalLines; 
                state = handleOrdinaryCharState(c); 
                break; 
            case Ord_Str:
                lineError = totalLines; 
                state = handleOrdinaryStrState(c); 
                break; 
        }
    }

    if (state == Begin_Comment) { 
        putchar('/'); 
    }


    if (state == Regular_Text || state == Begin_Comment|| state == Begin_Char || state == Ord_Char || state == Begin_Str || state == Ord_Str) {
        return EXIT_SUCCESS; 
    }

    else { 
        fprintf(stderr, "Error: line %d: unterminated comment\n", lineError); 
        return EXIT_FAILURE; 
    } 
}
