#include<stdio.h>
#include<stdlib.h>
#include<math.h>
//node of the singly linked list. Every node contain just 1 digit and a pointer to next node.
struct node{
    int digit;
    struct node* next;
};
typedef struct node Node;

//create a node with the givrn digit value
Node* create_node(int value)
{
    Node* new_node;
    new_node = (Node*) malloc(sizeof(Node));
    if (new_node){
        new_node->digit = value;
        new_node->next = NULL;
        return new_node;
    }
}
//Node* create_

//Use kind of long multiplication algorithm to compute the result.
//In every step, the current digits stored in the linked list are multiplied with the next number 'num' and the value stored in each node is changed.
//If a carry remains, a new node is created and it is stored in the new node.

void multiply(Node* head, int num)
{
    Node* curr_node = head;
    Node* prev_node;
    int carry = 0;
    int val, res;
    while (curr_node != NULL)
    {
        val = (curr_node->digit * num) + carry;
        res = val % 10;
        curr_node->digit = res;
        carry = floor(val/10);
        prev_node = curr_node;
        curr_node = curr_node->next;
    }
    if (carry != 0){
        int curr_carry;
        Node* new_node;
        while (carry != 0)
        {
            curr_carry = carry % 10;
            carry = floor(carry/10);

            new_node = create_node(curr_carry);
            
            prev_node->next = new_node;
            prev_node = new_node;
        }
    }
}

//This function calls multiply function for every number 'num' till 'n', and the number of times it is called for a number 'num' is 'num' itself.
Node* calculate_n_hash(int n, Node* head)
{
    if (n == 0 || n == 1){
        return head;
        }
    else{
        for (int num = 2; num <= n; num++)
        {
            for (int j = 0; j < num; j++)
            {
                multiply(head, num);
            }
        }
        return head;
    }
}


//To count number of zeros appearing at the end of n#.
int get_end_zeros(Node* head)
{
    int count = 0;
    while (head -> digit == 0 && head != NULL)
    {
        count++;
        head = head->next;
    }
    return count;
}

// To reverse the linked list, so that it could be optimally printed and used to search for the pattern 'k'.
Node* reverse(Node*head)
{
    Node* prev_node = NULL;
    Node* curr_node = head;
    Node* next_node = head->next;
    while(curr_node != NULL)
    {
        curr_node->next = prev_node;
        prev_node = curr_node;
        curr_node = next_node;
        if(next_node != NULL)
            next_node = next_node->next;
    }
    return prev_node;
}

// To count the number of times a pattern 'k' appears in n#(that is the linked list headed by head_num).
int get_num_of_k_occurances(Node* head_num, Node* head_k)
{
    Node* curr_n = head_num;
    Node* curr_k = head_k;
    Node* temp = NULL;
    int flag = 0;
    int count = 0;
    while(curr_n != NULL)
    {
        if (curr_n->digit != (curr_k->digit)){
            if(flag == 0){
                curr_n = curr_n->next;
            }
            else{
                flag = 0;
                curr_k = head_k;
                curr_n = temp;
                temp = NULL;
            }
        }
        else {
            if (temp == NULL){
                    temp = curr_n->next;
                }
            if(curr_k->next != NULL){
                flag = 1;
                curr_k = curr_k->next;
                curr_n = curr_n->next;
            }
            else{
                flag = 0;
                curr_k = head_k;
                curr_n = temp;
                temp = NULL;
                count++;
            }
        }
    }
    return count;
}

//To print the final value of n# which is stored in the Linked list.
void display(Node* head)
{
    if (head == NULL){
        printf("\tEmpty\n");
    }
    else{
        Node* curr_node = head;

        while (curr_node != NULL){
            printf("%d", curr_node->digit);
            curr_node = curr_node->next;
        }
    }
}

//To free memory at the end of programm
Node* free_memory(Node* head)
{
    if (head = NULL){
        return head;
    }

    else{
        Node* curr_node;

        while(head != NULL){
            curr_node = head;
            head = head->next;
            curr_node->next = NULL;
            free(curr_node);
        }
        return head;
    }
}

//Driver function which scans and calls all the other function when needed.
int main()
{
    int t;
    scanf("%d", &t);
    while (t--)
    {
        int n;
        scanf("%d", &n);

        int num_of_zeros, num_of_k_occurance;
        char k[10];                                         //string of the pattern k, which we need to find in n#.
        char* p = k;                                        //pointer pointing at the starting of the string.
        scanf("\n%s", k);

        Node* k_head = NULL;                                    //head of linked list that store digits of pattern k at each node
        Node* k_tail;
        while (*p != '\0')
        {
            if(k_head == NULL){
                k_head = create_node(*p - '0');
                k_tail = k_head;
            }
            else{
                k_tail->next = create_node(*p - '0');
                k_tail = k_tail->next;
            }
            p = p + 1;
        }
        k_tail = NULL;

        Node* head = create_node(1);
        head = calculate_n_hash(n, head);
        num_of_zeros = get_end_zeros(head);

        head = reverse(head);

        num_of_k_occurance = get_num_of_k_occurances(head, k_head);

        printf("%d %d ", num_of_zeros, num_of_k_occurance);
        display(head);
        if(t != 0){                                                   //To avoid newline character at the end of program.
            printf("\n");
        }
        head = free_memory(head);
        k_head = free_memory(k_head);                                   //free memory at end of each test case.
    }

    return 0;
}
