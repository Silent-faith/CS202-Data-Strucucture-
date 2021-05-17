#include<iostream>
#include<array>
#include<vector>
#include<iterator>
#include<queue>
#include<stack>
#include<set>
#include<string>

using namespace std;

//I have made seperate nodes to reduce the space required. If same node would have been used in all queries, then the components not required in certain querry would take some useless space.

struct bfs_node{
    int key;
    char color;
    int parent;
    int time;
};
typedef struct bfs_node BFS_node;

struct dfs_node{
    int key;
    char color;
    int parent;
    int start_time;
    int end_time;
};
typedef struct dfs_node DFS_node;

struct topological_node{
    int key;
    int indegree;
    int color;
};
typedef struct topological_node T_node;

struct Weight_node{
    int key;
    int dist;
};
typedef struct Weight_node weight_node;

struct Dijkstra_node{
    int key;
    char color;
    int min_dist_from_node;
};
typedef struct Dijkstra_node DJ_node;


// Comparison function for implementing priority queue.
struct compare
{
    bool operator()(DJ_node* p, DJ_node* q)
    {
        if (p->min_dist_from_node == q->min_dist_from_node){                        //If both distances are equal then lexicographical order is prefered
            return p->key > q->key;
        }
        return p->min_dist_from_node > q->min_dist_from_node;
    }
};

// Functions to initialize different nodes:

void create_arr_bfs(BFS_node* arr[], int n)
{
    for (int i = 1; i <= n; i++){
        BFS_node* new_node = (BFS_node*)malloc(sizeof(BFS_node));
        new_node->key = i;
        new_node->color = 'w';
        new_node->parent = 0;
        new_node->time = 0;
        arr[i] = new_node;
    }
}

void create_arr_dfs(DFS_node* arr[], int n)
{
    for (int i = 1; i <= n; i++){
        DFS_node* new_node = (DFS_node*)malloc(sizeof(DFS_node));
        new_node->key = i;
        new_node->color = 'w';
        new_node->parent = 0;
        new_node->start_time = 0;
        new_node->end_time = 0;
        arr[i] = new_node;
    }
}

void create_arr_topological(T_node* arr[], int n)
{
    for (int i = 1; i <= n; i++){
        T_node* new_node = (T_node*)malloc(sizeof(T_node));
        new_node->key = i;
        new_node->indegree = 0;
        new_node->color = 'w';
        arr[i] = new_node;
    }
}

void create_arr_dj_node(DJ_node* arr[], int n)
{
    for (int i = 1; i <= n; i++){
        DJ_node* new_node = (DJ_node*)malloc(sizeof(DJ_node));
        new_node->key = i;
        new_node->min_dist_from_node = 999999;
        new_node->color = 'w';
        arr[i] = new_node;
    }
}

//Function to free memory (Generalized by use of Template)

template<typename T>
void free_memory(T arr[], int n)
{
    for (int i = 1; i <= n; i++)
    {
        free(arr[i]);
    }
}

// Perform BFS and print output
void perform_BFS(vector<BFS_node*> adj_ls[], BFS_node* arr[], int n, int d, int s)
{
    int dist_count[n+2] = {0};
    int eTree = 0, eForward = 0, eBackward = 0, eCross = 0;

    queue<BFS_node*> to_be_processed;
    to_be_processed.push(arr[s]);
    arr[s]->color = 'g';
    arr[s]->parent = 0;
    arr[s]->time = 0;
    if (d == 0)
    {
        while (!to_be_processed.empty())
        {
            BFS_node* under_process = to_be_processed.front();
            to_be_processed.pop();
            under_process->color = 'b';
            
            auto itr = adj_ls[under_process->key].begin();
            while (itr != adj_ls[under_process->key].end())
            {
                if ((*itr)->color == 'w')
                {
                    to_be_processed.push(*itr);
                    (*itr)->color = 'g';
                    (*itr)->parent = under_process->key;
                    (*itr)->time = (under_process->time) + 1;
                    
                    eTree++;
                    dist_count[(*itr)->time]++;
                }
                else if ((*itr)->color == 'g')
                {
                    eCross++;
                }
                
                itr++;
            }
        }
    }
    else
    {
        while(!to_be_processed.empty())
        {
            BFS_node* under_process = to_be_processed.front();
            to_be_processed.pop();
            under_process->color = 'b';

            auto itr = adj_ls[under_process->key].begin();
            while (itr != adj_ls[under_process->key].end())
            {
                if ((*itr)->color == 'w')
                {
                    to_be_processed.push(*itr);
                    (*itr)->color = 'g';
                    (*itr)->parent = under_process->key;
                    (*itr)->time = (under_process->time) + 1;
                    
                    eTree++;
                    dist_count[(*itr)->time]++;
                }
                else
                {
                    BFS_node* a;                                                //vertex with lower found time
                    BFS_node* b;                                                //vertex with higher found time


                    if ((*itr)->time > under_process->time){
                        a = under_process;
                        b = *itr;
                    }
                    else if ((*itr)->time < under_process->time){
                        a = *itr;
                        b = under_process;
                    }
                    else{
                        a = *itr;
                        b = under_process;
                    }

                    // We check if the node 'a' is an ancestor of node 'b' by moving up the tree using parent of node b.
                    //If yes then it is backward edge, else it is cross edge.
                    while (a->time != b->time){
                        b = arr[b->parent];
                    }
                    if (a->key == b->key){
                        eBackward++;
                    }
                    else{
                        eCross++;
                    }
                }
                itr++;
            }
        }
    }

    for (int i = 1; i <= n; i++){
        if (dist_count[i] != 0){
            cout << dist_count[i] << " ";
        }
        else{
            cout << 0 << " ";
            break;
        }
    }
    if (d == 0)
        cout << eTree << " " << eCross << '\n';

    else
        cout << eTree << " " << eBackward << " " << eForward << " " << eCross << '\n';
}

void perform_DFS(vector<DFS_node*> adj_ls[], DFS_node* arr[], int n, int d, int s)
{
    int eTree = 0, eForward = 0, eBackward = 0, eCross = 0;

    stack<int> key_stack;

    key_stack.push(arr[s]->key);
    arr[s]->color = 'g';
    arr[s]->start_time = 1;

    int curr_time = 1;                                              // to keep record of the current time of start and end of each node.

    int processed_till_now[n+1] = {0};                              // This will be used to prevent traversal to the same nodes again when we get out of a branch of each node.
                                                                    // In other words it keep record of which adjacent nodes have been traversed, and then we skip those nodes to prevent extra counts.

    int top_key;
    int remaining = 1, last_white = 1;
    
    if (d == 0)
    {
        while (remaining != 0){
            while (!key_stack.empty())
            {
                top_key = key_stack.top();
                auto itr = adj_ls[top_key].begin();
                advance(itr, processed_till_now[top_key]);                  // skip already processeed nodes.

                while ((itr < adj_ls[top_key].end()) && (*itr)->color != 'w')
                {
                    if ((*itr)->key != arr[top_key]->parent){
                        if ((*itr)->color == 'g'){
                            eBackward++;
                        }
                    }
                    processed_till_now[top_key]++;
                    itr++;
                }   
            
                if (itr == adj_ls[top_key].end()){
                    arr[top_key]->color = 'b';
                    curr_time++;
                    arr[top_key]->end_time = curr_time;
                    key_stack.pop();
                }
                else{
                    eTree++;
                    key_stack.push((*itr)->key);
                    (*itr)->color = 'g';
                    (*itr)->parent = top_key;
                    curr_time++;
                    (*itr)->start_time = curr_time;
                    processed_till_now[top_key]++;
                }
            }

            // To traverse other connected cmponents in the graph, we see if some nodes are still marked white, i.e not seen.
            //Then we add them in queue and apply the same steps.

            remaining = 0;
            for(int i = last_white; i <= n; i++){
                if(arr[i]->color == 'w'){
                    remaining = i;
                    last_white = i;
                    break;
                }
            }
            if (remaining != 0){
                key_stack.push(remaining);
                arr[remaining]->color = 'g';
                curr_time++;
                arr[remaining]->start_time = curr_time;
            }
        }
    }
    else
    {
        while (remaining != 0){
            while (!key_stack.empty())
            {
                top_key = key_stack.top();
                auto itr = adj_ls[top_key].begin();
                advance(itr, processed_till_now[top_key]);

                while ((itr < adj_ls[top_key].end()) && (*itr)->color != 'w')
                {
                    if ((*itr)->color == 'g'){
                        eBackward++;
                    }
                    else if ((*itr)->color == 'b' && arr[top_key]->start_time < (*itr)->start_time)
                    {
                        eForward++;
                    }
                    else if ((*itr)->color == 'b' && arr[top_key]->start_time > (*itr)->end_time)
                    {
                        eCross++;
                    }
                    processed_till_now[top_key]++;
                    itr++;
                }   
            
                if (itr == adj_ls[top_key].end()){
                    arr[top_key]->color = 'b';
                    curr_time++;
                    arr[top_key]->end_time = curr_time;
                    key_stack.pop();
                }
                else{
                    eTree++;
                    key_stack.push((*itr)->key);
                    (*itr)->color = 'g';
                    (*itr)->parent = top_key;
                    curr_time++;
                    (*itr)->start_time = curr_time;
                    processed_till_now[top_key]++;
                }
            }
            remaining = 0;
            for(int i = last_white; i <= n; i++){
                if(arr[i]->color == 'w'){
                    remaining = i;
                    last_white = i;
                    break;
                }
            }
            if (remaining != 0){
                key_stack.push(remaining);
                arr[remaining]->color = 'g';
                curr_time++;
                arr[remaining]->start_time = curr_time;
            }
        }
    }
    cout << arr[s]->end_time << " ";
    if (d == 0)
        cout << eTree << " " << eBackward << '\n';

    else
        cout << eTree << " " << eBackward << " " << eForward << " " << eCross << '\n';
}


//Here I am applying Knuts algorithm with a different approach. 
//Rather then using a queue to store the keys of nodes with indegree zero, which are yet to be seen, we use a multiset.
//This allow us to operate on lexicographicaly smaller nodes first, which i the required output in the querry.

int perform_topological_sort(vector<T_node*> adj_ls[], T_node* arr[], int n)
{
    multiset<int> key_ms;
    int count_zero_indegree = 0;

    // Count nodes with indegree zero.
    for (int i = 1; i <= n; i++)
    {
        if (arr[i]->indegree == 0){
            count_zero_indegree++;
            key_ms.insert(i);
        }
    }
    int top_key;
    vector<int> topological_order;                      //we will store topological order in this vector.

    // We delete the lexicographically smallest node till now with indegree zero, then reduce indegree of adjacent nodes by 1.
    //And if some node' indegree becomes 0, we add it to multiset.
    while (!key_ms.empty())
    {
        top_key = *(key_ms.begin());
        key_ms.erase(key_ms.begin());

        auto itr = adj_ls[top_key].begin();
        while (itr < adj_ls[top_key].end())
        {
            (*itr)->indegree--;
            if ((*itr)->indegree == 0)
            {
                count_zero_indegree++;
                key_ms.insert((*itr)->key);
            }
            itr++;
        }
        topological_order.push_back(top_key);
    }

    // If all nodes are not deleted at the end of the above loop, it means that topological sorting is not possible for this graph.
    if (count_zero_indegree != n)
        return 0;

    auto order_itr = topological_order.begin();
    if (order_itr != topological_order.end()){
        cout << *order_itr;
        order_itr++;
    }
    while(order_itr != topological_order.end()){
        cout << " " << *order_itr;
        order_itr++;
    }
    cout << '\n';
    return 1;
}

void perform_dijkstra(vector<DJ_node*>adj_ls[], DJ_node* arr[], vector<weight_node*>w_info[], int n, int d, int s)
{
    // priority queue implementing min heap, storing nodes in increasing order of their minimum distance from source
    // Also, the nodes which are in this queue are not yet a part of solution set.

    priority_queue<DJ_node*, vector<DJ_node*>, compare> pq;

    // When we find a node with min distance, we mark it as black, which signifies it is now a part of solution set.
    arr[s]->min_dist_from_node = 0;
    arr[s]->color = 'b';
    
    for (int i = 1; i <= n; i++){
        pq.push(arr[i]);
    }

    int top_key, neg_edge = 0;
    while (!pq.empty())
    {
        top_key = pq.top()->key;
        arr[top_key]->color = 'b';
        auto itr = adj_ls[top_key].begin();
        auto itr2 = w_info[top_key].begin();
        
        while (itr != adj_ls[top_key].end())
        {
            if ((*itr)->color == 'w')
            {
                if ((*itr2)->dist < 0)
                {
                    neg_edge = 1;
                    break;
                }

                // Relax Operation:

                else if ((*itr)->min_dist_from_node > arr[top_key]->min_dist_from_node + (*itr2)->dist)
                {
                    (*itr)->min_dist_from_node = arr[top_key]->min_dist_from_node + (*itr2)->dist;
                }
            }
            else if ((*itr)->color == 'b' && (*itr2)->dist < 0){
                neg_edge = 1;
                break;
            }
            itr++;
            itr2++;
        }
        pq.pop();
    }
    if (neg_edge == 1){
        cout << -1 << '\n';
    }
    else{
        cout << arr[1]->min_dist_from_node;
        for (int i = 2; i <= n; i++){
            cout << " " << arr[i]->min_dist_from_node;
        }
        cout << "\n";
    }
}

void perform_Bellman_Ford(vector<DJ_node*>adj_ls[], DJ_node* arr[], vector<weight_node*>w_info[], int n, int d, int s)
{
    arr[s]->min_dist_from_node = 0;

    queue<int> key_queue;
    int top_key;

    int remaining = 1, last_white = 1;
    int relax_count = 0;
    int effective_relax_count = 0;

    int change_occured = 0;

    for (int iteration = 1; iteration <= n-1; iteration++)
    {
        for (int i = 1; i <= n; i++){                   // Reset the colors before each iteration
            arr[i]->color = 'w';
        }
        change_occured = 0;                             // Keep track if a change has occured in an iteration.
        remaining = 1;
        last_white = 1;
        key_queue.push(s);
        arr[s]->color = 'b';

        while (remaining != 0){
            while (!key_queue.empty())
            {
                top_key = key_queue.front();
                key_queue.pop();

                auto itr = adj_ls[top_key].begin();
                auto itr2 = w_info[top_key].begin();

                while (itr != adj_ls[top_key].end())
                {   
                    relax_count++;
                    if ((*itr)->min_dist_from_node > arr[top_key]->min_dist_from_node + (*itr2)->dist && arr[top_key]->min_dist_from_node != 999999)
                    {
                        change_occured = 1;
                        effective_relax_count++;
                        (*itr)->min_dist_from_node = arr[top_key]->min_dist_from_node + (*itr2)->dist;
                    }
                    if ((*itr)->color == 'w'){
                        key_queue.push((*itr)->key);
                        (*itr)->color = 'b';
                    }
                    itr++;
                    itr2++;
                }
            }
            remaining = 0;
            for(int i = last_white; i <= n; i++){
                if(arr[i]->color == 'w'){
                    remaining = i;
                    last_white = i;
                    break;
                }
            }
            if (remaining != 0){
                key_queue.push(remaining);
                arr[remaining]->color = 'b';
            }
        }
        if (change_occured == 0)
            break;
    }

    // If changes have taken place till n-1th iterationn then check one more time to confirm wether there is a negetive edge weight cycle or not.
    // if change occur in this step, then -ve edge cylce is present, else not present.
    if (change_occured != 0){
        for (int i = 1; i <= n; i++){
            arr[i]->color = 'w';
        }
        change_occured = 0;
        remaining = 1;
        last_white = 1;
        key_queue.push(s);
        arr[s]->color = 'b';

        while (remaining != 0){
            while (!key_queue.empty())
            {
                top_key = key_queue.front();
                key_queue.pop();

                auto itr = adj_ls[top_key].begin();
                auto itr2 = w_info[top_key].begin();

                while (itr != adj_ls[top_key].end())
                {   
                    relax_count++;
                    if ((*itr)->min_dist_from_node > arr[top_key]->min_dist_from_node + (*itr2)->dist)
                    {
                        change_occured = 1;
                        effective_relax_count++;
                        (*itr)->min_dist_from_node = arr[top_key]->min_dist_from_node + (*itr2)->dist;
                    }
                    if ((*itr)->color == 'w'){
                        key_queue.push((*itr)->key);
                        (*itr)->color = 'b';
                    }
                    itr++;
                    itr2++;
                }
            }
            remaining = 0;
            for(int i = last_white; i <= n; i++){
                if(arr[i]->color == 'w'){
                    remaining = i;
                    last_white = i;
                    break;
                }
            }
            if (remaining != 0){
                key_queue.push(remaining);
                arr[remaining]->color = 'b';
            }
        }
    }
    if (change_occured == 1){
        cout << -1 << '\n';
    }
    else{
        for (int i = 1; i <= n; i++)
        {
            cout << arr[i]->min_dist_from_node << " ";
        }
        cout << relax_count << " " << effective_relax_count;
        cout << '\n';
    }
}


int main(){
    int t;
    cin >> t;
    int querry;

    while(t--){
        cin >> querry;

        if (querry == 1){
            int n, d, s;
            cin >> n >> d >> s;

            BFS_node* arr[n+1];
            create_arr_bfs(arr, n);                            // Create all vertices of the graph

            vector<BFS_node*> adj_ls[n+1];                      // Adjacency list

            // Take input and make adjacency list at the same time.
            for (int i = 1; i <= n; i++)
            {
                for (int j = 1; j <= n; j++)
                {
                    int x;
                    cin >> x;
                    if (x == 1)
                    {
                        adj_ls[i].push_back(arr[j]);
                    }
                }
            }
            
            perform_BFS(adj_ls, arr, n, d, s);
            free_memory<BFS_node*>(arr, n);
        }

        else if(querry == 2){
            int n, d, s;
            cin >> n >> d >> s;

            DFS_node* arr[n+1];
            create_arr_dfs(arr, n);

            vector<DFS_node*> adj_ls[n+1];
            for (int i = 1; i <= n; i++)
            {
                for (int j = 1; j <= n; j++)
                {
                    int x;
                    cin >> x;
                    if (x == 1)
                    {
                        adj_ls[i].push_back(arr[j]);
                    }
                }
            }
            perform_DFS(adj_ls, arr, n, d, s);
            free_memory<DFS_node*>(arr, n);
        }

        else if(querry == 3){
            int n;
            cin >> n;

            T_node* arr[n+1];
            create_arr_topological(arr, n);

            vector<T_node*> adj_ls[n+1];
            for (int i = 1; i <= n; i++)
            {
                for (int j = 1; j <= n; j++)
                {
                    int x;
                    cin >> x;
                    if (x == 1)
                    {
                        arr[j]->indegree++;
                        adj_ls[i].push_back(arr[j]);
                    }
                }
            }
            if (!perform_topological_sort(adj_ls, arr, n))
            {
                cout << -1 << '\n';
            }
            free_memory<T_node*>(arr, n);
        }

        else if(querry == 4){
            int n, d, s;
            cin >> n >> d >> s;

            DJ_node* arr[n+1];
            create_arr_dj_node(arr, n);
            vector<DJ_node*> adj_ls[n+1];

            vector<weight_node*> w_info[n+1];                               // vector to store the info of weights in the adjacency list.
            for (int i = 1; i <= n; i++)
            {
                for (int j = 1; j <= n; j++)
                {
                    int x;
                    cin >> x;
                    if (i != j && x < 999999)
                    {
                        weight_node* nn;
                        nn = (weight_node*)malloc(sizeof(weight_node));
                        nn->key = j;
                        nn->dist = x;
                        w_info[i].push_back(nn);
                        adj_ls[i].push_back(arr[j]);
                    }
                }
            }
            perform_dijkstra(adj_ls, arr, w_info, n, d, s);
            free_memory<DJ_node*>(arr, n);
            auto itr = w_info->begin();
            while(itr!= w_info->end()){
                free((*itr));
                itr++;
            }
        }
        
        else if(querry == 5){
            int n, d, s;
            cin >> n >> d >> s;

            DJ_node* arr[n+1];
            create_arr_dj_node(arr, n);
            vector<DJ_node*> adj_ls[n+1];
            
            vector<weight_node*> w_info[n+1];
            for (int i = 1; i <= n; i++)
            {
                for (int j = 1; j <= n; j++)
                {
                    int x;
                    cin >> x;
                    if (i != j && x < 999999)
                    {
                        weight_node* nn;
                        nn = (weight_node*)malloc(sizeof(weight_node));
                        nn->key = j;
                        nn->dist = x;
                        w_info[i].push_back(nn);
                        adj_ls[i].push_back(arr[j]);
                    }
                }
            }
            perform_Bellman_Ford(adj_ls, arr, w_info, n, d, s);
            free_memory<DJ_node*>(arr, n);
            auto itr = w_info->begin();
            while(itr!= w_info->end()){
                free((*itr));
                itr++;
            }
        }
    }
}