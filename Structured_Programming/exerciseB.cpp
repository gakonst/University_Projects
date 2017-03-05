#include <iostream.h>
#include <alloc.h>
#include <process.h>
class product{
    int code,number,category,bonus;
    float amount,price;
  public:
     product(int c,int n,float pr);
     product(int c,float am,float pr);
     product(int c,int n,int b,float pr);
     int get_code(){return code;}
     friend float sales(product &p);
};

product::product(int c,int n,float pr)
{
  category=1;
  code=c;
  number=n;
  price=pr;
}
product::product(int c,int n,int b,float pr)
{
  category=2;
  code=c;
  number=n;
  bonus=b;
  price=pr;
}
product::product(int c,float am,float pr)
{
  category=3;
  code=c;
  amount=am;
  price=pr;
}

float sales(product &p)
{
  int n;
  float amount;
  float price;
  switch(p.category){
    case 1:
      do{
        cout<<"Δώστε τον αριθμό των τεμαχίων = ? ";
	cin>>n;
	if(n>p.number||n<0) cout<<"Δεν υπάρχουν τόσα διαθέσιμα τεμάχια στην αποθήκη\n";
      }while(n>p.number||n<0);
      price=n*p.price;
      p.number-=n;
      cout<<"Τεμάχια στην αποθήκη "<<p.number<<"\n";
      break;
    case 2:
      do{
	cout<<"Δώστε τον αριθμό των τεμαχίων = ? ";
	cin>>n;
	if(n>p.number||n<0) cout<<"Δεν υπάρχουν τόσα διαθέσιμα τεμάχια στην αποθήκη\n";
      }while(n>p.number||n<0);
      if(p.bonus<2)
	price=n*p.price;
      else
	price=n*p.price-(n/p.bonus)*p.price;
      p.number-=n;
       cout<<"Τεμάχια στην αποθήκη "<<p.number<<"\n";
      break;
    case 3:
      do{
	cout<<"Δώσε το βάρος του προϊόντος = ? ";
	cin>>amount;
	if(amount>p.amount||amount<0)cout<<"Η ποσότητα δεν μπορεί να καλυφθεί από το απόθεμα στην αποθήκη\n";
      }while(amount>p.amount||amount<0);
      price=amount*p.price;
      p.amount-=amount;
      cout<<"Κιλά στην αποθήκη "<<p.amount<<"\n";
    }
    return price;
}

void main()
{
  int i,n,category,t_n,code,bonus;
  float price,amount;
  product *p;
  cout<<"Δώστε τον αριθμό των προϊόντων που διαθέτει το κατάστημα = ? ";
  cin>>n;
  if((p=(product *)malloc(n*sizeof(product)))==NULL){
    cout<<"Δεν υπάρχει αρκετή διαθέσιμη μνήμη\n";
    exit(1);
  }
  
  for(i=0;i<n;i++){
    cout<<"\n";
    do{
      cout<<"Δώστε την κατηγορία  του προϊόντος = ? ";
      cin>>category;
      if(category<1||category>3)
	cout<<"Λανθασμένη κατηγορία προϊόντος\n";
    }while(category<1||category>3);
    cout<<"Δώστε τον κωδικό του προϊόντος = ? ";
    cin>>code;
    switch(category){
      case 1:{
	cout<<"Δώστε τον αριθμό των τεμαχίων στην αποθήκη = ? ";
	cin>>t_n;
	cout<<"Δώστε την τιμή του τεμαχίου = ? ";
	cin>>price;
	p[i]=product(code,t_n,price);
	break;
      }
      case 2:{
	cout<<"Δώστε τον αριθμό των τεμαχίων στην αποθήκη = ? ";
	cin>>t_n;
	cout<<"Δώστε την τιμή του τεμαχίου = ? ";
	cin>>price;
	cout<<"Δώσε τον παράγοντα bonus = ? ";
        cin>>bonus;
	p[i]=product(code,t_n,bonus,price);
	break;
      }
      case 3:{
	cout<<"Δώστε το συνολικό βάρος του προϊόντος στην αποθήκη = ? ";
	cin>>amount;
	cout<<"Δώστε την τιμή ανά μονάδα βάρους = ? ";
	cin>>price;
	p[i]=product(code,amount,price);
      }
    }
  }
  
  price=0;
  cout<<"\nΕπόμενος πελάτης\n";
  for(;;){
    cout<<"\nΔώστε τον κωδικό του προϊόντος / 0 για τον επόμενο πελάτη /-1 για έξοδο = ? ";
    cin>>code;
    if(code<0)break;
    if(code==0){
      cout<<"\n\nΠληρωτέο ποσό = "<<price<<"\n";
      price=0;
      cout<<"\nΕπόμενος πελάτης\n";
      continue;
    }
    for(i=0;i<n;i++)
      if(code==p[i].get_code())break;
    if(i==n){
      cout<<"Δεν υπάρχει προϊόν με κωδικό "<<code<<"\n";
      continue;
    }
    price+=sales(p[i]);
  }
}




