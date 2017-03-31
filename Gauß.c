#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>


void scanMatrix(FILE *infile, double *mat, int rows, int cols)
{
    int i,j;
    for( j = 0; j < rows; j++) {
        for( i = 0; i < cols; i++) {
            fscanf(infile,"%lf",&(mat[cols * j  + i]));
            //printf("%g",mat[cols*j+i]);
        }
    }
}


double bigabsVal(double* mat, int rows, int whichrow)
{
	if(whichrow>=rows){
		printf("\nNot so many rows!\n");
	}
//------------------------------------------------------
	double abs=0;
	for(int i=rows*whichrow;i<rows*(whichrow+1);i++){
		double act=sqrt(mat[i]*mat[i]);
		if(act>abs){
			abs=act;
		}
	}
	double absperrow=abs;
	//printf("\n%g\n",absperrow);
	return absperrow;
}


void multwithanum(double* mat, int multiplicand, double multiplier, int cols){
	if(multiplicand>=cols){
		printf("\nNot so many cols!\n");
	}
//------------------------------------------------------------
	for(int i=cols*multiplicand;i<((multiplicand+1)*cols);i++){
		mat[i]=mat[i]*multiplier;
		//printf("%g",mat[i]);
	}
}



/*
double baVunderDiag(double* mat, int rows, int whichcol){
	if(whichcol>=rows){
		printf("\nNot so many rows!\n");
	}
//------------------------------------------------------------------
	double abs=sqrt((mat[whichcol*(1+rows)])*(mat[whichcol*(1+rows)]));
	for(int i=(whichcol*(1+rows)+rows);i<(rows*(rows-1)+1+whichcol);i+=rows){
		double act=sqrt(mat[i]*mat[i]);
		if(abs<act){
			abs=act;
		}
	}
	double maxunderdiag=abs;
	printf("%g\n",maxunderdiag);
	return maxunerdiag
}*/


int baVLunderDiag(double* mat, int rows, int whichcol){
	if(whichcol>=rows){
		printf("\nNot so many rows!\n");
	}
//------------------------------------------------------------------
	int Location=(whichcol)*5+whichcol;
	int Locrow=0;
	double abs=sqrt((mat[whichcol*(1+rows)])*(mat[whichcol*(1+rows)]));
	for(int i=(whichcol*(1+rows)+rows);i<(rows*(rows-1)+1+whichcol);i+=rows){
		double act=sqrt(mat[i]*mat[i]);
		if(abs<act){
			abs=act;
			Location=i;
		}
	}

	/*if(abs==0){
		printf("\nSingular matrix detected!!!\n");
		return -1;
	}*/

	for(int i=0;i<rows;i++){
		if(i*5<=Location){
			Locrow=i;
		}	
	}
	
	//double maxunderdiag=abs;
	//printf("%d\n",Location);
	//printf("%d\n",Locrow);
	return Locrow;
}


void maxabsinpart(double* mat, int rows, int whichnow, double pivot){
	if(whichnow>=rows){
		printf("\nNot so many rows!\n");
	}
//-------------------------------------------------------------
	int i=whichnow*(1+rows);
	int counter=1;
	int jump;
	double abs=sqrt((mat[whichnow*(1+rows)])*(mat[whichnow*(1+rows)]));
	while(i<rows*rows){
		jump=1;
		if(counter%(rows-whichnow)==0){
			jump=whichnow+1;
		}	
		i+=jump;
		counter++;
		
		double act=sqrt(mat[i]*mat[i]);
		if(abs<act){
			abs=act;
		}

	}
	pivot=abs;
	printf("%g\n",pivot);

}


void printmatrix(double* mat,int rows,int cols)
{
	int i,j;
    for( j = 0; j < rows; j++) {
        for( i = 0; i < cols; i++) {  
	    printf("%g ",mat[cols*j+i]);          
        }
	printf("\n");
    }
}


void swaprow(double* mat, int rows, int thisrow, int thatrow){
		
	double storage=0;
	for(int i=thisrow*rows;i<(thisrow+1)*rows;i++){
		storage=mat[i];
		mat[i]=mat[(thatrow-thisrow)*rows+i];
		mat[(thatrow-thisrow)*rows+i]=storage;
	}
}


void swapcol(double* mat, int rows, int thiscol, int thatcol, int* colcounter){
	if(thiscol==thatcol){
		printf("You can not swap a row with himself!\n");
	}
//---------------------------------------------------------------------------------	
	int k=0;
	while(k<(rows-1)*2){
		if(colcounter[k]==0){
			break;
		}
		k++;
	} 
	colcounter[k]=thiscol;
	colcounter[k+1]=thatcol;

	//for(int i=0;i<(rows-1)*2;i++){printf("%d",colcounter[i]);}
	
	
	double storage=0;
	int d=thatcol-thiscol;
	for(int i=thiscol;i<(rows*rows-(rows-thatcol-1));i+=rows){
		storage=mat[i];
		mat[i]=mat[i+d];
		mat[i+d]=storage;
	}
}


void divrows(double* mat, int rows, int minuend, int subtrahend, double n/*double* divrow*/){
	for(int i=minuend*rows;i<(minuend+1)*rows;i++){
		mat[/*divrow*/i]=mat[i]-(1/n)*mat[i+((subtrahend-minuend)*rows)];
		//printf("%g",divrow[i]);
		if((sqrt(mat[i]*mat[i])<0.000001)){
			mat[i]=0;
		}
	}
}

//---------------------------------Read matrix---------------------------------	


int main(int argc, char* argv[]){
double* matrix;
double* eye;
double pivot=0;
int rows,cols;
int* colcounter;
/*double* divrow;*/
if(argc!=4){
	fprintf(stderr, "Not the right number of arguments!\n");
	exit(-1);
}
rows=atoi(argv[1]);
cols=atoi(argv[2]);
matrix=(double*)calloc(rows*cols,sizeof(double));
colcounter=(int*)calloc((rows-1)*2,sizeof(int));
/*divrow=(double*)calloc(rows,sizeof(double));*/
eye=(double*)calloc(rows*cols,sizeof(double));

for(int i=0;i<(rows-1)*2;i++){colcounter[i]=0;}

for(int i=0;i<rows*cols;i++){
	if(i%(rows+1)==0){eye[i]=1;}
	else{eye[i]=0;}
}

FILE* f=fopen(argv[3],"r");
scanMatrix(f,matrix,rows,cols);
fclose(f);

//-------------------------------Singularity check-----------------------------------
/*
int n=0;
double det=0;
int jump;
int m;
double det1,det2;
while(n<rows){
	det1=1;
	det2=1;
	m=n;	
	for(m;m<(rows+n);m++){
		if((m+(m-n)*rows+1)%rows==0){
			jump=1;
		}
		else{
		jump=0;
		}	
		det1*=matrix[(m-jump)+((m-jump)-n)*rows+jump];
		printf("%g\n",det1);
		det2=det2*matrix[m+rows*(cols-1)-((m+jump)-n)*rows];
		printf("\n%g\n",det2);
	}
	det+=det1-det2;
	printf("\n%g\n",det);
	n+=1;
}

//printf("\n%g\n",det);

	if(det==0){
		printf("\nSingular matrix detected!!!\n");
		return -1;
	}
*/
//---------------------------------Invert matrix-------------------------------------
int k=0;
while(k < cols){
	double a=bigabsVal(matrix,rows,k);
	multwithanum(matrix,k,1/a,cols);
	multwithanum(eye,k,1/a,cols);
	k++;
}


int l=0;
int y=0;
double s;
for(int i=0;i<rows*cols;i+=rows+1){
	int Loc=baVLunderDiag(matrix,rows,l);
	swaprow(matrix,rows,l,Loc);
	swaprow(eye,rows,l,Loc);
	s=1/matrix[i];
	multwithanum(matrix,y,s,cols);
	multwithanum(eye,y,s,cols);
	double num;

	for(int k=0;k<rows-y-1;k++){
		num=matrix[i]/matrix[i+rows*(k+1)];
		divrows(matrix,rows,y+1+k,y,num);
		divrows(eye,rows,y+1+k,y,num);		
	}
	y++;
	l++;	
}


/*
double num;
int y=0;
for(int i=0;i<rows*cols;i+=rows+1){
	multwithanum(matrix,y,1/matrix[i],cols);
	multwithanum(eye,y,1/matrix[i],cols);
	for(int k=0;k<rows-y-1;k++){
		num=matrix[i]/matrix[i+rows*(k+1)];
		printmatrix(matrix,rows,cols);
		divrows(matrix,rows,y+1+k,y,num);
		divrows(eye,rows,y+1+k,y,num);		
	}
	y++;
	
}
*/

int q=rows-1;
double num;
for(int i=rows*cols-1;i>0;i-=(rows+1)){
	for(k=0;k<q;k++){
		num=matrix[i]/matrix[i-rows*(k+1)];
		divrows(matrix,rows,q-k-1,q,num);
		divrows(eye,rows,q-k-1,q,num);		
	}
	q--;
	
}
;

//printf("\n");
//printmatrix(matrix,rows,cols);
printf("\n");
printmatrix(eye,rows,cols);
printf("\n");
/*multwithanum(matrix,2,2,cols);
bigabsVal(matrix,rows,absperrow,2);
baVunderDiag(matrix,rows,3,maxunderdiag);
printmatrix(matrix,rows,cols);
maxabsinpart(matrix,rows,2,pivot);
swaprow(matrix,rows,2,3);
printmatrix(matrix,rows,cols);
swapcol(matrix,rows,2,4,colcounter);
swapcol(matrix,rows,0,2,colcounter);
printmatrix(matrix,rows,cols);
divrows(matrix,rows,2,3,/*divrow*//*);*/
return 0;
}
