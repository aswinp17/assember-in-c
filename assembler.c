#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int nxt_program_address = 0;
int labelindex = 0;
int nxt_memory_adddress = 16;
int userdefvarindex  =  0;
//table structure
struct table {
 char jump[20];
 char n[20];
};


//tables*****************************************************************************
struct table labeltable[20] = {
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
};


struct table user_def_var[20] = {

{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
{"0","0"},
};



struct table  jump[] = { 
   {"null" ," 000"},
   { "JGT" , "001"},
   {"JEQ" , "010"},
   { "JGE" , "011"},
   { "JLT" , "100"},
   { "JNE" , "101"},
   {"JLE" , "110"},
   {"JMP" , "111"} };

struct table dest[] = {
    {"null" , "000"},
    {"M" , "001"},
    {"D" , "010"},
    {"MD" , "011"},
    {"A" , "100"},
    {"AM" , "101"},
    {"AD" , "110"},
    {"AMD" , "111"}
                };
struct table comp[] = {  
    {"0" , "101010"},
    {"1" , "111111"},
    {"-1" , "111010"},
    {"D" , "001100"},
    {"A" , "110000"},
    {"M" , "110000"},
    {"!D" , "001101"},
    {"!A" , "110001"},
    {"!M" , "110001"},
    {"-D" , "001111"},
    {"-A" , "110011"},
    {"-M" , "110011"},
    {"D+1" , "011111"},
    {"1+D" , "011111"},
    {"1+A" , "110111"},
    {"A+1" , "110111"},
    {"1+M" , "110111"},
    {"M+1" , "110111"},
    {"1-D" , "001110"},
    {"D-1" , "001110"},
    {"1-A" , "110010"},
    {"A-1" , "110010"},
    {"1-M" , "110010"},
    {"M-1" , "110010"},
    {"D+A" , "000010"},
    {"A+D" , "000010"},
    {"D+M" , "000010"},
    {"M+D" , "000010"},
    {"D-A" , "010011"},
    {"A-D" , "010011"},
    {"D-M" , "010011"},
    {"M-D" , "010011"},
    {"A-D" , "000111"},
    {"D-A" , "000111"},
    {"M-D" , "000111"},
    {"D-M" , "000111"},
    {"D&A" , "000000"},
    {"A&D" , "000000"},
    {"D&M" , "000000"},
    {"M&D" , "000000"},
    {"D|A" , "010101"},
    {"A|D" , "010101"},
    {"D|M" , "010101"},
    {"M|D", "010101"} };


struct table predef[] = {
    {"SP" , "0"},
    {"LCL" , "1"},
    {"ARG" , "2"},
    {"THIS" , "3"},
    {"THAT" , "4"},
    {"R0" , "0"},
    {"R1" , "1"},
    {"R2" , "2"},
    {"R3" , "3"},
    {"R4" , "4"},
    {"R5" , "5"},
    {"R6" , "6"},
    {"R7" , "7"},
    {"R8" , "8"},
    {"R9" , "9"},
    {"R10" , "10"},
    {"R11" , "11"},
    {"R12" , "12"},
    {"R13" , "13"},
    {"R14" , "14"},
    {"R15" , "15"},
    {"SCREEN" , "16384"},
    {"KBD" , "24576"}
};
//**************************************************************************************************
void reverse(char s[])
{
  int c, i, j;
  for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}






/* itoa: convert n to characters in s */
void itoaa(int n, char s[])
{
  int i, sign;
  if ((sign = n) < 0) /* record sign */
  n = -n;
  /* make n positive */
  i = 0;
  do {
  /* generate digits in reverse order */
    s[i++] = n % 10 + '0'; /* get next digit */
    } while ((n /= 10) > 0);
  /* delete it */
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}


//function to  strip  white spaces
char*  strip(char text[]) {

  int d = 0, c = 0;
  char *blank;
  blank = (char *) malloc( 20 *  sizeof(char));




  while (text[c] != '\0')
   {
      if (!(text[c] == ' ' && text[c+1] == ' ')) {
        blank[d] = text[c];
        d++;
      }
      c++;
   }
  // printf("reaccchhed");
   blank[d] = '\0';
   return   blank;
}

// function  to check comments
int notcomment(char s[])
{
  if(s[0] == '/' && s[1] == '/')
   return  1 ;


}


//search  in table
int tablesearch(struct table* a, char* s, int  size) {
 
  int len  = size / sizeof(struct table);
  for (int i = 0; i < len ; i++) {
      if( (strcmp(a[i].jump,s)) == 0 )
      return 1;
      }
  return 0;
}



//return value t0 corresponding key


char* tablevalue(struct table* a, char* s,int size) {
  int len  = size / sizeof(struct table);
  
  for (int i = 0; i < len ; i++) {
    if( (strcmp(a[i].jump,s) == 0 ))
    return a[i].n ;
  }
}   



//function to check if string is a digit
int digit(char c[])  {
 
 int i = 0,k = 0;
 for (i = 0 ; c[i]  != '\0';  i++){
    if(isdigit(c[i])) 
        k = 1;
    else {
        k = 0;
        break;
          }
      }
 return k;
}



//convert int to binary15
char *  binary(unsigned int num)
{
unsigned int mask=16384;   //mask = [1000 0000 0000 0000]
//printf("Binary Eqivalent : ");
char a[15], *c;
c = (char *) malloc(15* sizeof(char));
int i  =  0;
 
while(mask > 0)
   {
   if((num & mask) == 0 ){
         //printf("0");
         c[i] = '0';
        //strcpy(c[i],0);
         mask = mask >> 1 ; 
         i++;
   }
   else{
         //printf("1");
         c[i] ='1';
       // strcpy(c[i],"1");     
  mask = mask >> 1 ;  // Right Shift
  i++;
   } 
 }
 return c;
}

//firstpass
void firstpass(char s[]) {
  char c[20];
  int labelsize;
  char *string;
  labelsize = sizeof(labeltable);
  char *token = strtok(s, " (=-)");
  string = &token[0];
  if (s[0] == '(')  {
    if(!(tablesearch(labeltable,string,labelsize))){
       strcpy(labeltable[labelindex].jump, string);
       itoaa(nxt_program_address,c);
       strcpy(labeltable[labelindex].n ,c);
       labelindex += 1;
       }
    }
  nxt_program_address += 1;
}     


//secondpass

void secondpass(char s[]) {
  FILE *fp = fopen("output.hack", "a");
  char command[20];
  strcpy(command,s);
  char *token = strtok(s, " ;(=-@\n");
  char *desti, *compu, *jumpu, a[17], b[16],c[16];
  int n;
  char *string;
  int compsize;
  int destisize;
  int jumpsize;
  int predefsize;
  int labelsize;
  int userdefsize;
  compsize = sizeof(comp);
  destisize = sizeof(dest);
  jumpsize = sizeof(jump);
  predefsize = sizeof(predef);
  labelsize = sizeof(labeltable);
  userdefsize = sizeof(user_def_var);
  if(s[0] == '(')
     return;
  if(strchr(command,'=')) {
     compu = tablevalue(comp ,&token[2],compsize);
     desti = tablevalue(dest ,token,destisize); 
     
     //a = "111";
     strcpy(a,"111");
     //b = "000\n";
     strcpy(b,"000\n");
     strcat(a,compu);
     strcat(a,desti);
     strcat(a,b);
     fprintf(fp,"%s",a);
     fclose(fp);
     return;
     }
  
  if(strchr(command,';')) {
     compu = tablevalue(comp ,token,compsize);
     jumpu = tablevalue(jump ,&token[2],jumpsize); 
     
     //a = "111";
     strcpy(a,"111");
     //b = "000";
     strcpy(b,"000");
     strcat(a,compu);
     strcat(a,b);
     strcat(a,jumpu);
     strcat(a,"\n");
     fprintf(fp,"%s",a);
     fclose(fp);
     return;
     }
  
  if(strchr(command,'@')) {
     
    if(digit(token)){
      n = atoi(token);
      //b  = binary(n);
      string = binary(n);
      strcpy(b,string);
      strcpy(a,"0");
      strcat(a,b);
      strcat(a,"\n");
      fprintf(fp,"%s",a);
      fclose(fp);
      return;
      }
   
    if(tablesearch(labeltable,token,labelsize)) {
      //b = tablevalue(labeltable,token[0]);
      string = &token[0];
      strcpy(b,tablevalue(labeltable,string,labelsize));
      n = atoi(b);
      strcpy(b,binary(n));
      strcpy(a,"0");
      strcat(a,b);
      strcat(a,"\n");
      fprintf(fp,"%s",a);
      fclose(fp);
      return;
      }
    
    if(tablesearch(predef,token,predefsize) ){
     // b = tablevalue(predeftable,token[0]);
      
      strcpy(b,tablevalue(predef,token,predefsize));
      n = atoi(b);
      strcpy(b,binary(n));
      strcpy(a,"0");
      strcat(a,b);
      strcat(a,"\n");
      fprintf(fp,"%s",a);
      fclose(fp);
      return;
      }

    if(tablesearch(user_def_var,token,userdefsize)) {
      //b = tablevalue(user_def_var,token[0]);
      strcpy(b,tablevalue(user_def_var,token,userdefsize));
      n = atoi(b);
      strcpy(b,binary(n));
      strcpy(a,"0");
      strcat(a,b);
      strcat(a,"\n");
      fprintf(fp,"%s",a);
      fclose(fp);
      return;
      }
    strcpy(user_def_var[userdefvarindex].jump ,token);
    itoaa(nxt_memory_adddress,c);
   strcpy(user_def_var[userdefvarindex].n,c);
    
    strcpy(a,"0");
    //b  = binary(nxt_memory_adddress);
    strcpy(b,binary(nxt_memory_adddress));
    strcat(a,b);
    strcat(a,"\n");
    fprintf(fp,"%s",a);
    fclose(fp);
    nxt_memory_adddress += 1;
    userdefvarindex += 1;
    return;  
  }
  else return;





}



// function to remove \n at end of line

void remove_new_line(char* string)
{
  size_t length = strlen(string);
  while(length>0)
  {
       if(string[length-1] == '\n')
       {
                --length;
                string[length] ='\0';
       }
       else
           break;
  }

}























//main function  usage::: ./a.out input file name
int main (int argc, char* argv[]) {
   FILE *fp;
   char str[60];
   char a[100];
   char s[60];
   
   /* opening file for reading */
   fp = fopen(argv[1] , "r");
   if(fp == NULL) {
      perror("Error opening file");
      return(-1);
   }
   //first pass
   printf("\n first pass"); 
   while( fgets (str, 60, fp)!=NULL ) {
       remove_new_line(str);
       if(    strcpy(s, strip(str)) ){
           if(notcomment(s)) {
               firstpass(s);
            }
        }
      }
        
 fclose(fp);
 
 // printf("\n finishhed 1st pasas"); 
  fp = fopen("a.txt" , "r");
//  printf("\n file opend 2");
/*  if(fp == NULL) {
      perror("Error opening file");
      return(-1);
  }*/
   //second pass
  printf("\n second  pass");  
  while( fgets (str, 60, fp)!=NULL ) {
      
         remove_new_line(str);
       if(  strcpy(s, strip(str))) {
           if(notcomment(s)) {
               secondpass(s);
            }
        }
       }




  printf("\n the output  machinecodde save in output.hack  ");






   
  
   
   fclose(fp);
   
   return(0);
}


