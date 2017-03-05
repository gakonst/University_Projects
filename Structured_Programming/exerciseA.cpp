#include <iostream.h>
#include <string.h>
#include <process.h>
#include <alloc.h>
#define FMAX 100
class member{
    int id,f_num;
    int friends[FMAX];
    char login[6],pass[11],name[21];
  public:
    member();
    void add_friend(int k);
    void del_friend(int k);
    void set_id(int k){id=k;}
    int get_id(){return id;}
    char *get_name(){return name;}
    char *get_login(){return login;}
    char *get_pass(){return pass;}
    int *get_friends(int &fn){fn=f_num;return friends;}
};

member::member()
{
  char pass1[11],pass2[11];
  cout<<"Δώστε το όνομα σας (μέγιστο 20 χαρακτήρες)= ? ";
  cin>>name;
  cout<<"Δώστε ένα login name (μέγιστο 5 χαρακτήρες)= ? ";
  cin>>login;
  cout<<"Δώστε ένα password ( μέγιστο 10 χαρακτήρες)= ? ";
  cin>>pass1;
  cout<<"Δώστε ξανά το password = ? ";
  cin>>pass2;
  if(strcmp(pass1,pass2)){
    cout<<"\nΛάθος κωδικός\n";
    id=1;
    return;
  }
  strcpy(pass,pass1);
  f_num=0;
  id=0;
}

void member::add_friend(int fid)
{
  if(f_num<FMAX){
    friends[f_num]=fid;
    f_num++;
  }
  else{
    cout<<"Η λίστα των φίλων είναι πλήρης\n";
    return;
  }
}

void member::del_friend(int fid)
{
  int i;
  for(i=0;i<f_num;i++)
    if(friends[i]==fid){
      f_num--;
      if(i==f_num)return;
      friends[i]=friends[f_num];
      return;
    }
}


void main()
{
  int k,i,n=0,my_id=0,id,fl,fn,*fr;;
  char login[6],pass[11],name[21];
  member *p;
  for(;;){
    cout<<"\nΓια να γραφτείτε στη σελίδα\n";
    member newm;
    if(newm.get_id())continue;
    if((!strcmp("admin",newm.get_login()))&&(!strcmp("apass",newm.get_pass())))break;
    fl=0;
    for(i=0;i<n;i++){
      if(strcmp(p[i].get_login(),newm.get_login()))continue;
      cout<<"\nΥπάρχει ήδη μέλος με login "<<newm.get_login()<<"\n";
      fl=1;
      break;
    }
    if(fl)continue;
    if(n==0)
      p=(member *)malloc(sizeof(member));
    else
      p=(member *)realloc(p,(n+1)*sizeof(member));
    p[n]=newm;
    p[n].set_id(n+1);
    n++;
    cout<<"\nΚαλώς ήρθατε στη σελίδα "<<p[n-1].get_name()<<"\n\n";
  }
  cout<<"\nΣτη σελίδα γράφτηκαν συνολικά "<<n<<" άτομα\n\n";
  for(i=0;i<n;i++)
    cout<<p[i].get_name()<<"\n";

  for(;;){
    if(my_id==0){
      cout<<"\nΓια να μπείτε στην σελίδα\n";
      cout<<"Δώστε το login name = ? ";
      cin>>login;
      cout<<"Δώστε το password = ? ";
      cin>>pass;
      if((!strcmp("admin",login))&&(!strcmp("apass",pass)))break;
      for(i=0;i<n;i++){
        if(strcmp(p[i].get_login(),login))continue;
        if(strcmp(p[i].get_pass(),pass))continue;
        cout<<"\nΚαλώς ήρθατε στη σελίδα "<<p[i].get_name()<<"\n\n";
        my_id=p[i].get_id();
        break;
      }
      if(my_id==0){
        cout<<"\nΔεν έχετε εγγραφεί ως μέλος στη σελίδα\n\n";
        continue;
      }
    }
    cout<<"\nΔώστε:\n";
    cout<<"1 Για να αναζητήσετε μέλη στη σελίδα\n";
    cout<<"2 Για να καταχωρίσετε ένα μέλος ως φίλο\n";
    cout<<"3 Για να διαγράψετε ένα μέλος από φίλο\n";
    cout<<"4 Για να τυπώσετε τα ονόματα των φίλων σας\n";
    cout<<"Άλλο για έξοδο\n";
    cin>>k;
    switch(k){
      case 1:
        cout<<"Δώστε το όνομα του μέλους που αναζητείτε = ? ";
	cin>>name;
        fl=1;
        for(i=0;i<n;i++){
	  if(strcmp(p[i].get_name(),name))continue;
	  cout<<"\nΟ/Η "<<name<<" είναι μέλος της σελίδας με ID "<<p[i].get_id()<<"\n";
          fl=0;
	  break;
	}
	if(fl) cout<<"Ο/Η "<<name<< " δεν είναι γραμμένος στη σελίδα\n";
        break;
      case 2:
        cout<<"Δώστε το ID του μέλους που θέλετε να καταχωρίσετε ως φίλο = ? ";
	cin>>id;
	if(id<1||id>n){
	  cout<<"Δεν υπάρχει μέλος με ID "<<id<<"\n";
	  break;
        }
	p[my_id-1].add_friend(id);
	cout<<"Ο/Η "<<p[id-1].get_name()<<" καταχωρήθηκε ως φίλος του "<<p[my_id-1].get_name()<<"\n";
        break;
      case 3:
        cout<<"Δώστε το ID του μέλους που θέλετε να διαγράψετε από φίλο = ? ";
	cin>>id;
	fr=p[my_id-1].get_friends(fn);
        fl=1;
	for(i=0;i<fn;i++){
	  if(id==fr[i]){
            p[my_id-1].del_friend(id);
	    cout<<"Ο/Η "<<p[id-1].get_name()<<" διαγράφηκε από φίλος του "<<p[my_id-1].get_name()<<"\n";
	    fl=0;;
	    break;
	  }
	}
	if(fl)cout<<"Το μέλος με ID "<<id<<" δεν ανήκει στους φίλους σας\n";
	break;
      case 4:
	fr=p[my_id-1].get_friends(fn);
	cout<<"Οι φίλοι του "<<p[my_id-1].get_name()<<" είναι\n";
        for(i=0;i<fn;i++)
          cout<<p[fr[i]-1].get_name()<<"\n";
	continue;
      default:
	my_id=0;
    }
  }       
}

              