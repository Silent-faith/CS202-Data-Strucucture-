#include<stdio.h>
#include<stdlib.h>

//typedef node for furthur ease.
struct node
{
    int key;
    struct node* next;
};
typedef struct node Node;

//initialize and return a new node.
Node* create_node(int value)
{
    Node* new_node;
    new_node = (Node*) malloc(sizeof(Node));
    if (new_node){
        new_node->key = value;
        new_node->next = NULL;
        return new_node;
    }
}

//free up all allocated space before exiting program.
Node* free_all_space_before_exit(Node* head)
{
    if (head = NULL)
    {
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

//create a new node and insert at beginning of Linked List. If Linked List is empty, create a new one.
Node* insert_at_beginning(Node* head, int value)
{
    Node* new_node = create_node(value);
    if (head == NULL){
        head = new_node;
        return head;
    }
    else{
        new_node->next = head;
        head = new_node;
        return head;
    }
}

//create a new node and insert at end of Linked List. If Linked List is empty, create a new one.
Node* insert_at_end(Node* head, int value)
{
    Node* new_node = create_node(value);
    if (head == NULL){
        head = new_node;
        return head;
    }
    else{
        Node* curr_node = head;
        while (curr_node->next != NULL){
            curr_node = curr_node->next;
        }
        curr_node->next = new_node;
        return head;
    }
}

//delete first node of the linked list if it exists.
Node* delete_first_node(Node* head)
{
    if (head == NULL){
        printf("\tCannot delete as NO nodes in the linked-list\n");
        return head;
    }
    else
    {
        Node* curr_node = head;
        head = head->next;                      //move head a node ahead and delete first node.
        curr_node->next = NULL;
        free(curr_node);
        return head;
    }
}

//delete last node of the linked list if it exists.
Node* delete_last_node(Node* head)
{
    if (head == NULL){
        printf("\tCannot delete as NO nodes in the linked-list\n");
        return head;
    }
    else if (head->next == NULL)
    {
        Node* curr_node = head;
        head = NULL;                            //delete the head node directly if it is the only one.
        free(curr_node);
        return head;
    }
    else{
        Node* prev_node = head;
        Node* curr_node = head->next;

        while (curr_node->next != NULL){
            prev_node = prev_node->next;
            curr_node = curr_node->next;            //traverse till the end and delete the last node.
        }
        prev_node->next = NULL;
        free(curr_node);
        return head;
    }
}

//delete the node with the first occurance of input value, if it exists.
Node* delete_specific_node(Node* head, int value)
{
    if (head == NULL){
        printf("\tCannot delete as no such node in the linked-list\n");
        return head;
    }

    else if (head->next == NULL){
        if (head->key == value){
            Node* curr_node = head;
            head = NULL;
            free(curr_node);
        }
        return head;
    }

    else{
        if (head->key == value){
            head = delete_first_node(head);                 //use previously defined function to delete first node if it contain input value.
        }
        else{
            Node* prev_node = head;
            Node* curr_node = head->next;

            while (curr_node->key != value && curr_node->next != NULL){
                prev_node = prev_node->next;
                curr_node = curr_node->next;
            }
            if (curr_node->key == value){
                prev_node->next = curr_node->next;
                curr_node->next = NULL;
                free(curr_node);
            }
            else{
                printf("\tCannot delete as no such node in the linked-list\n");
            }
        }
        return head;
    }
}

//find the node with the first occurance of input value, if it exists.
void find_node(Node* head, int value)
{
    if (head == NULL){
        printf("\tCannot find any such node in the linked-list\n");
    }
    else
    {
        int position = 1;
        Node* curr_node = head;
        while (curr_node->next != NULL && curr_node->key != value){
            position++;
            curr_node = curr_node->next;
        }
        if (curr_node->key == value){
            printf("\tValue %d node is at position %d in the linked list.\n\tDifference in its allocated memory to that of current head : %ld\n", value, position, curr_node-head);
        }
        else
        {
            printf("\tCannot find any such node in the linked-list\n");
        }
        
    }
    
}

//print the size of linked list(number of nodes)
void print_size_of_LL(Node* head)
{
    if (head == NULL){
        printf("\tSize of the linked list = 0\n");
    }
    else{
        int count = 0;
        Node* curr_node = head;
        while (curr_node != NULL){
            count++;
            curr_node = curr_node->next;
        }
        printf("\tSize of the linked list = %d\n", count);
    }
}

void display(Node* head)
{
    if (head == NULL){
        printf("\tEmpty\n");
    }
    else{
        Node* curr_node = head;
        printf("\tLinked list: ");
        printf("%d", curr_node->key);
        curr_node = curr_node->next;

        while (curr_node != NULL){
            printf(" => %d", curr_node->key);
            curr_node = curr_node->next;
        }
        printf("\n");
    }
}

void Wrong_Input()
{
    printf("\tWrong Input.\n");
}

//main driver function for scanning and implementing different function choices.
int main()

{
    int choice;
    int data_value;

    Node* head = NULL;
    int flag = 0;                   //to keep track if user has given an invalid value as input.
    do{
        if(flag == 0){
            printf("Enter the choice:\n");
        }
        else{
            printf("Enter your choice again:\n");
        }
        scanf("%d", &choice);

        if (choice == 0){
            head = free_all_space_before_exit(head);
        }
        else if (choice == 1){
            flag = 0;
            printf("Enter the data value:\n");
            scanf("%d", &data_value);
            head = insert_at_beginning(head, data_value);
        }
        else if (choice == 2){
            flag = 0;
            printf("Enter the data value:\n");
            scanf("%d", &data_value);
            head = insert_at_end(head, data_value);
        }
        else if (choice == 3){
            flag = 0;
            head = delete_first_node(head);
        }
        else if (choice == 4){
            flag = 0;
            head = delete_last_node(head);
        }
        else if (choice == 5){
            flag = 0;
            printf("Enter the data value:\n");
            scanf("%d", &data_value);
            head = delete_specific_node(head, data_value);
        }
        else if (choice == 6){
            flag = 0;
            printf("Enter the data value:\n");
            scanf("%d", &data_value);
            find_node(head, data_value);
        }
        else if (choice == 7){
            flag = 0;
            print_size_of_LL(head);
        }
        else if (choice == 8){
            flag = 0;
            display(head);
        }
        else{
            flag = 1;
            Wrong_Input();
        }
    }while (choice != 0);

    return 0;
}