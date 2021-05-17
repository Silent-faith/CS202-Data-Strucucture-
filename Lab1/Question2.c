#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct node{
    int coeff, pow;
    struct node* next;
} Node;

//To create new nodes.
Node* create_node(int coeff, int pow)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->coeff = coeff;
    new_node->pow = pow;
    new_node->next = NULL;
    return new_node;
}

//To insert new nodes in the polnomial
Node* insert_at_end(Node* head, int coeff, int pow)
{
    Node* new_node = create_node(coeff, pow);
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

//To get polynomial P(3) by adding P(1) and P(2).
Node* add_polynomials(Node* head_p1, Node* head_p2)
{
    Node* head_p3 = NULL;
    Node* curr_node_p1 = head_p1;
    Node* curr_node_p2 = head_p2;
    int sum;
    while (curr_node_p1 != NULL && curr_node_p2 != NULL)
    {
        if (curr_node_p1->pow == curr_node_p2->pow){                            //for terms with same power
            sum = curr_node_p1->coeff + curr_node_p2->coeff;
            if (sum != 0){                                                      //To ignore terms with zero coefficient
                head_p3 = insert_at_end(head_p3, sum, curr_node_p1->pow);
            }
            curr_node_p1 = curr_node_p1->next;
            curr_node_p2 = curr_node_p2->next;
        }
        //Below we handle terms whose powers are unique to one of the polynomial
        else if (curr_node_p1->pow < curr_node_p2->pow){
            head_p3 = insert_at_end(head_p3, curr_node_p2->coeff, curr_node_p2->pow);
            curr_node_p2 = curr_node_p2->next;
        }
        else{
            head_p3 = insert_at_end(head_p3, curr_node_p1->coeff, curr_node_p1->pow);
            curr_node_p1 = curr_node_p1->next;
        }
    }
    //If P(1) has less number of terms than P(2)
    while (curr_node_p1 == NULL && curr_node_p2 != NULL)
    {
        head_p3 = insert_at_end(head_p3, curr_node_p2->coeff, curr_node_p2->pow);
        curr_node_p2 = curr_node_p2->next;
    }
    //If P(2) has less number of terms than P(1)
    while (curr_node_p1 != NULL && curr_node_p2 == NULL)
    {
        head_p3 = insert_at_end(head_p3, curr_node_p1->coeff, curr_node_p1->pow);
        curr_node_p1 = curr_node_p1->next;
    }
    return head_p3;
}

//To get polynomial P(4) by subtracting P(1) and P(2).
Node* subtract_polynomials(Node* head_p1, Node* head_p2)
{
    Node* head_p4 = NULL;
    Node* curr_node_p1 = head_p1;
    Node* curr_node_p2 = head_p2;
    int differance;
    while (curr_node_p1 != NULL && curr_node_p2 != NULL)
    {
        if (curr_node_p1->pow == curr_node_p2->pow){                                    //for terms with same power
            differance = curr_node_p1->coeff - curr_node_p2->coeff;
            if (differance != 0){                                                           //To ignore terms with zero coefficient
                head_p4 = insert_at_end(head_p4, differance, curr_node_p1->pow);
            }
            curr_node_p1 = curr_node_p1->next;
            curr_node_p2 = curr_node_p2->next;
        }
        //Below we handle terms whose powers are unique to one of the polynomial
        else if (curr_node_p1->pow < curr_node_p2->pow){
            head_p4 = insert_at_end(head_p4, -(curr_node_p2->coeff), curr_node_p2->pow);
            curr_node_p2 = curr_node_p2->next;
        }
        else{
            head_p4 = insert_at_end(head_p4, curr_node_p1->coeff, curr_node_p1->pow);
            curr_node_p1 = curr_node_p1->next;
        }
    }
    //If P(1) has less number of terms than P(2)
    while (curr_node_p1 == NULL && curr_node_p2 != NULL)
    {
        head_p4 = insert_at_end(head_p4, -(curr_node_p2->coeff), curr_node_p2->pow);
        curr_node_p2 = curr_node_p2->next;
    }
    //If P(2) has less number of terms than P(1)
    while (curr_node_p1 != NULL && curr_node_p2 == NULL)
    {
        head_p4 = insert_at_end(head_p4, curr_node_p1->coeff, curr_node_p1->pow);
        curr_node_p1 = curr_node_p1->next;
    }

    return head_p4;
}

//To display the polynomials in terms of x
void display(Node* head)
{
    if (head == NULL){
        printf("0\n");
    }
    else{
        Node* curr_node = head;
        if (curr_node->pow > 0){
            if (curr_node->coeff > 0){
                printf("%dx^%d", curr_node->coeff, curr_node->pow);
            }
            else{
                printf("- %dx^%d", abs(curr_node->coeff), curr_node->pow);
            }
        }
        else{
            if (curr_node->coeff > 0){
                printf("%d", curr_node->coeff);
            }
            else{
                printf("- %d", abs(curr_node->coeff));
            }
        }
        curr_node = curr_node->next;

        while (curr_node != NULL){
            if (curr_node->pow > 0){
                if (curr_node->coeff > 0){
                    printf(" + %dx^%d", curr_node->coeff, curr_node->pow);
                }
                else{
                    printf(" - %dx^%d", abs(curr_node->coeff), curr_node->pow);
                }
            }
            else{
                if (curr_node->coeff > 0){
                    printf(" + %d", curr_node->coeff);
                }
                else{
                    printf(" - %d", abs(curr_node->coeff));
                }
            }
            curr_node = curr_node->next;
        }
        printf("\n");
    }
}

//Calls the display funtion for each polynomial
void display_in_x(Node* head_p1, Node* head_p2, Node* head_p3, Node* head_p4)
{
    printf("P1(x) : ");
    display(head_p1);
    printf("P2(x) : ");
    display(head_p2);
    printf("P3(x) = P1(x) + P2(x) : ");
    display(head_p3);
    printf("P4(x) = P1(x) - P2(x) : ");
    display(head_p4);
}

//Iterates through the linked list and calculate the value of polynommial at a given value of x.
int iterate(Node* head, int x)
{
    //printf("iterate");
    int sum = 0;
    Node* curr_node = head;
    while(curr_node != NULL){
        sum += (curr_node->coeff) * (pow(x, curr_node->pow));
        curr_node = curr_node->next;
    }
    return sum;
}

//To display the values of polynomials at given value of x
void display_calculated_values(Node* head_p1, Node* head_p2, Node* head_p3, Node* head_p4, int x)
{
    printf("P1(%d) = %d\n", x, iterate(head_p1, x));
    printf("P2(%d) = %d\n", x, iterate(head_p2, x));
    printf("P3(%d) = %d\n", x, iterate(head_p3, x));
    printf("P4(%d) = %d", x, iterate(head_p4, x));
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

//Driver function of the program
int main()
{
    int t;
    scanf("%d", &t);
    while(t--)
    {
        Node* head_p1 = NULL;                                    //Head node of P(1)
        Node* head_p2 = NULL;                                    //Head node of P(2)
        int n1, n2, coeff, x;

        scanf("%d", &n1);
        for (int pow=n1; pow>=0; pow--)                         //scans for P(1)
        {
            scanf("%d", &coeff);
            if (coeff != 0){                                        //ignore zero coefficients
                head_p1 = insert_at_end(head_p1, coeff, pow);
            }
        }

        scanf("%d", &n2);
        for (int pow=n2; pow>=0; pow--)                         //scans for P(2)
        {
            scanf("%d", &coeff);
            if (coeff != 0){                                            //ignore zero coefficients
                head_p2 = insert_at_end(head_p2, coeff, pow);
            }
        }
        scanf("%d", &x);

        Node* head_p3 = add_polynomials(head_p1, head_p2);                  //Head  node for P(3)
        Node* head_p4 = subtract_polynomials(head_p1, head_p2);             //Head  node for P(4)
        display_in_x(head_p1, head_p2, head_p3, head_p4);
        display_calculated_values(head_p1, head_p2, head_p3, head_p4, x);
        
        head_p1 = free_memory(head_p1);
        head_p2 = free_memory(head_p2);
        head_p3 = free_memory(head_p3);
        head_p4 = free_memory(head_p4);
        if(t != 0){                                                   //To avoid newline character at the end of program.
            printf("\n");
        }
    }
    return 0;
}