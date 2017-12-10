#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

struct node
{
    int data[257],bin[26],noofones,isimplicant,minarr[1000];
    char term[26];
    struct node* right;
};

struct node *root,*head,*improot,*save,*fin,*compare[100],*newroot;
int var,min,number=1,columns=2,check=1,limit=-1,imptable[100][100],counter=0,essential[1000],t=0,no=0,minterms[1000];
int compminterms[1000],nof,u=0,complimit,savecount;
char a[26],b[26],functions[100][1000];       //variable names are stored as alphabets, can be modified to work for more variables

void group1();          //the minterms are grouped according to the number of ones
void arrange();         //the minterms are arranged according to their magnitude
void swap(struct node*,struct node*);           //data of two nodes is swapped
void disp();            //various column with pairings are displayed
void further_groupings();           //the minterms are paired
void end_loop(struct node*);            //the extra node in a list is deleted
void display_implicants();              //the implicants are displayed
void implicants(struct node*);          //initializes each term as an implicant
void collect(struct node*);                 //converts the term from binary notation to variables
void variables();       //the variables for the function are stored
void convert();             //reduces the prime implicants which occur more than once to one
void implicants_table();        //the prime implicants table is formed and essential implicants are found
void other_implicants();        //the prime implicants other than the essential ones are collected
void final_terms();     //the final terms in the minimized function are noted
void store_minterms();      //minterms are stored in an array
void refresh();
void tab();     //tabulation method is used to find the minimized function
void common();          //the combination with minimum terms is calculated
void del(struct node*,struct node*);        //for deletion of a node of a linked list
void answer();          //the function to be taken are displayed

int main()
{
    int z=1,i;
    printf("Enter the number of functions :- ");
    scanf("%d",&nof);
    printf("\nEnter the number of variables : ");
    scanf("%d",&var);
    for(i=0;i<nof;i++)
    {
        printf("For function F%d\n",z);
        tab();
        printf("\n\n");
        refresh();
        z++;
    }
    i=0;
    common();
    answer();
    return 0;
}

void answer()
{
    int i,x=savecount;
    printf("The functions that should be taken are :-");
    while(nof--)
    {
        printf("  F%d",nof+1);
        if(x%2==1)
        {
            printf("'");
        }
        if(nof!=0)
        {
            if(nof==1)
            {
                printf(" &");
            }
            else
            {
                printf(",");
            }
        }
        x=x/2;
    }
    printf("\n\n");
}

void common()
{
    int i,j,k=0,arr[nof],a[nof];
    int numb,counter2=0,counter1=999999;
    for(i=0;i<2*nof;i=i+2)
    {
        arr[k]=i;
        k++;
    }
    struct node* newtemp;
    struct node* temp;
    newtemp=newroot=(struct node*)malloc(sizeof(struct node));
    int counter=0;
    int b=(int)pow(2,nof);
    while(counter<b)
    {
        int x=counter;
        counter2=0;
        for(i=0;i<nof;i++)
        {
            a[i]=x%2;
            x=x/2;
        }
        for(i=0;i<nof;i++)
        {
            temp=compare[arr[i]+a[nof-1-i]];
            while(temp!=NULL)
            {
                int y;
                for(y=0;temp->term[y]!='\0';y++)
                {
                    newtemp->term[y]=temp->term[y];
                }
                newtemp=newtemp->right=(struct node*)malloc(sizeof(struct node));
                temp=temp->right;
            }
        }
        newtemp->right=NULL;
        end_loop(newroot);
        struct node *temp1,*temp2;
        temp1=newroot;
        while(temp1!=NULL&&temp1->right!=NULL)      // not equal to null condition if the node next to temp1 gets deleted
        {
            temp2=temp1->right;
            while(temp2!=NULL)
            {
                if(strcmp(temp1->term,temp2->term)==0)
                {
                    del(newroot,temp2);
                }
                temp2=temp2->right;
            }
            temp1=temp1->right;
        }
        temp1=newroot;
        while(temp1!=NULL)
        {
            temp1=temp1->right;
            counter2++;
        }
        if(counter2<counter1)
        {
            counter1=counter2;
            savecount=counter;
        }
        newroot=newtemp=(struct node*)malloc(sizeof(struct node));
        counter++;
    }
}

void del(struct node* first, struct node* temp)
{
    while(first->right!=temp)
    {
        first=first->right;
    }
    first->right=temp->right;
}

void tab()
{
    int i,j,k,x;
    struct node* temp;
    printf("\nEnter the number of minterms : ");
    scanf("%d",&min);
    i=min;
    root=temp=(struct node*)malloc(sizeof(struct node));
    printf("\nEnter the minterms one by one\n\n");
    while(i--)      //minterms are stored
    {
        scanf("%d",&temp->data[0]);
        j=temp->data[0];
        temp->noofones=0;
        x=var;
        k=0;
        while(x--)
        {
            if(j%2==1)          //converts the minterms to binary notation
            {
                temp->bin[k]=1;
                temp->noofones++;       //the number of ones in binary notation
            }
            else
            {
                temp->bin[k]=0;
            }
            j=j/2;
            k++;
        }
        temp=temp->right=(struct node*)malloc(sizeof(struct node));
    }
    temp->right=NULL;
    end_loop(root) ;
    arrange();      //various functions are called according to their needs
    store_minterms();
    int y;
    for(y=0;y<2;y++)
    {
        group1();
        disp();
        end_loop(root);
        head=(struct node*)malloc(sizeof(struct node));
        while(check>0)
        {
            further_groupings();
            collect(head);
        }
        save->right=NULL;           //storing null value in link field of list storing prime implicants
        end_loop(improot);
        collect(improot);
        display_implicants();
        variables();
        implicants_table();
        other_implicants();
        final_terms();
        end_loop(fin);
        convert();
        refresh();
        int v;
        for(v=0;v<1000;v++)
        {
            minterms[v]='\0';
        }
        for(v=0;v<=complimit;v++)
        {
            minterms[v]=compminterms[v];
        }
        limit=complimit;
        int x;
        x = (int)pow(2,var);
        min = x - min;
        i = min;
        struct node* temp;
        temp = root;
        int y=0;
        while(i--)      //minterms are stored
        {
            temp->data[0]=minterms[y];
            j=temp->data[0];
            temp->noofones=0;
            x=var;
            k=0;
            while(x--)
            {
                if(j%2==1)          //converts the minterms to binary notation
                {
                    temp->bin[k]=1;
                    temp->noofones++;       //the number of ones in binary notation
                }
                else
                {
                    temp->bin[k]=0;
                }
                j=j/2;
                k++;
            }
            temp=temp->right=(struct node*)malloc(sizeof(struct node));
            y++;
        }
        temp->right=NULL;
        end_loop(root) ;
    }
}

void refresh()
{
    number=1;columns=2;check=1;limit=-1;counter=0;t=0;no=0;
}

void arrange()          //arranging the minterms in increasing order of magnitude
{
    struct node *temp1,*temp2;
    temp1=temp2=root;
    while(temp1!=NULL)
    {
        temp2=root;
        while(temp2!=NULL)
        {
            if(temp1->data[0]<temp2->data[0])       //if not in order their values are exchanged with swap function
            {
                swap(temp1,temp2);
            }
            temp2=temp2->right;
        }
        if(temp1->right==NULL)
        {
            limit=temp1->data[0];           //the magnitude of the last minterm is recorded later for prime implicants table
        }
        temp1=temp1->right;
    }
}

void store_minterms()       //array to store all the minterms
{
    int i=0,j=0,k=0;
    struct node* temp;
    struct node* prev;
    temp=root;
    for(j=0;j<temp->data[0];j++)        // storing the minterms of compliment function
    {
        compminterms[k]=j;
        k++;
    }
    while(temp!=NULL)
    {
        if(temp->right!=NULL)
        {
            for(j=temp->data[0]+1;j<temp->right->data[0];j++)       //minterms of compliment function
            {
                compminterms[k]=j;
                k++;
            }
        }
        else
        {
            int x = (int)pow(2,var);
            for(j=temp->data[0]+1;j<x;j++)
            {
                compminterms[k]=j;
                k++;
            }
        }
        minterms[i]=temp->data[0];
        i++;
        temp=temp->right;
    }
    complimit=compminterms[--k];        //max value of minterms of compliment
}

void swap(struct node* temp1,struct node* temp2)        //swapping all the data of two nodes
{
    int x,y,i=0;
    i=var;
    for(i=0;i<var;i++)      //binary notation is exchanged
    {
        y=temp1->bin[i];
        temp1->bin[i]=temp2->bin[i];
        temp2->bin[i]=y;
    }
    y=temp1->noofones;          //no. of ones is exchanged
    temp1->noofones=temp2->noofones;
    temp2->noofones=y;
    x=temp1->data[0];           //data(minterm) is exchanged
    temp1->data[0]=temp2->data[0];
    temp2->data[0]=x;
}

void group1()       //where the minterms are arranged according to the number of ones
{
    int i,count=0,j,k=0;
    struct node *temp,*next;
    temp=save=root;
    root=next=(struct node*)malloc(sizeof(struct node));
    for(i=0;i<=var;i++)
    {
        temp=save;
        while(temp!=NULL)
        {
            if(temp->noofones==i)       //minterms are arranged according to no. of ones , first 0 ones then 1 ones... and so on
            {
                next->data[0]=temp->data[0];
                k++;
                for(j=0;j<var;j++)
                {
                    next->bin[j]=temp->bin[j];
                }
                next->noofones=temp->noofones;
                next=next->right=(struct node*)malloc(sizeof(struct node));
            }
            temp=temp->right;
        }
    }
    minterms[k]=-1;
    next->right=NULL;
}

void disp()     //for displaying the various column with pairings
{
    int i,j=min;
    struct node* temp;
    temp=root;
    while(temp->right!=NULL)
    {
        temp=temp->right;
    }
    temp->right=NULL;
    number++;         //number tells us which column is being printed
}

void end_loop(struct node* ptr)         //reducing the number of nodes in a list with one extra node
{
    struct node* temp;
    temp=ptr;
    while(temp->right->right!=NULL)
    {
        temp=temp->right;
    }
    temp->right=NULL;
}

void further_groupings()    //grouping based on difference in binary notation
{
    int i,count,k,j,x;
    struct node *temp,*next,*p,*imp;
    check=0;
    if(columns==2)      //for second column
    {
        imp=improot=(struct node*)malloc(sizeof(struct node));
        p=head;
    }
    else        //for other columns
    {
        imp=save;
        root=head;
        p=head=(struct node*)malloc(sizeof(struct node));
    }
    temp=root;
    implicants(root);
    while(temp!=NULL)
    {
        next=temp->right;
        while(next!=NULL)
        {
            count=0;
            if(next->noofones-temp->noofones==1)        //if two terms differ in their no. of ones by one
            {
                for(i=0;i<var;i++)
                {
                    if(temp->bin[i]!=next->bin[i])
                    {
                        k=i;            //the place in which they differ is noted
                        count++;
                    }
                }
            }
            if(count==1)        //checks if the two terms differ by one place in binary notation
            {
                temp->isimplicant=0;        //if they do then they are not a prime implicant
                next->isimplicant=0;
                check++;
                for(i=0;i<var;i++)
                {
                    p->bin[i]=temp->bin[i];         //binary notation is stored
                }
                p->bin[k]=-1;
                x=0;
                for(j=0;j<columns/2;j++)            //data from first term is stored
                {
                    p->data[x]=temp->data[j];
                    x++;
                }
                for(j=0;j<columns/2;j++)            //data from second term is stored
                {
                    p->data[x]=next->data[j];
                    x++;
                }
                p->noofones=temp->noofones;
                p=p->right=(struct node*)malloc(sizeof(struct node));           // one extra node that is to be deleted
            }
            next=next->right;
        }
        temp=temp->right;
    }
    p->right=NULL;
    if(check!=0)
    {
        end_loop(head);     //extra node is deleted
    }
    temp=root;
    while(temp!=NULL)           //for selecting the prime implicants
    {
        if(temp->isimplicant==1)        // if term is a prime implicant it is stored separately in list with head pointer improot
        {
            i=0;
            for(i=0;i<columns/2;i++)
            {
                imp->data[i]=temp->data[i];
            }
            imp->data[i]=-1;
            for(i=0;i<var;i++)
            {
                imp->bin[i]=temp->bin[i];
            }
            imp=imp->right=(struct node*)malloc(sizeof(struct node));
        }
        temp=temp->right;
    }
    save=imp;
    columns=columns*2;
    number++;
}

void display_implicants()       //displays the implicants
{
    int i=0;
    struct node* temp;
    temp=improot;
    while(temp!=NULL)
    {
        temp=temp->right;
        counter++;
    }
}

void implicants(struct node* ptr)       //initializing each term as a prime implicant
{
    struct node* temp;
    temp=ptr;
    while(temp!=NULL)
    {
        temp->isimplicant=1;
        temp=temp->right;
    }
}

void collect(struct node* improot)          //reduces the terms that occur more than once to a single
{
    int common=0,i;
    struct node *temp1,*temp2,*temp3;
    temp1=temp2=improot;
    while(temp1!=NULL)
    {
        temp2=temp1->right;
        while(temp2!=NULL)
        {
            common=0;
            for(i=0;i<var;i++)          //if their binary notation is same one will be deleted
            {
                if(temp2->bin[i]==temp1->bin[i])
                {
                    common++;
                }
            }
            if(common==var)
            {
                temp3=improot;
                while(temp3->right!=temp2)      //the repeated term is deleted
                {
                    temp3=temp3->right;
                }
                temp3->right=temp2->right;
                temp2=temp3;
            }
            temp2=temp2->right;
        }
        temp1=temp1->right;
    }
}

void variables()            //stores variables(alphabets)
{
    int i;              //here variables are taken as alphabets and A is the least significant bit
    for(i=0;i<26;i++)       //the capital letters are the variables and the small letters are their compliments
    {
        a[i]=65+i;      //variables
        b[i]=97+i;      //their compliments
    }
}

void convert()          //it converts the binary notation of each term to variables
{
    int i,j;
    struct node* temp;
    temp=fin;
    while(temp!=NULL)
    {
        j=0;
        for(i=0;i<var;i++)
        {
            if(temp->bin[i]==0)
            {
                temp->term[j]=b[i];
                j++;
            }
            if(temp->bin[i]==1)
            {
                temp->term[j]=a[i];
                j++;
            }
        }
        temp=temp->right;
    }
}

void implicants_table()         //function for creating prime implicants table as well as selecting essential prime implicants
{
    struct node* temp;
    int i,j,k,m,n,x,y,count=0,count2=0,a=0;
    for(i=0;i<counter;i++)
    {
        for(j=0;j<=limit;j++)
        {
            imptable[i][j]=0;           //0 or - is placed in all places of a table
        }
    }
    i=0;
    j=0;
    k=0;
    temp=improot;
    while(temp!=NULL)
    {
        k=0;
        while(temp->data[k]!=-1)
        {
            imptable[i][temp->data[k]]=1;  // 1 or X is placed for the column with same index as that of the number in the pair
            k++;
        }
        i++;
        temp=temp->right;
    }
    temp=improot;
    i=0;
    for(i=0;i<counter;i++)      //for finding essential prime implicants
    {
        for(j=0;j<=limit;j++)
        {
            count=0;
            if(imptable[i][j]==1)
            {
                y=j;
                x=i;
                for(k=0;k<counter;k++)
                {
                    if(imptable[k][j]==1)       //checks if there is only one X in a column
                    {
                        count++;
                    }
                }
                if(count==1)  //places - in place of X in every column of the table whose one row contains only one X in a column
                {
                    essential[t]=x;
                    t++;
                    for(n=0;n<=limit;n++)
                    {
                        if(imptable[i][n]==1)
                        {
                            for(m=0;m<counter;m++)
                            {
                                imptable[m][n]=0;
                            }
                        }
                    }
                }
            }
        }
    }
    essential[t]=-1;
    i=0;
}

void other_implicants()     //after finding the essential prime implicants other terms necessary are marked
{
    no=0;           //to check if any term is found in each iteration
    int count1=0,count2=0;
    int i,j;
    for(i=0;i<counter;i++)
    {
        count1=0;
        for(j=0;j<=limit;j++)
        {
            if(imptable[i][j]==1)       //no. of X's or 1's are calculated
            {
                no++;
                count1++;
            }
        }
        if(count1>count2)       //to find the term with maximum X's in a row
        {
            essential[t]=i;
            count2=count1;
        }
    }
    for(j=0;j<=limit;j++)           //removing the X's in the row as well a those X's which are in same column
    {
        if(imptable[essential[t]][j]==1)
        {
            for(i=0;i<counter;i++)
            {
                imptable[i][j]=0;
            }
        }
    }
    t++;
    essential[t]=-1;
    if(no>0)            //if one or more terms is found the function is called again otherwise not
    {
        other_implicants();
    }
}

void final_terms()          //in this function all the terms in the minimized expression are stored in a linked list
{
    int i=0,j,c=0,x;
    struct node *temp,*ptr;
    compare[u]=fin=temp=(struct node*)malloc(sizeof(struct node));
    u++;
    while(essential[i]!=-1)
    {
        ptr=improot;
        x=essential[i];
        for(j=0;j<x;j++)        //so that pointer points to the node whose index was stored in array named essential
        {
            ptr=ptr->right;
        }
        j=0;
        while(ptr->data[j]!=-1)         // the data of the node is stored
        {
            temp->data[j]=ptr->data[j];
            j++;
        }
        temp->data[j]=-1;
        for(j=0;j<var;j++)          //the binary code is stored
        {
            temp->bin[j]=ptr->bin[j];
        }
        temp=temp->right=(struct node*)malloc(sizeof(struct node));
        i++;
        c++;
    }
    temp->right=NULL;
}
