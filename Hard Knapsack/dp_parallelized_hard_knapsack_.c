#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<stdbool.h>
#include<string.h>
#include <windows.h>
#include<math.h>
#include <dos.h>
#include <dir.h>
#define SIZE 100000
//#define binsize 50
char mssg[SIZE];



//**Only condition for knapsack encryption is message must be a multiple of number of knapsack elements.**//

// Function for finding length
int length(char *p){
	int j=0,len=0;
	
	while(p[j]!='\0'){
		len+=1;
		j++;
	}
	
	return len;
}

//Function for Message Encryption using Public Key 
int * Encryption(int n,char *message,int *public_key,int *encrypted,int mssg_length){
	
	//printf("\nMessage length : %d",mssg_length);

	int pos=0,sum,i,j;
	
	for(i=0;i<(int)mssg_length/n;i+=1){
		
		//printf("i value : %d",i);
		sum=0;
		
		j=i*n;

		while(j<(i+1)*n && message[j]!='\0'){
			
			if(message[j]=='1'){
				
				sum+=public_key[j%n];
				//printf("\n%d ",public_key[j%n]);
					
			}
			
			j++;
		
		}	
		
		//printf("Sum is : %d",sum);
		encrypted[pos]=sum;	
		pos++;
		//printf("\n");
	
		
	}
	
	//printf("\n\nEncrypted message is : ");
	
//	i=0;
//	while(encrypted[i]!='\0'){
//		printf("%d ",encrypted[i]);
//		i++;
//	}
	
	
	return encrypted;

}

//Function for finding Multiplicative Inverse during Decryption Process
int find_multiplicative_inverse(int x,int y){
	
	int i;
	
	for(i=1;i<y;i++){
		if(((i*x)%y)==1)
			return i; 
	}
	
	
}


void subsetSum(int arr[],int n,int k){
	
	int i,j;
    
    int elem_pos[n];
    for(i=0;i<n;i++)elem_pos[i]=0;
    
    int m=pow(2,n);
    
    #pragma omp distribute simd
    for(i=m-1;i>0;i--){
        int sum=0,b=i;
        
        for(j=0;j<n;j++){
            sum+=(b%2)*arr[j];
            b=b/2;
        }
        if(sum==k){
            int b=i;
            for(j=0;j<n;j++){
                
                if(b%2)elem_pos[j]=1;//printf("%d ",arr[j]);
                b=b/2;
            }
        }
    }
	
    int original_mssg=0;
    
    for(j=0;j<n;j++){
    	original_mssg=original_mssg+pow(2,j)*elem_pos[n-1-j];
	}
	
	printf("%c",(char)original_mssg);
	
}




// Function for Decryption of Message using Private Key
void Decryption(int *encrypt_pointer,int W,int m,int encrypt_items,int *private_key,int n){
	
	int n_inverse=find_multiplicative_inverse(W,m);
	//printf("\nModulo inverse is : %d , encrypt_items is : %d",n_inverse,encrypt_items);
	printf("\n");
	
	int i=0,decrypt;
	
	while(i<encrypt_items){
		
		decrypt=(encrypt_pointer[i]*n_inverse)%m;
		//printf("Encrypt Pointer : %d",encrypt_pointer[i]);
		subsetSum(private_key,n,decrypt);
		i++;
		
	}

	
}


//Function for generating Private Key
int * generate_private_key(int *arr){
	return arr;
}


//Function for generating Public Key
int * generate_public_key(int *arr,int *public_array,int n,int m,int C){
	
	int i;
	
	#pragma omp parallel for private(i)
	for(i=0;i<n;i++){
		public_array[i]=(arr[i]*C)%m;
	}
	
	return public_array;
}

//Function for checking if given Sequence is Superincreasing or not(***This condition is important for knapsac***)
bool check_superincreasing(int *arr,int n){
	
	int i,j,sum;
	
	for(i=1;i<n;i++){
		sum=0;
		#pragma omp parallel for reduction(+ : sum)
		for(j=0;j<i;j++){
			sum+=arr[j];
		}
		
		if(arr[i]<sum){
			return false; 
		}
	}
	
	return true;
}


//Function for checking Prime
bool isPrime(int x){
	
	int p,flag=0;
	#pragma omp parallel
	for (p=2; p<=x/2; ++p) {
        // condition for non-prime
        if (x % p == 0) {
            flag = 1;
            break;
        }
    }
    
    if(flag==0){
    	return true;
	}
	
	else{
		return false;
	}
}

//Function for checking if Multiplier is compatible with modulus
bool check_multiplier(int m,int C){
	
	if(m>C && m%C!=0 && isPrime(m))
		return true;
	
	return false;
}


//Function for Generating modulus
int generate_modulus(int *arr,int n,int W){
	int i,arr_sum=0;
	
	#pragma omp parallel for reduction(+:arr_sum)
	for(i=0;i<n;i++){
		arr_sum+=arr[i];
	}

	i=arr_sum+1;
		
	while(1){
		if(check_multiplier(i,W)){
			return i;
		}
		i++;	
	}
}

//Function for displaying Public Key
void displaypublickey(int *public_key,int n){
	
	int j;
	printf("\n\nPublic Key is : ");
	for(j=0;j<n;j++){
		printf("%d ",public_key[j]);
	}
}

//Function for displaying Private Key
void displayprivatekey(int *private_key,int n){
	
	int j;
	printf("\n\nPrivate Key is : ");
	for(j=0;j<n;j++){
		printf("%d ",private_key[j]);
	}
}

//Set color of text
void SetColor(int ForgC)
 {
     WORD wColor;

      HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO csbi;

                       //We use csbi for the wAttributes word.
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
                 //Mask out all but the background attribute, and add in the forgournd     color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
 }
 
 
//included this change1
void stringToBinary(char* s) {
    if(s == NULL) return 0; /* no input string */
    size_t len = strlen(s);
    size_t i;int j;
    //char *binary = malloc(len*8 + 1); // each char is one byte (8 bits) and + 1 at the end for null terminator
    mssg[0] = '\0';
    for(i = 0; i < len; ++i) {
        char ch = s[i];
        for(j = 7; j >= 0; --j){
            if(ch & (1 << j)) {
                strcat(mssg,"1");
            } else {
                strcat(mssg,"0");
            }
        }
    }
    //return binary;
}
 
 

int main(void){
	int n;
	int W;
	int i;
	
	SetColor(15);
	
	printf("\t\t\t****PARALLEL 0-1 HARD KNAPSACK FOR PUBLIC KEY ENCRYPTION (Parallel-Time)****");
	printf("\n\nEnter number of elements in Knapsack : ");
	scanf("%d",&n);
	
	int knapsack[n];
	
	printf("\nEnter knapsack elements : ");
	
	for(i=0;i<n;i++){
		scanf("%d",&knapsack[i]);
	}
	
	printf("\nEnter the total weight of the knapsack : ");
	scanf("%d",&W);
	
	char str[SIZE];
	
//	if(check_superincreasing(knapsack,n)){
		
	
		int *private_key = generate_private_key(knapsack);
		//displayprivatekey(private_key,n);		
		
		
		int m = generate_modulus(knapsack,n,W);
		//printf("\n\nModulus is  : %d",m);
		

		int public_array[n];
		int *public_key = generate_public_key(knapsack,public_array,n,m,W);
		//displaypublickey(public_key,n);
		
		
		printf("\n\nEnter message to be encoded : ");
		scanf("%s",str);//unable to use either gets or fgets since it is malfunctioning in c.
		stringToBinary(str);
		//printf("Ascii value of given String is : %s",mssg);
		
		int mssg_length=length(mssg);
		int encrypted[(mssg_length/n)+1];
		
		SetColor(14);
		double start = omp_get_wtime();
		printf("\nEncryption process started");
//		for(i=0;i<5;i++){
//			printf(".");
//			sleep(3);
//		}
		int *encrypt_p=Encryption(n,&mssg,public_key,&encrypted,mssg_length);
		printf("\nEncryption Successfull!!!!");

		
		SetColor(2);
		printf("\n\nDecryption process started");
//		for(i=0;i<5;i++){
//			printf(".");
//			sleep(3);
//		}
		printf("\n\nDecrypted Message is : ");
		Decryption(encrypt_p,W,m,(mssg_length/n),private_key,n);
		printf("\nDecryption Successfull!!!!");
		
		double end = omp_get_wtime( );
		
		double total=end-start;
		printf("Total Time for Parallel Knapsack is : %lf",total);

		SetColor(15);
	
		//("Superincreasing sequence");
		
//	}
	
//	else{
//		printf("Not a Superincreasing sequence");
//	}
//	
	
	return 0;
}

