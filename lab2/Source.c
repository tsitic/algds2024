#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

struct treaps {
    int key;
    int val;
    struct treaps* left;
    struct treaps* right;
};

struct treaps* rhtgRotation(struct treaps* y) {
    struct treaps* x = y->left;
    struct treaps* T2 = x->right;
    x->right = y;
    y->left = T2;
    return x;
}

struct treaps* lftRotation(struct treaps* x) {
    struct treaps* y = x->right;
    struct treaps* T2 = y->left;
    y->left = x;
    x->right = T2;
    return y;
}

struct treaps* newNode(int key) {
    struct treaps* temp = (struct treaps*)malloc(sizeof(struct treaps));
    temp->key = key;
    temp->val = rand() % 100;
    temp->left = temp->right = NULL;
    return temp;
}

struct treaps* insert(struct treaps* root, int key) {
    if (!root)
        return newNode(key);

    if (key <= root->key) {
        root->left = insert(root->left, key);

        if (root->left && root->left->val > root->val)
            root = rhtgRotation(root);
    }
    else {
        root->right = insert(root->right, key);

        if (root->right && root->right->val > root->val)
            root = lftRotation(root);
    }
    return root;
}

struct treaps* join(struct treaps* left, struct treaps* right) {
    if (!left) return right;
    if (!right) return left;

    if (left->val > right->val) {
        left->right = join(left->right, right);
        return left;
    }
    else {
        right->left = join(left, right->left);
        return right;
    }
}

void split(struct treaps* root, int key, struct treaps** left, struct treaps** right) {
    if (!root) {
        *left = *right = NULL;
    }
    else if (key <= root->key) {
        split(root->left, key, left, &root->left);
        *right = root;
    }
    else {
        split(root->right, key, &root->right, right);
        *left = root;
    }
}

struct treaps* delNode(struct treaps* root, int key) {
    if (!root)
        return root;

    if (key < root->key)
        root->left = delNode(root->left, key);
    else if (key > root->key)
        root->right = delNode(root->right, key);
    else {
        struct treaps* temp = join(root->left, root->right);
        free(root);
        return temp;
    }
    return root;
}

void printTreap(struct treaps* root, int k) {
    if (root == NULL) {
        return;
    }

    printTreap(root->right, k + 1);

    for (int i = 0; i < k; i++) {
        printf("\t");
    }
    printf("%d(%d)\n", root->key, root->val);

    printTreap(root->left, k + 1);
}


int compTreaps(struct treaps* first, struct treaps* second) {
    if (first == NULL && second == NULL) {
        return 1;
    }
    if (first == NULL || second == NULL) {
        return 0;
    }
    if (first->key != second->key) {
        return 0;
    }
    if (compTreaps(first->left, second->left) && compTreaps(first->right, second->right)) {
        return 1;
    }
    return 0;
}



void testing() {
    struct TestCase {
        struct treaps* tree;
        int key;
        char* description;
        struct treaps* afterdel;
        struct treaps* afteradd;
    };

    struct TestCase tests[] = {
        {NULL, 10, "Дерево без элементов",NULL, newNode(10)},
        {newNode(5), 5, "Дерево с одним элементом",NULL, newNode(5)},
        {insert(newNode(5),10),10,"Дерево с двумя элементами",newNode(5),insert(newNode(5),10)},
        {insert(insert(newNode(5), 3), 7), 5, "Дерево с тремя элементами",insert(newNode(3),7),insert(insert(newNode(5), 3), 7)},
    };

    int numTests = sizeof(tests) / sizeof(tests[0]);

    printf("Начало тестирования:\n");

    for (int i = 0; i < numTests; ++i) {
        printf("\nТест %d: %s\n", i + 1, tests[i].description);

        // Удаление 
        if (tests[i].tree) {
            printf("Удаление элемента: ", tests[i].key);
            tests[i].tree = delNode(tests[i].tree, tests[i].key);
            if (compTreaps(tests[i].tree,tests[i].afterdel)) {
                printf("успешно\n");
            }
            else {printf("неуспешно\n"); }
        }

        // Вставка
        printf("Добавление элемента: ", tests[i].key);
        tests[i].tree = insert(tests[i].tree, tests[i].key);
        if (compTreaps(tests[i].tree,tests[i].afteradd)) {
            printf("успешно\n");
        }
        else {
            printf("неуспешно\n");
        }

        struct treaps* left = NULL;
        struct treaps* right = NULL;
        split(tests[i].tree, tests[i].key, &left, &right);
        if (i == 0) {
            printf("Сцепление и расцепление невозможно\n");
            continue;
        }
        if (/*left && right*/ compTreaps(join(left,right),tests[i].afteradd)) {
            printf("Расцепление успешно\n");
            printf("Сцепление успешно\n");
        }
        else {
            printf("Расцепление неуспешно\n");
            printf("Сцепление неуспешно\n");
        }
    }

    printf("\nДЛЯ ДЕРЕВА С ТРЕМЯ ЭЛЕМЕНТАМИ РАБОТАЕТ НЕ ВСЕГДА ТАК КАК ПРИОРИТЕТ ЗАДАЁТСЯ СЛУЧАЙНО\n И ДАЖЕ ЕСЛИ ЕГО ПРЕДСКАЗЫВАТЬ, ТО ВСЁ РАВНО В ВИДУ ТОГО ЧТО ТЕСТЫ ДЛЯ СРАВНЕНИЯ СОЗДАЮТСЯ ПОСЛЕ СОЗДАНИЯ ДЕРЕВА\n ПРИОРИТЕТ МОЖЕТ ОТЛИЧАТЬСЯ\n");
    printf("\nТестирование завершено.\n");
}




int main() {
    setlocale(LC_CTYPE, "Russian");
    srand(time(NULL));

    struct treaps* root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("Дерево после вставок:\n\n");
    printTreap(root, 0);

    printf("\n\nУдаляем элемент с ключом 20\n\n");
    root = delNode(root, 20);

    printf("\nДерево после удаления:\n\n");
    printTreap(root, 0);

    struct treaps* left = NULL;
    struct treaps* right = NULL;

    printf("\n\nРасцепляем дерево по ключу 40\n\n");
    split(root, 40, &left, &right);

    printf("\n\n\nЛевое дерево:\n");
    printTreap(left, 0);

    printf("\n\n\nПравое дерево:\n");
    printTreap(right, 0);

    printf("\n\nСцепляем левое и правое деревья\n\n");
    root = join(left, right);

    printf("\n\nДерево после сцепления:\\n");
    printTreap(root, 0);

    testing();
    return 0;
}
