/*
 * 2BitToMultiFasta.c
 *
 *
 *
 *
 *
*/

#include <stdio.h>
#include <stdlib.h>

int bitSeparator(char byt, FILE * ofp){
		int i;
                        for(i = 3; i>0; --i){
                                char temp = byt & ('\x03' << (2*i));//get nucleotide from byte
                                temp =  (temp >> (i*2)-1)&'\x06';//shift to position in ascii char and make sure no ones were carried
                                if(!(temp^'\x04')){//if temp is a T, not A,G,C
                                        temp = 'T';
                                }
                                else{
                                        temp = temp | '\x41';   //convert to ascii char
                                }
                                fprintf(ofp, "%c", temp);
                        }
                        byt = byt & '\x03';//get last nucleotide
                        byt = byt << 1;//shift to position in ascii char
                        if(!(byt^'\x04')){//if temp is a T, not A,G,C
                                byt = 'T';
                        }
                        else{
                                byt = byt | '\x41';     //convert to ascii char
                        }
                        fprintf(ofp, "%c", byt);
//                      fprintf(ofp, "%c", '\n');
		return 0;	
		}



int twoBit(const char * filename,const char * outFileName){
	FILE * ifp;
	FILE * ofp;
	ifp = fopen(filename,"r");
//	char * lineptr = NULL;
//	size_t * len = 0;
//	getline(&lineptr,len, ifp);
//	char newFileName[100];
//	printf("Give a name to the file this program will create and write to:" );
//	gets(newFileName);
	ofp = fopen(outFileName, "w");
	char flinec;
	while((flinec = getc(ifp)) != '\n'){
		fprintf(ofp, "%c", flinec);
	}	
	fprintf(ofp, "%c", '\n');
	char byt;
	int eof = 0;//not end of file
	char spacearray[8]; // Checking for spaces signifying new sequence
	while(!eof){
		int s = 0;
		byt = getc(ifp);
		if((~ byt)){//check if eof or all Gs

		while(byt == ' '){
		spacearray[s] = byt;
		s++;
		byt = getc(ifp);
		}
		if(s!=8){
		int j;
			for(j=0; j<s; ++j){
			bitSeparator(spacearray[s],ofp);
			}	
		}
		else{
			char newSeq;
			while((newSeq = getc(ifp))!= '\n'){
				fprintf(ofp,"%c",newSeq);
			}

		}
		


	bitSeparator(byt, ofp); // helped function for commented-out code below





//			int i;
//			for(i = 3; i>0; --i){
//				char temp = byt & ('\x03' << (2*i));//get nucleotide from byte
//				temp =  (temp >> (i*2)-1)&'\x06';//shift to position in ascii char and make sure no ones were carried
//				if(!(temp^'\x04')){//if temp is a T, not A,G,C
//					temp = 'T';
//				}
//				else{
//					temp = temp | '\x41';	//convert to ascii char
//				}
//				fprintf(ofp, "%c", temp);
//			}
//			byt = byt & '\x03';//get last nucleotide
//			byt = byt << 1;//shift to position in ascii char
//			if(!(byt^'\x04')){//if temp is a T, not A,G,C
//				byt = 'T';
//			}
//			else{
//				byt = byt | '\x41';	//convert to ascii char
//			}
//			fprintf(ofp, "%c", byt);
//			fprintf(ofp, "%c", '\n');
		}else{
			int i=1;
			char checkeof;
			char eofarr[30];
			eofarr[0] = byt;
			while(!((checkeof=getc(ifp)) ^ '\xff') && i < 30){//extract up to 30 bytes of -1
			//	checkeof = getc(ifp);
				eofarr[i]=checkeof;
				++i;
			}

			if(i!=30){
				int k=0;
				for(k;k<i;++k){
				//	ungetc(eofarr[i],ifp);	//if a non -1 byte was extracted undo extraction of last i bytes
				
				bitSeparator(eofarr[k],ofp);
					
				}
				bitSeparator(checkeof, ofp);
			}else{
				eof = 1;//if 30 -1 bytes were extracted end of file is true
			}
		}
	}
	fclose(ofp);
	fclose(ifp);
//	free(lineptr);
	return 0;
}

int main(int argc, char *argv[]){
	if(argc == 3){
		twoBit(argv[1],argv[2]);
		return 0;
        }else{
		 return 1;
	}
 }
