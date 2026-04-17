#include<stdio.h>
int gcd(int a, int b){
	if(b==0){
		return a;
	}
	else{
		return gcd(b,a%b);
	}
}
int inv(int to_inverse, int mod_by){
	int i = 1;
	while(i<mod_by){
		if(((i*to_inverse)%mod_by) == 1){
			return i;
		}
		i++;
	}
}
int main(){
	int a,b;
	printf("Enter two numbers :");
	scanf("%d%d",&a,&b);
	int x = gcd(a,b);
	if(x==1){
		printf("The inverse is : %d",inv(a,b));
	}
	else{
		printf("Modular Inverse not Possible");
	}
}
