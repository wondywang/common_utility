#include <stdio.h>
#include <setjmp.h>

jmp_buf env;
jmp_buf env2;
void foo() {
	int i = 100;
	int a = 200;
	printf("[1]before jmp\n");
	int ret = setjmp(env);
	printf("[2]begin setjmp(1)\n");
	if(ret == 0) {
		a = 30;
		printf("[3]a=%d, i=%d, return 0!!!!\n", a, i);
	} else {
		printf("[4]a=%d, i=%d, return %d\n", a, i, ret);
		longjmp(env2,  888);
	}
	printf("[5]a=%d, i=%d, after jmp\n", a, i);
	return;
}
int main() {
	int a = 20;
	foo();
	printf("[6]a=%d, after foo()\n", a);
	int ret = setjmp(env2);
	printf("[7]begin setjmp(2)\n");
	if(ret == 0) {
		longjmp(env, 999);
		printf("[8]init setjmp\n");
	} else {
		printf("[9]a=%d, from longjmp()\n", a);
	}
	printf("[10]a=%d, after longjmp()\n", a);
	return 0;
}
