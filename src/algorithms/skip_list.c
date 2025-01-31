/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:02:23 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 20:54:00 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/algorithms/skip_list.h"

static int random_level(void) {
	int level = 1;
	while ((rand() / (double)RAND_MAX) < SKIP_LIST_P && level < MAX_LEVEL) {
		level++;
	}
	return level;
}

static SkipNode *create_node(int level, int key, void *value) {
	SkipNode *node = malloc(sizeof(SkipNode));
	node->forward  = malloc(sizeof(SkipNode *) * (level + 1));
	node->key	   = key;
	node->value	   = value;
	node->level	   = level;
	return node;
}

SkipList *skip_list_create(void) {
	SkipList *list = malloc(sizeof(SkipList));
	list->level	   = 1;
	list->size	   = 0;
	list->header   = create_node(MAX_LEVEL, -1, NULL); // Set value to NULL
	for (int i = 0; i <= MAX_LEVEL; i++) {
		list->header->forward[i] = NULL;
	}
	return list;
}

void skip_list_destroy(SkipList *list) {
	SkipNode *current = list->header;
	while (current) {
		SkipNode *next = current->forward[0];
		free(current->forward);
		free(current);
		current = next;
	}
	free(list);
}

int skip_list_insert(SkipList *list, int key, void *value) {
	SkipNode *update[MAX_LEVEL + 1];
	SkipNode *current = list->header;

	for (int i = list->level - 1; i >= 0; i--) {
		while (current->forward[i] && current->forward[i]->key < key) {
			current = current->forward[i];
		}
		update[i] = current;
	}
	current = current->forward[0];

	if (current && current->key == key) {
		current->value = value;
		return 0;
	}

	int new_level = random_level();
	if (new_level > list->level) {
		for (int i = list->level; i < new_level; i++) {
			update[i] = list->header;
		}
		list->level = new_level;
	}

	SkipNode *new_node = create_node(new_level, key, value);
	for (int i = 0; i < new_level; i++) {
		new_node->forward[i]  = update[i]->forward[i];
		update[i]->forward[i] = new_node;
	}
	list->size++;
	return 1;
}

SkipNode *skip_list_search(SkipList *list, int key) {
	SkipNode *current = list->header;

	for (int i = list->level - 1; i >= 0; i--) {
		while (current->forward[i] && current->forward[i]->key < key) {
			current = current->forward[i];
		}
	}

	current = current->forward[0];
	if (current && current->key == key) {
		return current;
	}
	return NULL;
}

int skip_list_delete(SkipList *list, int key) {
	SkipNode *update[MAX_LEVEL + 1];
	SkipNode *current = list->header;

	for (int i = list->level - 1; i >= 0; i--) {
		while (current->forward[i] && current->forward[i]->key < key) {
			current = current->forward[i];
		}
		update[i] = current;
	}
	current = current->forward[0];

	if (!current || current->key != key) {
		return 0;
	}

	for (int i = 0; i < list->level; i++) {
		if (update[i]->forward[i] != current) {
			break;
		}
		update[i]->forward[i] = current->forward[i];
	}

	while (list->level > 1 && list->header->forward[list->level - 1] == NULL) {
		list->level--;
	}

	free(current->forward);
	free(current);
	list->size--;
	return 1;
}

void skip_list_print(SkipList *list) {
	for (int i = list->level - 1; i >= 0; i--) {
		SkipNode *node = list->header->forward[i];
		printf("Level %d: ", i);
		while (node) {
			printf("%d -> ", node->key);
			node = node->forward[i];
		}
		printf("NULL\n");
	}
}