int c = 92;
int main(){
	int a = 4;
	int b = a;
	if(b == 0){
		a = b + 1;
	} else {
		b = a + 1;
	}
	int arr[10];
	arr[3] = 5;
	foo(c);
	return a;
}
int foo(int a){
	c += 2;
	int* temp = &c;
	char *d =(char*)temp;
	return 9;
}
void bar(){
	return;
}
