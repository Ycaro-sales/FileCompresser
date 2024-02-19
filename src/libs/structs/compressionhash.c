#include "compressionhash.h"
#include "huffmantree.c"
#include "huffmantree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void _hufftree_getPaths(struct huffman_node *node,
                               CompressedHash *compressed_hash, char *path) {
        if (isLeaf(node)) {
                compressed_hash->paths[node->character] = path;
        } else {
                char *tmpRight = malloc(sizeof(path) + sizeof(char));
                char *tmpLeft = malloc(sizeof(path) + sizeof(char));

                strncpy(tmpRight, path, sizeof(path) + sizeof(char));
                strncpy(tmpLeft, path, sizeof(path) + sizeof(char));

                _hufftree_getPaths(node->left, compressed_hash,
                                   strcat(tmpLeft, "0"));
                _hufftree_getPaths(node->right, compressed_hash,
                                   strcat(tmpRight, "1"));
        }
}

CompressedHash *hufftree_getPaths(struct huffman_tree *tree) {
        CompressedHash *tmp = malloc(sizeof *tmp);

        char *path = "";

        _hufftree_getPaths(tree->root, tmp, path);

        return tmp;
}
