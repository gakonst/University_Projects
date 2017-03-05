#include <iostream.h>
#include <alloc.h>
#include <process.h>
class vector;
class matrix{
    int n;
    float **a;
  public:
    matrix(int m);
    int get_n(){return n;}
    float *operator[](int k);
    vector operator!();
    void out();
    void free_m();
 };

 class vector{
    int n;
    float *v;
  public:
    vector(){}
    vector(int k);
    void set(int i,float t);
    int operator>(vector w);
    vector operator=(matrix x);
    vector operator-(vector x);
    float operator++();
    void out();
    void free_v(){free(v);}
};
//-------------------------------Συναρτήσεις της κλάσης matrix-----------------------------
 matrix::matrix(int m)
 {
   int i,j;
   n=m;
   a=(float **)malloc(n*sizeof(float *));
   if(a==NULL){
       cout<<"Δεν υπάρχει διαθέσιμη μνήμη Θέση 1\n";
       exit(1);
     }
   for(i=0;i<n;i++){
     a[i]=(float *)malloc(n*sizeof(float));
     if(a[i]==NULL){
       cout<<"Δεν υπάρχει διαθέσιμη μνήμη Θέση 2 γραμμή "<<i<<"\n";
       exit(1);
     }
   }
   cout<<"Δώστε κατά γραμμές τα στοιχεία του πίνακα a["<<n<<"]["<<n<<"]\n";
   for(i=0;i<n;i++)
     for(j=0;j<n;j++)
       cin>>a[i][j];
 }

float *matrix::operator[](int k)
{ return a[k];}

vector matrix::operator!()
{
  vector t(n);
  int i,j;
  float ar,ac;
  for(i=0;i<n;i++){
    ar=0;
    ac=0;
    for(j=0;j<n;j++)
      if(i!=j)
	ar+=a[i][j]<0 ? -a[i][ qj] : a[i][j];
    for(j=0;j<n;j++)
      if(i!=j)
	ac+=a[j][i]<0 ? -a[j][i] : a[j][i];
    if(ar<ac)
      t.set(i,ar);
    else
      t.set(i,ac);
  }
  return t;
}

void matrix::out()
{
  int i,j;
  for(i=0;i<n;i++){
    for(j=0;j<n;j++)
      cout<<a[i][j]<<"  ";
    cout<<"\n";
  }
}
void matrix::free_m()
{
  int i;
  for(i=0;i<n;i++)
    free(a[i]);
  free(a);
}

//-------------------------------Συναρτήσεις της κλάσης vector-----------------------------

vector::vector(int k)
{
  int i;
  n=k;
  v=(float *)malloc(n*sizeof(float));
  if(v==NULL){
    cout<<"Δεν υπάρχει διαθέσιμη μνήμη Θέση 1\n";
    exit(1);
  }
}

void vector::set(int i,float t)
{v[i]=t;}

int vector::operator>(vector b)
{
  int i,fl=1;
  if(n!=b.n){
    cout<<"Δεν είναι δυνατή η σύγκριση\n";
    exit(1);
  }
  for(i=0;i<n;i++)
    if(v[i]<=b.v[i]){
      fl=0;
      return fl;
    }
  return fl;
}

void vector::out()
{
  int i;
  for(i=0;i<n;i++)
    cout<<v[i]<<"\n";
}

vector vector::operator=(matrix x)
{
  int i;
  n=x.get_n();
  v=(float *)malloc(n*sizeof(float));
  if(v==NULL){
    cout<<"Δεν υπάρχει διαθέσιμη μνήμη Θέση 1\n";
    exit(1);
  }
  for(i=0;i<n;i++)
    v[i]=x[i][i]<0?-x[i][i]:x[i][i];
  return *this;
}

vector vector::operator-(vector x)
{
  int i;
  vector t(n);
  if(n!=x.n){
    cout<<"Δεν είναι δυνατή η καταχώρηση\n";
    exit(1);
  }
  for(i=0;i<n;i++)
    t.v[i]=v[i]-x.v[i];
  return t;
}

float vector::operator++()
{
  int i;
  float sum=0;
  for(i=0;i<n;i++)
    sum+=v[i];
  return sum;
}

void main()
{ int i,n;
  cout<<"Δώστε τον αριθμό των γραμμών του πίνακα a = ? ";
  cin>>n;
  matrix a(n);
  vector v,d,df;
  cout<<"\n";
  d=a;
  v=!a;

  if(d>v){
    cout<<"Ο πίνακας a είναι διαγωνίως υπερτερών\n";
    df=d-v;
    cout<<"Διαφορές απολύτων τιμών\n";
    df.out();
    cout<<"’θροισμα διαφορών = "<<++df<<"\n";
  }
  else
    cout<<"Ο πίνακας a δεν είναι διαγωνίως υπερτερών\n";
  a.free_m();
  v.free_v();
  d.free_v();
  df.free_v();
}