#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

// In the  querry number P, the current code computes the total permutations using the formula using dynamic programming.
// However you can comment the "kth_found" condition in the "find_all_stack_permutations_lexicographically" function to make the code get through all permutations and increase the count of permutations.
// But this will give output after a very long time. For n = 20, this would take approx 15 minutes for going through all permutations as their number is very huge.


// The code assumes input operators and operands in part I and E would be space seperated

// These are some global variables to keep track of important output from P querry.
int kth_found = 0;                                          // This is set to 1 whenever the k-th lexicographical permutation is found.
char kth_plus_minus_seq[65];                                // This is to store the K-th sequence
long long int lex_count = 0;                                // This is to count the number of permutations found till a point of time.


// Node for stacks.
struct node{
    int value;
    struct node* next;
};
typedef struct node Node;

// function to create a new node.
Node* create_node(int value)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

// function to push elements into the stack and retuen the new top.
Node* add_to_stack(Node* curr, int value)
{
    if(curr == NULL){
        curr = create_node(value);
    }
    else{
        Node* new_node = create_node(value);
        new_node->next = curr;
        curr = new_node;
    }
    return curr;
}

// function to pop from a stack or to deque from a queue.
// this works for both queue and stack.
Node* pop_or_deque(Node* curr)
{
    Node* temp = curr;
    //printf("%d ", curr->value);
    curr = curr->next;
    temp->next = NULL;
    free(temp);
    temp = NULL;

    return curr;
}

//Function to free allocated memory after the querry is complete.
Node* free_memory(Node* curr)
{
    Node* temp = curr;
    while (curr != NULL)
    {
        curr = curr->next;
        temp->next = NULL;
        free(temp);
        temp = curr;
    }
    return curr;
}

// Add node to a queue.
Node* add_to_queue(Node* rear, int value)
{
    Node* new_node = create_node(value);
    rear->next = new_node;
    rear = new_node;
    return rear;
}

// Function to check wheather the values at index start and curr should be swapped or not
// Return 1 if yes and 0 if no.
int swap_condition(char plus_minus[], int start, int curr)
{
    if (start == curr){                                                 //swap because it would move the start index one step forward in the next recursive call.
        return 1;
    }
    for (int i = start; i < curr; i++){
        if (plus_minus[i] == plus_minus[curr])                           // do not swap because these cases are already handles by the other recursive calls. 
        return 0;                                                        //Prevent repeatition of permutations
    }

    if (plus_minus[curr] == '+')                                        // If the above conditions are not true then we can swap '+' as push option dosen't produce invalid permuation.
        return 1;

    int num_plus = 0, num_minus = 0;
    for (int i = 0; i < start; i++){                                    // If we are swaping '-' then we have to check wether the permutation produced after swapping is valid or not.
        if (plus_minus[i] == '+')
            num_plus++;
        else
            num_minus++;
    }
    num_minus++;                                                        // To simulate swapping of '-' with '+'.

    if (num_plus >= num_minus)                                          // If the total no. of '+' >= '-' after swapping then no problem.
        return 1;
    else
        return 0;
}

//swap values at index a and b.
void swap(char str[], int a, int b)
{
    char temp = str[a];
    str[a] = str[b];
    str[b] = temp;
}

//  recursive function to get the permutations lexicographically.
void find_all_stack_permutations_lexicographically(char plus_minus[], int start, int end, int k)
{
    //printf("%d\n",kth_found);
    if (kth_found == 1){                                                            // use this to stop furthur computation after getting the k_th permutation.
        return;
    }

    else if (start == end + 1){                                                     // if the start index has reached the end, then this gives our new permutation.
        lex_count++;                                                                // increment permutation count.
        // if (lex_count % 100000 == 0)                 
            //printf("%lli  \n", lex_count);

        if (lex_count == k)                                                         // if this is  the k-th permutation, save it in kth_plus_minus_seq, declared globally
        {
            int i;
            for (i = 0; i <= end; i++){
                kth_plus_minus_seq[i] = plus_minus[i];
            }
            kth_plus_minus_seq[i] = '\0';
            kth_found = 1;                                                          // make kth_found as 1 to stop furthur computation.
        }
    }
    else
    {
        int swapable;
        for (int curr = start; curr <= end; curr++)                                                     // start from 0th index (passed from the main function) and recursively find new  permutations by systematically swapping elements.
        {
            swapable = swap_condition(plus_minus, start, curr);
            if (swapable)
            {   
                swap(plus_minus, start, curr);                                                          // swap if possible.
                find_all_stack_permutations_lexicographically(plus_minus, start+1, end, k);             // next recursive call.
                swap(plus_minus, start, curr);                                                          // swap the same index again to get original string back.
            }
        }
    }
}

// The no. of stack permutations follows Catalan Numbers.
// Lets denote nth Catalan number by C(n)
// Then :

// C(n) = C(0)C(n-1) + C(1)(n-2) + C(2)C(n-3) + .......... + C(n-2)C(1) + C(n-1)C(0)

// This code uses dynamic programming principle for calculating the same.

long long int compute_catalan_num(int n)
{
    long long int catalan_array[n+1];                                       // save the newly calculated catalan numbers in the array to use later and save computation. 
    catalan_array[0] = 1;
    catalan_array[1] = 1;

    for (int i = 2; i <= n; i++)
    {
        catalan_array[i] = 0;
        for (int j = 0; j < i; j++)
        {
            catalan_array[i] += catalan_array[j] * catalan_array[i - j - 1];
        }
    }
    return catalan_array[n];
}

int main()
{
    char* operators[] = {"*", "/", "%", "+", "-", "<<", ">>", "<", "<=", ">", ">=", "==", "!=", "&", "^", "|", "#"};     //array of strings for all operators.
    int t, z;
    scanf("%d %d", &t, &z);
    char querry_type;
    scanf("\n%c", &querry_type);
    while(t--)
    {
        // implemented stack using linked list
        if (querry_type == 'S')
        {
            int no_of_elements = 0;
            Node* top = NULL;
            int parameter;
            scanf("%d", &parameter);
            
            while (parameter >= -3)                                                     // if the input is < -3 then stop furthur calculation until next querry.
            {
                if (parameter > 0)                                                      // push to stack
                {
                    top = add_to_stack(top, parameter);
                    no_of_elements ++;
                }

                else if (parameter == 0)                                                // check if empty
                {
                    if (no_of_elements == 0)
                        printf("E");
                    else
                        printf("NE");
                }

                else if (parameter == -1)                                               // pop from stack and print the popped value
                {
                    if (top != NULL){
                        printf("%d", top->value);
                        top = pop_or_deque(top);
                        no_of_elements--;
                    }
                    else{
                        printf("E");
                    }
                }

                else if (parameter == -2)                                              // since stack is implemented using LL it is never full.
                {
                    printf("NF");
                }

                else if (parameter == -3)                                                  //priint no. of elements in stack
                {
                    printf("%d", no_of_elements);
                }
                scanf("%d", &parameter);
                if (parameter <= -3)
                    printf(" ");
            }
            top = free_memory(top);
            printf("\n");
        }

        // implemented queue using linked list
        else if (querry_type == 'Q')
        {
            int no_of_elements = 0;
            Node* front = NULL;
            Node* rear = NULL;
            int parameter;
            scanf("%d", &parameter);

            while (parameter >= -3)
            {
                if (parameter > 0)
                {
                    if (front == NULL && rear == NULL){
                        rear = create_node(parameter);
                        front = rear;
                    }
                    else{
                        rear = add_to_queue(rear, parameter);
                    }
                    no_of_elements++;
                }

                else if (parameter == 0)
                {
                    if (no_of_elements == 0)
                        printf("E ");
                    else
                        printf("NE ");
                }

                else if (parameter == -1)
                {
                    if (front != NULL){
                        printf("%d ", front->value);
                        front = pop_or_deque(front);
                        if (front == NULL){
                            rear = front;
                        }
                        no_of_elements--;
                    }
                }

                else if (parameter == -2)
                {
                    printf("NF ");
                }

                else if (parameter == -3)
                {
                    printf("%d ", no_of_elements);
                }

                scanf("%d", &parameter);
            }
            front = free_memory(front);
            rear = front;
            printf("\n");
        }

        else if (querry_type == 'I')
        {
            int operator_count = 0;
            int operand_count = 0;
            
            char* operators_stack[z+2];                       //stack for operators implemented using arrays
            int top = 0;

            char* to_be_printed[50];                            // array of strings to store the final output to be printed if there is no error.
            int tbp_top = 0;
            
            int odd_even_check = 0;                                 // to check if the opereators are at odd poitions and operands are at even positins, otherwise there is error
            int break_flag = 0;

            char* op_tracker[50];                                   // store the allocated locations to free them at the end of querry.
            int op_tracker_top = 0;

            char* op = (char*)malloc(5);                            // input string (can be opoerator or operand)
            scanf("%s", op);
            op_tracker[op_tracker_top] = op;
            op_tracker_top++;
            //printf("op = %s\n", op);
            while (strcmp(op, operators[16]) != 0)                          // while input is not '#
            {
                //printf("hi\n");
                int i;                          // if 0 <= i <= 15  then input is operator
                for (i = 0; i < 16; i++){                                                       
                    //printf("hi\n");
                    //printf("%d\n", strcmp(operator, operators[i]) == 0);
                    if (strcmp(op, operators[i]) == 0){                                         
                        if (odd_even_check % 2 == 0){
                            break_flag = 1;
                            break;
                        }
                        operator_count++;
                        if (top == 0){                                                  //if stack is empty add operaator directly
                            operators_stack[top] = op;
                            top++;
                            //printf("%s\n", operators_stack[top-1]);
                        }

                        else{
                            int x;
                            int y;
                            for (x = 0; x < 16; x++){
                                if (strcmp(op, operators[x]) == 0)                  // check which operator was input
                                    break;
                            }
                            if (x>=0 && x <=2)                                      // set y according to the precedence table.
                                y = 2;
                            else if (x >= 3 && x <= 4)
                                y = 4;
                            else if (x >= 3 && x <= 4)
                                y = 4;
                            else if (x >= 5 && x <= 6)
                                y = 6;
                            else if (x >= 7 && x <= 10)
                                y = 10;
                            else if (x >= 11 && x <= 12)
                                y = 12;
                            else
                                y = x;
                            
                            int flag = 1;
                            while (flag == 1)                                                               //add the top of operator stack to the output untill the precedence of current operator is greater than the top.
                            {
                                int j;
                                for (j = 0; j <= y; j++){                                                       // check the precedence of operator currently at top of stack
                                    if (top > 0 && strcmp(operators_stack[top-1], operators[j]) == 0){
                                        to_be_printed[tbp_top] = operators_stack[top-1];
                                        top--;
                                        tbp_top++;
                                        flag = 1;                          
                                        break;
                                    }
                                }
                                if (j == y+1)
                                    flag = 0;
                            }

                            operators_stack[top] = op;
                            top++;
                        }
                        break;
                    }
                }
                if (i == 16){                                                               // if i was not in range( 0, 15 ) then it is a  operand
                    if (odd_even_check % 2 == 1){
                        break_flag = 1;
                    }
                    else{
                        operand_count++;
                        to_be_printed[tbp_top] = op;
                        tbp_top++;
                    }
                }

                if (break_flag == 1){
                    break;                                              // if error is encountered.... break
                }

                op = (char*)malloc(5);
                scanf("%s", op);
                op_tracker[op_tracker_top] = op;
                op_tracker_top++;
                //printf("op = %s\n", op);
                odd_even_check++;

            }

            if ((break_flag == 1) || (operator_count != operand_count-1)){                       // operand count chould be 1 more then operator count as all opoerators are binary
                printf("Error\n");
            }
            else{                                                                   // if no error.... print the output
                for (int i = 0; i < tbp_top; i++){
                    printf("%s", to_be_printed[i]);
                    if (i != tbp_top - 1 || top > 0)
                        printf(" ");
                }
                for (int i = top-1; i >= 0; i--){
                    printf("%s", operators_stack[i]);
                    if (i != 0)
                        printf(" ");
                }
                printf("\n");
            }
            for (int i = 0; i < op_tracker_top; i++){
                free(op_tracker[i]);
            }
        }

        else if (querry_type == 'E')
        {                                                               // E is very similar to I, only difference being that we have  to compute rather than printing.
            int operator_count = 0;
            int operand_count = 0;
            
            char* operators_stack[z];
            int operator_top = 0;

            int odd_even_check = 0;
            int break_flag = 0;

            Node* operand_top = NULL;                                   // LL stack for integers
            
            char* op_tracker[20];
            int op_tracker_top = 0;

            char* op = (char*)malloc(5);
            scanf("%s", op);
            op_tracker[op_tracker_top] = op;
            op_tracker_top++;

            int flag = 0;
            while (strcmp(op, operators[16]) != 0)
            {
                int i;
                for (i = 0; i < 16; i++){
                    if (strcmp(op, operators[i]) == 0){
                        if (odd_even_check % 2 == 0){
                            break_flag = 1;
                            break;
                        }
                        operator_count++;
                        if (operator_top == 0){
                            operators_stack[operator_top] = op;
                            //printf("or_stack = %s\n", operators_stack[operator_top]);
                            operator_top++;
                        }

                        else{
                            int x;
                            int y;
                            for (x = 0; x < 16; x++){
                                if (strcmp(op, operators[x]) == 0)
                                    break;
                            }
                            if (x>=0 && x <=2)
                                y = 2;
                            else if (x >= 3 && x <= 4)
                                y = 4;
                            else if (x >= 3 && x <= 4)
                                y = 4;
                            else if (x >= 5 && x <= 6)
                                y = 6;
                            else if (x >= 7 && x <= 10)
                                y = 10;
                            else if (x >= 11 && x <= 12)
                                y = 12;
                            else
                                y = x;
                            
                            for (int j = 0; j <= y; j++){
                                if (strcmp(operators_stack[operator_top-1], operators[j]) == 0){
                                    flag = 1;
                                    break;
                                }
                            }
                            if (flag == 0){
                                operators_stack[operator_top] = op;
                                //printf("or_stack = %s\n", operators_stack[operator_top]);
                                operator_top++;
                            }
                            else{                                              // if lower predence operator is encountered... compute the result of the currently saved operators.
                                int a, b, res;
                                while (operator_top > 0){
                                    a = operand_top->value;
                                    operand_top = pop_or_deque(operand_top);
                                    
                                    b = operand_top->value;
                                    operand_top = pop_or_deque(operand_top);
                                    
                                    if (strcmp(operators_stack[operator_top-1], operators[0]) == 0)
                                        res = b * a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[1]) == 0)
                                        res = b / a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[2]) == 0)
                                        res = b % a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[3]) == 0)
                                        res = b + a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[4]) == 0)
                                        res = b - a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[5]) == 0)
                                        res = b << a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[6]) == 0)
                                        res = b >> a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[7]) == 0)
                                        res = b < a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[8]) == 0)
                                        res = b <= a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[9]) == 0)
                                        res = b > a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[10]) == 0)
                                        res = b >= a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[11]) == 0)
                                        res = b == a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[12]) == 0)
                                        res = b != a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[13]) == 0)
                                        res = b & a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[14]) == 0)
                                        res = b ^ a;
                                    else if (strcmp(operators_stack[operator_top-1], operators[15]) == 0)
                                        res = b | a;
                                    
                                    operand_top = add_to_stack(operand_top, res);
                                    //printf("od_stack = %d\n", operand_top->value);
                                    operator_top--;

                                }
                                operators_stack[operator_top] = op;
                                //printf("or_stack = %s\n", operators_stack[operator_top]);
                                operator_top++;
                                flag = 0;
                            }
                        }
                        break;
                    }
                }
                if (i == 16){
                    if (odd_even_check % 2 == 1){
                        break_flag = 1;
                    }
                    else{
                        operand_count++;
                        operand_top = add_to_stack(operand_top, atoi(op));
                        //printf("od_stack = %d\n", operand_top->value);
                    }
                }

                if (break_flag == 1){
                    break;
                }

                op = (char*)malloc(5);
                scanf("%s", op);
                op_tracker[op_tracker_top] = op;
                op_tracker_top++;
                //printf("op = %s\n", op);
                odd_even_check++;

            }
            if ((break_flag == 1) || (operator_count != operand_count-1)){
                printf("Error\n");
            }
            else{
                int a, b, res;
                while (operator_top > 0){                                                   // if the stacks are not empty after input is over....  compute here
                    a = operand_top->value;
                    operand_top = pop_or_deque(operand_top);
                    
                    b = operand_top->value;
                    operand_top = pop_or_deque(operand_top);
                                    
                    if (strcmp(operators_stack[operator_top-1], operators[0]) == 0)
                        res = b * a;
                    else if (strcmp(operators_stack[operator_top-1], operators[1]) == 0)
                        res = b / a;
                    else if (strcmp(operators_stack[operator_top-1], operators[2]) == 0)
                        res = b % a;
                    else if (strcmp(operators_stack[operator_top-1], operators[3]) == 0)
                        res = b + a;
                    else if (strcmp(operators_stack[operator_top-1], operators[4]) == 0)
                        res = b - a;
                    else if (strcmp(operators_stack[operator_top-1], operators[5]) == 0)
                        res = b << a;
                    else if (strcmp(operators_stack[operator_top-1], operators[6]) == 0)
                        res = b >> a;
                    else if (strcmp(operators_stack[operator_top-1], operators[7]) == 0)
                        res = b < a;
                    else if (strcmp(operators_stack[operator_top-1], operators[8]) == 0)
                        res = b <= a;
                    else if (strcmp(operators_stack[operator_top-1], operators[9]) == 0)
                        res = b > a;
                    else if (strcmp(operators_stack[operator_top-1], operators[10]) == 0)
                        res = b >= a;
                    else if (strcmp(operators_stack[operator_top-1], operators[11]) == 0)
                        res = b == a;
                    else if (strcmp(operators_stack[operator_top-1], operators[12]) == 0)
                        res = b != a;
                    else if (strcmp(operators_stack[operator_top-1], operators[13]) == 0)
                        res = b & a;
                    else if (strcmp(operators_stack[operator_top-1], operators[14]) == 0)
                        res = b ^ a;
                    else if (strcmp(operators_stack[operator_top-1], operators[15]) == 0)
                        res = b | a;
                        
                    operand_top = add_to_stack(operand_top, res);
                    operator_top--;
                }
                printf("%d \n", operand_top->value);
                operand_top = pop_or_deque(operand_top);
            }

            for (int i = 0; i < op_tracker_top; i++){
                free(op_tracker[i]);
            }
        }
        
        else if (querry_type == 'P')
        {
            int n, k;
            scanf("%d %d", &n, &k);

            char plus_minus[2*n+1];                                         // I have used '+' and '-' analogy for the push and pop operations.
            for (int i = 0; i < 2 * n; i++){
                if (i % 2 == 0)
                    plus_minus[i] = '+';
                else
                    plus_minus[i] = '-';
            }                                                               // the initial array of '+' and '-' corresponds to the lexicographically smallest stack permutation.

            long long int catalan = compute_catalan_num(n);                              
            printf("%lli ", catalan);

            find_all_stack_permutations_lexicographically(plus_minus, 0, 2*n - 1, k);               //call the recursive function to get all permutations.
                                                                                                    //Also the permutations comes out to be in the correct order because we are starting swapping from the end of the original plus minus sequence.
            //printf("%lli\n", lex_count);                                                                

            Node* stack_top = NULL;        
            int len = strlen(kth_plus_minus_seq);
            int num = 1;
            for (int i = 0; i < len; i++)                                                           //convert the kth sequence of '+' and '-' into numbers
            {
                if (kth_plus_minus_seq[i] == '+'){
                    stack_top = add_to_stack(stack_top, num);
                    num++;
                }
                else{
                    printf("%d", stack_top->value);
                    stack_top = pop_or_deque(stack_top);
                    if (i != len-1)
                        printf(" ");
                }
            }
            printf("\n");
            
            lex_count = 0;
            kth_found = 0;
        }   
        
        else
        {
            printf("Error\n");
        }
        
        do{                                                                                     //For scanning the waste input until the  next querry comes.
            if(t != 0)
                scanf("\n%c", &querry_type);
        }

        while (t != 0 && querry_type != 'S' && querry_type != 'Q' && querry_type != 'I' && querry_type != 'E' && querry_type != 'P');
    }
    return 0;
}