#include<bits/stdc++.h>
using namespace std;
struct Opcode{
        char name[10];
        char code[35];
        char format[5];
        struct Opcode *next;
};
struct Symbol{
    char name[50];
    int add;
    struct Symbol *next;
};


Symbol *head=NULL;

Opcode* hash_table[13] = {NULL};

int* Binary(int num){
    int temp=num;
    int *arr=new int[10]();
    for(int i=9;i>=0;i--){
        arr[i]=temp%2;
        temp/=2;
    }
    return arr;
}

char* MCode(int dec) {
     char *str = new char[5];
     for(int i=4;i>=0;i--){
        str[i]=(dec%2)+'0';
        dec/=2;
     }
     return str;
}

int getHashIndex(char name[]){
    int sum=0,i=0;
    while(name[i]!='\0'){
        sum+=name[i++];
    }
    return sum%13;
}
void insertAtIndex(Opcode *Node,int index){
    if(hash_table[index] == NULL){
        hash_table[index] = Node;
        Node->next = NULL;
    }
    else{
        Opcode* temp = hash_table[index];
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = Node;
        Node->next=NULL;
    }
}
void insertIntoHashMap(Opcode *Node){
    int index = getHashIndex(Node->name);
    insertAtIndex(Node,index);
}
int * getAddressCode(char * temp){
    Symbol * t = head;
    int * val;
    int num;
    while(t != NULL){
        if(!strcmp(temp,t->name)){
            num = t->add;
            break;
        }
        t = t->next;
    }
    val =Binary(num);
    return val;
}
char * findRegister(char* temp){
    char *s;
    if (strcmp(temp,"R0") == 0){
        s = "000";
    }
    else if (strcmp(temp,"R1") == 0)
        s = "001";
    else if (strcmp(temp,"R2") == 0)
        s = "010";
    else if (strcmp(temp,"R3") == 0)
        s = "011";
    else if (strcmp(temp,"R4") == 0)
        s = "100";
   return s;
}
char *getConstantCode(int temp){
    return MCode(temp);
}
Opcode* getOpcodeNode(char *op){
    Opcode* temp = NULL;
    int index = getHashIndex(op);
    if(hash_table[index] == NULL){
        printf("Wrong Opcode");
        return NULL;
    }
    else{
        temp = hash_table[index];
        while(strcmp(temp->name,op)!=0 && temp!=NULL){
            temp = temp->next;
        }
        if(temp == NULL){
            printf("Opcode not found!");
            return NULL;
        }
        else{
            return temp;
        }
    }
}
char * getOpcodeFormat(Opcode* temp){
    return temp->format;
}
int main(){
    FILE *input_opcode;
    FILE *output_machine_code;
    FILE *input_instructions;
    int ilc=0;
    int base = 0;
    char c,c2,c3,temp;
    char opcode[100];
    char machine_code[100];
    char format[5];
    base = 0;
    input_opcode = fopen("input_opcode.txt","r+");
    if (input_opcode == NULL)
        printf("FILE OPENING PROBLEM");
    do{
        c = fscanf(input_opcode,"%s",opcode);
        c2= fscanf(input_opcode,"%s",machine_code);
        c3= fscanf(input_opcode,"%s",format);
        Opcode* Node=new Opcode();
        strcpy(Node->name,opcode);
        strcpy(Node->code,machine_code);
        strcpy(Node->format,format);
        insertIntoHashMap(Node);
    }while(c!=EOF && c2!=EOF && c3!=EOF);
    int i=0;
    for(i =0;i<13;i++){
        if(hash_table[i]!=NULL){
        Opcode* temp = hash_table[i];
            while(temp!=NULL){
                temp = temp->next;
            }
        }
    }
    input_instructions = fopen("input_instructions.txt","r+");
    output_machine_code = fopen("output_machine_code.txt","w+");
    char k;
    char op[100];
    while ( fgets ( op, sizeof op, input_instructions ) != NULL ){
       int l=0;
        while(op[l+1]!='\0'){
            if(op[l]==':'){
           Symbol *t;
           Symbol *temp = new Symbol();
           int i=0;
           for(;i<l;i++)
                temp->name[i] = op[i];
            temp->name[i] = '\0';
            temp->add = ilc + 1 + base;
            temp->next = NULL;
           if(head == NULL)
                head = temp;
           else{
                t= head;
                while(t->next!=NULL)
                    t= t->next;
                t->next = temp;
           }
       }
       l++;
     }
     ilc++;
 }
   fclose(input_instructions);
   input_instructions = fopen("input_instructions.txt","r+");
   int * binary;
   int count;
    do{
       k=fscanf(input_instructions,"%s",op);
       int l=0;
       while(op[l+1]!='\0'){
           l++;
       }
       if(op[l]==':'){
            fprintf(output_machine_code,"\n");
    }
       else{
           char temp[100];
           char temp2[100];
           char temp3[100];
           int temp4;
            Opcode* current_node = getOpcodeNode(op);
            fprintf(output_machine_code,"%s",current_node->code);

            if (strcmp("z",getOpcodeFormat(current_node))==0){
                fprintf(output_machine_code,"\n");
            }
            else if(strcmp("a",getOpcodeFormat(current_node))==0){
                k = fscanf(input_instructions,"%s",temp);
                binary = getAddressCode(temp);
                for(count=0;count<10;count++){
                    fprintf(output_machine_code,"%d",binary[count]);
                }
                fprintf(output_machine_code,"\n");
            }
            else if(strcmp("rrr",getOpcodeFormat(current_node))==0){
                k = fscanf(input_instructions,"%s",temp);
                k = fscanf(input_instructions,"%s",temp2);
                k = fscanf(input_instructions,"%s",temp3);
                fprintf(output_machine_code,"%s",findRegister(temp));
                fprintf(output_machine_code,"%s",findRegister(temp2));
                fprintf(output_machine_code,"%s",findRegister(temp3));
                fprintf(output_machine_code,"\n");
            }
       }
    }while(k!=EOF);
    fclose(input_instructions);
    fclose(output_machine_code);
    fclose(input_opcode);

    Symbol *p;
    p=head;
    FILE *f = fopen("symbol_table.txt","w+");
    printf("successful");
    while(p!=NULL){
        fprintf(f,"%s => ",p->name);
        fprintf(f,"%d\n",p->add);
        p = p->next;
    }
    return 0;
}
