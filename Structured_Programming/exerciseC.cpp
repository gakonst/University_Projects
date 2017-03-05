#include <iostream.h>
#include <alloc.h>
#include <stdlib.h>
#include <process.h>
class circuitS{
  int n;
  char *id;
  float *Res,*Inte,Tres;
public:
  circuitS(int k,int seed);
  circuitS(int k);
  void out();
  float set_tres();
  float get_tres(){return Tres;}
  void chek(float V);
  ~circuitS();
};

circuitS::circuitS(int k,int seed)
{
  int i,m,a,b;
  n=k;
  id="S";
  Res=(float *)malloc(n*sizeof(float));
  Inte=(float *)malloc(n*sizeof(float));
  if(Res==NULL || Inte==NULL){
    cout<<"��� ������� ��������� ����� ��� �������� ���� 1\n";
    exit(1);
  }
  srand(seed);
  cout<<"������� "<<id<<"\n";
  cout<<"����� �� �������� ���� ��� ����� ������ �� ���������� �� ����� ��� ����������� [a,b)= ? ";
  cin>>a>>b;
  for(i=0;i<n;i++)
    Res[i]=rand()%(b-a)+a;
  cout<<"����� �� ���������� �������� ��� ��� ����� ��� ��������� �������� [a,b) = ? ";
  cin>>a>>b;
  for(i=0;i<n;i++)
    Inte[i]=rand()%(b-a)+a;
}

circuitS::circuitS(int k)
{
  int i;
  n=k;
  id="S";
  Res=(float *)malloc(n*sizeof(float));
  Inte=(float *)malloc(n*sizeof(float));
  if(Res==NULL || Inte==NULL){
    cout<<"��� ������� ��������� ����� ��� �������� ���� 2\n";
    exit(1);
  }
  cout<<"������� "<<id<<"\n";
  cout<<"����� ��� ����� ��� ����������� ��� ������������ ���������� "<<id<<" ("<<n<<" �����)\n";
  for(i=0;i<n;i++)
    cin>>Res[i];
  cout<<"����� ��� ����������� ����� ��� ��� �������� ��������� ��������\n";
  for(i=0;i<n;i++)
    cin>>Inte[i];
}

circuitS::~circuitS()
{
 free(Res);
 free(Inte);
 cout<<"S off\n";
}

void circuitS::out()
{ int i;
  cout<<"����������� ������� "<<id<<"\n";
  for(i=0;i<n;i++)
    cout<<"��������� "<<i+1<<" "<<Res[i]<<" om  ����� "<<Inte[i]<<" Ampere\n";
  set_tres();
  cout<<"����� ��������� "<<Tres<<"om\n";
}


float circuitS::set_tres()
{
  int i;
  Tres=0;
  for(i=0;i<n;i++)
    Tres+=Res[i];
  return Tres;
}

void circuitS::chek(float V)
{
  int i,fl=1;
  float I;
  Tres=get_tres();
  I=V/Tres;
  for(i=0;i<n;i++)
    if(I>Inte[i]){
      cout<<"������� "<<id<<" � ��������� "<<i+1<<" ��������\n";
      fl=0;
    }
  if(fl)cout<<"������� "<<id<<" OK\n";
}

class circuitP{
  int n;
  char *id;
  float *Res,*Inte,Tres;
public:
  circuitP(int k,int seed);
  circuitP(int k);
  void out();
  float set_tres();
  float get_tres(){return Tres;}
  void chek(float V);
  ~circuitP();
};

circuitP::circuitP(int k,int seed)
{
  int i,m,a,b;
  n=k;
  Res=(float *)malloc(n*sizeof(float));
  Inte=(float *)malloc(n*sizeof(float));
  if(Res==NULL || Inte==NULL){
    cout<<"��� ������� ��������� ����� ��� �������� ���� 3\n";
    exit(1);
  }
  srand(seed);
  id="P";
  cout<<"������� "<<id<<"\n";
  cout<<"����� �� �������� ���� ��� ����� ������ �� ���������� �� ����� ��� ����������� [a,b)= ? ";
  cin>>a>>b;
  for(i=0;i<n;i++)
    Res[i]=rand()%(b-a)+a;
  cout<<"����� �� ���������� �������� ��� ��� ����� ��� ��������� �������� [a,b) = ? ";
  cin>>a>>b;
  for(i=0;i<n;i++)
    Inte[i]=rand()%(b-a)+a;
}

circuitP::circuitP(int k)
{
  int i;
  n=k;
  id="P";
  Res=(float *)malloc(n*sizeof(float));
  Inte=(float *)malloc(n*sizeof(float));
  if(Res==NULL || Inte==NULL){
    cout<<"��� ������� ��������� ����� ��� �������� ���� 4\n";
    exit(1);
  }
  cout<<"������� "<<id<<"\n";
  cout<<"����� ��� ����� ��� ����������� ��� ������������ ���������� "<<id<<" ("<<n<<" �����)\n";
  for(i=0;i<n;i++)
    cin>>Res[i];
  cout<<"����� ��� ����������� ����� ��� ��� �������� ��������� ��������\n";
  for(i=0;i<n;i++)
    cin>>Inte[i];
}

circuitP::~circuitP()
{
 free(Res);
 free(Inte);
 cout<<"P off\n";
}

void circuitP::out()
{ int i;
  cout<<"����������� ������� "<<id<<"\n";
  for(i=0;i<n;i++)
    cout<<"��������� "<<i+1<<" "<<Res[i]<<" om  ����� "<<Inte[i]<<" Ampere\n";
  set_tres();
  cout<<"����� ��������� "<<Tres<<"om\n";
}

float circuitP::set_tres()
{
  int i;
  Tres=0;
  for(i=0;i<n;i++)
    Tres+=1./Res[i];
    Tres=1./Tres;
  return Tres;
}

void circuitP::chek(float V)
{
  int i,fl=1;
  float I;
  for(i=0;i<n;i++){
    I=V/Res[i];
    if(I>Inte[i]){
      cout<<"������� "<<id<<" � ��������� "<<i+1<<" ��������\n";
      fl=0;
    }
  }
  if(fl)cout<<"������� "<<id<<" OK\n";
}

class circuit:public circuitS,public circuitP{
    int ID;
    float Tres,V,I;
  public:
    circuit(float v,int n,int m);
    circuit(float v,int n,int s,int m);
    circuit(float v,int n,int m,int s,char c);
    circuit(float v,int n,int s1,int m,int s2);
    float set_tres(int k);
    float get_tres(){return Tres;}
    void chek(int k);
};

circuit::circuit(float v,int n,int m):circuitS(n),circuitP(m){V=v;}
circuit::circuit(float v,int n,int s,int m):circuitS(n,s),circuitP(m){V=v;}
circuit::circuit(float v,int n,int m,int s,char c):circuitS(n),circuitP(m,s){V=v;}
circuit::circuit(float v,int n,int s1,int m,int s2):circuitS(n,s1),circuitP(m,s2){V=v;}

float circuit::set_tres(int k)
{
  float TresCS,TresCP;
  TresCS=circuitS::set_tres();
  TresCP=circuitP::set_tres();
  if(k)
    Tres=TresCS+TresCP;
  else{
    Tres=1./TresCS+1./TresCP;
    Tres=1./Tres;
  }
  return Tres;
}

void circuit::chek(int k)
{
  float V1,V2;
  Tres=set_tres(k);
  I=V/Tres;
  V1=I*circuitS::get_tres();
  circuitS::chek(V1);
  V2=I*circuitP::get_tres();
  circuitP::chek(V2);
}
    

void main()
{ int ns,np,ss,sp;
  float V;
  cout<<"����� ��� ���� ��� ����������� ��� ������� ������� V = ? ";
  cin >>V;
  cout<<"����� ��� ������ ��� ����������� ��� ������������ ���������� S = ? ";
  cin>>ns;
  cout<<"����� ��� ������ ��� ����������� ��� ������������ ���������� P = ? ";
  cin>>np;
  cout<<"\n ��������� �\n";
  circuit C1(V,ns,np);
  C1.circuitS::out();
  C1.circuitP::out();
  cout<<"\n������� ����������� ���������� �� �����\n";
  C1.chek(1);
  cout<<"������� ����������� ���������� ���������\n";
  C1.chek(0);
  cout<<"\n ��������� B\n";
  cout<<"����� ���� ������� ������ �� ����� ��� �� ���������� ������� ����� ���\n";
  cout<<"�� �������� ��� ������������ ���������� S seed = ? ";
  cin>>ss;
  circuit C2(V,ns,ss,np);
  C2.circuitS::out();
  C2.circuitP::out();
  cout<<"\n������� ����������� ���������� �� �����\n";
  C2.chek(1);
  cout<<"������� ����������� ���������� ���������\n";
  C2.chek(0);
  cout<<"\n ��������� �\n";
  cout<<"����� ���� ������� ������ �� ����� ��� �� ���������� ������� ����� ���\n";
  cout<<"�� �������� ��� ������������ ���������� P seed = ? ";
  cin>>sp;
  circuit C3(V,ns,np,sp,'c');
  C3.circuitS::out();
  C3.circuitP::out();
  cout<<"\n������� ����������� ���������� �� �����\n";
  C3.chek(1);
  cout<<"������� ����������� ���������� ���������\n";
  C3.chek(0);
  cout<<"\n ��������� �\n";
  cout<<"����� ���� ������� ������ �� ����� ��� �� ���������� ������� ����� ���\n";
  cout<<"�� �������� ��� ������������ ���������� S seed = ? ";
  cin>>ss;
  cout<<"����� ���� ������� ������ �� ����� ��� �� ���������� ������� ����� ���\n";
  cout<<"�� �������� ��� ������������ ���������� P seed = ? ";
  cin>>sp;
  circuit C4(V,ns,ss,np,sp);
  C4.circuitS::out();
  C4.circuitP::out();
  cout<<"������� ����������� ���������� �� �����\n";
  C4.chek(1);
  cout<<"������� ����������� ���������� ���������\n";
  C4.chek(0);
}