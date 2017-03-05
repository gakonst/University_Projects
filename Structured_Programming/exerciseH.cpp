#include <iostream.h>
#include <alloc.h>
#include <string.h>
#include <process.h>
class oil_burner{
    float max_P,fuel_rate,fuel_cost,machine_cost,servis_cost;
    int days;
    char Obj_id[11];
    static char *C_id;
  public:
    oil_burner();
    float get_P_f(){return max_P/fuel_rate;}
    float get_cost(){return machine_cost;}
    float get_op_cost(int days){return servis_cost+fuel_rate*fuel_cost*days;}
    char *get_obj_id(){return Obj_id;}
    float get_maxP(){return max_P;}
    static char *get_C_id(){return C_id;}
};

char *oil_burner::C_id="OIL";

oil_burner::oil_burner()
{
  cout<<"\nΔώστε την ταυτότητα του καυστήρα (max 10 χαρακτήρες) = ? ";
  cin>>Obj_id;
  cout<<"Δώστε την μέγιστη ισχύ = ? ";
  cin>>max_P;
  cout<<"Δώστε την ημερήσια κατανάλωση πετρελαίου σε λίτρα = ? ";
  cin>>fuel_rate;
  cout<<"Δώστε την τιμή για ένα λίτρο πετρελαίου = ? ";
  cin>>fuel_cost;
  cout<<"Δώστε το κόστος του καυστήρα = ? ";
  cin>>machine_cost;
  cout<<"Δώστε το κόστος συντήρησης = ? ";
  cin>>servis_cost;
}

class gas_burner{
    float max_P,fuel_rate,fuel_cost,machine_cost,servis_cost,connection_cost;
    int days;
    char Obj_id[11];
    static char *C_id;
  public:
    gas_burner();
    float get_P_f(){return max_P/fuel_rate;}
    float get_cost(){return machine_cost+connection_cost;}
    float get_op_cost(int days){return servis_cost+fuel_rate*fuel_cost*days;}
    char *get_obj_id(){return Obj_id;}
    float get_maxP(){return max_P;}
    static char *get_C_id(){return C_id;}
};

char *gas_burner::C_id="GAS";

gas_burner::gas_burner()
{
  cout<<"\nΔώστε την ταυτότητα του καυστήρα (max 10 χαρακτήρες) = ? ";
  cin>>Obj_id;
  cout<<"Δώστε την μέγιστη ισχύ = ? ";
  cin>>max_P;
  cout<<"Δώστε την ημερήσια κατανάλωση αερίου σε κυβικά μέτρα = ? ";
  cin>>fuel_rate;
  cout<<"Δώστε την τιμή για ένα κυβικό μέτρο αερίου = ? ";
  cin>>fuel_cost;
  cout<<"Δώστε το κόστος του καυστήρα = ? ";
  cin>>machine_cost;
  cout<<"Δώστε το κόστος σύνδεσης στο δίκτυο = ? ";
  cin>>connection_cost;
  cout<<"Δώστε το κόστος συντήρησης = ? ";
  cin>>servis_cost;
}

template  <class burner>
class device{
    burner *B;
    int n;
  public:
    device();
    burner *b_short();
    int get_n(){return n;}
};
template <class burner>
device<burner>::device()
{
  cout<<"\nΔώστε τον αριθμό των καυστήρων τύπου "<<burner::get_C_id()<<" = ? ";
  cin>>n;
  B=new burner[n];
  if(B==0){
    cout<<"Δεν υπάρχει διαθέσιμη μνήμη στη θέση 1\n";
    exit(1);
  }
}

template <class burner>
burner *device<burner>::b_short()
{
  int i,f=1,k;
  k=n;
  burner *tem;
  tem=(burner *)malloc(sizeof(burner));
  if(tem==NULL){
    cout<<"Δεν υπάρχει διαθέσιμη μνήμη στη θέση 2\n";
    exit(1);
  }
  while(f){
    f=0;
    for(i=0;i<k-1;i++){
      if(B[i].get_P_f()>B[i+1].get_P_f())continue;
      *tem=B[i];
      B[i]=B[i+1];
      B[i+1]=*tem;
      f=1;
    }
    k--;
  }
  free(tem);
  return B;
}

template <class burner1,class burner2>
char *choice(burner1 *b1,int n1,burner2 *b2,int n2,float &p_out,float min_P,float max_c,int days)
{
  int i,j;
  p_out=0;
  float b1cb,b1co,b2cb,b2co;

  for(i=0;i<n1;i++)
    if(b1[i].get_maxP()>=min_P && b1[i].get_cost()<=max_c)break;
  for(j=0;j<n2;j++)
    if(b2[j].get_maxP()>=min_P && b2[j].get_cost()<=max_c)break;

  if(i==n1&&j<n2)
    return b2[j].get_obj_id();
  
  if(i<n1&&j==n2)
    return b1[i].get_obj_id();

  if(i<n1&&j<n2){
    b1cb=b1[i].get_cost();
    b1co=b1[i].get_op_cost(days);
    b2cb=b2[j].get_cost();
    b2co=b2[j].get_op_cost(days);
    if(b1cb==b2cb && b1co==b2co){
      char *b1id,*b2id,msg[100];
      b1id=b1[i].get_obj_id();
      b2id=b2[j].get_obj_id();
      strcpy(msg,"Οι καυστήρες ");
      strncat(msg,b1id,4);
      strncat(msg," και ",5);
      strncat(msg,b2id,4);
      strncat(msg," είναι ισοδύναμοι",20);
      return msg;
    }

    if(b1cb==b2cb)
      if(b1co<b2co)
	return b1[i].get_obj_id();
      else
	return b2[j].get_obj_id();

    if(b1cb<b2cb && b1co<=b2co)
      return b1[i].get_obj_id();
    
    if(b1cb>b2cb && b1co>=b2co)
      return b2[j].get_obj_id();
    
    if(b1cb>b2cb && b1co<b2co){
      p_out=(b1cb-b2cb)/(b2co-b1co);
      return b1[i].get_obj_id();
    }

    if(b1cb<b2cb && b1co>b2co){
      p_out=(b2cb-b1cb)/(b1co-b2co);
      return b2[j].get_obj_id();
    }
  }
  return "Δεν βρέθηκε καυστήρας που να ικανοποιεί τις προδιαγραφές\n";
}

void main()
{
  char *out;
  int on,gn,days;
  float p_out,max_c,min_P;
  cout<<"Δώστε την ελάχιστη ισχύ για τον καυστήρα = ? ";
  cin>>min_P;
  cout<<"Δώστε το μέγιστο κόστος για την αγορά και εγκατάσταση του καυστήρα = ? ";
  cin>>max_c;
  cout<<"\nΔώστε τον αριθμό των ημερών που λειτουργεί η θέρμανση = ? ";
  cin>>days;
  oil_burner *ob;
  gas_burner *gb;
  device<oil_burner> B1;
  device<gas_burner> B2;
  ob=B1.b_short();
  gb=B2.b_short();
  on=B1.get_n();
  gn=B2.get_n();   
  out=choice(ob,on,gb,gn,p_out,min_P,max_c,days);
  cout<<"\nΠροτεινόμενος καυστήρας\n";
  cout<<out<<"\n";
  cout<<"Απόσβεση σε "<<p_out<<" χρόνια\n";
}