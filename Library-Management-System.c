#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
struct Book                                 //Book structure
{
    int BookId;
    char name[20];
    float price;
    int quantity;
};
struct Student                              //Student structure
{
    int rollno;
    char name[20];
    int BookId,quantity;
    char status[10];
};

int menu();                                 //Function declarations
struct Book AddBook();
int IssueBook(FILE **,FILE **);
void PrintAllBook(FILE **);
int EditBookD(FILE **);
void PrintIssueData(FILE **);
void DeleteEntry(FILE **);
int BookSubmission(FILE **,FILE **);
void ChangeQuantity(FILE **);
int EDIT(FILE **);

int main()
{
    struct Book b;
    FILE *fp,*sp;                           //fp - file pointer for book.dat
    char choice='y';                        //sp - file pointer for student.dat
    fp=fopen("book.dat","rb+");             //book.dat file opening
    if(fp==NULL)
    {
        fp=fopen("book.dat","wb+");
        if(fp==NULL)
        {
            printf("\nDatabase of Book crashed!");
            _getch();
            exit(0);
        }
    }
    sp=fopen("student.dat","rb+");          //student.dat file opening
    if(sp==NULL)
    {
        sp=fopen("student.dat","wb+");
        if(sp==NULL)
        {
            printf("\nCannot open student Database");
            _getch();
            exit(0);
        }
    }
    printf("\n\t\tLibrary Management Application %c",3);
    while(1)
    {
        printf("\n\nPress any key continue...");
        _getch();
        system("cls");
        switch(menu())
        {
        case 1:
            PrintAllBook(&fp);
            break;
        case 2:
            choice='y';
            while(choice=='y')
            {
                b=AddBook();
                fseek(fp,0,SEEK_END);
                fwrite(&b,sizeof(b),1,fp);
                printf("\nBook record added successfully");
                printf("\nAnother addition?(y/n) -> ");
                fflush(stdin);
                choice=_getche();
            }
            break;
        case 3:
            choice='y';
            while(choice=='y')
            {
                if(EditBookD(&fp)==0)
                    printf("\nNo such record found");
                printf("\nAnother modification? (y/n) -> ");
                fflush(stdin);
                choice=_getche();
            }
            break;
        case 4:
            choice='y';
            while(choice=='y')
            {
                DeleteEntry(&fp);
                printf("\nAnother deletion? (y/n) -> ");
                choice=_getche();
            }
            break;
        case 5:
            IssueBook(&fp,&sp);
            break;
        case 6:
            BookSubmission(&fp,&sp);
            break;
        case 7:
            PrintIssueData(&sp);
            break;
        case 8:
            ChangeQuantity(&fp);
            break;
        case 9:
            //EDIT(&fp);
            fclose(fp);
            fclose(sp);
            exit(0);
        default:
            printf("\nInvalid choice");
        }
    }
}
int menu()
{
    printf("\n\t\tLibrary Management App %c\n",3);
    printf("\n1. Print available books");
    printf("\n2. Add books to library");
    printf("\n3. Edit a book credential");
    printf("\n4. Delete a particular record");
    printf("\n5. Issue a book");
    printf("\n6. Book Submission by student");
    printf("\n7. Issued book's record");
    printf("\n8. Change quantity of a Book");
    printf("\n9. Save and Exit");
    fflush(stdin);
    printf("\nEnter your choice -> ");
    return(_getche()-48);
}
struct Book AddBook()
{
    struct Book b;
    printf("\nEnter the BookID -> ");
    scanf("%d",&b.BookId);
    printf("\nEnter the name of the Book\n(18 character Max allowed)-> ");
    fflush(stdin);
    gets(b.name);
    printf("\nEnter the price of the book -> ");
    scanf("%f",&b.price);
    printf("\nEnter the quantity of the Book -> ");
    scanf("%d",&b.quantity);
    return b;
};
int IssueBook(FILE **fp,FILE **sp)
{
    struct Book b;
    struct Student s;
    int i=0,recsize,bookId,quantity,rollno;
    char choice='y';
    recsize=sizeof(b);
    printf("\nEnter the Book ID -> ");
    scanf("%d",&bookId);
    rewind(*fp);
    while(fread(&b,recsize,1,*fp)==1)
    {
        if(bookId==b.BookId)
        {
            i++;
            break;
        }
    }
    if(i)
    {
        printf("\nRecord found");
        printf("\nQuantity Available -> %d",b.quantity);
        while(choice=='y')
        {
            printf("\nQuantity of book to be issued -> ");
            scanf("%d",&quantity);
            if(quantity<=b.quantity)
            {
                printf("\nEnter the Roll No. of Student -> ");
                scanf("%d",&rollno);
                recsize=sizeof(s);
                rewind(*sp);
                while(fread(&s,recsize,1,*sp)==1)
                {
                    if(s.rollno==rollno && s.BookId==bookId && strcmp(s.status,"Due")==0)
                    {
                        printf("\nError : First submit previous issued book!");
                        return 0;
                    }
                }
                b.quantity=b.quantity-quantity;
                recsize=sizeof(b);
                fseek(*fp,-recsize,SEEK_CUR);
                fwrite(&b,recsize,1,*fp);

                s.rollno=rollno;
                printf("\nEnter the name of the Student -> ");
                fflush(stdin);
                gets(s.name);
                s.BookId=b.BookId;
                s.quantity=quantity;
                strcpy(s.status,"Due");
                fseek(*sp,0,SEEK_END);
                fwrite(&s,sizeof(s),1,*sp);
                printf("\nBook issued successfully!");
                return 1;
            }
            else
            {
                printf("\nNot enough book available");
                printf("\nWant to Re-enter quantity? (y/n) -> ");
                fflush(stdin);
                choice=_getche();
            }
        }
    }
    else
        printf("\nNo such record found!");
    return 0;
}
void PrintAllBook(FILE **fp)
{
    struct Book b;
    int i=0,recsize;
    recsize=sizeof(b);
    rewind(*fp);
    printf("\n\nS.No.  Book ID  Name\t\t   Price\tQuantity");
    while(fread(&b,recsize,1,*fp)==1)
        printf("\n%d\t%d\t%-19s %0.2f\t  %d",++i,b.BookId,b.name,b.price,b.quantity);
    printf("\n\n---*---*---*---");
}
int EditBookD(FILE **fp)
{
    struct Book b;
    int bookid,recsize;
    rewind(*fp);
    recsize=sizeof(b);
    printf("\nEnter the BookID to search the book -> ");
    scanf("%d",&bookid);
    while(fread(&b,recsize,1,*fp)==1)
    {
        if(bookid==b.BookId)
        {
            printf("\nRecord Found!");
            printf("\nOld Credentials");
            printf("\nS.No.  Book ID  Name\t\t   Price\tQuantity");
            printf("\n%d\t%d\t%-19s %0.2f\t  %d",1,b.BookId,b.name,b.price,b.quantity);
            printf("\nEnter new credential");
            b=AddBook();
            fseek(*fp,-recsize,SEEK_CUR);
            fwrite(&b,recsize,1,*fp);
            printf("\nRecord Modified Successfully!");
            return 1;
        }
    }
    return 0;
}
void PrintIssueData(FILE **sp)
{
    struct Student s;
    int recsize,i=0;
    recsize=sizeof(s);
    rewind(*sp);
    printf("\n\nS.No.  RollNO.  Name\t\t   BookID\tQuantity\tStatus");
    while(fread(&s,recsize,1,*sp)==1)
        printf("\n%d\t%d\t%-19s %d\t\t  %d\t\t %s",++i,s.rollno,s.name,s.BookId,s.quantity,s.status);
    printf("\n\n---*---*---*---");
}
void DeleteEntry(FILE **fp)
{
    struct Book b;
    FILE *ft;
    int i=0,bookid,recsize;
    recsize=sizeof(b);
    rewind(*fp);
    ft=fopen("temp.dat","wb");
    fseek(ft,0,SEEK_END);
    printf("\nEnter the Book ID to search -> ");
    scanf("%d",&bookid);
    while(fread(&b,recsize,1,*fp)==1)
    {
        if(bookid==b.BookId)
        {
            i++;
            break;
        }
    }
    if(i)
    {
        rewind(*fp);
        while(fread(&b,recsize,1,*fp)==1)
        if(bookid!=b.BookId)
            fwrite(&b,recsize,1,ft);
        fclose(*fp);
        fclose(ft);
        remove("book.dat");
        rename("temp.dat","book.dat");
        *fp=fopen("book.dat","rb+");
        printf("\nDeletion successful");
    }
    else
        printf("\nNo such Record found!");
}
int BookSubmission(FILE **fp,FILE **sp)
{
    struct Book b;
    struct Student s;
    int rollno,recsize,i=0,bookid;
    char choice='y';
    printf("\nEnter the roll no. of Student -> ");
    scanf("%d",&rollno);
    recsize=sizeof(s);
    rewind(*sp);
    printf("\n\nS.No.  RollNO.  Name\t\t   BookID\tQuantity\tStatus");
    while(fread(&s,recsize,1,*sp)==1)
    {
        if(s.rollno==rollno)
        {
            i++;
            printf("\n%d\t%d\t%-19s %d\t\t  %d\t\t %s",i,s.rollno,s.name,s.BookId,s.quantity,s.status);
        }
    }
    if(i)
    {
        printf("\nEnter the Book ID for submission -> ");
        scanf("%d",&bookid);
        rewind(*sp);
        while(fread(&s,recsize,1,*sp)==1)
        {
            if(s.rollno==rollno && bookid==s.BookId && (strcmp(s.status,"Due")==0) )
            {
                printf("\nConfirm Book Submission? (y/n) -> ");
                fflush(stdin);
                choice=_getche();
                if(choice=='y')
                {
                    recsize=sizeof(s);
                    fseek(*sp,-recsize,SEEK_CUR);
                    strcpy(s.status,"Submitted");
                    fwrite(&s,recsize,1,*sp);
                    i=0;
                    rewind(*fp);
                    recsize=sizeof(b);
                    while(fread(&b,recsize,1,*fp)==1)
                    {
                        if(s.BookId==b.BookId)
                        {
                            b.quantity=b.quantity+s.quantity;
                            fseek(*fp,-recsize,SEEK_CUR);
                            fwrite(&b,recsize,1,*fp);
                            printf("\nBook submission successful");
                            break;
                        }
                    }//putting book back
                }//confirm submission
                else
                    printf("\nBook submission Canceled!");
                return 0;
            }//end of file search condition
        }//end of while loop
    }
    printf("\nNo such record found!");
    return 0;
}
void ChangeQuantity(FILE **fp)
{
    struct Book b;
    int i,bookId,recsize;
    recsize=sizeof(b);
    rewind(*fp);
    printf("\nEnter the Book ID -> ");
    scanf("%d",&bookId);
    rewind(*fp);
    while(fread(&b,recsize,1,*fp)==1)
    {
        if(bookId==b.BookId)
        {
            i++;
            break;
        }
    }
    if(i)
    {
        fseek(*fp,-recsize,SEEK_CUR);
        printf("\nPrevious Quantity -> %d",b.quantity);
        printf("\nEnter the new quantity -> ");
        scanf("%d",&b.quantity);
        fwrite(&b,recsize,1,*fp);
        printf("\nBook quantity Changed");
    }
    else
        printf("\nNo such record found!");
}
int EDIT(FILE **fp)
{
    struct Book b;
    int bookid,recsize;
    char another='y';
    rewind(*fp);
    recsize=sizeof(b);
    while(fread(&b,recsize,1,*fp)==1 && another=='y')
    {
        printf("\nOld Credentials");
        printf("\nS.No.  Book ID  Name\t\t   Price\tQuantity");
        printf("\n%d\t%d\t%-19s %0.2f\t  %d",1,b.BookId,b.name,b.price,b.quantity);
        printf("\nEnter new credential");
        b=AddBook();
        fseek(*fp,-recsize,SEEK_CUR);
        fwrite(&b,recsize,1,*fp);
        printf("\nRecord Modified Successfully!");
        printf("\nAnother?(y/n)");
        fflush(stdin);
        another=_getche();
    }
    return 0;
}

//finished
