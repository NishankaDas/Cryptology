#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void capital(char a[]){
    int i = 0;
    for(i = 0; i<26; i++){
        a[i] = 'A' + i;
    }
}

void small(char a[]){
	int i = 0;
    for(i = 0; i<26; i++){
        a[i] = 'a' + i;
    }
}

int main(){
	
    char alpha[26], ALPHA[26], name[50000], *enc;
    int x,i,j,pos,key,cipher;
    
    FILE *file1, *file2;
    
    capital(ALPHA); 
    small(alpha);
    
    file1 = fopen("enc.txt","r");
    if(file1 == NULL){
    	printf("Error 404 File not Found");
    	return 0;
	}
	
	file2 = fopen("out.txt", "w");
	if(file2 == NULL){
		printf("Error in ENC file opening");
		fclose(file1);
		return 0;
	}
	
	
    printf("Enter the Key: ");
	scanf("%d",&key);
    while(fgets(name, sizeof(name), file1)){
    	
	    x = strlen(name);
	    enc = (char *)malloc(sizeof(char)*x);
	    
	    
	    
	    for(i = 0; i<x; i++){
	    	if(name[i] >= 'a' && name[i] <= 'z'){
	    		pos = name[i] - 'a';
	    		cipher = (pos - key + 26)%26;
	    		enc[i] = alpha[cipher];
			}
			else if(name[i]>=65 && name[i]<97){
				pos = name[i] - 'A';
	    		cipher = (pos - key + 26)%26;
	    		enc[i] = ALPHA[cipher];
			}
			else{
				enc[i] = name[i];
			}	
		}
		
		enc[i] = '\0';
        fputs(enc, file2);
        free(enc);
	}
	fclose(file1);
	fclose(file2);
	printf("Decryption DONE");
	return 0;
}
