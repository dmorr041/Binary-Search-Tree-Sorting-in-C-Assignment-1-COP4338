/*
 *	I affirm that this is my original work and not the work of anyone else.
 *
 *	Darien Quinn Morrison 3380082
 *
 *	COP 4338 
 *
 *	Binary Search Tree Sorting. We have a node structure for leafs in the tree,
 *	two string compare functions for placing strings in the correct locations 
 *	in the tree(one case sensitive, the other case insensitive), an insert 
 *	function to build the tree, a print function that uses inorder traversal
 *	to print the lines in the tree in the correct order, a delete tree function 
 *	to reclaim memory used in building the tree, and command line parsing using 
 *	the getopt function. 
 *
 *
 * */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
 *	Case sensitive string comparison function. Compares two strings for
 *	equality.
 *
 *	Returns negative value if s1 < s2 (alphabetically higher)
 *		positive value if s1 > s2 (alphabetically lower)
 *		zero if s1 == s2 (equal strings)
 * */
int str_cmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		++s1;
		++s2;
	}
	
	return (*s1 - *s2);
}

/*
 *	Case insensitive string comparison function. Compares two strings
 *	for equality. Converts the strings to uppercase first.
 *
 *	Returns negative value if s1 < s2 (alphabetically higher)
 *		positive value if s1 > s2 (alphabetically lower)
 *		zero if s1 == s2 (equal strings)
 * */
int str_cmp2(char const *s1, char const *s2)
{
	for(;; s1++, s2++)
	{
		int d = toupper(*s1) - toupper(*s2);

		if(d != 0 || !*s1)
			return d;
	}
}


/*
 *	Tree node structure.
 *
 *	Nodes have a string value, a count, and left and right children nodes 
 *	(stored as pointers)
 * */
struct node
{
	char value[100];	
	int count;		
	struct node* left;	
	struct node* right;	
};

/*
 *	Insert function for BST.
 *	If the caseflag is set, we use the str_cmp function we defined,
 *	which compares case sensitive. Otherwise, we use the other 
 *	string compare function. 
 *
 *	If the tree does not exist yet, we create a new node using 
 *	malloc to allocate memory for the node, and initialize the 
 *	values of the node. 
 *
 *	If the tree is not empty, we compare the string with the value
 *	in each node, starting at the root. If the string matches what's
 *	in a node, we increase the count for that node. Otherwise we 
 *	search the left or right subtrees based on the value returned in
 *	our string compare function. 
 *
 * */
insert(char key[100], struct node **leaf, int caseflag)
{
	if(caseflag == 1)
	{
		if(*leaf == NULL)
		{
			*leaf = (struct node*)malloc(sizeof(struct node));
			strcpy((*leaf) -> value, key);
			(*leaf) -> count = 1;
			(*leaf) -> left = NULL;
			(*leaf) -> right = NULL;
		}
		else if(str_cmp(key, (*leaf) -> value) == 0)
		{
			(*leaf) -> count++;
		}
		else if(str_cmp(key, (*leaf) -> value) > 0)
		{
			insert(key, &(*leaf) -> right, caseflag);
		}
		else if(str_cmp(key, (*leaf) -> value) < 0)
		{
			insert(key, &(*leaf) -> left, caseflag);
		}
	}
	else
	{
		if(*leaf == NULL)
		{
			*leaf = (struct node*)malloc(sizeof(struct node));
			strcpy((*leaf) -> value, key);
			(*leaf) -> count = 1;
			(*leaf) -> left = NULL;
			(*leaf) -> right = NULL;
		}
		else if(str_cmp2(key, (*leaf) -> value) == 0)
		{
			(*leaf) -> count++;
		}
		else if(str_cmp2(key, (*leaf) -> value) > 0)
		{
			insert(key, &(*leaf) -> right, caseflag);
		}
		else if(str_cmp2(key, (*leaf) -> value) < 0)
		{
			insert(key, &(*leaf) -> left, caseflag);
		}
	}	
}

/*
 *	Function to perform an inorder traversal of the BST to print
 *	out the lines of the input file in the correct order.
 *	
 *	Inorder traversal: Left, Node, Right
 *
 *	Prints duplicates using the node's count. Takes an output file
 *	pointer (either stdout or an output file) as a parameter.
 * */
void inorder_print(struct node *leaf, FILE * out)
{
	int temp;

	if(leaf != NULL)
	{
			inorder_print((leaf -> left), out);
			
			for(temp = 0; temp < (leaf -> count); temp++)
			{
				fprintf(out, "%s\n", (leaf -> value));
			}
			
			inorder_print((leaf -> right), out);		
	}
}

/*
 *	Function to reclaim the BST from memory (delete the tree
 *	from memory) using postorder traversal and free function.
 * */
void delete_tree(struct node *leaf)
{
	if(leaf != NULL)
	{
		delete_tree(leaf -> left);
		delete_tree(leaf -> right);
		free(leaf -> value);
		free(leaf);
	}
}













int main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int c, err = 0;
	int cflag, oflag = 0;	// Flags for the command line options

	// Usage statement
	static char usage[] = "Usage: bstsort [-c] [-o output_file_name] [input_file_name]";

	FILE * input = stdin;		// Input set to standard input by default
	FILE * output = stdout;		// Output set to standard output by default

	char contents[100];		// String buffer we'll store the contents of input in
	char * sentence;		// String pointer we'll store the sentences in (actual size of the sentence)

	struct node * root = NULL;		// Reference to the root. Initialized to 
					// 0 so other functions can recognize 
					// that the tree does not yet exist.


	// Command line options
	while ((c = getopt(argc, argv, "co:")) != -1)
		switch (c) {
		case 'c':
			cflag = 1;
			break;
		case 'o':
			oflag = 1;
			output = fopen(optarg, "w");
			break;
		case '?':
			printf("%s", usage);
			break;
		}




	// If we have a possible input file...
	if(argv[optind] != NULL)
	{
		// Open a new file in read mode
		input = fopen(argv[optind], "r");

		// If the file given doesn't exist...
		if(input == NULL)
		{
			printf("File not found.\n");	// Print file not found
			exit(0);	
		}
	}

	// While we have lines to read...
	while(fgets(contents, sizeof(contents), input) != NULL)
	{ 
		// Dynamically allocate memory for the sentences
		sentence = (char *)malloc(strlen(contents));
		
		// Copy the sentence from the buffer to the string pointer
		strcpy(sentence, contents);
		
		// Insert the strings into the tree
		insert(sentence, &root, cflag); 
	}

	// Use an inorder traversal to print the list out in the correct order, 
	// to the correct output location (either stdout or an output file)
	inorder_print(root, output);

	// Reclaim the tree from memory
	delete_tree(root);					
}
