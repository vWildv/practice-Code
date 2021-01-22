#include <bits/stdc++.h>
using namespace std;

#define SIZE 10
#define PAGE_SIZE 1
#define inf 0x3f3f3f3f

struct Node{
    int sz,pid,flag;
    Node *l;
    Node *r;
    int addr_s,addr_t;
    /**
    sz,�ڵ������ڴ���С
    pid,ռ�õ�ǰ�ڵ�Ľ���id
    flag,�ڵ�ռ�����,0ΪΪռ��,1Ϊռ��,-1Ϊ�ڵ㲻����
    *l,*r,���Һ���ָ��
    addr_s,�ڴ����ʼ�����ַ
    addr_t,�ڴ����������ַ
    */
};

struct p_node{
    int id,sz,addr_s,addr_t;
    bool operator < (const p_node &rhs) const{
        return id<rhs.id;
    }
    /**
    id,����id
    sz,���������ڴ��С
    addr_s,�������ڴ���ռ�ÿռ��������ʼ��ַ
    addr_t,�������ڴ���ռ�ÿռ�����������ַ
    */
};

struct Buddy_Heap{
    Node *root;
    set<p_node> ps;
    vector<p_node> tmps;

    void create(Node *rt,int sz,int s,int t){
        rt->sz=sz;
        rt->pid=-1;
        rt->flag=0;
        rt->addr_s=s;
        rt->addr_t=t;
        rt->l=NULL;
        rt->r=NULL;
    }

    bool alloc(Node *rt,int id,int sz){
        int s_sz=(rt->sz)>>1;
        ///�����ӽڵ��С
        if(s_sz<sz){
        ///�ӽڵ��ڴ���С,���ǵ�ǰ�ڵ��ͷſ���
            if(rt->sz>=sz&&rt->flag==0){
            ///rt->flagΪ0,�ڴ�����,���Է���
                rt->flag=1;
                rt->pid=id;
                ps.insert((p_node) {id,sz,rt->addr_s,rt->addr_t});
                ///�ڽ�����Ϣ�����в��������Ϣ
                return 1;
            }
            else return 0;    ///����ʧ��
        }
        if(rt->flag==1) return 0;
        if(rt->l==NULL){
            ///�����ӽڵ㲻����,�򴴽��ӽڵ�,��ǰ�ڵ�flagΪ-1
            rt->l=(Node*) malloc(sizeof(Node));
            create(rt->l,s_sz,rt->addr_s,(rt->addr_t+rt->addr_s)>>1);
            rt->r=(Node*) malloc(sizeof(Node));
            create(rt->r,s_sz,(rt->addr_t+rt->addr_s+1)>>1,rt->addr_t);
            rt->flag=-1;
        }
        bool s=alloc(rt->l,id,sz);
        ///������ӷ���
        if(!s) s=alloc(rt->r,id,sz);
        ///����ӷ���ʧ��ʱ,���Ҷ��ӷ���
        return s;
    }

    void relea(Node *rt,int id){
        if(rt->pid==id){
            ///�ҵ���Ҫ�ͷŵĽ���
            ps.erase(ps.lower_bound((p_node){id,0,0,0}));
            ///�ڼ����ж��ֲ��ҽ�����Ϣ,��ɾ��������Ϣ,�������ڽڵ�Ϊ����
            rt->flag=0;
            rt->pid=-1;
            return;
        }
        if(rt->flag==-1){
            ///���������Ҷ���
            relea(rt->l,id);
            relea(rt->r,id);
            ///�����Ҷ���Ѱ����Ҫ�ͷŵĽڵ�
            if(rt->l->flag==0&&rt->r->flag==0){
            ///���Ҷ����ͷ����ʱ,�򸸽ڵ����,�����ӽڵ㶼Ϊ����,��ϲ�
                free(rt->l); rt->l=NULL;
                free(rt->r); rt->r=NULL;
                ///���ڴ����ͷ����Ҷ��ӽڵ�,��ǰ�ڵ�״̬�Ӳ�����ת������
                rt->flag=0;
            }
        }
    }

    void outT(Node *rt,bool l,const string s){
        if(rt->r) outT(rt->r,0,s+(l?"|      ":"       "));
        cout<<s<<(l?'\\':'/')<<"------";
        if(rt->pid==-1) printf("(x,");
        else printf("(%d,",rt->pid);
        printf("%d)\n",rt->flag!=-1?rt->sz:-1);
        if(rt->l) outT(rt->l,1,s+(l?"       ":"|      "));
    }
    void printT(Node *rt){
        if(rt==NULL) return;
        if(rt->r) outT(rt->r,0,"");
        ///��ӡ�����
        if(rt->pid==-1) printf("(x,");
        else printf("(%d,",rt->pid);
        printf("%d)\n",rt->flag!=-1?rt->sz:-1);
        ///��ӡ���ڵ���Ϣ
        if(rt->l) outT(rt->l,1,"");
        ///��ӡ�Ҷ���
        set<p_node>::iterator it;
        for(it=ps.begin();it!=ps.end();it++){
            int tsz=it->sz;
            int tas=it->addr_s;
            int tat=it->addr_t;
            printf("pid:%d sz:%d add_s:%d add_t:%d\n",it->id,tsz,tas,tat);
        }
        ///��ӡ������Ϣ��
    }

    void eraseT(Node *rt){
        if(rt->l->l==NULL&&rt->l->r==NULL){
        ///�������ΪҶ�ӽڵ�,�ͷ������
            free(rt->l);
            rt->l=NULL;
        }
        if(rt->r->l==NULL&&rt->r->r==NULL){
        ///���Ҷ���ΪҶ�ӽڵ�,�ͷ��Ҷ���
            free(rt->r);
            rt->r=NULL;
        }
        if(rt->l) eraseT(rt->l);
        if(rt->r) eraseT(rt->r);
        rt->flag=0;
        rt->l=NULL; rt->r=NULL;
        ///���µݹ�,������Ҷ���
    }
    void anti_fgm(){
        eraseT(root);
        ///��ջ���
        set<p_node>::iterator it;
        for(it=ps.begin();it!=ps.end();it++){
            tmps.push_back((p_node){it->id,it->sz,it->addr_s,it->addr_t});
        }
        ps.clear();
        for(int i=0;i<tmps.size();i++){
            int tid=tmps[i].id,tsz=tmps[i].sz;
            alloc(root,tid,tsz);
        }
        tmps.clear();
        ///���ݱ��нڵ���Ϣ���·���
    }

    /**
    *root,���ڵ�
    set<p_node>,������Ϣ��ϣ��
    vector<p_node>,������Ϣ�ݴ�����
    create,�����ڵ���Ϣ
    alloc,���亯��
    relea,�ͷź���
    outT,printT,��ӡ�ѵ�����ͼ�������Ϣ
    eraseT,anti_fgm,��ն�,����Ƭ����
    */
}BH;

void init(){
    BH.root=(Node*) malloc(sizeof(Node));
    BH.create(BH.root,1<<SIZE,0,(1<<SIZE)*PAGE_SIZE-1);
}

void solve(){
    int op,psz,pid;
    printf("please choose the function that you want to try ");
    printf("(1.allocate 2.release 3.print 4.anti-fragment)\n");
    while(~scanf("%d",&op)&&op){
        if(op==1){
            printf("input the process id and the size: ");
            scanf("%d%d",&pid,&psz);
            printf("allocate %s\n",BH.alloc(BH.root,pid,psz)?"sucess":"failed");
        }
        else if(op==2){
            printf("input the id of the process that you want to release: ");
            scanf("%d",&pid);
            BH.relea(BH.root,pid);
        }
        else if(op==3) BH.printT(BH.root);
        else{
            BH.anti_fgm();
            BH.printT(BH.root);
        }
    }
}

int main(){
    init();
    solve();
}

