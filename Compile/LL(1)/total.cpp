#include<bits/stdc++.h>
using namespace std;
const int m=8;
const int n=9;
bool f1[m+5][n+5];
bool l1[m+5][n+5];
char Vn[]={'#','A','B','E','F','G','S','T','Z'};
char Vt[]={'#','a','b','d','e','f','g','(',')','='};
typedef pair<char,char> pi;
vector<string>vstr[1000];//�������Ĵ���ת���﷨�������ַ���
stack<pi> stack1,stack2;
map<char,int> Vnp,Vtp;//��ŷ��ս�����ս��
map<string,int> mV;//��Ŵʷ������Ľ�������ļ��ж�ȡ��ȡ
struct expre{
    char l;
    string r;
};
vector<expre> v;
stack<char>ss;//��Լʱʹ�õ�ջ
map<char,vector<char> >first_vt,last_vt;
int cmp1[305][305];// 0 == 1 < 2 >
int hash_Vn[105];//�����ж�һ���ַ��ǲ��Ƿ��ս����hash����
int hash_Vt[105];//�����ж�һ���ַ��ǲ����ս����hash����
char Cto(string str){ //��ʵ�ʴ���ת�����ķ��ж���ĸ�ʽ
    if(hash_Vt[str[0]]&&hash_Vn[str[0]]&&str.size()==1&&mV[str]!=2){
        return str[0];
    }
    if(str=="if"||str=="while"){
        return 'g';
    }
    if(mV[str]==1){
        return 'a';
    }
    if(mV[str]==2){
        return 'b';
    }
    if(mV[str]==3){
        return 'e';
    }
    if(mV[str]==4){
        return 'd';
    }
}
void Ctodo(string str){  //���ķ����й�Լ����,��������м����
    str+='#';
    cout<<"��ʼ�����봮"<<str<<"���й�Լ"<<endl;
    int cnt = 0;
    ss.push('#');
    ss.push(str[cnt++]);
    while(!ss.empty()||cnt<str.size()){
        cout<<"��ǰջ��Ԫ��"<<ss.top()<<endl;
        cout<<"��ǰ���������ַ�"<<str[cnt]<<endl;
        if(ss.top()=='#'&&str[cnt]=='#'){
            cout<<"����"<<endl;
            break;
        }
        else if(cmp1[(int)ss.top()][(int)str[cnt]]==1){
            cout<<"�ƽ�"<<endl;
            ss.push(str[cnt]);
            cnt++;
        }
        else if(cmp1[(int)ss.top()][(int)str[cnt]]==2){
            cout<<"��Լ"<<endl;
            ss.pop();
        }
        else if(cmp1[(int)ss.top()][(int)str[cnt]]==0){
            cout<<"�ƽ�"<<endl;
            ss.push(str[cnt]);
            cnt++;
        }
        else{
            cout<<"�˾����޷���Լ"<<endl;
            break;
        }
    }
}
void hash_num(){ // ���ս���ͷ��ս������hash����
    for(int i=0; i<strlen(Vn);i++){
        hash_Vn[(int)Vn[i]] = 1;
    }
    for(int i=0; i<strlen(Vt);i++){
        hash_Vt[(int)Vt[i]] = 1;
    }
}
void Getpqfun(){ //�õ����Ⱥ��������
    int f1[30];
    int g1[30];
    for(int i=0;i<30;i++) f1[i] =1;
    for(int i=0;i<30;i++) g1[i] =1;
    cout<<endl<<"\t";
    for (int i=0;i<strlen(Vt);i++){
    printf("%c\t",Vt[i]);
    }
    cout<<endl;
    int cnt=0;
    while(true){
        bool falg =true;
        for (int i=0;i<strlen(Vt);i++){
            for(int j=0;j<strlen(Vt);j++){
                if(cmp1[(int)Vt[i]][(int)Vt[j]]==-1) continue;
                if(cmp1[(int)Vt[i]][(int)Vt[j]]==1&&f1[i]>=g1[j]){
                    g1[j]=f1[i]+1;
                    falg =false;
                }
                else if(cmp1[(int)Vt[i]][(int)Vt[j]]==2&&f1[i]<=g1[j]){
                    f1[i]=g1[j]+1;
                    falg =false;
                }
            }
        }
        if(falg) break;
        printf("----------------------------------------------------%d----------------------------------------------------\n",++cnt);
        printf("f:\t");
        for(int j=0;j<strlen(Vt);j++){
            printf("%d\t",f1[j]);
        }
        printf("\ng:\t");
        for(int j=0;j<strlen(Vt);j++){
            printf("%d\t",f1[j]);
        }
        printf("\n");
    }
}
void GetpqMat(){//�õ����Ⱦ������
    memset(cmp1,-1,sizeof(cmp1));
    int len = v.size();
    for(int i=0; i<len; i++){
        string tmp = v[i].r;
        int len_tmp = tmp.size();
        for(int j=1;j<len_tmp;j++){
            if(hash_Vt[tmp[j-1]]&&hash_Vt[tmp[j+1]]&&hash_Vn[tmp[j]]){
                cmp1[(int)tmp[j-1]][(int)tmp[j+1]]=0;
            }
            if(hash_Vt[tmp[j-1]]&&hash_Vt[tmp[j]]){
                cmp1[(int)tmp[j-1]][(int)tmp[j]]=0;
            }
            else if(hash_Vt[(int)tmp[j-1]]&&hash_Vn[(int)tmp[j]]){
                for(int z=0;z<first_vt[tmp[j]].size();z++){
                    cmp1[(int)tmp[j-1]][(int)first_vt[tmp[j]][z]]=1;
                }
            }
            else if(hash_Vn[tmp[j-1]]&&hash_Vt[tmp[j]]){
                for(int z=0;z<last_vt[tmp[j-1]].size();z++){
                    cmp1[(int)last_vt[tmp[j-1]][z]][(int)tmp[j]]=2;
                }
            }
        }
    }
    for (int i=0;i<strlen(Vt);i++){
        cmp1[(int)'#'][(int)Vt[i]] = 1;
        cmp1[(int)Vt[i]][(int)'#'] = 2;
    }
    cmp1[(int)'#'][(int)'#'] = 0;
}
void Printpqto(int cnt){ //���ù�Լ���� �����������ʽ
    cout<<"��ʼ����������ȹ�Լ"<<endl;
    for(int i=0;i<cnt;i++){
        string _str ="";
        for(int j=0;j<vstr[i].size();j++){
            _str+=Cto(vstr[i][j]);
        }
        cout<<_str<<endl;
        Ctodo(_str);
        cout<<"���봮"<<_str<<"���Խ���"<<endl;
    }
}
void PrintGetpqMat(){//�������Ⱦ����� ������һ�������ʽ
    hash_num();
    cout<<"������ȹ�ϵ����"<<endl;
    GetpqMat();
    printf("\t");
;    for(int i=0;i<strlen(Vt);i++){
        printf("%c\t",Vt[i]);
    }
    printf("\n");
    for (int i=0;i<strlen(Vt);i++){
        printf("%c\t",Vt[i]);
        for(int j=0;j<strlen(Vt);j++){
            if(cmp1[(int)Vt[i]][(int)Vt[j]]==-1) {
                    printf("\t");continue;
            }
            printf("%d\t",cmp1[(int)Vt[i]][(int)Vt[j]]);
        }
        printf("\n");
    }
}
void PrintGetpqfun(){ //�������ȹ�ϵ������������Ⱥ�����
    cout<<"���Ⱥ�����"<<endl;
    Getpqfun();
}
int Getcnt(){ //�Ѵʷ������Ľ�����ļ��е�����map mV�У��������������������﷨�������ת��
    ifstream myfile1("token.txt");
    string str;
    string str1;
    int nMax = 0;
    while(getline(myfile1,str)){
        int st = 0;
        int cnt = 0;
        bool falg = true;
        int num = 0;
        for(int i=0;i<str.size();i++){
            if(str[i]==' '){
                cnt++;
            }
            if(cnt==1&&falg){
                num = str[i+1]-'0';
                falg = false;
            }
            if(cnt==2){
                st = i+1;
                break;
            }
        }
        cnt = 0;
        str1= str.substr(st);
        mV[str1] = num;
        if(num==5){
            nMax++;
            continue;
        }
        vstr[nMax].push_back(str1);
    }
    return nMax;
}
void Printvstr(int cnt){
    cout<<"��Ҫ��Լ�����봮��"<<endl;
    for(int i=0;i<cnt;i++){
        string _str ="";
        for(int j=0;j<vstr[i].size();j++){
            _str+=Cto(vstr[i][j]);
        }
        cout<<_str<<endl;
    }
}
void init(){
    for(int i=1;i<=m;i++){
        Vnp[Vn[i]]=i;
    }
    for(int i=1;i<=n;i++){
        Vtp[Vt[i]]=i;
    }
    v.push_back((expre){'Z',"S"});
    v.push_back((expre){'S',"A"});
    v.push_back((expre){'S',"B"});
    v.push_back((expre){'S',"G"});
    v.push_back((expre){'A',"ab"});
    v.push_back((expre){'B',"b=E"});
    v.push_back((expre){'E',"EdT"});
    v.push_back((expre){'E',"T"});
    v.push_back((expre){'T',"(E)"});
    v.push_back((expre){'T',"b"});
    v.push_back((expre){'T',"e"});
    v.push_back((expre){'F',"EfE"});
    v.push_back((expre){'G',"g(F)S"});
}
bool judge(char ch){
    for(int i=1;i<=m;i++){
        if(ch==Vn[i]){
            return false;
        }
    }
    return true;
}
void Insert1(char ch1,char ch2){
    int p1=Vnp[ch1];
    int p2=Vtp[ch2];
    if(!f1[p1][p2]){
        f1[p1][p2]=true;
        stack1.push(make_pair(ch1,ch2));
    }
}
void FirstVT(){
    for(int i=1;i<=m;i++){
        for(int j=1;j<=n;j++){
            f1[i][j]=false;
        }
    }
    int len=v.size();
    for(int i=0;i<len;i++){
        string tmp=v[i].r;
        if(judge(tmp[0])==1){
            Insert1(v[i].l,tmp[0]);
        }
        else if(tmp.size()>=2&&judge(tmp[0])==0&&judge(tmp[1])==1){
            Insert1(v[i].l,tmp[1]);
        }
    }
    while(!stack1.empty()){
        pi tmp1=stack1.top();
        stack1.pop();
        for(int i=0;i<len;i++){
            string tmp=v[i].r;
            if(tmp[0]==tmp1.first){
                Insert1(v[i].l,tmp1.second);
            }
        }
    }
}
void Insert2(char ch1,char ch2){
    int p1=Vnp[ch1];
    int p2=Vtp[ch2];
    if(!l1[p1][p2]){
        l1[p1][p2]=true;
        stack2.push(make_pair(ch1,ch2));
    }
}
void LastVT(){
    for(int i=1;i<=m;i++){
        for(int j=1;j<=n;j++){
            l1[i][j]=false;
        }
    }
    int len=v.size();
    for(int i=0;i<len;i++){
        string tmp=v[i].r;
        int l=tmp.size();
        if(judge(tmp[l-1])==1){
            Insert2(v[i].l,tmp[l-1]);
        }
        else if(l>=2&&judge(tmp[l-1])==0&&judge(tmp[l-2])==1){
            Insert2(v[i].l,tmp[l-2]);
        }
    }
    while(!stack2.empty()){
        pi tmp1=stack2.top();
        stack2.pop();
        for(int i=0;i<len;i++){
            string tmp=v[i].r;
            int l=tmp.size();
            if(tmp[l-1]==tmp1.first){
                Insert2(v[i].l,tmp1.second);
            }
        }
    }
}
int main(){
    init();
    FirstVT();
    cout<<"FirstVT:"<<endl;
    for(int i=1;i<=m;i++){
        cout<<Vn[i]<<":";
        for(int j=1;j<=n;j++){
            if(f1[i][j]){
                first_vt[Vn[i]].push_back(Vt[j]);
                cout<<Vt[j]<<" ";
            }
        }
        cout<<endl;
    }
    LastVT();
    cout<<"LastVT:"<<endl;
    for(int i=1;i<=m;i++){
        cout<<Vn[i]<<":";
        for(int j=1;j<=n;j++){
            if(l1[i][j]){
                last_vt[Vn[i]].push_back(Vt[j]);
                cout<<Vt[j]<<" ";
            }
        }
        cout<<endl;
    }

    PrintGetpqMat(); // ���������Ⱦ���
    PrintGetpqfun(); // ���������Ⱥ���
    int cnt = Getcnt();//����Ŵʷ�����������ļ�ȡ����Ž�map�У����ش��������ʷ�������һ���ֺ���һ�С�
    Printvstr(cnt);//������ת���﷨����ĸ�ʽ��ӡ����
    Printpqto(cnt);  // �����Լ�����Լ����
    return 0;
}

