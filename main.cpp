#include <cstdio>
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include <malloc.h>

void fill_array(uint8_t size, uint8_t** array){
	for (uint8_t i = 0; i < size; i++){
		for (uint8_t j = 0; j< size; j++){
			array[i][j] = rand() % 255;
		}
	}
};

void fill_zero_array(uint8_t size, uint8_t** array){
	for (uint8_t i = 0; i < size; i++){
		for (uint8_t j = 0; j < size; j++){
			array[i][j] = 0;
		}
	}
};

void print_array(uint8_t size, uint8_t** array){
	printf("-------------------------------array-------------------------\n");
	for (uint8_t i = 0; i < size; i++){
		printf("\t");
		for (uint8_t j = 0; j < size; j++){
			printf("%d ", array[i][j]);
		}
		printf("\n");
	}
};

uint8_t** get_array(uint8_t size){
	uint8_t** array;
	array = (uint8_t**) calloc(size, sizeof(*array));
	for (uint8_t i=0; i<size; i++){
		array[i] = (uint8_t*) calloc(size, sizeof(*array[i]));
	}
	return array;
};

int main()
{
	srand(time(0));
	uint8_t array_orig_size = rand()%12 + 3;
	uint8_t array_cut_size = array_orig_size - 1;
	uint8_t** array_orig;
	uint8_t** array_cut;
	uint8_t del_col = rand() % array_cut_size;
	uint8_t del_str = rand() % array_cut_size;
	printf("cuttet col: %d\n cottet str %d\n", del_col, del_str);
    	array_orig = get_array(array_orig_size);
	array_cut = get_array(array_cut_size);
	fill_array(array_orig_size, array_orig);
	print_array(array_orig_size, array_orig);
	fill_zero_array(array_cut_size, array_cut);
	print_array(array_cut_size, array_cut);
	uint8_t* array_orig_start = &array_orig[0][0];
	uint8_t* array_orig_end = &array_orig[array_orig_size-1][array_orig_size-1];
	uint8_t* array_cut_start = &array_cut[0][0];
	uint8_t* array_cut_end = &array_cut[array_cut_size-1][array_cut_size-1];
	asm(
	"movl %[source_start], %%esi\n\t"
	"movl %[dest_start], %%edi\n\t"
	"xor %%ecx, %%ecx\n\t"
	"xor %%edx, %%edx\n\t"
	"xor %%eax, %%eax\n\t"
	"loop_start:\n\t"
	"cmp %[del_str], %%dl\n\t"
	"je pass_str\n\t"
	"next_col:\n\t"
	"cmp %[del_col], %%dh\n\t"
	"je pass_col\n\t"
	"movb (%%esi), %%al\n\t"
	"movb %%al, (%%edi)\n\t"
	"incl %%esi\n\t"
	"incl %%edi\n\t"
	"jmp compare\n\t"
	"pass_col:\n\t"
	"incl %%esi\n\t"
	"compare:\n\t"
	"cmp %[size], %%dh\n\t"
	"je next_str\n\t"
	"incb %%dh\n\t"
	"jmp next_col\n\t"
	"next_str:\n\t"
	"cmp %%edi, %[dest_end]\n\t"
	"jl end\n\t"
	"xorb %%dh, %%dh\n\t"
	"incb %%dl\n\t"
	"xor %%eax, %%eax\n\t"
	"movb $16, %%al\n\t"
	"movb %[size], %%cl\n\t"
	"subb %%cl, %%al\n\t"
	"addl %%eax, %%edi\n\t"
	"xor %%eax, %%eax\n\t"
	"movb $16, %%al\n\t"
	"movb %[orig_size], %%cl\n\t"
	"subb %%cl, %%al\n\t"
	"addl %%eax, %%esi\n\t"
	"jmp loop_start\n\t"
	"pass_str:\n\t"
	"addl $16, %%esi\n\t"
	"jmp next_col\n\t"
	"end:\n\t"
	:
	:[source_start]"m"(array_orig_start),[source_end]"m"(array_orig_end),[dest_start]"m"(array_cut_start),[dest_end]"m"(array_cut_end),[del_col]"m"(del_col),[del_str]"m"(del_str),[size]"m"(array_cut_size),[orig_size]"m"(array_orig_size)
	:"%esi", "%edi", "%ecx", "%eax", "%edx", "cc", "memory"
	);
	print_array(array_cut_size, array_cut);
	return 0;
}
