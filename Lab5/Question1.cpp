#include<iostream>
#include<array>
#include<vector>
#include<iterator>
#include<queue>
#include<stack>
#include<set>
#include<string>
#include<limits>
#include<algorithm>

using namespace std;

struct node{
    int key;
    int parent;
    int start_time;
    int end_time;
    int corresponding_to;
    char color;
    int equivalent_to;
    int min_dis;
};
typedef struct node vertex;

struct weight_node{
    int key;
    int weight;
};
typedef struct weight_node w_node;

void print_matrix(int n, int ** matrix)
{
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= n; j++){
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
}

vector<vertex*> create_vertices(vector<vertex*> arr, vector<int> keys)
{
    int i = 0;
    auto itr = keys.begin();
    while (itr != keys.end())
    {
        vertex* new_node = (vertex*)malloc(sizeof(vertex));
        new_node->key = (*itr);
        new_node->parent = 0;
        new_node->start_time = 0;
        new_node->end_time = 0;
        new_node->color = 'w';
        new_node->corresponding_to = -1;
        new_node->equivalent_to = i;
        new_node->min_dis = -1;
        i++;
        arr.push_back(new_node);
        itr++;
    }
    return arr;
}

vector<int> find_min_edge_to_each_node(int n, vector<vector<vertex*>> adj_ls, vector<vector<w_node*>> w_info, vector<vertex*> vertices)
{
    vector<int> min_edge;
    for(int i = 0; i < n; i++)
        min_edge.push_back(INT_MAX);
    
    auto adj_ls_itr = adj_ls.begin();
    auto w_info_itr = w_info.begin();

    while (adj_ls_itr != adj_ls.end())
    {
        auto itr = (*adj_ls_itr).begin();
        auto itr2 = (*w_info_itr).begin();
        while (itr != (*adj_ls_itr).end())
        {
            if ((*itr2)->weight < min_edge[(*itr)->equivalent_to])
            {
                min_edge[(*itr)->equivalent_to] = (*itr2)->weight;
            }
            itr++;
            itr2++; 
        }
        adj_ls_itr++;
        w_info_itr++;
    }
    return min_edge;
}

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int n, m, s;
        cin >> n >> m >> s;

        int u, v, w;
        int **matrix = new int*[n+1];
        for (int i = 0; i < 10; i++){
            matrix[i] = new int[n+1];
        }

        for (int i = 0; i <= n; i++){
            for (int j = 0; j <= n; j++){
                *(*(matrix + i) + j) = 0;
            }
        }

        int neg_weight = 0;
        for (int i = 1; i <= m; i++){
            cin >> u >> v >> w;
            if (w == -1){
                cout << "-1\n";
                neg_weight = 1;
                break;
            }
            matrix[u][v] = w;
        }
        if (neg_weight == 1){
            continue;
        }
        stack<vector<vector<vertex*>>> adj_ls_stack;
        stack<vector<vector<w_node*>>> w_node_stack;
        stack<vector<vertex*>> vertices_stack;
        stack<vector<vector<w_node*>>> updated_node_stack;
        stack<vector<int>> vertices_keys;
        stack<vector<int>> cycle_stack;
        stack<vector<vector<int>>> correspondance_stack;

        vector<int> keys;
        for (int i = 1; i <= n; i++){
            keys.push_back(i);
        }
        vertices_keys.push(keys);

        vector<vertex*> arr;
        arr = create_vertices(arr, keys);
        vertices_stack.push(arr);
        
        vector<vector<vertex*>> adj_ls;
        vector<vector<w_node*>> w_info;
        for (int i = 1; i <= n; i++){
            vector<vertex*> n1;
            vector<w_node*> n2;
            for (int j = 1; j <= n; j++){
                if (matrix[i][j] != 0){
                    w_node* nn;
                    nn = (w_node*)malloc(sizeof(w_node));
                    nn->key = j;
                    nn->weight = matrix[i][j];
                    n2.push_back(nn);
                    n1.push_back(arr[j-1]);
                }
            }
            adj_ls.push_back(n1);
            w_info.push_back(n2);
        }
        adj_ls_stack.push(adj_ls);
        w_node_stack.push(w_info);


        int max_key_label = n;
        while(1){
            vector<vector<vertex*>> curr_adj_ls = adj_ls_stack.top();
            vector<vector<w_node*>> curr_w_info = w_node_stack.top();
            vector<vertex*> curr_arr = vertices_stack.top();
            vector<int> curr_keys = vertices_keys.top();

            vector<int> min = find_min_edge_to_each_node(n, curr_adj_ls, curr_w_info, curr_arr);
           
            vector<vector<w_node*>> updated_w_info;
            auto itr = curr_w_info.begin();
            auto itr_a = curr_adj_ls.begin();
            auto itr_key = curr_keys.begin();
            while(itr != curr_w_info.end())
            {
                vector<w_node*> n_vec;
                auto itr1 = (*itr).begin();
                auto itr2 = (*itr_a).begin();
                
                itr_key++;
                while(itr1 != (*itr).end())
                {
                    w_node* nn;
                    nn = (w_node*)malloc(sizeof(w_node));
                    nn->key = (*itr1)->key;
                    // cout << "Key : " << (*itr1)->key << "\n";
                    nn->weight = (*itr1)->weight - min[(*itr2)->equivalent_to];
                    // cout << "original : " << (*itr1)->weight << "\n";
                    // cout << "updated : " << nn->weight << "\n";
                    n_vec.push_back(nn);
                    itr1++;
                    itr2++;
                }
                updated_w_info.push_back(n_vec);
                itr++;
                itr_a++;
            }            
            updated_node_stack.push(updated_w_info);
            min.clear();
            //Now to check cylcle of zero weight edges

            vector<vector<vertex*>> zero_adj_ls;
            vector<vector<w_node*>> zero_w_info;

            auto adj_itr = curr_adj_ls.begin();
            auto weight_itr = updated_w_info.begin();
            while(adj_itr!= curr_adj_ls.end())
            {
                vector<vertex*> n1;
                vector<w_node*> n2;

                auto itr1 = (*adj_itr).begin();
                auto itr2 = (*weight_itr).begin();
                
                while (itr1 != (*adj_itr).end())
                {
                    if ((*itr2)->weight == 0)
                    {
                        n2.push_back(*itr2);
                        n1.push_back(curr_arr[(*itr1)->equivalent_to]);
                    }
                    itr1++;
                    itr2++;
                }
                zero_adj_ls.push_back(n1);
                zero_w_info.push_back(n2);
                adj_itr++;
                weight_itr++;
            }
            
            int top_key;
            int i = 0, remaining = 1, last_white = 0, curr_time = 1;
            int processesd_till_now[curr_keys.size()+1] = {0};
            
            auto key_itr = curr_keys.begin();

            stack<int> key_stack;
            key_stack.push(i);
            curr_arr[i]->start_time = 1;
            curr_arr[i]->color = 'g';

            vector<int> cycle_nodes;
            int cycle_found = 0;
            while (remaining != 0 && cycle_found == 0){
                
                while (!key_stack.empty())
                {
                    
                    top_key = key_stack.top();
                    
                    auto itr1 = zero_adj_ls[top_key].begin();
                    advance(itr1, processesd_till_now[top_key]);
                    
                    while ((itr1 != zero_adj_ls[top_key].end()) && (*itr1)->color != 'w')
                    {
                        if ((*itr1)->color == 'g'){
                            while (key_stack.top() != (*itr1)->equivalent_to)
                            {
                                cycle_nodes.push_back(key_stack.top());
                                key_stack.pop();
                            }
                            cycle_nodes.push_back(key_stack.top());
                            cycle_found = 1;
                            break;
                        }
                        processesd_till_now[top_key]++;
                        itr1++;
                    }
                    if (cycle_found == 1)
                        break;

                    if (itr1 == zero_adj_ls[top_key].end()){
                        curr_arr[top_key]->color = 'b';
                        curr_time++;
                        curr_arr[top_key]->end_time = curr_time;
                        key_stack.pop();
                    }
                    else{
                        key_stack.push((*itr1)->equivalent_to);
                        (*itr1)->color = 'g';
                        (*itr1)->parent = curr_arr[top_key]->key;
                        curr_time++;
                        (*itr1)->start_time = curr_time;
                        processesd_till_now[top_key]++;
                    }
                }
                if (cycle_found == 1)
                    break;
                remaining = 0;
                for (int i = last_white; i < curr_keys.size(); i++)
                {
                    if (curr_arr[i]->color == 'w'){
                        remaining = i;
                        last_white = i;
                        break;
                    }
                }
                if (remaining != 0)
                {
                    key_stack.push(remaining);
                    curr_arr[remaining]->color = 'g';
                    curr_time++;
                    curr_arr[remaining]->start_time = curr_time;
                }
            }
            if (cycle_found == 0){
                break;
            }
            cycle_stack.push(cycle_nodes);

            vector<int> new_keys;
            int new_key = max_key_label + 1;
            max_key_label++;

            int equi = 0;
            auto key_itr1 = curr_keys.begin();
            while (key_itr1 != curr_keys.end())
            {
                if (find(cycle_nodes.begin(), cycle_nodes.end(), equi) != cycle_nodes.end()){
                    key_itr1++;
                    equi++;
                    continue;
                }
                new_keys.push_back(*key_itr1);
                key_itr1++;
                equi++;
            }
            new_keys.push_back(new_key);
            vertices_keys.push(new_keys);

            vector<vertex*> new_arr;
            new_arr = create_vertices(new_arr, new_keys);
            vertices_stack.push(new_arr);
            
            int old_dictionary[new_key+1];
            key_itr1 = curr_keys.begin();
            equi = 0;
            while(key_itr1 != curr_keys.end())
            {
                old_dictionary[*key_itr1] = equi;
                equi++;
                key_itr1++;
            }
            int new_dictionary[new_key+1];
            key_itr1 = new_keys.begin();
            equi = 0;
            while(key_itr1 != new_keys.end())
            {
                new_dictionary[*key_itr1] = equi;
                equi++;
                key_itr1++;
            }
            vector<vector<vertex*>> new_adj_ls;
            vector<vector<w_node*>> new_w_info;
            vector<vector<int>> correspondance_ls;

            key_itr1 = curr_keys.begin();
            equi = 0;
            int a = 0;
            vector<int> counter;
            auto old_itr1 = curr_adj_ls.begin();
            auto old_itr2 = updated_w_info.begin();
            while (old_itr1 != curr_adj_ls.end())
            {
                vector<vertex*> n1;
                vector<w_node*> n2;
                vector<int> correspondance_vec;

                if (find(cycle_nodes.begin(), cycle_nodes.end(), equi) != cycle_nodes.end()){
                    counter.push_back(a);
                    a = 0;
                    a++;
                    equi++;
                    key_itr1++;
                    old_itr1++;
                    old_itr2++;
                    continue;
                }
                a++;
                auto itr1 = (*old_itr1).begin();
                auto itr2 = (*old_itr2).begin();
                while (itr1 != (*old_itr1).end())
                {
                    w_node* nn;
                    nn = (w_node*)malloc(sizeof(w_node));
                    if(find(cycle_nodes.begin(), cycle_nodes.end(), old_dictionary[(*itr2)->key]) != cycle_nodes.end()){
                        n1.push_back(new_arr[new_dictionary[new_key]]);
                        new_arr[new_dictionary[new_key]]->corresponding_to = (*itr2)->key;

                        correspondance_vec.push_back((*itr2)->key);

                        nn->key = new_key;
                        nn->weight = (*itr2)->weight;
                        itr1++;
                        itr2++;
                        n2.push_back(nn);
                        continue;
                    }
                    n1.push_back(new_arr[new_dictionary[(*itr2)->key]]);
                    nn->key = (*itr2)->key;
                    nn->weight = (*itr2)->weight;
                    n2.push_back(nn);
                    correspondance_vec.push_back(-1);
                    itr1++;
                    itr2++;
                }
                old_itr1++;
                old_itr2++;
                new_adj_ls.push_back(n1);
                new_w_info.push_back(n2);
                correspondance_ls.push_back(correspondance_vec);
                key_itr1++;
                equi++;
            }

            old_itr1 = curr_adj_ls.begin();
            old_itr2 = updated_w_info.begin();

            vector<vertex*> n1;
            vector<w_node*> n2;
            vector<int> correspondance_vac;
            auto counter_itr = counter.begin();
            while (counter_itr != counter.end())
            {
                advance(old_itr1, (*counter_itr));
                advance(old_itr2, (*counter_itr));
                counter_itr++;

                auto itr1 = (*old_itr1).begin();
                auto itr2 = (*old_itr2).begin();
                while (itr1 != (*old_itr1).end())
                {
                    w_node* nn;
                    nn = (w_node*)malloc(sizeof(w_node));
                    if (find(cycle_nodes.begin(), cycle_nodes.end(), old_dictionary[(*itr2)->key]) != cycle_nodes.end()){
                        itr1++;
                        itr2++;
                        continue;
                    }
                    n1.push_back(new_arr[new_dictionary[(*itr2)->key]]);
                    nn->key = (*itr2)->key;
                    nn->weight = (*itr2)->weight;
                    n2.push_back(nn);
                    correspondance_vac.push_back(-1);
                    itr1++;
                    itr2++;
                }
            }
            new_adj_ls.push_back(n1);
            new_w_info.push_back(n2);
            correspondance_ls.push_back(correspondance_vac);

            adj_ls_stack.push(new_adj_ls);
            w_node_stack.push(new_w_info);
            correspondance_stack.push(correspondance_ls);
        }

        int from, to;
        
        vector<vertex*> curr_arr = vertices_stack.top();
        vector<vector<vertex*>> curr_adj_ls = adj_ls_stack.top();
        vector<vector<w_node*>> curr_w_info = w_node_stack.top();
        vector<vector<w_node*>> updated_w_info = updated_node_stack.top();
        vector<int> curr_keys = vertices_keys.top();
        vector<vector<int>> correspondance_ls = correspondance_stack.top();

        auto k_itr = curr_keys.begin();
        auto itr_a = curr_adj_ls.begin();
        auto itr_b = updated_w_info.begin();
        auto itr_c = correspondance_ls.begin();

        int flag = 0;
        while(itr_a != curr_adj_ls.end())
        {
            auto itr1 = (*itr_a).begin();
            auto itr2 = (*itr_b).begin();
            auto itr3 = (*itr_c).begin();
            while (itr1 != (*itr_a).end())
            {
                if ((*itr1)->key == max_key_label && (*itr2)->weight == 0){
                    to = (*itr3);
                    flag = 1;
                    break;
                }
                itr1++;
                itr2++;
                itr3++;
            }
            if (flag == 1)
                break;
            itr_a++;
            itr_b++;
            itr_c++;
            k_itr++;
        }
        from = (*k_itr);
        //+++++++++++++++++++++++
        //free up space         +
        //+++++++++++++++++++++++

        auto fi1 = curr_arr.begin();
        while (fi1 != curr_arr.end()){
            free(*fi1);
            fi1++;
        }
        curr_arr.clear();
        auto fi2 = curr_adj_ls.begin();
        auto fi3 = curr_w_info.begin();
        auto fi4 = updated_w_info.begin();

        while(fi2 != curr_adj_ls.end()){
            auto itr1 = (*fi3).begin();
            auto itr2 = (*fi4).begin();
            while (itr1 != (*fi3).end()){
                free(*itr1);
                free(*itr2);
                itr1++;
                itr2++;
            }
            (*fi2).clear();
            (*fi3).clear();
            (*fi4).clear();
            fi2++;
        }
        curr_adj_ls.clear();
        curr_w_info.clear();
        updated_w_info.clear();
        curr_keys.clear();

        vertices_stack.pop();
        adj_ls_stack.pop();
        w_node_stack.pop();
        updated_node_stack.pop();
        vertices_keys.pop();
        correspondance_stack.pop();

        max_key_label--;

        while (max_key_label != n)
        {
            vector<vertex*> curr_arr = vertices_stack.top();
            vector<vector<vertex*>> curr_adj_ls = adj_ls_stack.top();
            vector<vector<w_node*>> curr_w_info = w_node_stack.top();
            vector<vector<w_node*>> updated_w_info = updated_node_stack.top();
            vector<int> curr_keys = vertices_keys.top();
            vector<vector<int>> correspondance_ls = correspondance_stack.top();

            auto itr_a = curr_adj_ls.begin();
            auto itr_b = updated_w_info.begin();
            auto itr_c = correspondance_ls.begin();
            auto k_itr = curr_keys.begin();
            int flag = 0;
            while(itr_a != curr_adj_ls.end())
            {
                auto itr1 = (*itr_a).begin();
                auto itr2 = (*itr_b).begin();
                auto itr3 = (*itr_c).begin();
                while (itr1 != (*itr_a).end())
                {
                    if ((*itr1)->key == to && (*k_itr) == from){
                        to = (*itr3);
                        from = (*k_itr);
                        flag = 1;
                    }
                    itr1++;
                    itr2++;
                    itr3++;
                }
                itr_a++;
                itr_b++;
                itr_c++;
                k_itr++;
            }
            
            auto fi1 = curr_arr.begin();
            while (fi1 != curr_arr.end()){
                free(*fi1);
                fi1++;
            }
            curr_arr.clear();
            auto fi2 = curr_adj_ls.begin();
            auto fi3 = curr_w_info.begin();
            auto fi4 = updated_w_info.begin();

            while(fi2 != curr_adj_ls.end()){
                auto itr1 = (*fi3).begin();
                auto itr2 = (*fi4).begin();
                while (itr1 != (*fi3).end()){
                    free(*itr1);
                    free(*itr2);
                    itr1++;
                    itr2++;
                }
                (*fi2).clear();
                (*fi3).clear();
                (*fi4).clear();
                fi2++;
            }
            curr_adj_ls.clear();
            curr_w_info.clear();
            updated_w_info.clear();
            curr_keys.clear();

            vertices_stack.pop();
            adj_ls_stack.pop();
            w_node_stack.pop();
            updated_node_stack.pop();
            vertices_keys.pop();
            correspondance_stack.pop();
            
            max_key_label--;
        }
        curr_arr = vertices_stack.top();
        curr_adj_ls = adj_ls_stack.top();
        curr_w_info = w_node_stack.top();
        updated_w_info = updated_node_stack.top();
        curr_keys = vertices_keys.top();

        vector<vector<int>> one_zero;
        itr_a = curr_adj_ls.begin();
        itr_b = updated_w_info.begin();
        k_itr = curr_keys.begin();
        while(itr_a != curr_adj_ls.end())
        {   
            vector<int> vec;
            auto itr1 = (*itr_a).begin();
            auto itr2 = (*itr_b).begin();
            while (itr1 != (*itr_a).end())
            {
                if ((*k_itr) == from and (*itr2)->key == to){
                    vec.push_back(1);
                    (*itr1)->parent = from;
                }
                else if ((*itr2)->weight == 0 && (*itr2)->key != to){
                    vec.push_back(1);
                }
                else{
                    vec.push_back(0);
                }
                itr1++;
                itr2++;
            }
            one_zero.push_back(vec);
            itr_a++;
            itr_b++;
            k_itr++;
        }

        int sum = 0;
        auto itr1 = curr_w_info.begin();
        auto itr2 = one_zero.begin();
        auto itr3 = curr_adj_ls.begin();
        auto ki = curr_keys.begin();
        while (itr1 != curr_w_info.end())
        {
            auto i1 = (*itr1).begin();
            auto i2 = (*itr2).begin();
            auto i3 = (*itr3).begin();
            while(i1 != (*itr1).end())
            {
                if ((*i2) == 1){
                    sum += (*i1)->weight;
                    (*i3)->parent = (*ki);
                }
                i1++;
                i2++;
                i3++;
            }
            itr1++;
            itr2++;
            itr3++;
            ki++;
        }
        cout << sum << " ";


        vector<vector<vertex*>> new_adj_ls;
        vector<vector<w_node*>> new_w_info;

        auto i1 = curr_adj_ls.begin();
        auto i2 = curr_w_info.begin();
        auto i3 = one_zero.begin();
        while (i1 != curr_adj_ls.end())
        {
            vector<vertex*>n1;
            vector<w_node*>n2;
            auto itr1 = (*i1).begin();
            auto itr2 = (*i2).begin();
            auto itr3 = (*i3).begin();
            while (itr1 != (*i1).end())
            {
                if ((*itr3) == 1){
                    n1.push_back((*itr1));
                    n2.push_back((*itr2));
                }
                itr1++;
                itr2++;
                itr3++;
            } 
            new_adj_ls.push_back(n1);
            new_w_info.push_back(n2);
            i1++;
            i2++;
            i3++;
        }

        int processed_till_now[n+1] = {0};

        curr_arr[s-1]->min_dis = 0;
        stack<int> key_stack;
        key_stack.push(s-1);
        int top_key;
        while (!key_stack.empty())
        {
            top_key = key_stack.top();
            auto itr = new_adj_ls[top_key].begin();
            auto itr2 = new_w_info[top_key].begin();
            advance(itr, processed_till_now[top_key]);
            advance(itr2, processed_till_now[top_key]);
            if (itr == new_adj_ls[top_key].end()){
                key_stack.pop();
            }

            if (itr < new_adj_ls[top_key].end())
            {
                key_stack.push((*itr)->key - 1);
                (*itr)->min_dis = (*itr2)->weight + curr_arr[top_key]->min_dis;
                processed_till_now[top_key]++;
            }
            
        }
        auto p_itr2 = curr_arr.begin();
        while (p_itr2 != curr_arr.end()){
            cout << (*p_itr2)->min_dis << " ";
            p_itr2++;
        }
        cout << "# ";

        auto p_itr = curr_arr.begin();
        while (p_itr != curr_arr.end()){
            cout << (*p_itr)->parent << " ";
            p_itr++;
        }
        cout << "\n";

    }

}