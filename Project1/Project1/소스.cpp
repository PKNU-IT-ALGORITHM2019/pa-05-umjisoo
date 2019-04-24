#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX 1000
#define BUFFER_LENGTH 100

typedef struct Node {
	char word[MAX];
	char form[MAX];
	char mean[MAX];
	Node *left;
	Node *right;
	Node *P;
}Node;

Node *root = NULL;

int count = 0;

Node* search(Node *p, char* data);
void insert(Node *root, Node*tmp);
void add();
void size();
void inorder(Node *p);
void remove(Node *p);
void load();
void Delete(char* tmp);
void deleteall(char* tmp);
void find(char* tmp);
void remove_blank(char* buffer, int i);
Node* successor(Node* p);
void process_command();
int main() {
	
	process_command();

	return 0;

}
void process_command() {
	load();
	char command[MAX], tmp[BUFFER_LENGTH],buffer[BUFFER_LENGTH];
	while (1) {
		printf("$ ");
		scanf(" %s", command);
		if (strcmp(command, "find") == 0) {
			scanf("%s", tmp);
			find(tmp);
		}
		else if (strcmp(command, "size") == 0)size();
		else if (strcmp(command, "add") == 0) add();
		else if (strcmp(command, "delete") == 0) {
			scanf("%s", tmp); Delete(tmp);
		}
		else if (strcmp(command, "deleteall") == 0) {
			scanf("%s", tmp);
			deleteall(tmp);
		}
		else if (strcmp(command, "exit") == 0) break;
	}
}
void Delete(char* tmp) {
	Node* p;
	p = search(root, tmp);
	if (p == NULL) printf("Not found.\n");
	else {
		remove(p);
		printf("Deleted successfully.\n");
	}
}
void deleteall(char* tmp) {
	int dele_count = 0;
	char buffer[BUFFER_LENGTH];
	FILE*de_fp = fopen(tmp, "r");
	while (!feof(de_fp)) {
		fscanf(de_fp,"%s",buffer);
		Node* dele = search(root, buffer);
		if (dele != NULL) {
			remove(dele);
			dele_count++;
		}
	}
	fclose(de_fp);
	printf("%d words were deleted successfully.\n", dele_count);
}
Node* newNode(char* data) {
	Node* n_Node= (Node*)malloc(sizeof(Node));
	strcpy(n_Node->word, data);
	n_Node->left = NULL;
	n_Node->right = NULL;
	n_Node->P = NULL;
	return n_Node;
}
void load() {
	char check;
	char buffer[MAX], command[MAX];
	char *token;
	FILE*in_fp = fopen("shuffled_dict.txt", "r");

	while (!feof(in_fp)) {
		if (fgets(buffer, MAX, in_fp) <= 0)
			continue;
		count++;
		token = strtok(buffer, "(");
		int i = 0;
		if (!isalpha(buffer[0])) {	
			remove_blank(buffer, i);
		}
		else {	
			buffer[strlen(buffer) - 1] = '\0';
		}
		Node* tmp=newNode(token);
		token = strtok(NULL, ")");
		if (token == NULL) strcpy(tmp->form, "\0");
		else strcpy(tmp->form, token);
		token = strtok(NULL, "\n");
		if (token == NULL) {
			strcpy(tmp->mean, tmp->form);
			strcpy(tmp->form, "\0");
		}
		else strcpy(tmp->mean, token);
		
		if (root == NULL) {
			root = tmp;
		}
		else insert(root, tmp);
	}
	fclose(in_fp);

}

void size() {
	printf("%d\n", count);
}
void insert(Node *p, Node*tmp) {

	if (strcmp(p->word, tmp->word) > 0) {
		if (p->left == NULL) { p->left = tmp; tmp->P = p; }
		else insert(p->left, tmp);
	}
	else if (strcmp(p->word, tmp->word) < 0) {
		if (p->right == NULL) { p->right = tmp; tmp->P = p; }
		else insert(p->right,tmp);
	}


}
void find(char* tmp) {
	Node *p;
	p = search(root, tmp);
	if (p != NULL) printf("word: %s\n   class: %s\n   mean: %s\n",p->word,p->form, p->mean);
	else printf("Not found.\n");

}
Node* successor(Node* p) {

	if (p->left == NULL)
		return p;
	else return successor(p->left);

}
Node* search(Node *p, char* data) {

	if (strcmp(p->word, data) == 0)
		return p;
	else if (strcmp(p->word, data) < 0) {
		if (p->right == NULL) return NULL;
		return search(p->right, data);
	}
	else {
		if (p->left == NULL) return NULL;
		return search(p->left, data);
	}

}

void add() {
	char word_tmp[BUFFER_LENGTH];
	char buffer[MAX];

	printf("word: ");
	scanf("%s", word_tmp);
	Node* tmp = newNode(word_tmp);

	printf("class: ");
	gets_s(buffer, MAX);
	strcpy(tmp->form, buffer);

	printf("meaning: ");
	gets_s(buffer, MAX);
	strcpy(tmp->mean, buffer);


	insert(root, tmp);
}
void remove(Node *p) {

	if (p->left == NULL && p->right == NULL) {
		if (strcmp(p->P->word, p->word) < 0)
			p->P->right = NULL;
		else p->P->left = NULL;
	}
	else if (p->left == NULL && p->right != NULL) {
		if (strcmp(p->P->word, p->word) < 0)
			p->P->right = p->right;
		else p->P->left = p->right;
	}
	else if (p->left != NULL && p->right == NULL) {
		if (strcmp(p->P->word, p->word) < 0)
			p->P->right = p->left;
		else p->P->left = p->left;
	}
	
	else {
		Node *delete_Node = successor(p->right);
		strcpy(p->word, delete_Node->word);
		strcpy(p->form, delete_Node->form);
		strcpy(p->mean, delete_Node->mean);
		remove(delete_Node);
	}
}
void remove_blank(char* buffer,int i) {
	while (1) {
		buffer[i] = buffer[i + 1];
		if (buffer[i + 1] == ' ') {
			buffer[i] = '\0';
			break;
		}
		i++;
	}
}