#include <iostream>
using namespace std;

class Tree {
public:
    int x;
    int height;
    Tree *left, *right, *mom;

    Tree(int x, Tree* mom = NULL) {
        this->x = x;
        this->height = 1;
        this->left = NULL; // можно просто left=NULL, потому что тут нет параметра
        this->right = NULL;
        this->mom = mom;
    }

    void Show(int level) {
        for(int i = 0; i < level; i++) cout << "   ";
        cout << x << endl;
    }
};
Tree* root = NULL;
Tree* del_el(Tree* t, int x);
void updateHeight(Tree* t);
Tree* do_AVL(Tree* t);

Tree* add(Tree* t, int x) {
    if (t == NULL) return new Tree(x);
    if (x < t->x) {
        t->left = add(t->left, x);
        if (t->left) t->left->mom = t;
    } else if (x > t->x) {
        t->right = add(t->right, x);
        if (t->right) t->right->mom = t;
    } updateHeight(t);
    t = do_AVL(t);
    updateHeight(t);
    return do_AVL(t);
}

void print_tree(Tree* t, int level) { 
    if (t == NULL) return;
    print_tree(t->right, level+1);
    t->Show(level);
    print_tree(t->left, level+1);
}

void deltree(Tree* t) {  
    if (t == NULL) return;
    deltree(t->right);
    deltree(t->left);
    delete t;
}

int count(Tree* t) {  
    if (t == NULL) return 0;
    return 1 + count(t->right) + count(t->left);
}

bool find(Tree* t, int x) {  
    if (t == NULL) return false;
    if (x == t->x) return true;
    if (x > t->x) return find(t->right, x);
    else return find(t->left, x);
} 

//AVL дерево
int get_height(Tree* t) {;
    return t ? t->height : 0;
}

void updateHeight(Tree* t) {
    t->height = 1 + max(get_height(t->left), get_height(t->right));
}

int proverka(Tree* t) {
    if (t == NULL) return 0;
    return get_height(t->left) - get_height(t->right);
}

Tree* rotateLL(Tree* t) { 
    Tree *par = t->mom;
    Tree *new_root = t->left;
    Tree *right_root = new_root->right; 

    new_root->right = t;
    t->left = right_root;

    if (right_root) right_root->mom = t;
    t->mom = new_root;
    new_root->mom = par;
    
    if (par) {
        if (par->left == t) par->left = new_root;
        else par->right = new_root;
    }
    updateHeight(t);
    updateHeight(new_root);

    return new_root;
}

Tree* rotateRR(Tree* t) { 
    Tree *par = t->mom;
    Tree *new_root = t->right; 
    Tree *left_root = new_root->left; 

    new_root->left = t;
    t->right = left_root;

    if (left_root) left_root->mom = t;
    t->mom = new_root;
    new_root->mom = par;
    
    if (par) {
        if (par->left == t) par->left = new_root;
        else par->right = new_root;
    }
    updateHeight(t);
    updateHeight(new_root);
    
    return new_root;
}

Tree* rotateLR(Tree* t) { 
    t->left = rotateRR(t->left);
    return rotateLL(t);
}

Tree* rotateRL(Tree* t) { 
    t->right = rotateLL(t->right);
    return rotateRR(t);
}

Tree* do_AVL(Tree* t) {
    int ves = proverka(t);
    if (ves > 1) {
        if (proverka(t->left) >= 0) return rotateLL(t);
        else return rotateLR(t);
    }
    if (ves < -1) {
        if (proverka(t->right) <= 0) return rotateRR(t);
        else return rotateRL(t);
    }
    return t;
}

Tree* minimum(Tree* t) {
    if (t->left == NULL) return t;
    return minimum(t->left);
}

Tree* move_mom(Tree* par, Tree* child) { // для случая с 1 ребенком 
    if (child) child->mom = par->mom;
    return child;
}

Tree* move_multikids_mom(Tree* t) { //для случая с 2 детьми
    Tree* temp = minimum(t->right);
    t->x = temp->x;
    t->right = del_el(t->right, temp->x);
    if (t->right) t->right->mom = t;
    return t;
}

Tree* del_el(Tree* t, int x) {
    if (t == NULL) return NULL;
    if (t->x < x) {
        t->right = del_el(t->right, x);
        if (t->right) t->right->mom = t;
    } else if (t->x > x) {
        t->left = del_el(t->left, x);
        if (t->left) t->left->mom = t;
    } else {
        if (t->right && t->left) {
            t = move_multikids_mom(t);
        } else if (t->left) {
            Tree* temp = move_mom(t, t->left);
            delete t;
            return temp;
        } else if (t->right) {
            Tree* temp = move_mom(t, t->right);
            delete t;
            return temp;
        } else {
            delete t;
            return NULL;
        }
    } updateHeight(t);
    t = do_AVL(t);
    updateHeight(t);
    return do_AVL(t);
}

int menu() {
    cout << "0. EXIT\n" << 
            "1. Add root\n" <<
            "2. Print tree\n" <<
            "3. Delete tree\n" <<
            "4. Delete exact root\n" <<
            "5. Find root\n";
    int a;
    cin >> a;
    return a;
}

int main() {
    bool alive = false;
    int n;
    while (true){
        switch(menu()){

            case 0: return 0;
                break;

            case 1:{ 
                cout << "Enter the value of new root: ";
                cin >> n;
                if (!alive){
                    root = new Tree(n);
                    alive = true;} 
                    else {
                    root = add(root, n);
                    if (root) root->mom = NULL;}
                break;}

            case 2: {
                if (alive) print_tree(root, 0);
                else cout << "Tree is empty\n";
                break;}
                    
            case 3: {
                if (!alive) cout << "Tree is empty\n";
                else deltree(root);
                root = NULL;
                alive = false;
                cout << "Tree has been deleted\n";
                break;}

            case 4: {
                if (!alive) cout << "Tree is completely empty\n";
                else { int del;
                cout << "Enter the root to be deleted: ";
                cin >> del;
                root = del_el(root, del);
                if (root) root->mom = NULL;
                if (root == NULL) alive = false;
                cout << "The root " << del << " has been cleared\n";}
                break;}

            case 5: {
                int find_el;
                cout << "Enter the root you want to find: ";
                cin >> find_el;
                if (alive) {
                    bool el = find(root, find_el);
                    if (el) cout << "Your element was found\n";
                    else cout << "Your element was NOT found\n";}
                else cout << "The tree is empty\n";
                break;}

            // case 6: {
            //     cout << "Now you have " << count << " student(s) in your list\n";
            //     break;} 

            default: cout << "ERROR\n";
        }
    }
}



