int c = 92;
int main(){
	int a = 4;
	int b = a;
	if(b == 0){
		a = b + 1;
	} else {
		b = a + 1;
	}
	return a;
}
int foo(){
	c += 2;
	return 9;
}
void bar(){
	return;
}
