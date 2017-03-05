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
        cout<<"����� ��� ������ ��� �������� = ? ";
	cin>>n;
	if(n>p.number||n<0) cout<<"��� �������� ���� ��������� ������� ���� �������\n";
      }while(n>p.number||n<0);
      price=n*p.price;
      p.number-=n;
      cout<<"������� ���� ������� "<<p.number<<"\n";
      break;
    case 2:
      do{
	cout<<"����� ��� ������ ��� �������� = ? ";
	cin>>n;
	if(n>p.number||n<0) cout<<"��� �������� ���� ��������� ������� ���� �������\n";
      }while(n>p.number||n<0);
      if(p.bonus<2)
	price=n*p.price;
      else
	price=n*p.price-(n/p.bonus)*p.price;
      p.number-=n;
       cout<<"������� ���� ������� "<<p.number<<"\n";
      break;
    case 3:
      do{
	cout<<"���� �� ����� ��� ��������� = ? ";
	cin>>amount;
	if(amount>p.amount||amount<0)cout<<"� �������� ��� ������ �� �������� ��� �� ������� ���� �������\n";
      }while(amount>p.amount||amount<0);
      price=amount*p.price;
      p.amount-=amount;
      cout<<"���� ���� ������� "<<p.amount<<"\n";
    }
    return price;
}

void main()
{
  int i,n,category,t_n,code,bonus;
  float price,amount;
  product *p;
  cout<<"����� ��� ������ ��� ��������� ��� �������� �� ��������� = ? ";
  cin>>n;
  if((p=(product *)malloc(n*sizeof(product)))==NULL){
    cout<<"��� ������� ������ ��������� �����\n";
    exit(1);
  }
  
  for(i=0;i<n;i++){
    cout<<"\n";
    do{
      cout<<"����� ��� ���������  ��� ��������� = ? ";
      cin>>category;
      if(category<1||category>3)
	cout<<"���������� ��������� ���������\n";
    }while(category<1||category>3);
    cout<<"����� ��� ������ ��� ��������� = ? ";
    cin>>code;
    switch(category){
      case 1:{
	cout<<"����� ��� ������ ��� �������� ���� ������� = ? ";
	cin>>t_n;
	cout<<"����� ��� ���� ��� �������� = ? ";
	cin>>price;
	p[i]=product(code,t_n,price);
	break;
      }
      case 2:{
	cout<<"����� ��� ������ ��� �������� ���� ������� = ? ";
	cin>>t_n;
	cout<<"����� ��� ���� ��� �������� = ? ";
	cin>>price;
	cout<<"���� ��� ��������� bonus = ? ";
        cin>>bonus;
	p[i]=product(code,t_n,bonus,price);
	break;
      }
      case 3:{
	cout<<"����� �� �������� ����� ��� ��������� ���� ������� = ? ";
	cin>>amount;
	cout<<"����� ��� ���� ��� ������ ������ = ? ";
	cin>>price;
	p[i]=product(code,amount,price);
      }
    }
  }
  
  price=0;
  cout<<"\n�������� �������\n";
  for(;;){
    cout<<"\n����� ��� ������ ��� ��������� / 0 ��� ��� ������� ������ /-1 ��� ����� = ? ";
    cin>>code;
    if(code<0)break;
    if(code==0){
      cout<<"\n\n�������� ���� = "<<price<<"\n";
      price=0;
      cout<<"\n�������� �������\n";
      continue;
    }
    for(i=0;i<n;i++)
      if(code==p[i].get_code())break;
    if(i==n){
      cout<<"��� ������� ������ �� ������ "<<code<<"\n";
      continue;
    }
    price+=sales(p[i]);
  }
}




