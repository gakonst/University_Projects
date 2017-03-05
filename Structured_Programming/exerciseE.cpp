#include <iostream.h>
#include <alloc.h>
#include <process.h>
//-------------------------class material-------------------------------
class material{
    float amount,sw;
    char id[11];
  public:
     material();
     float get_volume();
     char *get_id();
     void set_amount(float V);
};

material::material()
{
  cout<<"Δώστε την ταυτότητα του υλικού (Max 10 χαρακτήρες) = ? ";
  cin>>id;
  cout<<"Δώστε το ειδικό βάρος του υλικού = ? ";
  cin>>sw;
  cout<<"Δώστε την ποσότητα του υλικού που είναι διαθέσιμη = ? ";
  cin>>amount;
}

float material::get_volume()
{ return amount/sw;}

 char *material::get_id()
 { return id;}

void material::set_amount(float V)
 { amount=V*sw; }
//------------------------- class product (βασική) -------------------------------
class product{
  protected:
    static product **P;
    static int Np;
  public:
    product();
    virtual float calk_volume()=0;
    virtual float get_pieces()=0;
    virtual char *get_id()=0;
    static product **get_P(){return P;}
    static int get_Np(){return Np;}
};

product **product::P;
int product::Np=0;

product::product()
{
  Np++;
  if(Np==1){
     P=(product **)malloc(sizeof(product *));
     if(P==NULL){
       cout<<"Δεν υπάρχει αρκετή διαθέσιμη μνήμη (θέση 1)\n";
       exit(1);
     }
   }
   else{
     P=(product **)realloc(P,Np*sizeof(product *));
     if(P==NULL){
       cout<<"Δεν υπάρχει αρκετή διαθέσιμη μνήμη (θέση 2)\n";
       exit(1);
     }
   }
   P[Np-1]=this;
}
//-------------------------------------------------------------------------
void production(product **p,int np,material *m,int nm)
{
  int i,j,pieces,tp,sum=0;
  float m_volume,p_volume,pv;
  for(i=0;i<np;i++){
    pieces=p[i]->get_pieces();
    pv=p[i]->calk_volume();
    p_volume=pieces*pv;
    for(j=0;j<nm;j++){
      m_volume=m[j].get_volume();
      if(p_volume<=m_volume){
	cout<<"Προϊόν "<<p[i]->get_id()<<"  "<<pieces<<" τεμάχια από το υλικό "<<m[j].get_id()<<"\n";
	sum+=pieces;
	m_volume-=p_volume;
	m[j].set_amount(m_volume);
        break;
      }
      else{
	tp=m[j].get_volume()/pv;
        if(tp==0)continue;
	cout<<"Προϊόν "<<p[i]->get_id()<<"  "<<tp<<" τεμάχια από το υλικό "<<m[j].get_id()<<"\n";
	sum+=tp;
	m_volume-=tp*pv;
	m[j].set_amount(m_volume);
	pieces-=tp;
	p_volume=pieces*pv;
      }
    }
  }
  cout<<"Τεμάχια που κατασκευάστηκαν "<<sum<<"\n";
}

//-------------------class product1---------------------------------
class product1:public product{
   float rad,h,voliume;
   int pieces;
   char id[11];
 public:
   product1();
   float calk_volume();
   float get_pieces();
   char *get_id();
 };

 product1::product1()
 {
   cout<<"Δώστε την ταυτότητα του προϊόντος τύπου product1 (Max 10 χαρακτήρες) = ";
   cin>>id;
   cout<<"Δώστε την ακτίνα του κυλίνδρου rad = ? ";
   cin>>rad;
   cout<<"Δώστε το ύψος h = ? ";
   cin>>h;
   cout<<"Δώστε τον αριθμό των τεμαχίων = ?" ;
   cin>>pieces;
 }

 float product1::calk_volume()
 {
   voliume=rad*3.14*h;
   return voliume;
 }

 float product1::get_pieces()
 { return pieces;}

 char *product1::get_id()
 { return id;}

//-----------------------class product2--------------------------------

class product2:public product{
   float h,voliume;
   int pieces;
   char id[11];
 public:
   product2();
   float calk_volume();
   float get_pieces();
   char *get_id();
 };

 product2::product2()
 {
   cout<<"Δώστε την ταυτότητα του προϊόντος τύπου product2(Max 10 χαρακτήρες) = ";
   cin>>id;
   cout<<"Δώστε την πλευρά του κύβου = ? ";
   cin>>h;
   cout<<"Δώστε τον αριθμό των τεμαχίων = ?" ;
   cin>>pieces;
}

 float product2::calk_volume()
 {
   voliume=h*h*h;
   return voliume;
 }

 float product2::get_pieces()
 { return pieces;}

 char *product2::get_id()
 { return id;}
//------------------------------main--------------------------
void main()
{
 product1 P1[1];
 product2 P2[1];
 material M[2];
 production(product::get_P(),product::get_Np(),M,2);
}