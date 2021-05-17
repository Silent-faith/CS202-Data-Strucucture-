#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

struct node{                                                    //Node for BST and AVL tree
    int key;
    struct node* left;
    struct node* right;
};
typedef struct node Node;

struct sq_node{                                                 //Node for stack and queue
    Node* key;                                                  //Key stores pointer to structs of type Node rather than interger
    struct sq_node* next;
};
typedef struct sq_node SQ_Node;

struct linked_list{                                             //Node for Linked List(Used in perimeter Querry)
    int key;
    struct linked_list* next;
};
typedef struct linked_list LL;


// To allocate memory to different type of structs

Node* create_node(int value)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->key = value;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

SQ_Node* create_SQ(Node* key)
{
    SQ_Node* new_node = (SQ_Node*)malloc(sizeof(SQ_Node));
    new_node->key = key;
    new_node->next = NULL;
    return new_node;
}

LL* create_LL(int key)
{
    LL* new_node = (LL*)malloc(sizeof(LL));
    new_node->key = key;
    new_node->next = NULL;
    return new_node;
}


void insert_in_LL(int key, LL* head){
    LL* new_node = create_LL(key);
    
    LL* curr_node = head;
    while (curr_node->next != NULL){
        curr_node = curr_node->next;
    }
    curr_node->next = new_node;
}

void print_LL(LL* head)                                         //Used to print the perimeter and free up space at the same time
{
    LL* temp;
    while (head != NULL){
        printf("%d ", head->key);
        temp = head;
        head = head->next;
        temp->next = NULL;
        free(temp);
    }
}

SQ_Node* add_to_queue(SQ_Node* rear, Node* key)
{
    SQ_Node* new_node = create_SQ(key);
    rear->next = new_node;
    rear = new_node;
    return rear;
}

SQ_Node* push_stack(SQ_Node* top, Node* key)
{
    if(top == NULL){
        top = create_SQ(key);
    }
    else{
        SQ_Node* new_node = create_SQ(key);
        new_node->next = top;
        top = new_node;
    }
    return top;
}

SQ_Node* pop_or_dequeue(SQ_Node* curr)
{
    SQ_Node* temp = curr;
    //printf("%d ", curr->value);
    curr = curr->next;
    temp->next = NULL;
    free(temp);

    return curr;
}

int max(int a, int b)
{
    return (a >= b) ? a : b;
}


// Recursive functionality used to get the height of a perticular node
// Height of null is taken 0
int height(Node* root){
    if (root == NULL){
        return -1;
    }
    else{
        int h = 1 + max(height(root->left), height(root->right));
        return h;
    }
}

void inorder_traversal(Node* root)
{
    if (root != NULL){
        inorder_traversal(root->left);
        printf("%d ", root->key);
        inorder_traversal(root->right);
    }
}

void postorder_traversal(Node* root)
{
    if (root != NULL){
        postorder_traversal(root->left);
        postorder_traversal(root->right);
        printf("%d ", root->key);
    }
}

// Recursive function to iterate through the left subtree of the node to be deleted(if it has both right and left child)
// It find the rightmost node of the left subtree

void iterate(Node* to_be_deleted, Node* rightmosts_parent, Node* rightmost)
{
    if (rightmost->right == NULL)
    {
        to_be_deleted->key = rightmost->key;
        rightmosts_parent->right = rightmost->left;
        rightmost->left = NULL;
        free(rightmost);
    }
    else
    {
        iterate(to_be_deleted, rightmost, rightmost->right);
    }
}


void insert_in_BST(Node* root, int key)
{   
    if (key > root->key)
    {
        if (root->right == NULL){
            root->right = create_node(key);
        }
        else{
            insert_in_BST(root->right, key);
        }
    }
    else if (key < root->key)
    {
        if (root->left == NULL){
            root->left = create_node(key);
        }
        else{
            insert_in_BST(root->left, key);
        }
    }
}

Node* delete_from_BST(Node* root, int key)
{
    if (root != NULL)
    {
        if(root->key == key){
            if (root->left == NULL && root->right == NULL){
                return NULL;
            }
            else if (root->left != NULL && root->right == NULL){
                Node* temp = root;
                root = root->left;
                temp->left = NULL;
                free(temp);
                return root;
            }
            else if (root->left == NULL && root->right != NULL){
                Node* temp = root;
                root = root->right;
                temp->right = NULL;
                free(temp);
                return root;
            }
            else{
                Node* rightmosts_parent = root->left;                                      //root of left subtree
                Node* rightmost = rightmosts_parent->right;                                 //right child of rightmosts_parent
                if (rightmost == NULL){
                    rightmosts_parent->right = root->right;
                    root->right = NULL;
                    root->left = NULL;
                    free(root);
                    return rightmosts_parent;
                }
                else{
                    iterate(root, rightmosts_parent, rightmost);
                    return root;
                }
            }
        }   
        else if (key > root->key){
            root->right = delete_from_BST(root->right, key);
        }
        else{
            root->left = delete_from_BST(root->left, key);
        }
        return root;
    }
    else{
        return NULL;
    }
}

Node* rotate_right(Node* root)
{
    Node* z = root;
    Node* y = root->left;
    z->left = y->right;
    y->right = z;
    return y;
}

Node* rotate_left(Node* root)
{
    Node* z = root;
    Node* y = root->right;
    z->right = y->left;
    y->left = z;
    return y;
}

Node* make_rotations(Node* root)
{
    int height_diff = height(root->right) - height(root->left);

    if (height_diff < -1 && (height(root->left->left) >= height(root->left->right))){
        root = rotate_right(root);
        return root;
    }
    else if (height_diff < -1 && (height(root->left->left) < height(root->left->right))){
        root->left = rotate_left(root->left);
        root = rotate_right(root);
        return root;
    }
    else if (height_diff > 1 && (height(root->right->right) >= height(root->right->left))){
        root = rotate_left(root);
        return root;
    }
    else if (height_diff > 1 && (height(root->right->right) < height(root->right->left))){
        root->right = rotate_right(root->right);
        root = rotate_left(root);
        return root;
    }
    else{
        return root;
    }
}

//similar to iterate function defined above
Node* iterate_AVL(Node* root, Node* rightmosts_parent, Node* rightmost)
{
    if (rightmost->right == NULL)
    {
        root->key = rightmost->key;
        rightmosts_parent->right = rightmost->left;
        rightmost->left = NULL;
        free(rightmost);
        return make_rotations(rightmosts_parent);
    }
    else
    {
        rightmost = iterate_AVL(root, rightmost, rightmost->right);
        return make_rotations(rightmosts_parent);
    }
}

Node* insert_in_AVL(Node* root, int key)
{
    if (root == NULL){
        Node* newnode = create_node(key);
        return newnode;
    }
    else if (key > root->key){
        root->right = insert_in_AVL(root->right, key);
    }
    else if (key < root->key){
        root->left = insert_in_AVL(root->left, key);
    }

    int height_diff = height(root->right) - height(root->left);

    if (height_diff < -1 && key < root->left->key){                         //left left case
        root = rotate_right(root);
        return root;
    }
    else if (height_diff < -1 && key > root->left->key){                    //left right case
        root->left = rotate_left(root->left);
        root = rotate_right(root);
        return root;
    }
    else if (height_diff > 1 && key > root->right->key){                    //right right case
        root = rotate_left(root);
        return root;
    }
    else if (height_diff > 1 && key < root->right->key){                     //right left case
        root->right = rotate_right(root->right);
        root = rotate_left(root);
        return root;
    }
    else{
        return root;
    }
}

Node* delete_from_AVL(Node* root, int key)
{
    if (root != NULL){
        if (root->key == key){
            if (root->left == NULL && root->right == NULL){
                return NULL;
            }
            else if (root->left != NULL && root->right == NULL){
                Node* temp = root;
                root = root->left;
                temp->left = NULL;
                free(temp);
                return root;
            }
            else if (root->left == NULL && root->right != NULL){
                Node* temp = root;
                root = root->right;
                temp->right = NULL;
                free(temp);
                return root;
            }
            else{
                Node* rightmosts_parent = root->left;
                Node* rightmost = rightmosts_parent->right;
                if (rightmost == NULL){
                    rightmosts_parent->right = root->right;
                    root->right = NULL;
                    root->left = NULL;
                    free(root);
                    return make_rotations(rightmosts_parent);
                }
                else
                {
                    rightmosts_parent = iterate_AVL(root, rightmosts_parent, rightmost);
                    root->left = rightmosts_parent;
                    return root;
                }
            }
        }
        else if (key > root->key){
            root->right = delete_from_AVL(root->right, key);
        }
        else{
            root->left = delete_from_AVL(root->left, key);
        }
        return make_rotations(root);
    }
    else{
        return NULL;
    }
}


//Recursive function to delete the tree. Delete in bottom up manner.
void delete_tree(Node* root)
{
    if (root != NULL){
        delete_tree(root->left);
        delete_tree(root->right);
        root->left = NULL;
        root->right = NULL;
        free(root);
    }
}

int count_leaves(Node* root)
{
    if (root == NULL){
        return 0;
    }
    else if (root->left == NULL && root->right == NULL){
        return 1;
    }
    else{
        int count = count_leaves(root->left) + count_leaves(root->right);
        return count;
    }
}

// traverse the tree to find the leaf nodes from rigth to left. Used in the perimeter querry to print in clockwise manner.
int print_leaves_get_sum(Node* root, LL* head)
{
    if (root == NULL){
        return 0;
    }
    else if (root->left == NULL && root->right == NULL){
        insert_in_LL(root->key, head);
        return root->key;
    }
    else{
        return print_leaves_get_sum(root->right, head) + print_leaves_get_sum(root->left, head);
    }
}

// Travesrse the left boundary of the tree in bottom up manner to print clockwise parameter.
int back_from_left(Node* root, LL* head)
{
    if (root == NULL){
        return 0;
    }
    else if (root->left == NULL && root->right == NULL){
        return 0;
    }
    else{
        int sum = 0;
        if (root->left == NULL){
            sum += back_from_left(root->right, head);
        }
        else{
            sum += back_from_left(root->left, head);
        }
        insert_in_LL(root->key, head);
        //printf("%d ", root->key);
        return sum + root->key;
    }
}

int count_nodes(Node* root)
{
    if (root == NULL){
        return 0;
    }
    else{
        int count = 1 + count_nodes(root->left) + count_nodes(root->right);
        return count;
    }
}

// recursive code that return the max among the diameters which include root, which include root->left and which include root->right.
int get_diameter(Node* root)
{
    if (root == NULL){
        return 0;
    }
    else{
        int left_dia = get_diameter(root->left);
        int right_dia = get_diameter(root->right);
        int root_dia = 3 + height(root->left) + height(root->right);

        if (left_dia >= root_dia && left_dia >= right_dia)
            return left_dia;
        else if (root_dia >= left_dia && root_dia >= right_dia)
            return root_dia;
        else
            return right_dia;
    }
}

int find(Node* root, int x)
{
    int found = 0;
    if (x > 0){
        while (root != NULL){
            if (root->key == x){
                found = 1;
            }
            if (x > root->key)
                root = root->right;
            else
                root = root->left;
        }
    }
    return found;
}

int queue_len(SQ_Node* front)
{
    int count = 0;
    while (front != NULL){
        count++;
        front = front->next;
    }
    return count;
}

int main()
{
    int Z;                                          //no. of querries
    scanf("%d", &Z);    
    
    char querry;

    // seperate roots for AVL and BST. Atleast One of them is always empty.
    Node* AVL_root = NULL;
    Node* BST_root = NULL;

    int curr_tree_is_BST = 1;               //keep track of current tree

    char c;                                  //scan trash (white spaces)
    scanf("%c", &c);
    scanf("%c", &querry);
    
    while(Z--)
    {
        if (querry == 'T'){                                                      //T
            if (AVL_root != NULL){
                delete_tree(AVL_root);
                AVL_root = NULL;
            }
            if (BST_root != NULL){
                delete_tree(BST_root);
                BST_root = NULL;
            }
            curr_tree_is_BST = 1;
            
            char p;                                                     //scan trash (white spaces)
            int parameter;
            scanf("%c", &p);

            while (p != '\n')                                            //querry ends when newline is encountered
            {
                scanf("%d", &parameter);
                if (parameter > 0)
                {
                    if (BST_root == NULL){
                        BST_root = create_node(parameter);
                    }
                    else{
                        insert_in_BST(BST_root, parameter);
                    }
                }
                else if (parameter < 0)
                {
                    int key = abs(parameter);
                    BST_root = delete_from_BST(BST_root, key);
                }
                scanf("%c", &p);
            }
            if(Z!=0)
                scanf("%c", &querry);
        }

        else if (querry == 'H'){                                                 //H
            if (BST_root != NULL){
                delete_tree(BST_root);
                BST_root = NULL;
            }
            if (AVL_root != NULL){
                delete_tree(AVL_root);
                AVL_root = NULL;
            }
            curr_tree_is_BST = 0;
            char p;
            int parameter;
            scanf("%c", &p);

            while (p != '\n')
            {
                scanf("%d", &parameter);
                if (parameter > 0)
                {
                    AVL_root = insert_in_AVL(AVL_root, parameter);
                }
                else if (parameter < 0)
                {
                    int key = abs(parameter);
                    AVL_root = delete_from_AVL(AVL_root, key);
                }
                scanf("%c", &p);
            }
            scanf("%c", &querry);
        }

        else if (querry == 'A'){                                                 //A
            char p;
            int parameter;
            scanf("%c", &p);
            
            //if tree is BST:
            if (curr_tree_is_BST == 1){
                while (p != '\n')
                {
                    scanf("%d", &parameter);
                    if (parameter > 0)
                    {
                        if (BST_root == NULL){
                            BST_root = create_node(parameter);
                        }
                        else{
                            insert_in_BST(BST_root, parameter);
                        }
                    }
                    scanf("%c", &p);
                }
                scanf("%c",  &querry);
            }
            //If tree is AVL:
            else{
                while (p != '\n')
                {
                    scanf("%d", &parameter);
                    if (parameter > 0)
                    {
                        AVL_root = insert_in_AVL(AVL_root, parameter);
                    }
                    scanf("%c", &p);
                }
                scanf("%c",  &querry);
            }
        }

        else if (querry == 'U'){                                                 //U
            char p;
            int parameter;
            scanf("%c", &p);
            
            if (curr_tree_is_BST == 1){
                while (p != '\n')
                {
                    scanf("%d", &parameter);
                    if (parameter > 0)
                    {
                        int key = abs(parameter);
                        BST_root = delete_from_BST(BST_root, key);
                    }
                    scanf("%c", &p);
                }
                scanf("%c", &querry);
            }
            else{
                while (p != '\n')
                {
                    scanf("%d", &parameter);
                    if (parameter > 0)
                    {
                        int key = abs(parameter);
                        AVL_root = delete_from_AVL(AVL_root, key);
                    }
                    scanf("%c", &p);
                }
                scanf("%c", &querry);
            }
        }

        else if (querry == 'F'){                                                 //F, Find node with value x
            int x;
            char p;
            scanf("%c", &p);
            scanf("%d", &x);
            
            scanf("%c", &p);
            
            Node* curr;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;

            int found = find(curr, x);

            if (found == 1)
                printf("Yes");
            else
                printf("No");

            if (Z!=0){
                printf("\n");
                scanf("%c", &querry);
            }
        }

        else if (querry == 'Q'){                                                 //Q, Count leaves
            char p;
            scanf("%c", &p);
            Node* curr;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;

            int leaf_count = count_leaves(curr);
            printf("%d", leaf_count);
            if (Z!=0){
                printf("\n");
                scanf("%c", &querry);
            }
        }

        else if (querry == 'N'){                                                 //N, COunt no. of nodes
            char p;
            scanf("%c", &p);
            Node* curr;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;
            
            int node_count = count_nodes(curr);
            printf("%d", node_count);
            if (Z!=0){
                printf("\n");
                scanf("%c", &querry);
            }
        }

        else if (querry == 'P'){                                                 //P, iterative implementation of preorder traversal
            char p;
            scanf("%c", &p);
            Node* curr;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;

            if (curr != NULL){
                SQ_Node* top = NULL;                                                //Stack used to replicate the recursion.
                top = push_stack(top, curr);

                while (top != NULL){
                    curr = top->key;
                    top = pop_or_dequeue(top);

                    printf("%d ", curr->key);
                    if (curr->right != NULL){
                        top = push_stack(top, curr->right);
                    }
                    if (curr->left != NULL){
                        top = push_stack(top, curr->left);
                    }
                }
                if (Z!=0)
                printf("\n");
            }
            if (Z!=0){
                scanf("%c", &querry);
            }
        }

        else if (querry == 'I'){                                                 //I, Inorder traversal
            char p;
            scanf("%c", &p);
            Node* curr;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;
            inorder_traversal(curr);
            if (Z!=0){
                if(curr != NULL)
                    printf("\n");
                scanf("%c", &querry);
            }
        }

        else if (querry == 'S'){                                                 //S, Postorder traversal
            char p;
            scanf("%c", &p);
            Node* curr;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;
            postorder_traversal(curr);
            if (Z!=0){
                if(curr != NULL)
                    printf("\n");
                scanf("%c", &querry);
            }
        }

        else if (querry == 'L'){                                                //L, Level order traversal (using Queue)
            char p;
            scanf("%c", &p);
            SQ_Node* front = NULL;
            SQ_Node* rear = NULL;
            Node* curr;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;
            
            if (curr != NULL){
                rear = create_SQ(curr);
                front = rear;
                while (front != NULL){
                    curr = front->key;
                    if (front == rear){
                        front = pop_or_dequeue(front);
                        rear = front;
                    }
                    else{
                        front = pop_or_dequeue(front);
                    }
                    if (curr == NULL)
                        continue;

                    printf("%d ", curr->key);
                    if (rear == NULL){
                        rear = create_SQ(curr->left);
                        front = rear;
                        rear = add_to_queue(rear, curr->right);
                    }
                    else{
                        rear = add_to_queue(rear, curr->left);
                        rear = add_to_queue(rear, curr->right);
                    }
                }
                if(Z!=0)
                    printf("\n");
            }
            if (Z!=0){
                scanf("%c", &querry);
            }
        }

        else if (querry == 'D'){                                                //D, Depth of the tree
            char p;
            scanf("%c", &p);
            Node* curr;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;
            
            printf("%d", height(curr));
            if (Z!=0){
                printf("\n");
                scanf("%c", &querry);
            }
        }

        else if (querry == 'W'){                                                //W, Width of the tree (calculated using queue to count nodes in each level)
            char p;
            scanf("%c", &p);
            SQ_Node* front = NULL;
            SQ_Node* rear = NULL;
            Node* curr;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;
            
            if (curr != NULL){
                rear = create_SQ(curr);
                front = rear;
                
                int max = 0;
                int count = 0;

                while (front != NULL){
                    count = queue_len(front);
                    if (count > max){
                        max = count;
                    }

                    while (count--){
                        curr = front->key;
                        if (front == rear){
                            front = pop_or_dequeue(front);
                            rear = front;
                        }
                        else{
                            front = pop_or_dequeue(front);
                        }

                        if (curr->left != NULL){
                            if (rear == NULL){
                                rear = create_SQ(curr->left);
                                front = rear;
                            }
                            else{
                                rear = add_to_queue(rear, curr->left);
                            }
                        }

                        if (curr->right != NULL){
                            if (rear == NULL){
                                rear = create_SQ(curr->right);
                                front = rear;
                            }
                            else{
                                rear = add_to_queue(rear, curr->right);
                            }
                        }
                    }
                }
                printf("%d", max);
                if(Z!=0)
                    printf("\n");
            }
            if (Z!=0){
                scanf("%c", &querry);
            }
        }
        
        else if (querry == 'C'){                                                //C, Find common ancestor of i and j.
            char p;
            scanf("%c", &p);
            int i, j;
            scanf("%d %d", &i, &j);
            scanf("%c", &p);
            Node* curr;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;

            if (find(curr, i) && find(curr, j) && curr!=NULL){
                if (i == j){
                    printf("%d", i);
                }
                else{
                    int anc_found = 0;
                    while(anc_found==0){
                        if (curr->key == i || curr->key == j){
                            printf("%d", curr->key);
                            anc_found = 1;
                        }
                        else if (i > curr->key && j > curr->key){
                            curr = curr->right;
                        }
                        else if (i < curr->key && j < curr->key){
                            curr = curr->left;
                        }
                        else{
                            printf("%d", curr->key);
                            anc_found = 1;
                        }
                    }
                }
                
            }
            else{
                printf("-1");
            }
            if (Z!=0){
                printf("\n");
                scanf("%c", &querry);
            }
        }
        
        else if (querry == 'R'){                                                //R, Find and print path for i to j (not possible only when i and j are i different subtrees or j comes in higher level then i)
            char p;
            scanf("%c", &p);

            int i, j;
            scanf("%d %d", &i, &j);
            
            scanf("%c", &p);

            Node* curr;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;
            
            if (find(curr, i) == 0){
                printf("Source does not exist");
            }
            else if (find(curr, j) == 0){
                printf("Destination does not exist");
            }
            else if (i == j){
                printf("%d", i);
            }
            else{
                int i_found = 0, j_found = 0;
                int h = height(curr);
                int arr[h+1];
                char* str[h+1];

                int a = 0;
                while (i_found == 0 && j_found == 0){
                    if (curr->key == i){
                        i_found = 1;
                        printf("%d", i);
                    }
                    else if (curr->key == j){
                        j_found = 1;
                    }
                    else if (i > curr->key && j > curr->key){
                        curr = curr->right;
                    }
                    else if (i < curr->key && j < curr->key){
                        curr = curr->left;
                    }
                    else{
                        printf("Unreachable");
                        break;
                    }
                }
                if (i_found == 0 && j_found == 1){
                    printf("Unreachable");
                }
                else if (i_found == 1 && j_found == 0){
                    while (j_found == 0){
                        if (curr->key == j){
                            j_found = 1;
                        }
                        else if (j > curr->key){
                            curr = curr->right;
                            printf(" R %d", curr->key);
                        }
                        else if (j < curr->key){
                            curr = curr->left;
                            printf(" L %d", curr->key);
                        }
                    }
                }
            }
            if (Z!=0){
                printf("\n");
                scanf("%c", &querry);
            }
        }
        
        else if (querry == 'X'){                                                //X, find diameter
            char p;
            scanf("%c", &p);
            Node* curr;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;
            
            printf("%d", get_diameter(curr));
            if (Z!=0){
                printf("\n");
                scanf("%c", &querry);
            }
        }
        
        else if (querry == 'Y'){                                                //Y, print perimeter in clockwise manner
            char p;                                                             //The Linked list is used to store the perimeter in clockwise order to print after counting the value
            scanf("%c", &p);
            Node* curr;
            Node* root;
            if (curr_tree_is_BST == 1)
                curr = BST_root;
            else
                curr = AVL_root;
            root = curr;
            LL* head = NULL;

            int perimeter = 0;
            if (curr != NULL){
                while (1){
                    if (curr->left == NULL && curr->right == NULL){
                        break;
                    }
                    else if (curr->right == NULL){
                        if (head == NULL){
                            head = create_LL(curr->key);
                        }
                        else{
                            insert_in_LL(curr->key, head);
                        }

                        perimeter += curr->key;
                        curr = curr->left;
                    }
                    else{
                        if (head == NULL){
                            head = create_LL(curr->key);
                        }
                        else{
                            insert_in_LL(curr->key, head);
                        }
                        perimeter += curr->key;
                        curr = curr->right;
                    }
                }
                perimeter += print_leaves_get_sum(root, head);
                perimeter += back_from_left(root->left, head);
                printf("%d ", perimeter);
                print_LL(head);
                if (Z!=0)
                printf("\n");
            }
            if (Z!=0){
                scanf("%c", &querry);
            }
        }
    }
    delete_tree(BST_root);
    delete_tree(AVL_root);
    BST_root = NULL;
    AVL_root = NULL;
}