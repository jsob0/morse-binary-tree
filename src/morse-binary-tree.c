#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* The structure stores ascii letter and its variation in morse code. */
struct Letter {
    char ascii;
    char *morse;
};

/* Node structure is the building block of a binary tree, each node has two
branches left and right and a data element */
struct Node {
    struct Letter *letter;
    struct Node *left;
    struct Node *right;
};

/* The maximum length of a Morse code letter. */
#define MORSE_CODE_MAX_SIZE 5

/* A simple alphabet array stores ascii letters and digits. */
struct Letter ALPHABET[] = {
    {'a', ".-"},
    {'b', "-..."},
    {'c', "-.-."},
    {'d', "-.."},
    {'e', "."},
    {'f', "..-."},
    {'g', "--."},
    {'h', "...."},
    {'i', ".."},
    {'j', ".---"},
    {'k', "-.-"},
    {'l', ".-.."},
    {'m', "--"},
    {'n', "-."},
    {'o', "---"},
    {'p', ".--."},
    {'q', "--.-"},
    {'r', ".-."},
    {'s', "..."},
    {'t', "-"},
    {'u', "..-"},
    {'v', "...-"},
    {'w', ".--"},
    {'x', "-..-"},
    {'y', "-.--"},
    {'z', "--.."},
    {'0', "-----"},
    {'1', ".----"},
    {'2', "..---"},
    {'3', "...--"},
    {'4', "....-"},
    {'5', "....."},
    {'6', "-...."},
    {'7', "--..."},
    {'8', "---.."},
    {'9', "----."}
};

struct Letter LETTER_NULL = { 0, 0 };

/* Number of letters in the alphabet array. */
const size_t ALPHABET_SIZE = sizeof(ALPHABET) / sizeof(ALPHABET[0]);

const char *message_help = "\
Usage: %s \"MORSE CODE\"\n\
\n\
Translate Morse code into Ascii text using a binary tree.\n\
\n\
For more information or to report bugs, add me on discord sopa#7417\n";

const char *message_error_morse_code = "\nError in the %d letter, such a \
letter does not exist.\n";

/* Exit the program with an error message. */
void exit_with_error(const char *message, ...) {
    va_list ap;

    va_start(ap, message);
    vfprintf(stderr, message, ap);
    va_end(ap);

    exit(EXIT_FAILURE);
}

/* Create a new node with the specified letter. */
struct Node *create_node(struct Letter *letter) {
    struct Node *node = NULL;
    node = (struct Node *)malloc(sizeof(struct Node));
    if (NULL == node) {
        exit_with_error("Failed to allocate memory.\n");
    }

    node->letter = letter;
    node->left = node->right = NULL;

    return node;
}

/* Release the whole binary tree. */
void release_tree(struct Node *node) {
    if (NULL == node)
        return;

    release_tree(node->left);
    release_tree(node->right);

    free(node);
}

/* Create an empty perfect binary tree of the specified height. */
struct Node *create_empty_perfect_binary_tree(int height) {
    if (height < 0)
        return NULL;

    struct Node *root = create_node(&LETTER_NULL);
    root->left = create_empty_perfect_binary_tree(height - 1);
    root->right = create_empty_perfect_binary_tree(height - 1);

    return root;
}

void _insert_node(struct Node *node, struct Letter *letter, int i) {
    if (strlen(letter->morse) == i)
        node->letter = letter;

    else {
        // <- o
        if ('.' == letter->morse[i])
            _insert_node(node->left, letter, i + 1);
        // o ->
        else if ('-' == letter->morse[i])
            _insert_node(node->right, letter, i + 1);
    }
}

/* Insert a node into the tree. */
void insert_node(struct Node *node, struct Letter *letter) {
    _insert_node(node, letter, 0);
}

/* Create a perfect binary tree with the Morse code alphabet. */
struct Node *create_morse_code_tree(void) {
    struct Node *root = NULL;
    root = create_empty_perfect_binary_tree(MORSE_CODE_MAX_SIZE);

    for (size_t i = 0; i < ALPHABET_SIZE; i++)
        insert_node(root, &ALPHABET[i]);

    return root;
}

char _search_ascii_letter(struct Node *node, char *morse, int i) {
    if (morse == node->letter->morse)
        return node->letter->ascii;

    else if (strlen(morse) != i) {
        // <- o
        if ('.' == morse[i])
            return _search_ascii_letter(node->left, morse, i + 1);
        // o ->
        else if ('-' == morse[i])
            return _search_ascii_letter(node->right, morse, i + 1);
    }

    return node->letter->ascii;
}

/* Search the Ascii letter in the tree by passing the Morse code. */
char search_ascii_letter(struct Node *node, char *morse) {
    return _search_ascii_letter(node, morse, 0);
}

int main(int argc, char **argv) {

    // Check that user has provided two arguments (program path, morse code).
    if (2 != argc) {
        exit_with_error(message_help, argv[0]);
    }

    struct Node *root = NULL;
    root = create_morse_code_tree();
    
    int i = 0;
    char *morse = strtok(argv[1], " ");
    while (NULL != morse) {
        // Check that the Morse code is within the maximum size.
        if (strlen(morse) > MORSE_CODE_MAX_SIZE)
            exit_with_error(message_error_morse_code, i + 1);

        char ascii = search_ascii_letter(root, morse);

        if (!strcmp("/", morse)) putchar(' ');
        else if (0 != ascii) putchar(ascii);
        else
            exit_with_error(message_error_morse_code, i + 1);

        morse = strtok(NULL, " ");
        i++;
    }

    release_tree(root);
    return 0;
}