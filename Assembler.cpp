#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>

using namespace std;

fstream outfile;
fstream infile;
fstream op;

char data[400];
char opcode[200];
char mnemonic[10],type[10], prevt[10];
int lc,op_no,i,co=1,flag=0,lc1=0,j,k,math,tadr=0,ctr=0,pool[10],p,end_flag=0,tar_idx=0,tar_idx2=0, flag_s=0;

int t=0; //for token counter
int n=1; //for symbol table counter
int m=1; //for literal table counter
int pl=1; //for pool table counter
int l=0; // flag for LTORG

struct symtab	//For symbol table
{
	int no,add;
	char name[10];
};

struct temptab	//For intermediate code table
{
	char name[10],name1[10];
	int code;
};

struct constab	//For constant table
{
	int add;
	char name[10];
};

struct littab	//For literal table
{
	int no,add;
	char name[10];
};

struct targettab
{
	int lc_add,op_code,reg_no,addr;
	char l_s[30];
};

struct temptab tab[15];
struct constab con[10];
struct symtab sys[100];
struct littab lit[100];
struct targettab tar[100];

void load()
{
	infile.open("p2.txt");
	cout<<"file opened for reading"<<endl;

	infile>>data;
	cout<<"---------------------------------------"<<endl;
	cout<<" content: "<<data<<endl;
	if(infile.peek() == '\n' )
	{
		lc=0;
	}
	else
	{
		infile>>lc;
		cout<<"LC = "<<lc<<endl;
	}

	outfile.open("p1out.txt");

	outfile<<"(AD, 1)  (c,"<<lc<<")";
	cout<<"(AD, 1)  (c,"<<lc<<")";

	//cout<<"file opened for writing"<<endl;
	//cout<<"file written"<<endl;

	while(!infile.eof())
	{
		infile>>data;
	//	cout<<"data\t"<<data<<endl;

		op.open("OPCODE1.txt");
	//	cout<<"Opcode file opened for reading"<<endl;
		while(!op.eof())
		{
		//	cout<<"---------------------------"<<endl;
		//	cout<<"OPCODE content :"<<opcode<<endl;
		    op>>mnemonic;
			op>>type;
			op>>op_no;

			/* Fill intermediate code table by comparing input file with opcode file */
			if(strcmp(data,mnemonic) == 0)
			{
				tab[t].code = op_no;
				strcpy(tab[t].name1,mnemonic);
				strcpy(tab[t].name,type);
				t++;			// increment table location
				flag = 1;
				break;
			}
		}
		op.close();		// closed opcode file

	    if(strcmp(data,"=") == 0)
			flag = 1;
		if(strcmp(data,"+") == 0 || strcmp(data,"-") == 0)
		{
			flag = 1;
			strcpy(tab[t].name1,data);
			t++;
		}

		if(strcmp(prevt,"+") == 0 || strcmp(prevt,"-") == 0	|| strcmp(prevt,"DS") == 0)
		{
			tab[t].code = atoi(data);
			strcpy(tab[t].name1,data);
			strcpy(tab[t].name,"c");
			strcpy(tar[tar_idx2].l_s,"c");
			if(strcmp(prevt,"DS") == 0)
			{
				tar[tar_idx].addr = tab[t].code;
			}
			tar_idx2++;
			t++;
			flag = 1;
		}
		/*for(i = pl;i < m;i++)
			{
			if(strcmp(lit[i].name,data) == 0)
				{
				tab[t].code = i;
				strcpy(tab[t].name1,data);
				strcpy(tab[t].name,"l");
				t++;
				flag = 1;
				goto a;
				}
			}*/
		if(strcmp(prevt,"DC") == 0)
		{
			flag = 1;
			con[co].add = lc;
			strcpy(con[co].name,data);
			tab[t].code = m;
			strcpy(tab[t].name1,data);
			strcpy(tab[t].name,"c");
			strcpy(tar[tar_idx2].l_s,"c");
			tar_idx2++;
			t++;
			co++;
		}
		if(strcmp(prevt,"=") == 0)
		{
			flag = 1;
			lit[m].no = m;
			strcpy(lit[m].name,data);
			tab[t].code = m;
			strcpy(tab[t].name1,data);
			strcpy(tab[t].name,"l");
			strcpy(tar[tar_idx2].l_s,"l");
			tar_idx2++;
			t++;
			m++;
		}
a:		for(i = 1;i < n;i++)
		{
			if(strcmp(sys[i].name,data) == 0)
			{
				tab[t].code = i;
				strcpy(tab[t].name1,data);
				strcpy(tab[t].name,"s");
				if(t == 0)
				{
					sys[i].add = lc;
				}

				t++;
				/*strcpy(tar[tar_idx2].l_s,"s");
				cout<<tar_idx2<<": S\n";
				tar_idx2++;
				flag_s = 1;*/
				flag = 1;
				break;
			}
		}
		if(flag == 0)
		{
			sys[n].no = n;
			strcpy(sys[n].name,data);
			sys[n].add = lc;
			tab[t].code = n;
			strcpy(tab[t].name1,data);
			strcpy(tab[t].name,"s");
			strcpy(tar[tar_idx2].l_s,"s");
			tar_idx2++;

			t++;
			n++;
		}

		if(infile.peek() == '\n' )
		{
			outfile<<"\n";
			cout<<"\n";
			for(i = 0;i < t;i++)
			{
				if(strcmp(tab[i].name,"AD") == 0)
				{
					lc1 = 1;
					tar[tar_idx].lc_add = 0;
					tar_idx++;

					if(!(strcmp(prevt,"+") == 0 || strcmp(prevt,"-") == 0))
					{
						tar_idx2++;
					}

				}
			}
			if(lc1 == 0)
			{
				tar[tar_idx].lc_add = lc;
				outfile<<lc<<")";
				cout<<lc<<")\t";
				tar_idx++;

			}
			for(i = 0;i < t;i++)
			{
				if(strcmp(tab[i].name1,"+") == 0)
				{
					cout<<" ";
					continue;
				}
				if(strcmp(tab[i].name1,"-") == 0)
				{
					cout<<"\t";
					continue;
				}
				if( strcmp(tab[i].name, "IS") == 0)
				{
					tar[tar_idx].op_code = tab[i].code;
				}
				if( strcmp(tab[i].name, "R") == 0)
				{
					tar[tar_idx].reg_no = tab[i].code;
				}
				if( strcmp(tab[i].name, "-") == 0)
				{
					tar[tar_idx].reg_no = tab[i].code;
				}

				cout<<"("<<tab[i].name<<","<<tab[i].code<<")\t";
				outfile<<"("<<tab[i].name<<","<<tab[i].code<<")";

				if(strcmp(data,"LTORG") == 0)
				{
					for(k = 1; k<m; k++)
					{
						lit[k].add = lc;
						tar[tar_idx].lc_add = lc;
						cout<<"\n"<<lc<<")\t(l,"<<lit[k].no<<")";
						lc++;
						tar[tar_idx].addr = atoi(lit[k].name);
						tar_idx++;
						tar_idx2++;
					}

					cout<<endl;
					end_flag = m;
					l=1;
					p++;
					pool[1] = lit[1].no;

				}

				if(strcmp(data,"END") == 0)
				{
					for(k = end_flag; k<m; k++)
					{
						lit[k].add = lc;
						tar[tar_idx].lc_add = lc;
						cout<<"\n"<<lc<<")\t(l,"<<lit[k].no<<")";
						lc++;
						tar[tar_idx].addr = atoi(lit[k].name);
						tar_idx++;
						tar_idx2++;
					}
					pool[2] = lit[end_flag].no;
					p++;
				}
			}
			for(i = 0;i < t;i++)
				{
				if(strcmp(tab[i].name1,"ORIGIN") == 0)
					{
					for(j = 1;j < n;j++)
						{
						if(strcmp(tab[i+1].name1,sys[j].name) == 0)
							{
							if(strcmp(tab[i+2].name1,"+") == 0)
								{
								math = atoi(tab[i+3].name1);
								lc = sys[j].add + math;
								lc1 = 1;
								}
							if(strcmp(tab[i+2].name1,"-") == 0)
								{
								math = atoi(tab[i+3].name1);
								lc = sys[j].add - math;
								lc1 = 1;
								}
							}
						}
					}
				if(strcmp(tab[i+1].name1,"EQU") == 0)
					{
						tar_idx2++;
					if(t == 5)
						{
						for(j = 1;j < n;j++)
							{
							if(strcmp(tab[i+2].name1,sys[j].name) == 0)
								{
								if(strcmp(tab[i+3].name1,"+") == 0)
									{
									math = atoi(tab[i+4].name1);
									tadr = sys[j].add + math;
									}
								if(strcmp(tab[i+3].name1,"-") == 0)
									{
									math = atoi(tab[i+4].name1);
									tadr = sys[j].add - math;
									}
								}
							}
						for(j = 1;j < n;j++)
							{
							if(strcmp(tab[i].name1,sys[j].name) == 0)
								sys[j].add = tadr;
							}
						}
					 if(t == 3) 			// To check symbol tabke to intermediate code
						{
						for(j = 1;j < n;j++)
							{
							if(strcmp(tab[i+2].name1,sys[j].name) == 0)
								tadr = sys[j].add;
							}
						for(j = 1;j < n;j++)		// To update symbol tabke
							{
							if(strcmp(tab[i].name1,sys[j].name) == 0)
								sys[j].add = tadr;
							}
						}
					}
				}
				for(i = 0;i < t;i++)
				{
				if(strcmp(prevt,"=") == 0)
					{
					for(j = pl;j < m;j++)
						{
						if(strcmp(data,lit[j].name) == 0)
							{
							lit[j].add = lc;
							break;
							}
						}
					break;
					}
				}
				for(i = 0;i < t;i++)
				{
				if(strcmp(tab[i].name,"DL") == 0)
					{
						//tar[tar_idx+1].addr = tab[i].code;
					for(j = 0;j < n;j++)
						{
						if(strcmp(tab[i-1].name1,sys[j].name) == 0)
							{
							sys[j].add = lc;
							break;
							}
						}
					lc = lc + tab[i+1].code;
					tar[tar_idx].op_code = tab[i+1].code;

					lc1 = 1;
					break;
					}
				}
			if(lc1 == 0)
			{
				lc++;
			}
			t = 0;
			lc1 = 0;
		}

		strcpy(prevt,data);
		flag = 0;
		j=0;
		k=0;
		for(i=0; i<tar_idx2; i++)
		{
			if(strcmp(tar[i].l_s,"l") == 0)
			{
				tar[i].addr = lit[j+1].add;
				j++;
			}
			if(strcmp(tar[i].l_s,"s") == 0)
			{
				tar[i].addr = sys[k+1].add;
				k++;
			}

		}

	}
	cout<<"\n";

	infile.close();

	cout<<"\n--SYMBOL TABLE--"<<endl;
	for(i = 1;i < n;i++)
		{
		cout<<sys[i].no <<"\t" <<sys[i].name<<"\t"<<sys[i].add<<endl;
		}

	cout<<"\n--LITRAL TABLE--"<<endl;
	for(i = 1;i < m;i++)
	{
		cout<<lit[i].no <<"\t" << lit[i].name <<"\t" <<lit[i].add <<endl;
	}

	cout<<"\n--POOL TABLE--"<<endl;
	for(i = 1;i <= p;i++)
	{
		cout<<i<<"\t"<<pool[i]<<endl;
	}

	cout<<"\n-- TARGET TABLE--"<<endl;
	for(i = 0;i < tar_idx;i++)
	{
		cout<<i<<"\t"<<tar[i].lc_add<<"\t"<<tar[i+1].op_code<<"\t"<<tar[i+1].reg_no<<"\t"<<tar[i].addr<<endl;
	}
}
int main()
{
	load();
	return 0;
}
