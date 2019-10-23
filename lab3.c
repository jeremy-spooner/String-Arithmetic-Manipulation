/* lab3.c
 * Jeremy Spooner
 * ECE 222, Fall 2019
 * MP3
 * Subject: ECE222-1,#3
 *
 * Purpose:
 *  The purpose is to learn to program with strings.
 *  More specifically, there are two parts:
 *  1) Process an input string to verify the input is and set a binary command
 *     operator and corresponding string values
 *  2) Process the operand strings to predefined arithmetic systems on a finite
 *     field and print the output
 *
 * Assumptions:
 *  #1   The user is prompted to enter a pseudo arithmetic command.  The
 *       input must be verified to be grammatically correct.
 *
 *  #2:  The string and character type library cannot be used under
 *       any circumstances.  You are encouraged to develop your own
 *       functions to perform any similar operations that are needed.
 *
 *  #3   No changes to the code in main.  Your code must be placed in
 *       functions.  Additional functions are encouraged.
 *
 * Bugs:
 *  There are no known bugs in the program
 *
 */

// do not include any additional libraries
#include <stdio.h>

// do not change these constants
#define MAXLINE 80
#define MAXOPER 13

// named constants and strings
enum operations {NOOP, ADD, MUL, DIV, POW};
const char *operation_str[] = {"Invalid", "+", "*", "/", "^"};

// function prototypes
int process_input(const char *input, char *op_left, char *op_right);
void calc_output(const char *op_l, int op, const char *op_r, char *result);

// do not change any code in main.  We are grading based on the format
// of the output as given in the printfs in main.
int main()
{
    char input_line[MAXLINE];
    char left_operand[MAXOPER];
    char right_operand[MAXOPER];
    char answer[MAXOPER];
    int  operator;

    printf("\nMP3: Arithmetic on GF(47) with + * / ^ using letters\n");
    printf("Commands:\n\tabc+bbc\n\tturtle/frog\n\ttiger^one");
    printf("\tOperands are no more than 12 letters and no spaces\n");
    printf("\tCtrl-d to quit\n\n");
    printf("> ");

    // each call to fgets collects one line of input and stores in input_line
    // BEWARE: fgets includes the end-of-line character '\n' in input_line
    while (fgets(input_line, sizeof input_line, stdin) != NULL) {
        // clear for next round
        left_operand[0] = right_operand[0] = answer[0] = '\0';
        // check for valid grammar
        operator = process_input(input_line, left_operand, right_operand);

        if (operator == ADD || operator == MUL
                || operator == DIV || operator == POW) {

            // print parsed input
            printf("'%s'", left_operand);
            printf(" %s ", operation_str[operator]);
            printf("'%s' => ", right_operand);

            // perform pseudo arithmetic
            calc_output(left_operand, operator, right_operand, answer);

            // print result
            printf("'%s'\n\n", answer);
        } else {
            printf("# %s", input_line);
        }
        printf("> ");
    }
    printf("\nGoodbye\n");



    return 0;
}

/* Parse input of the form SOS where S is a string and O is a character.
 *
 * A string S must consist of up to 12 valid symbols a-z and A-U.
 * The operand O must be one character from: + * / ^
 * Any other characters found in the input, including spaces, are
 * grammatically incorrect and invalidate the input.
 *
 * There must be no spaces anywhere in the input, including between
 * either SO, OS, or leading or trailing spaces.
 *
 * Input: The input string is collected using fgets.  Recall the end-of-line
 *        character is included in the input string and marks the end of
 *        the input.  This string must not be changed.
 *
 * Output: There are three outputs from this function.
 *
 *   The return value is one of NOOP, ADD, MUL, DIV, POW which are
 *      named constants.  If the input is invalid for any reason
 *      then the output must be NOOP.  Otherwise the return value
 *      corresponds to operand O.
 *
 *   If the input is grammatically correct, then two strings are also
 *   returned, one for each of the left and right operands.  If the input
 *   in invalid the two output strings are undefined.
 */
int process_input(const char *input, char *op_left, char *op_right)
{
    //initilize variables
    char holder[MAXLINE];
    int i=0;
    int operator=0;
    int op_index=0;
    int numoperator=0;
    int max=0;
    int max_val=0;
    int c=0;
    //sets all characters in char arrays to NULL
    for(c=0; c<=MAXOPER; c++) {
        op_left[c]=0;
        op_right[c]=0;
    }

    //loops through while loop until end of input is reached and
    //reads in operator and finds size of input array
    int fact=0;
    while(fact==0) {
        holder[i]=input[i];
        if (input[i]=='+') {
            operator=1;
            numoperator++;
            op_index=i;
        }
        if (input[i]=='*') {
            operator=2;
            numoperator++;
            op_index=i;
        }
        if (input[i]=='/') {
            operator=3;
            numoperator++;
            op_index=i;
        }
        if (input[i]=='^') {
            operator=4;
            numoperator++;
            op_index=i;
        }
        if(!(((input[i]>='a')&&(input[i]<='z'))||((input[i]>='A')&&(input[i]<='U')))&&(max<2)) {
            max++;
            if(max==2) {
                max_val=i;
                fact=1;
            }
        }
        i++;
    }

    int j;
    //sets op op_left char array
    for(j=0; j<op_index; j++) {
        op_left[j]=holder[j];
    }
    op_left[op_index]=0;

    //clears left and right values and returns error opperator is invalid
    //inputs are inputted by the user
    if((op_index>MAXOPER)||op_index<1||((max_val-op_index)>MAXOPER)) {
        op_left[0]=0;
        op_right[0]=0;
        return NOOP;
    }
    //sets op_right
    for(j=op_index+1; j<max_val; j++) {
        op_right[j-op_index-1]=holder[j];
    }
    op_right[max_val-op_index-1]=0;
    //returns corresponding operator value
    if(operator==1) {
        return ADD;
    }
    if(operator==2) {
        return MUL;
    }
    if(operator==3) {
        return DIV;
    }
    if(operator==4) {
        return POW;
    }
    return NOOP;
}

/* Pseudo mathematical opertions on the two operands work as follows.
 *
 * Each character is converted to an integer in the range 1...46, where a is 0,
 * b is 1, c is 2, ..., z is 25.  The operation is then performed using
 * math on a finite field with no carries.
 *
 * If the two input strings are not the same length, then each output character
 * beyond the length of the shorter string should be a copy of the character
 * from the longer string but with the opposite case.
 *
 * Input: The two operand strings and the operator are assumed to be valid (and
 *        are verified as valid in the parse_input function).
 *
 * Output: The final string generated by the above rules is stored in the
 *         output string named result.  The input strings must not be
 *         changed.
 */
void calc_output(const char *l_op, int op, const char *r_op, char *result)
{
    //initiizes variables
    int c=0;
    int l_len = 0;
    int r_len = 0;
    int maxl=0;
    int maxr=0;
    char temp_l[MAXOPER-1];
    char temp_r[MAXOPER-1];


    //sets all the values according the given output generation table
    for(c=0; c<=MAXOPER; c++) {
        if(!(((r_op[c]>='a')&&(r_op[c]<='z'))||((r_op[c]>='A')&&(r_op[c]<='U')))&&(maxr<2)) {
            maxr++;
            r_len=c-1;
        }
        if(!(((l_op[c]>='a')&&(l_op[c]<='z'))||((l_op[c]>='A')&&(l_op[c]<='U')))&&(maxl<2)) {
            maxl++;
            l_len=c-1;
        }
        if((l_op[c]>='a')&&(l_op[c]<='z')) {
            temp_l[c]=l_op[c]-97;
        }
        if((l_op[c]>='A')&&(l_op[c]<='U')) {
            temp_l[c]=l_op[c]-39;
        }
        if((r_op[c]>='a')&&(r_op[c]<='z')) {
            temp_r[c]=r_op[c]-97;
        }
        if((r_op[c]>='A')&&(r_op[c]<='U')) {
            temp_r[c]=r_op[c]-39;
        }
    }

    //calculates the sum on a finite field by looping until both of the char
    //arrays have been looped through. The values are determined through the
    //equation (left side)+(right side)
    int count=0;
    if(op==1) {
        while((count<l_len)||(count<r_len)) {
            if(count<l_len) {
                if(count<r_len) {
                    result[count]=(temp_r[count]+temp_l[count])%47 ;
                } else {
                    if(temp_l[count]<26) {
                        result[count]=(temp_l[count]+26);
                    } else {
                        result[count]=(temp_l[count]-26)%47;
                    }
                }
            } else {
                if(temp_r[count]<26) {
                    result[count]=(temp_r[count]+26);
                } else {
                    result[count]=(temp_r[count]-26)%47;
                }
            }
            count=count+1;
        }
    }
    //calculates the product on a finite field by looping until both of the char
    //arrays have been looped through. The values are determined through the
    //equation (left side)*(right side)%47
    count=0;
    if(op==2) {
        while((count<l_len)||(count<r_len)) {
            if(count<l_len) {
                if(count<r_len) {
                    result[count]=(temp_r[count]*temp_l[count])%47 ;
                } else {
                    if(temp_l[count]<26) {
                        result[count]=(temp_l[count]+26);
                    } else {
                        result[count]=(temp_l[count]-26)%47;
                    }
                }
            } else {
                if(temp_r[count]<26) {
                    result[count]=(temp_r[count]+26);
                } else {
                    result[count]=(temp_r[count]-26)%47;
                }
            }
            count=count+1;
        }
    }
    //calculates the inversion on a finite field by looping until both of the char
    //arrays have been looped through. If either the base or exponent is set
    //to char value 'a' (zero) then the correspondong output char is 'a'.
    int i;
    int temp=0;
    int r=0;
    count=0;
    if(op==3) {
        while((count<l_len)||(count<r_len)) {
            if(count<l_len) {
                if(count<r_len) {
                    if((((temp_r[count])==0)||(temp_l[count])==0)) {
                        result[count]=0;
                    } else {
                        temp=0;
                        r=0;
                        while(temp!=1) {
                            r=r+1;
                            if((temp_l[count]==((r*temp_r[count])%47))) {
                                result[count]=r;
                                temp=1;
                            }
                        }
                    }
                } else {
                    if(temp_l[count]<26) {
                        result[count]=(temp_l[count]+26);
                    } else {
                        result[count]=(temp_l[count]-26)%47;
                    }
                }
            } else {
                if(temp_r[count]<26) {
                    result[count]=(temp_r[count]+26);
                } else {
                    result[count]=(temp_r[count]-26)%47;
                }
            }
            count=count+1;
        }
    }

    count=0;

//calculates th power on a finite field by looping until both of the char
//arrays have been looped through. It uses the equation the summation from 1
//to the given value of char on the right side of the operator % 47
    if(op==4) {
        while((count<l_len)||(count<r_len)) {
            if(count<l_len) {
                if(count<r_len) {
                    if((temp_r[count])==0) {
                        result[count]=1;
                    }
                    temp=1;
                    if((temp_r[count])!=0) {
                        for(i=0; i<temp_r[count]; i++) {
                            temp = (temp * temp_l[count])%47;
                        }
                        result[count]=temp%47;
                    }
                } else {
                    if(temp_l[count]<26) {
                        result[count]=(temp_l[count]+26);
                    } else {
                        result[count]=(temp_l[count]-26)%47;
                    }
                }
            } else {
                if(temp_r[count]<26) {
                    result[count]=(temp_r[count]+26);
                } else {
                    result[count]=(temp_r[count]-26)%47;
                }
            }
            count=count+1;
        }
    }

    //converts the chars back to their corresponding ASCII values
    for(c=0; c<(r_len+l_len); c++) {
        if(result[c]>=26) {
            result[c]=result[c]+39;
        }
        if((result[c]>=0)&&(result[c]<=25)) {
            result[c]=result[c]+97;
        }
    }
    //ends the result string
    if(r_len>=l_len) {
        result[r_len]=0;
    }
    if(l_len>r_len) {
        result[l_len]=0;
    }

}
