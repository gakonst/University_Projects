#include <iostream.h>
#include <alloc.h>
#include  <process.h>
#define CMAX 31
class  device{
  protected:
    char class_id[CMAX];
    static device **D;
    static int N;
    static device **ID;
    static int M;
  public:
    device(char *p);
    device(){};
    virtual float d_power()=0;
    virtual void set_intensity(float V)=0;
    virtual int get_tolerance()=0;
    virtual float get_cable()=0;
    virtual char *get_id()=0;
    virtual void create_units(int n)=0;

    void device_registry();

    char *get_class_id(){return class_id;}

    static device **get_D(){return D;}
    static int get_N(){return N;}
    static device **get_ID(){return ID;}
    static int get_M(){return M;}
};

device **device::D;
int device::N=0;
device **device::ID;
int device::M=0;

void device::device(char *c)
{ 
  int i=0;
  while(c[i]&&i<(CMAX-1)){
    class_id[i]=c[i];
    i++;
  }
  class_id[i]='\0';
  M++;
  if(M==1)
    ID=(device **)malloc(M*sizeof(device *));
  else
    ID=(device **)realloc(ID,M*sizeof(device *));
  if(ID==0){
    cout <<"Δεν υπάρχει διαθέσιμη μνήμη \n";
    exit(1);
  }
  ID[M-1]=this;
}

void device::device_registry()
{
  N++;
  if(N==1)
    D=(device **)malloc(N*sizeof(device *));
  else
    D=(device **)realloc(D,N*sizeof(device *));
  if(D==0){
    cout <<"Δεν υπάρχει διαθέσιμη μνήμη \n";
    exit(1);
  }
  D[N-1]=this;
}

float get_total_P(device **D, int N, float V, float *cable,int *fuce)
{ int i;
  float t_pow=0;
  for(i=0;i<N;i++){
    D[i]->set_intensity(V);
    switch(D[i]->get_tolerance()){
      case 10:
	cable[0]+=D[i]->get_cable();
	fuce[0]++;
        break;
      case 16:
	cable[1]+=D[i]->get_cable();
	fuce[1]++;
        break;
      case 20:
	cable[2]+=D[i]->get_cable();
	fuce[2]++;
        break;
      case 25:
	cable[3]+=D[i]->get_cable();
	fuce[3]++;
        break;
      default:
	cout<<"Η συσκευή "<<D[i]->get_id()<<" δε μπορεί να εγκατασταθεί στο κτήριο\n";
	cout<<"Απαιτείται ασφάλεια με ανοχή μεγαλύτερη των "<<D[i]->get_tolerance()<<" Ampere\n";
	continue;
    }
    t_pow+=D[i]->d_power();
  }
  return t_pow;
}

void main()
{
  int n,i,m;
  float total_P,V;
  float cable[4]={0,0,0,0};
  int fuce[4]={0,0,0,0};
  device **IP;
  IP=device::get_ID();
  m=device::get_M();
  cout<<"Δώστε την τάση του δικτυού = ? ";
  cin>>V;
 
  for(i=0;i<m;i++){
    cout<<"\nΔώσε τον αριθμό των συσκευών τύπου "<<IP[i]->get_class_id()<<" = ? ";
    cin>>n;
    IP[i]->create_units(n);
  }

  total_P=get_total_P(device::get_D(), device::get_N(),V,cable,fuce);
  cout<<"\nΑσφάλειες\n";
  cout<<"10 Ampere "<<fuce[0]<<" τεμάχεια\n";
  cout<<"16 Ampere "<<fuce[1]<<" τεμάχεια\n";
  cout<<"20 Ampere "<<fuce[2]<<" τεμάχεια\n";
  cout<<"25 Ampere "<<fuce[3]<<" τεμάχεια\n";

  cout<<"\nΔιατομές καλωδίων\n";
  cout<<"Διατομή 1.5 "<<cable[0]<<" μέτρα\n";
  cout<<"Διατομή 2.5 "<<cable[1]<<" μέτρα\n";
  cout<<"Διατομή  6  "<<cable[2]<<" μέτρα\n";
  cout<<"Διατομή 10  "<<cable[3]<<" μέτρα\n";

 
  cout<<"Η μέγιστη ισχύς που μπορεί να ζητηθεί από το κτήριο είναι "<<total_P<<" watt\n";
}  
//-------------------------------------------------------------------------
class device1:public device{
  float power;
  float intensity;
  float cable;
  char id[CMAX];
  public:
    device1(char *c_id);
    device1();
    float d_power(){return power;}
    void set_intensity(float V);
    int get_tolerance();
    float get_cable(){return cable;}
    char *get_id(){return id;}
    void create_units(int n);
}omh("OMH");

device1::device1(char *c_id):device(c_id){}

device1::device1()
{
  cout<<"Δώστε την ταυτότητα της συσκευής (Mέγιστο "<<CMAX-1<<" χαρακτήρες) = ? ";
  cin>>id;
  cout<<"Δώστε την ισχύ της συσκευής = ? ";
  cin>>power;
  cout<<"Δώστε την απόσταση της συσκευής από τον πίνακα = ? ";
  cin>>cable;
  device_registry();
}

void device1::set_intensity(float V)
{
  intensity=power/V;
}

int device1::get_tolerance()
{
  int toler=intensity;
  if(intensity<10)toler=10;
  if(intensity>=10&&intensity<16)toler=16;
  if(intensity>=16&&intensity<20)toler=20;
  if(intensity>=20&&intensity<25)toler=25;
  return toler;
}

void device1::create_units(int num)
{
  device1 *p;
  p=new device1[num];
  if(p==0){
    cout <<"Δεν υπάρχει διαθέσιμη μνήμη \n";
    exit(1);
  }
}                      

//----------------------------------------------------------------------------------

class device2:public device{
  float opower,mpower,init_c;
  float intensity;
  float cable;
  char id[CMAX];
  public:
    device2(char *c_id);
    device2();
    float d_power();
    void set_intensity(float V);
    int get_tolerance();
    float get_cable(){return cable;}
    char *get_id(){return id;}
    void create_units(int n);
}motor("motor");

device2::device2(char *c_id):device(c_id){}

device2::device2()
{
  cout<<"Δώστε την ταυτότητα της συσκευής (Mέγιστο "<<CMAX-1<<" χαρακτήρες) = ? ";
  cin>>id;
  cout<<"Δώσε την ωμική ισχύ της συσκευής = ? ";
  cin>>opower;
  cout<<"Δώσε την ισχύ του κινητήρα = ? ";
  cin>>mpower;
  cout<<"Δώσε τον συντελεστή εκκίνησης = ? ";
  cin>>init_c;
  cout<<"Δώστε την απόσταση της συσκευής από τον πίνακα = ? ";
  cin>>cable;
  device_registry();
}

float device2::d_power()
{
  return opower+mpower;
}

void device2::set_intensity(float V)
{
  intensity=(opower+mpower+mpower*init_c)/V;
}

int device2::get_tolerance()
{
  int toler=intensity;
  if(intensity<10)toler=10;
  if(intensity>=10&&intensity<16)toler=16;
  if(intensity>=16&&intensity<20)toler=20;
  if(intensity>=20&&intensity<25)toler=25;
  return toler;
}

void device2::create_units(int num)
{
  device2 *p;
  p=new device2[num];
  if(p==0){
    cout <<"Δεν υπάρχει διαθέσιμη μνήμη \n";
    exit(1);
  }
}          