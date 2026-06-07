#include <iostream>
#include <string>
#include <functional>
using namespace std;

class Tree {
public:
    unsigned int key;
    string info;
    int height;
    Tree *left, *right, *mom;

    Tree(unsigned int k, string s, Tree* mom = NULL) {
        this->key = k;
        this->info = s;
        this->height = 1;
        this->left = NULL;
        this->right = NULL;
        this->mom = mom;
    }
};

Tree* root = NULL;

int get_height(Tree* t) {
    return t ? t->height : 0;
}

void updateHeight(Tree* t) {
    if (t)
        t->height = 1 + max(get_height(t->left), get_height(t->right));
}

int balance_factor(Tree* t) {
    if (t == NULL) return 0;
    return get_height(t->left) - get_height(t->right);
}

Tree* rotateLL(Tree* t) {
    Tree* par = t->mom;
    Tree* new_root = t->left;
    Tree* right_of_new = new_root->right;

    new_root->right = t;
    t->left = right_of_new;

    if (right_of_new) right_of_new->mom = t;
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
    Tree* par = t->mom;
    Tree* new_root = t->right;
    Tree* left_of_new = new_root->left;

    new_root->left = t;
    t->right = left_of_new;

    if (left_of_new) left_of_new->mom = t;
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
    int bf = balance_factor(t);
    if (bf > 1) {
        if (balance_factor(t->left) >= 0) return rotateLL(t);
        else return rotateLR(t);
    }
    if (bf < -1) {
        if (balance_factor(t->right) <= 0) return rotateRR(t);
        else return rotateRL(t);
    }
    return t;
}

Tree* add(Tree* t, unsigned int key, string info) {
    if (t == NULL) return new Tree(key, info);
    if (key < t->key) {
        t->left = add(t->left, key, info);
        if (t->left) t->left->mom = t;
    } else if (key > t->key) {
        t->right = add(t->right, key, info);
        if (t->right) t->right->mom = t;
    } else {
        cout << "Элемент с ключом " << key << " уже существует!\n";
        return t;
    }
    updateHeight(t);
    t = do_AVL(t);
    updateHeight(t);
    return do_AVL(t);
}

void deltree(Tree* t) {
    if (t == NULL) return;
    deltree(t->left);
    deltree(t->right);
    delete t;
}

bool find(Tree* t, unsigned int key) {
    if (t == NULL) return false;
    if (key == t->key) return true;
    if (key > t->key) return find(t->right, key);
    else return find(t->left, key);
}

Tree* minimum(Tree* t) {
    if (t->left == NULL) return t;
    return minimum(t->left);
}

Tree* del_el(Tree* t, unsigned int key);

Tree* move_mom(Tree* par, Tree* child) {
    if (child) child->mom = par->mom;
    return child;
}

Tree* move_multikids_mom(Tree* t) {
    Tree* temp = minimum(t->right);
    t->key = temp->key;
    t->info = temp->info;
    t->right = del_el(t->right, temp->key);
    if (t->right) t->right->mom = t;
    return t;
}

Tree* del_el(Tree* t, unsigned int key) {
    if (t == NULL) return NULL;
    if (t->key < key) {
        t->right = del_el(t->right, key);
        if (t->right) t->right->mom = t;
    } else if (t->key > key) {
        t->left = del_el(t->left, key);
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
    }
    updateHeight(t);
    t = do_AVL(t);
    updateHeight(t);
    return do_AVL(t);
}

void print_tree(Tree* t, int level) {
    if (t == NULL) return;
    print_tree(t->right, level + 1);
    for (int i = 0; i < level; i++) cout << "   ";
    cout << "(" << t->key << ") " << t->info << "\n";
    print_tree(t->left, level + 1);
}

void inorder(Tree* t) {
    if (t == NULL) return;
    inorder(t->left);
    cout << t->key << "\t" << t->info << "\n";
    inorder(t->right);
}

void preorder(Tree* t) {
    if (t == NULL) return;
    cout << t->key << "\t" << t->info << "\n";
    preorder(t->left);
    preorder(t->right);
}

void postorder(Tree* t) {
    if (t == NULL) return;
    postorder(t->left);
    postorder(t->right);
    cout << t->key << "\t" << t->info << "\n";
}

int count_nodes(Tree* t) {
    if (t == NULL) return 0;
    return 1 + count_nodes(t->left) + count_nodes(t->right);
}

void print_table(Tree* t, int traverse_type) {
    cout << "\n";
    string tname;
    if (traverse_type == 1) tname = "симметричный";
    else if (traverse_type == 2) tname = "прямой";
    else tname = "обратный";

    cout << "Бинарное дерево - вид обхода: " << tname << "\n";
    cout << "N п/п\tКлюч\tИнформация\n";

    int n = 1;

    function<void(Tree*)> traverse;
    if (traverse_type == 1) {
        traverse = [&](Tree* node) {
            if (!node) return;
            traverse(node->left);
            cout << n++ << "\t" << node->key << "\t" << node->info << "\n";
            traverse(node->right);
        };
    } else if (traverse_type == 2) {
        traverse = [&](Tree* node) {
            if (!node) return;
            cout << n++ << "\t" << node->key << "\t" << node->info << "\n";
            traverse(node->left);
            traverse(node->right);
        };
    } else {
        traverse = [&](Tree* node) {
            if (!node) return;
            traverse(node->left);
            traverse(node->right);
            cout << n++ << "\t" << node->key << "\t" << node->info << "\n";
        };
    }
    traverse(t);
    cout << "Всего в дереве: " << count_nodes(t) << " узлов, включая листья\n\n";
}

Tree* delete_leaves_at_level(Tree* t, int target_level, int current_level) {
    if (t == NULL) return NULL;

    if (current_level == target_level) {

        if (t->left == NULL && t->right == NULL) {
            delete t;
            return NULL;
        }
        return t;
    }

    t->left  = delete_leaves_at_level(t->left,  target_level, current_level + 1);
    if (t->left)  t->left->mom  = t;
    t->right = delete_leaves_at_level(t->right, target_level, current_level + 1);
    if (t->right) t->right->mom = t;

    updateHeight(t);
    t = do_AVL(t);
    updateHeight(t);
    return do_AVL(t);
}

void print_menu() {
    cout << "0. Выход\n";
    cout << "1. Добавить элемент\n";
    cout << "2. Вывести дерево (графически)\n";
    cout << "3. Вывести дерево в виде таблицы (обход)\n";
    cout << "4. Удалить дерево\n";
    cout << "5. Удалить узел по ключу\n";
    cout << "6. Найти элемент\n";
    cout << "7. Удалить листья на заданном уровне\n";
    cout << "Введите действие: ";
}

int main() {
    bool alive = false;

    while (true) {
        print_menu();
        int choice;
        cin >> choice;

        switch (choice) {

            case 0:
                deltree(root);
                return 0;

            case 1: {
                unsigned int k;
                string s;
                cout << "Введите число: ";
                cin >> k;
                cout << "Введите информацию: ";
                cin >> s;
                if (!alive) {
                    root = new Tree(k, s);
                    alive = true;
                } else {
                    root = add(root, k, s);
                    if (root) root->mom = NULL;
                }
                break;
            }

            case 2: {
                if (!alive) cout << "Дерево пустое\n";
                else {
                    print_tree(root, 0);
                }
                break;
            }

            case 3: {
                if (!alive) { cout << "Дерево пустое\n"; break; }
                cout << "Выберите вид обхода (1 - симметричный, 2 - прямой, 3 - обратный):\n";
                int t;
                cin >> t;
                if (t < 1 || t > 3) { cout << "Неверный выбор\n"; break; }
                print_table(root, t);
                break;
            }

            case 4: {
                if (!alive) cout << "Дерево уже пустое\n";
                else {
                    deltree(root);
                    root = NULL;
                    alive = false;
                }
                break;
            }

            case 5: {
                if (!alive) { cout << "Дерево пустое\n"; break; }
                unsigned int k;
                cout << "Введите ключ для удаления: ";
                cin >> k;
                root = del_el(root, k);
                if (root) root->mom = NULL;
                if (root == NULL) alive = false;
                break;
            }

            case 6: {
                if (!alive) { cout << "Дерево пустое.\n"; break; }
                unsigned int k;
                cout << "Введите ключ для поиска: ";
                cin >> k;
                if (find(root, k)) cout << "Элемент найден\n";
                else cout << "Элемент не найден\n";
                break;
            }

            case 7: {
                if (!alive) { cout << "Дерево пустое\n"; break; }
                int level;
                cout << "Введите уровень: ";
                cin >> level;
                if (level < 0) { cout << "Уровень не может быть отрицательным\n"; break; }
                int before = count_nodes(root);
                root = delete_leaves_at_level(root, level, 0);
                if (root) root->mom = NULL;
                if (root == NULL) alive = false;
                int after = count_nodes(root);
                cout << "Удалено " << (before - after) << "листьев\n";
                break;
            }

            default:
                cout << "Неверный выбор, попробуйте снова\n";
        }
    }
}