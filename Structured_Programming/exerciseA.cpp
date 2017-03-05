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
  cout<<"����� �� ����� ��� (������� 20 ����������)= ? ";
  cin>>name;
  cout<<"����� ��� login name (������� 5 ����������)= ? ";
  cin>>login;
  cout<<"����� ��� password ( ������� 10 ����������)= ? ";
  cin>>pass1;
  cout<<"����� ���� �� password = ? ";
  cin>>pass2;
  if(strcmp(pass1,pass2)){
    cout<<"\n����� �������\n";
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
    cout<<"� ����� ��� ����� ����� ������\n";
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
    cout<<"\n��� �� ��������� ��� ������\n";
    member newm;
    if(newm.get_id())continue;
    if((!strcmp("admin",newm.get_login()))&&(!strcmp("apass",newm.get_pass())))break;
    fl=0;
    for(i=0;i<n;i++){
      if(strcmp(p[i].get_login(),newm.get_login()))continue;
      cout<<"\n������� ��� ����� �� login "<<newm.get_login()<<"\n";
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
    cout<<"\n����� ������ ��� ������ "<<p[n-1].get_name()<<"\n\n";
  }
  cout<<"\n��� ������ ��������� �������� "<<n<<" �����\n\n";
  for(i=0;i<n;i++)
    cout<<p[i].get_name()<<"\n";

  for(;;){
    if(my_id==0){
      cout<<"\n��� �� ������ ���� ������\n";
      cout<<"����� �� login name = ? ";
      cin>>login;
      cout<<"����� �� password = ? ";
      cin>>pass;
      if((!strcmp("admin",login))&&(!strcmp("apass",pass)))break;
      for(i=0;i<n;i++){
        if(strcmp(p[i].get_login(),login))continue;
        if(strcmp(p[i].get_pass(),pass))continue;
        cout<<"\n����� ������ ��� ������ "<<p[i].get_name()<<"\n\n";
        my_id=p[i].get_id();
        break;
      }
      if(my_id==0){
        cout<<"\n��� ����� �������� �� ����� ��� ������\n\n";
        continue;
      }
    }
    cout<<"\n�����:\n";
    cout<<"1 ��� �� ����������� ���� ��� ������\n";
    cout<<"2 ��� �� ������������ ��� ����� �� ����\n";
    cout<<"3 ��� �� ���������� ��� ����� ��� ����\n";
    cout<<"4 ��� �� �������� �� ������� ��� ����� ���\n";
    cout<<"���� ��� �����\n";
    cin>>k;
    switch(k){
      case 1:
        cout<<"����� �� ����� ��� ������ ��� ���������� = ? ";
	cin>>name;
        fl=1;
        for(i=0;i<n;i++){
	  if(strcmp(p[i].get_name(),name))continue;
	  cout<<"\n�/� "<<name<<" ����� ����� ��� ������� �� ID "<<p[i].get_id()<<"\n";
          fl=0;
	  break;
	}
	if(fl) cout<<"�/� "<<name<< " ��� ����� ��������� ��� ������\n";
        break;
      case 2:
        cout<<"����� �� ID ��� ������ ��� ������ �� ������������ �� ���� = ? ";
	cin>>id;
	if(id<1||id>n){
	  cout<<"��� ������� ����� �� ID "<<id<<"\n";
	  break;
        }
	p[my_id-1].add_friend(id);
	cout<<"�/� "<<p[id-1].get_name()<<" ������������ �� ����� ��� "<<p[my_id-1].get_name()<<"\n";
        break;
      case 3:
        cout<<"����� �� ID ��� ������ ��� ������ �� ���������� ��� ���� = ? ";
	cin>>id;
	fr=p[my_id-1].get_friends(fn);
        fl=1;
	for(i=0;i<fn;i++){
	  if(id==fr[i]){
            p[my_id-1].del_friend(id);
	    cout<<"�/� "<<p[id-1].get_name()<<" ���������� ��� ����� ��� "<<p[my_id-1].get_name()<<"\n";
	    fl=0;;
	    break;
	  }
	}
	if(fl)cout<<"�� ����� �� ID "<<id<<" ��� ������ ����� ������ ���\n";
	break;
      case 4:
	fr=p[my_id-1].get_friends(fn);
	cout<<"�� ����� ��� "<<p[my_id-1].get_name()<<" �����\n";
        for(i=0;i<fn;i++)
          cout<<p[fr[i]-1].get_name()<<"\n";
	continue;
      default:
	my_id=0;
    }
  }       
}

              