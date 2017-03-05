#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

static const float DegToRad = M_PI/180;


struct Vertex
{
    float x,y,z;
};

vector<Vertex>  vv; //点集
vector<int>     iv; //面
vector<Vertex>  nv; //法向量

void gen_base(float ht, float hb, float hf, float radius, float theta, int slice)
{
    //中心点
    Vertex o = { 0, 0, ht };
    vv.push_back(o);

    // top vertex
    float a = theta / 2;
    float da = (M_PI*2-theta)/(slice-1);
    for ( int i=0; i<slice; ++i )
    {
        Vertex v = { 0, 0, ht };
        v.x = radius*cosf(a);
        v.y = radius*sinf(a);
        a += da;
        vv.push_back(v);
    }

    // bottom vertex
    for ( int i=0; i<slice; ++i )
    {
        Vertex v = vv[i+1];
        v.z = hb;
        vv.push_back(v);
    }

    //插入两个前面板点
    Vertex vf1=vv[slice];
    vf1.z = hf;
    vv.push_back(vf1);
    Vertex vf2=vv[1];
    vf2.z = hf;
    vv.push_back(vf2);

    // top face
    for ( int i=0; i<slice; ++i )
    {
        iv.push_back(0);
        iv.push_back(i+1);
        iv.push_back(i+2);
    }
    iv[iv.size()-1] = 1;

    // round face
    for ( int i=1; i<slice; ++i )
    {
        iv.push_back(i);
        iv.push_back(slice+i);
        iv.push_back(i+1);

        iv.push_back(i+1);
        iv.push_back(slice+i);
        iv.push_back(slice+i+1);
    }

    // front face
    int m=2*slice;
    iv.push_back(slice);
    iv.push_back(m+1);
    iv.push_back(m+2);

    iv.push_back(slice);
    iv.push_back(m+2);
    iv.push_back(1);

}

void correct_for_wheel(float radius, float theta, int slice, float hw, float* phi)
{
    static const float e = 0.005f;
    float wa = asinf((hw/2+e)/radius); //车轮占圆周角

    float a = theta / 2;
    float da = (M_PI*2-theta)/(slice-1);
    for ( int wi=0; wi<4; ++wi )
    {
        int j = (int)ceilf((phi[wi]-wa-a)/da)-1;
        int k = (int)floorf((phi[wi]+wa-a)/da)+1;

        //插入两个顶点两个面
        if (j!=0)
        {
            Vertex v1 = vv[slice+1+j];
            vv.push_back(v1);
            iv.push_back(slice+j);
            iv.push_back(vv.size()-1);
            iv.push_back(slice+1+j);
        }
        if (k!=slice-1)
        {
            Vertex v2 = vv[slice+1+k];
            vv.push_back(v2);
            iv.push_back(slice+1+k);
            iv.push_back(vv.size()-1);
            iv.push_back(slice+1+k+1);
        }

        for ( int i=j; i<=k; ++i )
        {
//            const float rwe = hw/2+e;
//            const float rwx = hw/2+(k-i+j-i)/(k-j)*rwe;
//            vv[slice+1+i].z = sqrtf(rwe*rwe-rwx*rwx);
            float z=hw+e;
            if ( i==j || i==k ) z=hw/2+e;
            vv[slice+1+i].z = z;
        }
    }
}

void normalize(Vertex& v)
{
    double r=sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
    if (r!=0.0)
    {
        v.x /= r;
        v.y /= r;
        v.z /= r;
    }
}

void gen_normals()
{
    for (size_t i=0; i<iv.size()/3; ++i)
    {
        size_t i1=iv[i*3];
        size_t i2=iv[i*3+1];
        size_t i3=iv[i*3+2];
        Vertex v1 = {
            vv[i2].x-vv[i1].x,
            vv[i2].y-vv[i1].y,
            vv[i2].z-vv[i1].z
        };
        Vertex v2 = {
            vv[i3].x-vv[i2].x,
            vv[i3].y-vv[i2].y,
            vv[i3].z-vv[i2].z
        };
        Vertex v3 = {
            -v1.z*v2.y+v1.y*v2.z,
             v1.z*v2.x-v1.x*v2.z,
            -v1.y*v2.x+v1.x*v2.y
        };
        normalize(v3);
        nv.push_back(v3);
    }
}

void out_cstyle(const char* vertex_name, const char* index_name, char const* normal_name)
{
    cout<<"\nstatic const float "<<vertex_name<<"[] = {\n";
    vector<Vertex>::iterator it1=vv.begin();
    for ( ; it1!=vv.end()-1; ++it1 )
    {
        cout<<'\t'
            <<it1->x<<", "
            <<it1->y<<", "
            <<it1->z<<",\n";
    }
    cout<<'\t'
        <<it1->x<<", "
        <<it1->y<<", "
        <<it1->z<<"\n};\n";

    cout<<"\nstatic const unsigned int "<<index_name<<"[] = {";
    size_t i=0;
    for ( ; i<iv.size()-1; ++i )
    {
        if (i%3==0) cout<<"\n\t";
        cout<<iv[i]<<",";
    }
    cout<<iv[i]<<"\n};\n";

    cout<<"\nstatic const float "<<normal_name<<"[] = {\n";
    vector<Vertex>::iterator it2=nv.begin();
    for ( ; it2!=nv.end()-1; ++it2 )
    {
        cout<<'\t'
            <<it2->x<<", "
            <<it2->y<<", "
            <<it2->z<<",\n";
    }
    cout<<'\t'
        <<it2->x<<", "
        <<it2->y<<", "
        <<it2->z<<"\n};\n";
}

void out_txt()
{
    cout<<vv.size()*3<<'\n';
    cout<<iv.size()<<'\n';

    vector<Vertex>::iterator vv_it=vv.begin();
    for ( ; vv_it!=vv.end(); ++vv_it )
    {
        cout<<vv_it->x<<"\n"
            <<vv_it->y<<"\n"
            <<vv_it->z<<"\n";
    }

    vector<int>::iterator iv_it=iv.begin();
    for ( ; iv_it!=iv.end(); ++iv_it )
    {
        cout<<*iv_it<<"\n";
    }
    vector<Vertex>::iterator nv_it=nv.begin();
    for ( ; nv_it!=nv.end(); ++nv_it )
    {
        cout<<nv_it->x<<'\n'
            <<nv_it->y<<'\n'
            <<nv_it->z<<'\n';
    }
}

int main()
{
    float radius,theta;
    cout<<"车体直径(毫米):";
    cin>>radius;
    radius /= 2000.0f;
    cout<<"车体前直边所占角度(角度):";
    cin>>theta;
    theta *= DegToRad;

    int slice;
    cout<<"网格质量(圆周等分数):";
    cin>>slice;

    float h1,h2,h3;
    cout<<"车底离地面高度(毫米):";
    cin>>h1;
    h1 /= 1000.0f;
    cout<<"车轮高度(即直径)(毫米):";
    cin>>h2;
    h2 /= 1000.0f;
    cout<<"车顶离地面高度(毫米):";
    cin>>h3;
    h3 /= 1000.0f;

    float phi[4];
    cout<<"四个车轮的角度(角度)\n";
    for ( int i=0; i<4; ++i )
    {
        cout<<i<<": ";
        cin>>phi[i];
        phi[i] *= DegToRad;
    }

    gen_base(h3,h1,h2,radius,theta,slice);
    correct_for_wheel(radius,theta,slice,h2,phi);
    gen_normals();

    out_cstyle("robot_vertices","robot_indices","robot_normals");
    //out_txt();

    return 0;
}

