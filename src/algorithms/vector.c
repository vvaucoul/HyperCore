/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:16:57 by vvaucoul          #+#    #+#             */
/*   Updated: 2025/01/30 12:32:06 by vvaucoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib/algorithms/vector.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 1 // Capacité initiale par défaut

int vector_init(Vector *vector, int initial_capacity) {
	if (initial_capacity <= 0) {
		initial_capacity = INITIAL_CAPACITY;
	}

	vector->capacity = initial_capacity;
	vector->size	 = 0;
	vector->data	 = malloc(initial_capacity * sizeof(void *));

	if (!vector->data) {
		// Si l'allocation échoue, retourne une erreur
		fprintf(stderr, "Error: Failed to allocate memory for vector\n");
		return -1;
	}
	return 0;
}

int vector_resize(Vector *vector) {
	// Si la capacité actuelle est 0, initialisez avec INITIAL_CAPACITY
	if (vector->capacity == 0) {
		vector->capacity = INITIAL_CAPACITY;
	} else {
		// Sinon, doublez la capacité
		vector->capacity *= 2;
	}

	// Allouez de nouvelle mémoire
	void *new_data = realloc(vector->data, vector->capacity * sizeof(void *));

	if (!new_data) {
		// Si l'allocation échoue, retourne une erreur
		fprintf(stderr, "Erreur: Impossible d'agrandir le vecteur\n");
		return -1;
	}

	vector->data = new_data;
	return 0;
}

int vector_push_back(Vector *vector, const void *element, size_t element_size) {
	// Si le tableau est plein, redimensionnez-le
	if (vector->size >= vector->capacity) {
		if (vector_resize(vector) != 0) {
			return -1;
		}
	}

	// Copiez l'élément dans la mémoire allouée
	void *new_element = malloc(element_size);
	if (!new_element) {
		fprintf(stderr, "Erreur: Impossible d'allouer la mémoire pour l'élément\n");
		return -1;
	}
	memcpy(new_element, element, element_size);

	// Placez l'élément dans le tableau
	void **array		= (void **)vector->data;
	array[vector->size] = new_element;

	vector->size++;
	return 0;
}

int vector_insert(Vector *vector, int index, const void *element, size_t element_size) {
	if (index < 0 || index > vector->size) {
		fprintf(stderr, "Erreur: Index hors limites pour insertion\n");
		return -1;
	}

	if (vector->size >= vector->capacity) {
		if (vector_resize(vector) != 0) {
			return -1;
		}
	}

	void **array = (void **)vector->data;
	// Shift elements to the right
	for (int i = vector->size; i > index; i--) {
		array[i] = array[i - 1];
	}

	void *new_element = malloc(element_size);
	if (!new_element) {
		fprintf(stderr, "Erreur: Impossible d'allouer la mémoire pour l'élément\n");
		return -1;
	}
	memcpy(new_element, element, element_size);
	array[index] = new_element;

	vector->size++;
	return 0;
}

int vector_erase(Vector *vector, int index) {
	if (index < 0 || index >= vector->size) {
		fprintf(stderr, "Erreur: Index hors limites pour suppression\n");
		return -1;
	}

	void **array = (void **)vector->data;
	free(array[index]);

	// Shift elements to the left
	for (int i = index; i < vector->size - 1; i++) {
		array[i] = array[i + 1];
	}

	vector->size--;
	return 0;
}

int vector_clear(Vector *vector) {
	void **array = (void **)vector->data;
	for (int i = 0; i < vector->size; i++) {
		free(array[i]);
	}
	vector->size = 0;
	return 0;
}

int vector_empty(Vector *vector) {
	return vector->size == 0;
}

void *vector_front(Vector *vector) {
	if (vector->size == 0) {
		fprintf(stderr, "Error: Cannot access front of empty vector\n");
		return NULL;
	}
	return vector_get(vector, 0);
}

void *vector_back(Vector *vector) {
	if (vector->size == 0) {
		fprintf(stderr, "Error: Cannot access back of empty vector\n");
		return NULL;
	}
	return vector_get(vector, vector->size - 1);
}

void *vector_get(Vector *vector, int index) {
	if (index < 0 || index >= vector->size) {
		// Si l'index est hors des limites, affichez une erreur
		fprintf(stderr, "Error: Index out of bounds [%d] (size: %d)\n", index, vector->size);
		return NULL;
	}

	void **array = (void **)vector->data;
	return array[index];
}

int vector_size(Vector *vector) {
	return vector->size;
}

int vector_capacity(Vector *vector) {
	return vector->capacity;
}

void vector_destroy(Vector *vector) {
	// Libérez chaque élément alloué
	if (vector->data != NULL) {
		void **array = (void **)vector->data;
		for (int i = 0; i < vector->size; i++) {
			free(array[i]);
		}
		free(vector->data);
		vector->data = NULL;
	}
	vector->capacity = 0;
	vector->size	 = 0;
}